


////////////////////////////////////////////////////////////////////////////////////////////////////

// ccipd includes
#include "ccipdLoadVolume.h" // loading our mask
#include "ccipdLandmarkTypes.h"
#include "ccipdNormalDirectionCalculator.h"
#include "ccipdClosestPoint.h"
#include "ccipdClosestPoints.h"

// itk includes
#include "ccipdDisableWarningsMacro.h"
#include <itkImage.h>
#include <itkPointSet.h>
#include "ccipdEnableWarningsMacro.h"

#include "ccipdSegmentationOptions.h"
#ifdef ENABLE_GUI_CORE

// ccipd includes
#include "ccipdIsosurfaceRendering.h"
#include "ccipdLandmarksToActor.h"
#include "ccipdMaskToLevelset.h"

// vtk includes
#include "ccipdDisableWarningsMacro.h"
#include <vtkSmartPointer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkCamera.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkProperty.h>
#include "ccipdEnableWarningsMacro.h"

using ccipd::MaskToLevelset;

#endif // ENABLE_GUI_CORE

////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////
using std::cout;
using std::endl;
using std::cerr;
using std::string;
using ccipd::GetClosestSurfacePoint;
using ccipd::GetClosestSurfacePoints;
using ccipd::InternalPixelType;
using ccipd::LandmarkPointType;
using ccipd::LandmarkType;
using ccipd::LoadVolumeMask;
using ccipd::LandmarkSetConstPointer;
using ccipd::NormalDirectionCalculator;
using ccipd::VolumeDimension;
using ccipd::VolumeMaskImageConstPointer;
////////////////////////////////////////////////////////////////////////////////////////////////




int main( int argc, char * argv[] )
{

  if ( argc == 1 )
    {
    cout << "Usage: " << argv[ 0 ] << " MaskImage" << endl;
    return EXIT_SUCCESS;
    }

  // load a file
  const string maskFileName = argv[ 1 ];

  cout << "Loading " << maskFileName << ":" << endl;

  const bool ignoreOrientation = true;
  const VolumeMaskImageConstPointer mask(
    LoadVolumeMask( maskFileName, ignoreOrientation ) );

  if ( !mask )
    {
    cerr << "Error: unable to load " << maskFileName << endl;
    return EXIT_FAILURE;
    }

  cout << "Loading " << maskFileName << " done." << endl;

  const NormalDirectionCalculator< VolumeDimension >::ConstPointer
    calculator = NormalDirectionCalculator< VolumeDimension >::ConstNew(
      mask.GetPointer(), true );

  // create some points
  const LandmarkPointType
    zeroPoint( LandmarkType( 0 ) ),
    closestZeroPoint( GetClosestSurfacePoint( zeroPoint, *calculator ) );

  cout << "Point = " << zeroPoint << endl;
  cout << "Distance = " << calculator->GetSignedDistanceToSurface( zeroPoint ) << endl;
  cout << "Normal = " << calculator->GetNormalDirection( zeroPoint ) << endl;
  cout << "Closest point = " << closestZeroPoint << endl;

  const itk::Index< VolumeDimension > index =
    mask->GetBufferedRegion().GetIndex();
  LandmarkPointType cornerPoint;
  mask->TransformIndexToPhysicalPoint( index, cornerPoint );
  const LandmarkPointType
    closestCornerPoint( GetClosestSurfacePoint( cornerPoint, *calculator ) );
  
  cout << "Point = " << cornerPoint << endl;
  cout << "Distance = " << calculator->GetSignedDistanceToSurface( cornerPoint ) << endl;
  cout << "Normal = " << calculator->GetNormalDirection( cornerPoint ) << endl;
  cout << "Closest point = " << closestCornerPoint << endl;
 
  // optionally render
#ifdef ENABLE_GUI_CORE

  using ccipd::GenerateActorFromLandmarks;
  using ccipd::GenerateVTKActorFromIsosurface;
  using ccipd::LandmarkSetType;
  using ccipd::LandmarkSetPointer;
  using ccipd::VolumeImageConstPointer;
  using ccipd::vtkRenderWindowPointer;
  using ccipd::vtkRendererPointer;
  using ccipd::vtkRenderWindowInteractorPointer;
  using ccipd::vtkPropPointer;
  using ccipd::vtkActorPointer;

  // create a levelset from the mask
  cout << "Mask to levelset:" << endl;
  const VolumeImageConstPointer levelset(
    MaskToLevelset< InternalPixelType >( mask.GetPointer(), false, false, 4.0, 0.0, false ) );
  cout << "Mask to levelset done." << endl;

  // create an actor from the levelset
  cout << "Levelset to actor:" << endl;
  const vtkActorPointer levelsetActor =
    GenerateVTKActorFromIsosurface( levelset, 0 );
  cout << "Levelset to actor done." << endl;

  // show as a wire frame
  levelsetActor->GetProperty()->SetRepresentationToWireframe();
  levelsetActor->GetProperty()->SetOpacity( 0.3 );

  // create a landmark collection
  const LandmarkSetPointer landmarks = LandmarkSetType::New();
  landmarks->Initialize();
  landmarks->SetPoint( 0, zeroPoint          );
  landmarks->SetPoint( 1, closestZeroPoint   );
  landmarks->SetPoint( 2, cornerPoint        );
  landmarks->SetPoint( 3, closestCornerPoint );

  // just for stepping through
  GetClosestSurfacePoints( *landmarks, *calculator );

  //Create a renderer, render window, and interactor
  const vtkRenderWindowPointer renderWindow = vtkRenderWindowPointer::New();
  const vtkRendererPointer     renderer     = vtkRendererPointer    ::New();
  renderWindow->AddRenderer( renderer );

  // render the actors
  renderer->AddActor( levelsetActor );
  renderer->AddActor( GenerateActorFromLandmarks( landmarks ) );

  vtkRenderWindowInteractorPointer renderWindowInteractor =
    vtkRenderWindowInteractorPointer::New();
  renderWindowInteractor->SetRenderWindow( renderWindow );
   
  // make it interact like paraview
  typedef vtkSmartPointer< vtkInteractorStyleTrackballCamera > vtkInteractorStylePointer;
  const vtkInteractorStylePointer style = vtkInteractorStylePointer::New();
  renderWindowInteractor->SetInteractorStyle( style );

  // render the scene
  renderer->ResetCamera();
  renderWindow->Render();

  // start interacting
  renderWindowInteractor->Start();

#endif // ENABLE_GUI_CORE

  // exit
  return EXIT_SUCCESS;

} // main
