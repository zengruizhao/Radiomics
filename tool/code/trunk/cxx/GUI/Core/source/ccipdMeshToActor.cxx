


//////////////////////////////////////////////////////////////////////////////////////////

// ccipd includes
#include "ccipdCore.h"
#include "ccipdMeshToActor.h"
#include "ccipdITKMeshToVTKUnstructuredGrid.h" // for creating a VTK type
#include "ccipdITKLandmarksToVTKPolyData.h"    // for viewing the mesh landmarks

#include "ccipdDisableWarningsMacro.h"

// vtk includes
#include <vtkVersion.h>
#include <vtkUnstructuredGrid.h> // the actual vtk data
#include <vtkSmartPointer.h>     // for memory management
#include <vtkPolyData.h>         // the vtk representation of our points
#include <vtkPolyDataMapper.h>   // to display points
#include <vtkDataSetMapper.h>    // to displaying a grid (triangles)
#include <vtkActor.h>            // something to render
#include <vtkProperty.h>         // to change the colors

// itk includes
#include <itkMesh.h>

#include "ccipdEnableWarningsMacro.h"

//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
using std::cout;
using std::endl;
using std::cerr;
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////
vtkActorPointer GenerateTrianglesActorFromMesh(  const VolumeMeshType * const mesh )
{

  // convert to vtk unstructured grid
  const vtkUnstructuredGridPointer grid = ConvertMeshToUnstructuredGrid( mesh );

  // create a mapper and actor for the triangulation
  vtkDataSetMapperPointer triangulationMapper = vtkDataSetMapperPointer::New();
  triangulationMapper->
#if VTK_MAJOR_VERSION >= 6
    SetInputData
#else
    SetInput
#endif
      ( grid );
 
  const vtkActorPointer trianglesActor = vtkActorPointer::New();
  trianglesActor->SetMapper( triangulationMapper );

  // change the colors of the triangulation
  const double
    triangulationR       = 0.5,
    triangulationG       = 0.9,
    triangulationB       = 0.9,
    triangulationOpacity = 0.5;
  trianglesActor->GetProperty()->SetColor(
    triangulationR, triangulationG, triangulationB );
  trianglesActor->GetProperty()->SetOpacity( triangulationOpacity );

  trianglesActor->GetProperty()->EdgeVisibilityOn();

  return trianglesActor;

} // GenerateTrianglesActorFromMesh
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace
