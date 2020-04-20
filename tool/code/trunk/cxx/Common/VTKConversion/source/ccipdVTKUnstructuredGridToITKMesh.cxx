


//////////////////////////////////////////////////////////////////////////////////////////
// ccipd includes
#include "ccipdVTKUnstructuredGridToITKMesh.h"
#include "ccipdVTKPointsToITKLandmarks.h" // for converting points

#include "ccipdDisableWarningsMacro.h"
  // itk includes
  #include <itkMesh.h>
  #include <itkTetrahedronCell.h> // these are our cells

  // vtk includes
  #include <vtkSmartPointer.h>
  #include <vtkUnstructuredGrid.h>
  #include <vtkPoints.h> // for extracting the point data
  #include <vtkCellArray.h>
#include "ccipdEnableWarningsMacro.h"
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////
VolumeMeshPointer ConvertVTKUnstructuredGridToITKMesh(
  const vtkUnstructuredGridPointer grid )
{

  // based on http://bit.ly/HfVlDY

  // create a mesh
  VolumeMeshPointer outputMesh = VolumeMeshType::New();

  // first get the points
  const vtkPointsPointer inputPoints = grid->GetPoints();

  // now, convert them to landmarks
  const LandmarkSetPointer landmarks = ConvertVTKPointsToLandmarks( inputPoints );

  // give the landmarks to the mesh
  outputMesh->SetPoints( landmarks->GetPoints() );

  // get the cells
  typedef vtkSmartPointer< vtkCellArray > vtkCellArrayPointer;
  const vtkCellArrayPointer cells = grid->GetCells();

  // how will we identify each cell?
  typedef VolumeMeshType::CellIdentifier CellIdentifier;
  CellIdentifier currentCellID = 0;

  // we must iterate through all the cells in the grid
  int anotherCell = 1;
  while ( anotherCell != 0 )
    {

    // get the cell
    vtkIdType numberOfPoints;
    vtkIdType * whichPoints;
    anotherCell = cells->GetNextCell( numberOfPoints, whichPoints );

    // if it's a tetrahedron...
    const bool isTetrahedron = ( numberOfPoints == 4 );
    if ( isTetrahedron )
      {

      // we must create a cell to input into our mesh
      typedef VolumeMeshType::CellType  CellType;
      typedef CellType::CellAutoPointer CellAutoPointer;
      typedef CellType::PointIdentifier CellPointIdentifier;

      typedef itk::TetrahedronCell< CellType > TetrahedronCellType;

      CellAutoPointer currentOutputCell;
      currentOutputCell.TakeOwnership( new TetrahedronCellType );

      // now we must populate the tetrahedron
      currentOutputCell->SetPointId( 0, static_cast < CellPointIdentifier > ( whichPoints[ 0 ] ) );
      currentOutputCell->SetPointId( 1, static_cast < CellPointIdentifier > ( whichPoints[ 1 ] ) );
      currentOutputCell->SetPointId( 2, static_cast < CellPointIdentifier > ( whichPoints[ 2 ] ) );
      currentOutputCell->SetPointId( 3, static_cast < CellPointIdentifier > ( whichPoints[ 3 ] ) );

      // give the tetrahedron to the mesh
      outputMesh->SetCell( currentCellID++, currentOutputCell );

      } // isTetrahedron

    } // while another cell

  return outputMesh;

} // VTKUnstructuredGridToITKMesh
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace ccipd
