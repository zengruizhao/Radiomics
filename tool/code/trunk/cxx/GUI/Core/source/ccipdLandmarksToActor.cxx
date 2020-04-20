


//////////////////////////////////////////////////////////////////////////////////////////
// ccipd includes
#include "ccipdCore.h"
#include "ccipdLandmarksToActor.h"
#include "ccipdITKLandmarksToVTKPolyData.h" // for converting to poly data
#include "ccipdPolyDataToSpheres.h"         // for generating spheres

// vtk includes
#include <vtkSmartPointer.h>     // for memory management
#include <vtkPolyData.h>         // the vtk representation of our landmarks
#include <vtkActor.h>            // something to render
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{


  
//////////////////////////////////////////////////////////////////////////////////////////
vtkActorPointer GenerateActorFromLandmarks(
  const LandmarkSetType * const landmarks,
  const double                  sphereRadius )
{

  if ( !landmarks ) return vtkActorPointer();

  // create a vtk poly data from the mesh landmarks
  const vtkPolyDataPointer polyData = ConvertLandmarksToPolyData( landmarks );

  // convert it to an actor
  const vtkActorPointer landmarksActor =
    GenerateSpheresFromPolyData( polyData, sphereRadius );

  return landmarksActor;

}  // GenerateLandmarksActorFromMesh
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace
