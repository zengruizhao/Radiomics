


//////////////////////////////////////////////////////////////////////////////////////////
// ccipd include
#include "ccipdCore.h"
#include "ccipdPolyDataToSpheres.h"

// vtk includes
#include <vtkVersion.h>
#include <vtkSmartPointer.h>     // for memory management
#include <vtkPolyData.h>         // the vtk representation of our landmarks
#include <vtkPolyDataMapper.h>   // for mapping the glyphs
#include <vtkGlyph3D.h>          // to display spheres at each landmark
#include <vtkSphereSource.h>     // the geometric source for the glyphs
#include <vtkActor.h>            // something to render
#include <vtkProperty.h>         // to change the colors
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{


  
//////////////////////////////////////////////////////////////////////////////////////////
vtkActorPointer GenerateSpheresFromPolyData(
  const vtkPolyDataPointer & polyData,
  const double               sphereRadius )
{

  // create a spherical glyph at each landmark
  typedef vtkSmartPointer< vtkSphereSource > vtkSphereSourcePointer;
  const vtkSphereSourcePointer sphereSource = vtkSphereSourcePointer::New();
  sphereSource->SetRadius( sphereRadius );

  // update the resolution of the spheres
  const int resolution = 30;
  sphereSource->SetPhiResolution  ( resolution );
  sphereSource->SetThetaResolution( resolution );

  typedef vtkSmartPointer< vtkGlyph3D > vtkGlyph3DPointer;
  vtkGlyph3DPointer glyphs = vtkGlyph3DPointer::New();

  // give the data to the glyphs
  glyphs->SetSourceConnection( sphereSource->GetOutputPort() );
  glyphs->
#if VTK_MAJOR_VERSION >= 6
    SetInputData
#else
    SetInput
#endif
      ( polyData );

  // we are using a constant sphere radius, so no scaling
  glyphs->ScalingOff();

  // create a mapper and actor for the points
  vtkPolyDataMapperPointer mapper = vtkPolyDataMapperPointer::New();
  mapper->SetInputConnection( glyphs->GetOutputPort() );
  
  const vtkActorPointer spheresActor = vtkActorPointer::New();
  spheresActor->SetMapper( mapper );

  // change the colors of the landmarks
  const double
    polyR       = 1.0,
    polyG       = 1.0,
    polyB       = 0.1,
    polyOpacity = 1.0;
  spheresActor->GetProperty()->SetColor( polyR, polyG, polyB );
  spheresActor->GetProperty()->SetOpacity( polyOpacity );

  return spheresActor;

}  // GenerateSpheresFromPolyData
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace
