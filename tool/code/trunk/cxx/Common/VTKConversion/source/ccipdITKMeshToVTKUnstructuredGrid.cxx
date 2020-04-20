


//////////////////////////////////////////////////////////////////////////////////////////
// ccipd includes
#include "ccipdITKMeshToVTKUnstructuredGrid.h"
#include "ccipdITKLandmarksToVTKPoints.h" // for converting our point data

#include "ccipdDisableWarningsMacro.h"
  // itk includes
  #include <itkMesh.h>
  #include <itkTetrahedronCell.h>
  #include <itkHexahedronCell.h>

  // vtk includes
  #include <vtkUnstructuredGrid.h>
  #include <vtkSmartPointer.h>
  #include <vtkCellArray.h>

  // boost includes
  #include <boost/config.hpp>
#include "ccipdEnableWarningsMacro.h"

#ifdef _MSC_VER
#pragma warning( disable: 4482 )
#endif // _MSC_VER
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
using std::cerr;
using std::endl;
//////////////////////////////////////////////////////////////////////////////////////////



// based on http://bit.ly/HfVlDY


namespace ccipd
{


  
//////////////////////////////////////////////////////////////////////////////////////////
class VisitVTKCellsClass
{


private:
  vtkCellArray * m_Cells;
  int          * m_LastCell;
  int          * m_TypeArray;

public:
  // typedef the itk cells we are interested in

  typedef VolumeMeshType::CellTraits CellTraits;
  typedef itk::CellInterface<
    MeshPixelType,
    CellTraits
  > CellInterfaceType;

  typedef itk::TetrahedronCell<
    CellInterfaceType
  > TetrahedronCellType;

  typedef itk::HexahedronCell<
    CellInterfaceType
  > HexahedronCellType;

  // Set the vtkCellArray that will be constructed
  void SetCellArray( vtkCellArray * cells )
  {
    m_Cells = cells;
  }

  // Set the cell counter pointer
  void SetCellCounter( int * counter )
  {
    m_LastCell = counter;
  }

  // Set the type array for storing the vtk cell types
  void SetTypeArray( int * typeArray )
  {
    m_TypeArray = typeArray;
  }

  template< class TCellType >
  void Visit( const unsigned long cellId, const TCellType * const cell )
  {

    (void)cellId; // ignore warnings about it being unused

    // how many point id's define the cell?
    BOOST_CONSTEXPR_OR_CONST bool
      isTetrahedron = std::is_same< TCellType, TetrahedronCellType >::value,
      isHexahedron  = std::is_same< TCellType, HexahedronCellType  >::value;

    static_assert( isTetrahedron || isHexahedron, "Unknown cell type." );

    BOOST_CONSTEXPR_OR_CONST unsigned int numberOfPointsInCell =
      isTetrahedron ? 4 : 8;

    // since vtkIdType may not be the same as a point id iterator,
    // we will play it safe and just copy them out to a temporary variable
    // and cast them

    vtkIdType tempIds[ numberOfPointsInCell ];
    for ( unsigned int pointIndex = 0;
      pointIndex < numberOfPointsInCell; ++pointIndex )
      {
      tempIds[ pointIndex ] = static_cast< vtkIdType >(
        *( cell->GetPointIds() + pointIndex ) );
      }

    // insert the cell with the given id's
    m_Cells->InsertNextCell(
      numberOfPointsInCell,
      tempIds );

    // say what type it is
    m_TypeArray[ *m_LastCell ] = isTetrahedron ?
      VTKCellType::VTK_TETRA :
      VTKCellType::VTK_HEXAHEDRON;

    // increment
    ++( *m_LastCell );
    
  } // Visit
  
}; // class VisitVTKCellsClass
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
template
void VisitVTKCellsClass::Visit< VisitVTKCellsClass::TetrahedronCellType >(
  const unsigned long cellId, const TetrahedronCellType * const cell );

template
void VisitVTKCellsClass::Visit< VisitVTKCellsClass::HexahedronCellType >(
  const unsigned long cellId, const HexahedronCellType * const cell );
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
vtkUnstructuredGridPointer ConvertMeshToUnstructuredGrid( const VolumeMeshType * mesh )
{

  // Get the number of points in the mesh
  typedef VolumeMeshType::PointIdentifier PointIdentifier;
  const PointIdentifier numberOfPoints = mesh->GetNumberOfPoints();

  // check to make sure we have some points
  if( numberOfPoints == 0 )
    {
    mesh->Print(std::cerr);
    cerr << "No points in Grid " << endl;
    return vtkUnstructuredGridPointer();
    }

  // Create a vtkUnstructuredGrid
  vtkUnstructuredGridPointer grid = vtkUnstructuredGridPointer::New();

  // Create the vtkPoints object and set the number of points
  const vtkPointsPointer gridPoints = ConvertLandmarksToVTKPoints( mesh );

  // Set the points on the vtk grid
  grid->SetPoints( gridPoints );

  // Now create the cells using the MulitVisitor
  // 1. Create a MultiVisitor
  typedef VolumeMeshType::CellType::MultiVisitor MultiVisitor;
  MultiVisitor::Pointer multiVisitor = MultiVisitor::New();

  // 2. Create a tetrahedron visitor
  typedef VolumeMeshType::CellTraits CellTraits;
  typedef VisitVTKCellsClass::TetrahedronCellType TetrahedronCellType;
  typedef itk::CellInterfaceVisitorImplementation<
    MeshPixelType,
    CellTraits,
    TetrahedronCellType,
    VisitVTKCellsClass
  > TetrahedronVisitorType;

  typedef TetrahedronVisitorType::Pointer TetrahedronVisitorPointer;
  const TetrahedronVisitorPointer tetrahedronVisitor = TetrahedronVisitorType::New();

  // 3. Create a hexahedron visitor
  typedef VisitVTKCellsClass::HexahedronCellType HexahedronCellType;
  typedef itk::CellInterfaceVisitorImplementation<
    MeshPixelType,
    CellTraits,
    HexahedronCellType,
    VisitVTKCellsClass
  > HexahedronVisitorType;

  typedef HexahedronVisitorType::Pointer HexahedronVisitorPointer;
  const HexahedronVisitorPointer hexahedronVisitor = HexahedronVisitorType::New();

  // 4. Set up the visitors
  int vtkCellCount = 0; // running counter for current cell being inserted
  PointIdentifier numberOfCells = mesh->GetNumberOfCells();
  int * types = new int[ numberOfCells ]; // type array for vtk

  // create vtk cells and estimate the size
  typedef vtkSmartPointer< vtkCellArray > vtkCellArrayPointer;
  const vtkCellArrayPointer cells = vtkCellArrayPointer::New();

  cells->EstimateSize( (vtkIdType) numberOfCells, 8 );

  // Set the TypeArray CellCount and CellArray for both visitors
  tetrahedronVisitor->SetTypeArray  ( types         );
  tetrahedronVisitor->SetCellCounter( &vtkCellCount );
  tetrahedronVisitor->SetCellArray  ( cells         );
  
  // Set the TypeArray CellCount and CellArray for both visitors
  hexahedronVisitor->SetTypeArray  ( types         );
  hexahedronVisitor->SetCellCounter( &vtkCellCount );
  hexahedronVisitor->SetCellArray  ( cells         );

  // add the visitors to the multivisitor
  multiVisitor->AddVisitor( tetrahedronVisitor );
  multiVisitor->AddVisitor( hexahedronVisitor  );
  
  // Now ask the mesh to accept the multivisitor which
  // will Call Visit for each cell in the mesh that matches the
  // cell types of the visitors added to the MultiVisitor
  mesh->Accept( multiVisitor );

  // Now set the cells on the vtk grid with the type array and cell array
  grid->SetCells( types, cells );

  // return the vtkUnstructuredGrid
  return grid;

} // MeshToUnstructuredGrid
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace
