#ifndef __ccipdRenderPointPicker_hxx
#define __ccipdRenderPointPicker_hxx



////////////////////////////////////////////////////////////////////////////////////////////////////

// ccipd includes
#include "ccipdRenderPointPicker.h"
#include "ccipdVTKForwardDeclarations.h"          // vtk types
#include "ccipdMouseInteractorStylePointPicker.h" // for picking points
#include "ccipdImageToProp.h"                     // for creating a vtk prop from an image

#include "ccipdDisableWarningsMacro.h"
  // itk includes
  #include <itkPointSet.h>

  // vtk includes
  #include <vtkSmartPointer.h>
  #include <vtkActor.h>
  #include <vtkRenderer.h>
  #include <vtkRenderWindow.h>
  #include <vtkRenderWindowInteractor.h>
  #include <vtkCamera.h>
#include "ccipdEnableWarningsMacro.h"
////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////
using std::cout;
using std::endl;
////////////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{




////////////////////////////////////////////////////////////////////////////////////////////////
template< typename TImageType >
void RenderPointPicker(
  const TImageType * const image,
  const unsigned int       slice,
  const bool               verbose,
  vtkRenderWindow * const  renderWindow )
{

  // just create an empty landmark set
  return RenderPointPicker( image, slice, verbose, renderWindow,
    LandmarkSetConstPointer( LandmarkSetType::New() ) );

} // RenderPointPicker
////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////
template< typename TImageType >
void RenderPointPicker(
  const TImageType * const        image,
  const unsigned int              slice,
  const bool                      verbose,
  vtkRenderWindow * const         renderWindow,
  const LandmarkSetConstPointer & initialLandmarks )
{
 
  // make sure we have an image
  if ( !image )
    return;

  // convert it to a vtk image for viewing
  const vtkPropPointer imageProp = CreatePropFromImage( image, slice );

  typedef ccipd::MouseInteractorStylePointPicker  StyleType;
  typedef vtkSmartPointer< StyleType           > StyleTypePointer;

  // create the vtk objects 
  const vtkRendererPointer renderer = vtkRendererPointer::New();

  // make sure the camera is correct since itk flips the y axis
  vtkCamera * const camera = renderer->GetActiveCamera();
  camera->SetViewUp( 0, -1, 0 );

  // connect the components
  renderer    ->AddViewProp( imageProp );
  renderWindow->AddRenderer( renderer  );

  // http://bit.ly/JtSrwd 

  StyleType * const styleRaw = 
    initialLandmarks.IsNull() ?
    new StyleType() :
    new StyleType( initialLandmarks, renderWindow );

  const StyleTypePointer style = StyleTypePointer::Take( styleRaw );

  // show the points as we pick them
  style->SetVerbose( verbose );
  
  // make sure we have an interactor
  vtkRenderWindowInteractorPointer
    interactor = renderWindow->GetInteractor();

  if ( !interactor )
    {
    interactor = vtkRenderWindowInteractorPointer::New();
    interactor->SetRenderWindow( renderWindow );
    } // !interactor
  
  interactor->SetInteractorStyle( style );

  // the x axis is actually also flipped,
  // so we will place the camera "behind" the image
  renderer->ResetCamera(); // view the whole scene
  const double * const position = camera->GetPosition();
  camera->SetPosition( position[ 0 ], position[ 1 ], -position[ 2 ] );

  // render
  renderWindow->Render();

} //RenderPointPicker
////////////////////////////////////////////////////////////////////////////////////////////////



} // namespace ccipd
#endif // __ccipdRenderPointPicker_hxx
