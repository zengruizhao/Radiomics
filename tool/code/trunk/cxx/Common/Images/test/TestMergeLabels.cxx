//////////////////////////////////////////////////////////////////////////////////////////
// ccipd includes
#include "ccipdCore.h"
#include "ccipdSaveImage.h"      // for saving results
#include "ccipdLoadVolume.h"     // for loading images
#include "ccipdVolumeTypes.h"    // for image types
#include "ccipdProcessLabels.h"

// std includes
#include <iostream>             // for cout
#include <string>               // for filenames
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
using std::cout;
using std::cerr;
using std::endl;
using std::vector;
using std::string; 

using ccipd::VolumeMaskImageType;
using ccipd::VolumeMaskImageConstPointer;
using ccipd::VolumeImagePointer;
//////////////////////////////////////////////////////////////////////////////////////////



int main( int argc, char * argv[] )
{

  if ( argc < 6 )
  {
    cout << endl << "Usage:" << 
      argv[ 0 ] << " inputFile1 outValue1 inputFile2 outputValue2 outFile " <<
      endl << endl;
    return EXIT_SUCCESS;
  }

  // extract the inputs
  const string
    imageFileName1( argv[ 1 ] ),
    imageFileName2( argv[ 3 ] ),
    outputFileName( argv[ 5 ] );
 
  const int 
    outLabel1 = atoi( argv[ 2 ]),
    outLabel2 = atoi( argv[ 4 ]);
 
  // load the image
  cout << "Loading File 1: " << imageFileName1 << ":" << endl;
  const VolumeMaskImageConstPointer vol1(
    ccipd::LoadVolumeMask( imageFileName1 ) );
  cout << "Loading File 2: " << imageFileName2 << ":" << endl;
  const VolumeMaskImageConstPointer vol2(
    ccipd::LoadVolumeMask( imageFileName2 ) );
  
  if ( !vol1 || !vol2 )
    {
    cerr << "Unable to load volumes!" << endl;
    return EXIT_FAILURE;
    }

  const VolumeMaskImageConstPointer mergedVol(
    ccipd::MergeLabels(vol1, vol2, outLabel1, outLabel2 ) );

  if ( !mergedVol )
    {
    cerr << "Unable to merge volumes!" << endl;
    return EXIT_FAILURE;
    }

  // save the result
  cout << "Saving extraction result:" << endl;
  const bool savedSuccessfully =
    ccipd::SaveImage( mergedVol.GetPointer(), outputFileName, true );
  if ( !savedSuccessfully )
    {
    std::cerr << "Saving extraction result failed." << endl;
    return EXIT_FAILURE;
    }
  cout << "Saving extraction result done." << endl;

  return EXIT_SUCCESS;

} // main
