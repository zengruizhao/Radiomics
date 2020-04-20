


//////////////////////////////////////////////////////////////////////////////////////////
// ccipd includes
#include "ccipdCore.h"
#include "ccipdLoadVolume.h"          // for loading in a mask
#include "ccipdResizeImage.h"         // for making it isotropic
#include "ccipdMaskToLevelset.h"      // for extracting a levelset
#include "ccipdIsosurfaceRendering.h" // for creating actors

// std includes
#include <cstdlib>

#include "ccipdDisableWarningsMacro.h"
  // vtk includes
  #include <vtkSmartPointer.h>     // for memory management
  #include <vtkActor.h>            // something to render
  #include <vtkProperty.h>
  #include <vtkRenderer.h>         // to render something
  #include <vtkRenderWindow.h>
  #include <vtkRenderWindowInteractor.h>

  // itk includes
  #include <itkTimeProbesCollectorBase.h>   // for timing things
  #include <itkMemoryProbesCollectorBase.h> // for memory management
  #include <itkImage.h>
#include "ccipdEnableWarningsMacro.h"
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
using std::cout;
using std::endl;
using std::cerr;
using std::string;
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
// just define a macro for starting and stopping things
#define ccipdTestLevelsetRenderingStartMacro( processString ) \
  cout << processString << ":" << endl;                 \
  memoryMeter.Start( processString );                   \
  chronoMeter.Start( processString );

#define ccipdTestLevelsetRenderingStopMacro( processString ) \
  memoryMeter.Stop( processString );                   \
  chronoMeter.Stop( processString );                   \
  cout << processString << " done." << endl;
//////////////////////////////////////////////////////////////////////////////////////////



int main( int argc, char * argv[] )
{



  //////////////////////////////////////////////////////////////////////////////////////////////////
  itk::TimeProbesCollectorBase   chronoMeter;
  itk::MemoryProbesCollectorBase memoryMeter;
  //////////////////////////////////////////////////////////////////////////////////////////////////


  
  //////////////////////////////////////////////////////////////////////////////////////////////////
  // get a mask
  if ( argc == 1 )
    {
    cout << "Usage: " << argv[ 0 ] << " maskFile" << endl;
    return EXIT_SUCCESS;
    }

  // load the mask
  const string maskFile = argv[ 1 ];
  ccipdTestLevelsetRenderingStartMacro( "Loading mask" );

  ccipd::VolumeMaskImageConstPointer mask = 
    ccipd::LoadVolumeMask( maskFile ).GetPointer();

  if ( !mask )
    {
    cerr << "Error: no mask loaded." << endl;
    return EXIT_FAILURE;
    }

  ccipdTestLevelsetRenderingStopMacro( "Loading mask" );
  //////////////////////////////////////////////////////////////////////////////////////////////////
  


  //////////////////////////////////////////////////////////////////////////////////////////////////
  // generate a levelset
  ccipdTestLevelsetRenderingStartMacro( "Isotropic" );

  mask = ccipd::ResizeMaskIsotropically( mask ).GetPointer();

  ccipdTestLevelsetRenderingStopMacro( "Isotropic" );
  //////////////////////////////////////////////////////////////////////////////////////////////////



  //////////////////////////////////////////////////////////////////////////////////////////////////
  // generate a levelset
  ccipdTestLevelsetRenderingStartMacro( "Mask to Levelset" );

  const bool
    normalize = false,
    sigmoid   = false,
	resizeIsotropically = false;
  const float
    smooth = 1.0; // this will make it smaller, but look better

  const ccipd::VolumeImageConstPointer levelset =
    ccipd::MaskToLevelset< ccipd::InternalPixelType >(
	mask.GetPointer(), normalize, sigmoid, 0, smooth, resizeIsotropically).GetPointer();

  ccipdTestLevelsetRenderingStopMacro( "Mask to Levelset" );
  //////////////////////////////////////////////////////////////////////////////////////////////////



  //////////////////////////////////////////////////////////////////////////////////////////////////
  // convert to vtk actors
  ccipdTestLevelsetRenderingStartMacro( "Levelset to Actor" );

  const ccipd::InternalPixelType isosurfaceValue = 0;
  const ccipd::vtkActorPointer levelsetActor =
    ccipd::GenerateVTKActorFromIsosurface( levelset, isosurfaceValue );

  ccipdTestLevelsetRenderingStopMacro( "Levelset to Actor" );
  //////////////////////////////////////////////////////////////////////////////////////////////////



  //////////////////////////////////////////////////////////////////////////////////////////////////
  // somewhat based on http://bit.ly/HLuDgp
  ccipdTestLevelsetRenderingStartMacro( "Rendering" );

  //Create a renderer, render window, and interactor
  ccipd::vtkRendererPointer renderer = ccipd::vtkRendererPointer::New();

  ccipd::vtkRenderWindowPointer renderWindow = ccipd::vtkRenderWindowPointer::New();
  renderWindow->AddRenderer(renderer);

  ccipd::vtkRenderWindowInteractorPointer renderWindowInteractor =
    ccipd::vtkRenderWindowInteractorPointer::New();
  renderWindowInteractor->SetRenderWindow(renderWindow);
 
  // add the actors to the scene
  renderer->AddActor( levelsetActor );

  // create a background color
  const double
    backgroundR = 0.1,
    backgroundG = 0.1,
    backgroundB = 0.1;
  renderer->SetBackground( backgroundR, backgroundG, backgroundB );
  
  //Render and interact
  renderWindow->Render();

  ccipdTestLevelsetRenderingStopMacro( "Rendering" );
  //////////////////////////////////////////////////////////////////////////////////////////////////



  //////////////////////////////////////////////////////////////////////////////////////////////////
  // display how long things took
  cout << endl << "Time information:" << endl;
  chronoMeter.Report( cout );
  cout << endl << "Memory information:" << endl;
  memoryMeter.Report( cout );
  cout << endl;
  //////////////////////////////////////////////////////////////////////////////////////////////////



  //////////////////////////////////////////////////////////////////////////////////////////////////
  // show the rendered result
  renderWindowInteractor->Start();
  return EXIT_SUCCESS;
  //////////////////////////////////////////////////////////////////////////////////////////////////



} // main
