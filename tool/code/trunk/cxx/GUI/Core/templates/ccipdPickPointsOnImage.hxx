#ifndef __ccipdPickPointsOnImage_hxx
#define __ccipdPickPointsOnImage_hxx



////////////////////////////////////////////////////////////////////////////////////////////////////
// ccipd includes
#include "ccipdPickPointsOnImage.h"
#include "ccipdRenderPointPicker.h"
#include "ccipdVTKForwardDeclarations.h"          // vtk types
#include "ccipdMouseInteractorStylePointPicker.h" // for picking points

#include "ccipdDisableWarningsMacro.h"
  // itk includes
  #include <itkPointSet.h>
  
  // vtk includes
  #include <vtkSmartPointer.h>
  #include <vtkRenderWindow.h>
  #include <vtkRenderWindowInteractor.h>
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
LandmarkSetPointer PickPointsOnImage(
  const TImageType * const image,
  const unsigned int       slice,
  const bool               verbose )
{

  // make sure we have an image
  if ( !image )
    return LandmarkSetPointer();

  // create a render window
  const vtkRenderWindowPointer renderWindow = vtkRenderWindowPointer::New();

  // set it up with a point picker and our image
  RenderPointPicker( image, slice, verbose, renderWindow );

  // start the interaction
  renderWindow->SetWindowName( // has to be called after Render()
    "Use Ctrl + Right Click to select points" );

  if ( verbose )
    {
    cout << "Slice = " << slice                        << endl;
    cout << endl << "Usage:"                           << endl;
    cout << "Use Ctrl + Left Drag to rotate."          << endl;
    cout << "Use Ctrl + Right Click to select points." << endl;
    cout << "Use Right Drag to zoom."                  << endl;
    cout << "Use Middle Wheel to zoom."                << endl;
    cout << "Use Left Drag to change the windowing."   << endl;
    cout << "Use Middle Drag to pan."                  << endl;
    cout << endl;
    }

  renderWindow->GetInteractor()->Start();

  // we know this is our type
  typedef MouseInteractorStylePointPicker StyleType;
  const StyleType * const style = static_cast< StyleType * >(
    renderWindow->GetInteractor()->GetInteractorStyle() );

  // get the points we've picked
  const LandmarkSetPointer landmarks( style->GetLandmarks() );

  if ( verbose )
    {

    // now, output the final points
    cout << "Landmarks Set = " << endl << landmarks << endl;

    // loop through each point
    for ( LandmarkSetType::PointIdentifier landmarkIndex = 0;
      landmarkIndex < landmarks->GetNumberOfPoints(); ++landmarkIndex )
      {
      // display the current point
      cout << "Point " << landmarkIndex <<
        " = " << landmarks->GetPoint( landmarkIndex ) << endl;
      } // for landmarkIndex

    } // verbose

  return landmarks;
  
} // PickPointsOnImage
////////////////////////////////////////////////////////////////////////////////////////////////



} // namespace ccipd
#endif // __ccipdPickPointsOnImage_hxx
