


//////////////////////////////////////////////////////////////////////////////////////////

// ccipd includes
#include "ccipdCore.h"
#include "ccipdLoadVolume.h" // for loading in a mask
#include "ccipdFEMTetrahedronGenerator.h" // for generating the FEM
#include "ccipdFEMHexahedronGenerator.h"
#include "ccipdFEMToActor.h" // for generating our VTK actors from the FEM
#include "ccipdBoundingBox.h"
#include "ccipdCropImage.h"
#include "ccipdDownsampleImage.h"

#ifdef _MSC_VER
#pragma warning( push, 0 )
#pragma warning( disable: 4701 )
#pragma warning( disable: 4702 )
#endif // _MSC_VER

// std includes
#include <cstdlib>

// vtk includes
#include <vtkSmartPointer.h>     // for memory management
#include <vtkActor.h>            // something to render
#include <vtkRenderer.h>         // to render something
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>

// itk includes
#include <itkImage.h>
#include <itkFEMObject.h>
#include <itkTimeProbesCollectorBase.h>   // for timing things
#include <itkMemoryProbesCollectorBase.h> // for memory management

#ifdef _MSC_VER
#pragma warning( default: 4701 )
#pragma warning( default: 4702 )
#pragma warning( pop )
#endif // _MSC_VER

//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
using std::cout;
using std::endl;
using std::cerr;
using std::string;
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
// just define a macro for starting and stopping things
#define ccipdTestFEMRenderingStartMacro( processString ) \
  cout << processString << ":" << endl;                 \
  memoryMeter.Start( processString );                   \
  chronoMeter.Start( processString );

#define ccipdTestFEMRenderingStopMacro( processString ) \
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
  ccipdTestFEMRenderingStartMacro( "Loading mask" );

  ccipd::VolumeMaskImageConstPointer mask( 
    ccipd::LoadVolumeMask( maskFile ) );

  if ( !mask )
    {
    cerr << "Error: no mask loaded." << endl;
    return EXIT_FAILURE;
    }

  ccipdTestFEMRenderingStopMacro( "Loading mask" );
  //////////////////////////////////////////////////////////////////////////////////////////////////
  


  //////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef NDEBUG
  // crop the mask to the visible region
  ccipdTestFEMRenderingStartMacro( "Cropping mask" );
  mask = ccipd::CropImage( mask.GetPointer(), ccipd::GetBoundingBox( mask ) );
  ccipdTestFEMRenderingStopMacro( "Cropping mask" );

  /*
  ccipdTestFEMRenderingStartMacro( "Downsampling mask" );
  mask = ccipd::DownsampleImage( mask );
  ccipdTestFEMRenderingStopMacro( "Downsampling mask" );
  */

#endif // NDEBUG
  //////////////////////////////////////////////////////////////////////////////////////////////////



  //////////////////////////////////////////////////////////////////////////////////////////////////
  // generate a fem
  ccipdTestFEMRenderingStartMacro( "Mask to FEM" );

  typedef
    //ccipd::FEMTetrahedronGenerator
    ccipd::FEMHexahedronGenerator
  FEMGeneratorType;

  // create a generator
  const FEMGeneratorType::Pointer
    generatorPointer = FEMGeneratorType::New();

  FEMGeneratorType & generator = *generatorPointer;

  generator.SetVerbose( true );
  generator.SetMask( mask );

  // how many nodes to use?
  typedef FEMGeneratorType::DownsampleRateType DownsampleRateType;

  const double nodesPerDimension =
#ifdef NDEBUG
    100;
#else
    10;
#endif

  // how big is our image?
  const auto maskSize = mask->GetBufferedRegion().GetSize();

  // set the downsample rate
  generator.SetDownsampleRateX( static_cast< DownsampleRateType >(
    ceil( maskSize[ 0 ] / nodesPerDimension ) ) );
  generator.SetDownsampleRateY( static_cast< DownsampleRateType >(
    ceil( maskSize[ 1 ] / nodesPerDimension ) ) );
  generator.SetDownsampleRateZ( static_cast< DownsampleRateType >(
    ceil( maskSize[ 2 ] / nodesPerDimension ) ) );

  //generator.SetLevelsetThreshold( 0.1f );

  const ccipd::FEMObjectPointer FEM = generator.GetFEM();

  ccipdTestFEMRenderingStopMacro( "Mask to FEM" );
  //////////////////////////////////////////////////////////////////////////////////////////////////


  
  //////////////////////////////////////////////////////////////////////////////////////////////////
  // convert to vtk actors
  ccipdTestFEMRenderingStartMacro( "FEM to Actors" );

  ccipd::vtkActorPointer
    landmarksActor,
    trianglesActor,
    loadsArrowsActor,
    loadsSphereActor;
  ccipd::GenerateVTKActorsFromFEM(
    FEM,
    landmarksActor,
    trianglesActor,
    loadsArrowsActor,
    loadsSphereActor,
    0.2, 0.6, 10, true );

  ccipdTestFEMRenderingStopMacro( "FEM to Actors" );
  //////////////////////////////////////////////////////////////////////////////////////////////////



  //////////////////////////////////////////////////////////////////////////////////////////////////
  // somewhat based on http://bit.ly/HLuDgp
  ccipdTestFEMRenderingStartMacro( "Rendering" );

  //Create a renderer, render window, and interactor
  ccipd::vtkRendererPointer renderer = ccipd::vtkRendererPointer::New();

  ccipd::vtkRenderWindowPointer renderWindow = ccipd::vtkRenderWindowPointer::New();
  renderWindow->AddRenderer(renderer);

  ccipd::vtkRenderWindowInteractorPointer renderWindowInteractor =
    ccipd::vtkRenderWindowInteractorPointer::New();
  renderWindowInteractor->SetRenderWindow(renderWindow);
 
  // add the actors to the scene
  renderer->AddActor( landmarksActor );
  renderer->AddActor( trianglesActor );

  // create a background color
  const double
    backgroundR = 0.1,
    backgroundG = 0,
    backgroundB = 0.2;
  renderer->SetBackground( backgroundR, backgroundG, backgroundB );
  
  //Render and interact
  renderWindow->Render();

  ccipdTestFEMRenderingStopMacro( "Rendering" );
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
