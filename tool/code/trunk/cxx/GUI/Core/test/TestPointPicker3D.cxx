


////////////////////////////////////////////////////////////////////////////////////////////////////

// ccipd includes
#include "ccipdLoadVolume.h"        // loading our image
#include "ccipdPreprocessMRI.h"
#include "ccipdPickPointsOnVolume.h" // for picking points
#include "ccipdImageToProp.h"
#include "ccipdLandmarksToActor.h"
#include "ccipdUpdateVTKImageWindowLevel.h"
#include "ccipdBoundingBoxCreation.h"
#include "ccipdBoundingBoxToActor.h"
#include "ccipdMaskToLevelset.h"

// std includes
#include <string>
#include <cstdlib>
#include <iostream>

#include "ccipdDisableWarningsMacro.h"

// itk includes
#include <itkImage.h>
#include <itkPointSet.h>

// vtk includes
#include <vtkSmartPointer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkCamera.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkProperty.h>

// qt includes
#include <QApplication>

#include "ccipdEnableWarningsMacro.h"

////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////
using std::cout;
using std::endl;
using std::cerr;
using std::string;
using ccipd::LoadVolume;
using ccipd::InternalPixelType;
//using ccipd::MaskToLevelset;
using ccipd::GenerateVTKActorFromBoundingBox;
//using ccipd::CreateMaskFromBoundingBox;
using ccipd::CreatePropFromImage;
using ccipd::CreateBoundingBox;
using ccipd::VolumeRegionType;
using ccipd::VolumeMaskImageConstPointer;
using ccipd::VolumeImageConstPointer;
using ccipd::LandmarkSetConstPointer;
using ccipd::PickPointsOnImage;
using ccipd::RescaleIntensitiesMRI;
using ccipd::GenerateActorFromLandmarks;
using ccipd::UpdateVTKImageWindowLevel;
using ccipd::vtkRenderWindowPointer;
using ccipd::vtkRendererPointer;
using ccipd::vtkRenderWindowInteractorPointer;
using ccipd::vtkPropPointer;
using ccipd::vtkActorPointer;
////////////////////////////////////////////////////////////////////////////////////////////////



int main( int argc, char * argv[] )
{

  if ( argc == 1 )
    {
    cout << "Usage: " << argv[ 0 ] << " VolumeImage" << endl;
    return EXIT_SUCCESS;
    }

  // load a file
  const string volumeFileName = argv[ 1 ];

  cout << "Loading " << volumeFileName << ":" << endl;

  const bool ignoreOrientation = true;
  const VolumeImageConstPointer volume( LoadVolume(
    volumeFileName, ignoreOrientation ) );

  if ( !volume )
    {
    cerr << "Error: unable to load " << volumeFileName << endl;
    return EXIT_FAILURE;
    }

  cout << "Loading " << volumeFileName << " done." << endl;

  // QT Stuff
  QApplication app( argc, argv );
 
  // pick points
  cout << "Picking points:" << endl;
  const LandmarkSetConstPointer landmarks(
    PickPointsOnImage( volume ) );

  // the verbose variable will also display
  // the final points once you are done picking
  if ( !landmarks )
    {
    cerr << "Picking points failed." << endl;
    return EXIT_FAILURE;
    }
  cout << "Picking points done." << endl;
  
  // create a bounding box from this points
  cout << "Creating bounding box:" << endl;
  const VolumeRegionType boundingBox =
    CreateBoundingBox( *landmarks, *volume );
  cout << "Creating bounding box done." << endl;

  // create an actor from the bounding box
  cout << "Rendering bounding box:" << endl;
  const vtkActorPointer boundingBoxActor =
    GenerateVTKActorFromBoundingBox( boundingBox, volume.GetPointer() );
  cout << "Rendering bounding box done." << endl;

  //Create a renderer, render window, and interactor
  const vtkRenderWindowPointer renderWindow = vtkRenderWindowPointer::New();
  const vtkRendererPointer     renderer     = vtkRendererPointer    ::New();
  renderWindow->AddRenderer( renderer );

  // add in the image
  srand( static_cast< unsigned int >( time( NULL ) ) );
  const unsigned int
    totalSlices = static_cast< unsigned int >(
      volume->GetBufferedRegion().GetSize()[ 2 ] / 2 ),
    slice = rand() % totalSlices;
    
  cout << "Slice = " << slice << endl;

  const vtkPropPointer imageProp =
    CreatePropFromImage(
      RescaleIntensitiesMRI( volume, 0, 1 ).GetPointer(), slice );
  UpdateVTKImageWindowLevel( imageProp, 0.5, 0.5 );
  
  // render the actors
  renderer->AddActor( boundingBoxActor );
  renderer->AddActor( imageProp );
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

  // exit
  return EXIT_SUCCESS;

} // main

