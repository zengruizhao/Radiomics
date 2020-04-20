


//////////////////////////////////////////////////////////////////////////////////////////
// ccipd includes
#include "ccipdVolumeTypes.h"
#include "ccipdLoadVolume.h"  // for reading in a mask
#include "ccipdMaskToDistanceImage.h" // for getting the surface image
#include "ccipdResizeImage.h" // for downsampling
#include "ccipdSaveImage.h"   // for saving output

// std includes
#include <cstdlib>           // for EXIT_SUCCESS
#include <iostream>          // for cout
#include <string>

#include "ccipdDisableWarningsMacro.h"
  // itk includes
  #include <itkImage.h>
#include "ccipdEnableWarningsMacro.h"
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
using std::cout;
using std::endl;
using std::cerr;
using std::string;
//////////////////////////////////////////////////////////////////////////////////////////



int main ( int argc, char * argv[] )
{

  if ( argc == 1 )
    {
    cout << endl << "Usage:";
    cout << argv[ 0 ] << " segmentationMask groundTruthMask outputMask [= resized.mha]" << endl;

    return EXIT_SUCCESS;
    } // 1 argument

  // try to load a volume
  const string segmentationInputFileName( argv[ 1 ] );
  const string groundTruthFileName (argv[ 2 ] );

  const ccipd::VolumeMaskImageConstPointer segmentationMask(
      ccipd::LoadVolumeMask( segmentationInputFileName ) );

  
  const ccipd::VolumeMaskImageConstPointer groundTruthMask(
      ccipd::LoadVolumeMask( groundTruthFileName ) );

  if ( !segmentationMask )
    {
    cerr << "Error: Unable to load " << segmentationInputFileName << endl;
    return EXIT_FAILURE;
    }

  if ( !groundTruthMask )
    {
    cerr << "Error: Unable to load " << groundTruthFileName << endl;
    return EXIT_FAILURE;
    }
  // okay, we have a volume loaded
  // now calculate the levelset
 const  ccipd::VolumeImageConstPointer  distanceImage( 
    ccipd::MaskToDistanceImage<ccipd::InternalPixelType>( 
      segmentationMask.GetPointer(), 
      groundTruthMask.GetPointer(), 
      0.1f, 
      0.0f ) );

  // try to save it
  const string outputFileName = argc > 2 ? string( argv[ 3 ] ) : "./resized.mha";
  const bool saved = ccipd::SaveImage( distanceImage.GetPointer(), outputFileName, true );

  if ( !saved )
    {
    cerr << "Unable to save output " << outputFileName << endl;
    return EXIT_FAILURE;
    }

  // all good
  return EXIT_SUCCESS;

} // main
