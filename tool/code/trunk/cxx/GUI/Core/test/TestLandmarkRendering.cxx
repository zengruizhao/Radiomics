


////////////////////////////////////////////////////////////////////////////////////////////////////
// ccipd includes
#include "ccipdLandmarksToMesh.h"           // for converting landmarks to a mesh type
#include "ccipdITKLandmarksToVTKPolyData.h" // for converting to a poly type
#include "ccipdLoadVolume.h"                // for loading a mask
#include "ccipdMaskToLandmarks.h"           // for extracting landmarks

// std includes
#include <cstdlib>
#include <string>

#include "ccipdDisableWarningsMacro.h"
  // vtk includes
  #include <vtkVersion.h>
  #include <vtkSmartPointer.h>
  #include <vtkPolyData.h>
  #include <vtkPolyDataMapper.h>
  #include <vtkActor.h> 
  #include <vtkRenderer.h>
  #include <vtkRenderWindow.h>
  #include <vtkRenderWindowInteractor.h>

  // itk includes
  #include <itkMesh.h>
  #include <itkImage.h>
  #include <itkPointSet.h>
#include "ccipdEnableWarningsMacro.h"
////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////
using std::string;
using std::cout;
using std::endl;
using std::cerr;
////////////////////////////////////////////////////////////////////////////////////////////////////



int main( int argc, char * argv[] )
{



  //////////////////////////////////////////////////////////////////////////////////////////////////
  // get a mask
  if ( argc == 1 )
    {
    cout << "Usage: " << argv[ 0 ] << " maskFile" << endl;
    return EXIT_SUCCESS;
    }

  // load the mask
  const string maskFile = argv[ 1 ];
  cout << "Loading mask from " << maskFile << ":" << endl;

  ccipd::VolumeMaskImagePointer mask = 
    ccipd::LoadVolumeMask( maskFile );

  if ( !mask )
    {
    cerr << "Error: no mask loaded." << endl;
    return EXIT_FAILURE;
    }

  cout << "Loading mask from " << maskFile << " done." << endl;
  //////////////////////////////////////////////////////////////////////////////////////////////////



  //////////////////////////////////////////////////////////////////////////////////////////////////
  // get my parameters
  const std::size_t
    numberOfBorderLandmarks   = 1000,
    numberOfInternalLandmarks = 1000;
  const float
    borderWidth = 1;
  //////////////////////////////////////////////////////////////////////////////////////////////////



  //////////////////////////////////////////////////////////////////////////////////////////////////
  // get some landmarks

  cout << "Extracting landmarks from mask:" << endl;

  const ccipd::LandmarkSetConstPointer landmarks = 
    ccipd::ExtractLandmarkSetFromMask(
      mask,
      numberOfBorderLandmarks,
      numberOfInternalLandmarks,
      borderWidth ).GetPointer();

  cout << "Extracting landmarks from mask done." << endl;

  //////////////////////////////////////////////////////////////////////////////////////////////////



  //////////////////////////////////////////////////////////////////////////////////////////////////
  // create a vtk poly data from the landmarks
  cout << "Computing poly data:" << endl;
  const ccipd::vtkPolyDataPointer polyData = ccipd::ConvertLandmarksToPolyData( landmarks );
  cout << "Computing poly data done." << endl;
  //////////////////////////////////////////////////////////////////////////////////////////////////



  //////////////////////////////////////////////////////////////////////////////////////////////////
  // somewhat based on http://bit.ly/HLuDgp
  cout << "Rendering:" << endl;

  //Create a mapper and actor
  ccipd::vtkPolyDataMapperPointer polyMapper = ccipd::vtkPolyDataMapperPointer::New();
  polyMapper->
#if VTK_MAJOR_VERSION >= 6
      SetInputData
#else
      SetInput
#endif
        ( polyData );
 
  ccipd::vtkActorPointer actor = ccipd::vtkActorPointer::New();
  actor->SetMapper( polyMapper );
  
  //Create a renderer, render window, and interactor
  ccipd::vtkRendererPointer renderer = ccipd::vtkRendererPointer::New();

  ccipd::vtkRenderWindowPointer renderWindow = ccipd::vtkRenderWindowPointer::New();
  renderWindow->AddRenderer(renderer);

  ccipd::vtkRenderWindowInteractorPointer renderWindowInteractor =
    ccipd::vtkRenderWindowInteractorPointer::New();
  renderWindowInteractor->SetRenderWindow(renderWindow);
 
  //Add the actor to the scene
  renderer->AddActor(actor);

  // create a background color
  const double
    backgroundR = 0.3,
    backgroundG = 0,
    backgroundB = 0;
  renderer->SetBackground( backgroundR, backgroundG, backgroundB );
 
  //Render and interact
  renderWindow->Render();
  cout << "Rendering done." << endl;

  renderWindowInteractor->Start();

  //////////////////////////////////////////////////////////////////////////////////////////////////


  
  return EXIT_SUCCESS;



} // main
