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

using ccipd::VolumeMaskImageConstPointer;
using ccipd::VolumeImagePointer;
//////////////////////////////////////////////////////////////////////////////////////////



#define epsilon 1e-6; 

int main( int argc, char * argv[] )
{

  if ( argc < 4 )
  {
    cout << endl << "Usage:" << 
      argv[ 0 ] << " MultiLabelFileName outputFileName Label" << endl;
    return EXIT_SUCCESS;
  }

  // extract the inputs
  const string
    imageFileName( argv[ 1 ] ),
    outputFileNamePrefix ( argv[ 2 ] );
 
  const int 
    inLabel  = argc > 3 ? static_cast< int >( strtod( argv[ 3 ], NULL ) ) : 1,
    outLabel = 1;
 
  vector<int> inLabels;
  inLabels.push_back(inLabel);

  // load the image
  cout << "Loading " << imageFileName << ":" <<endl;
  VolumeImagePointer MultiLabelVol  = ccipd::LoadVolume( imageFileName ) ;

  VolumeMaskImageConstPointer SingleLabelVol = ccipd::ExtractLabels(MultiLabelVol, inLabels, outLabel);

  // save the result
  cout << "Saving extraction result:" << endl;
  const bool savedSuccessfully =
    ccipd::SaveImage( SingleLabelVol.GetPointer(), outputFileNamePrefix, true );
  if ( !savedSuccessfully )
    {
    std::cerr << "Saving extraction result failed." << endl;
    return EXIT_FAILURE;
    }
  cout << "Saving extraction result done." << endl;

  return EXIT_SUCCESS;

} // main
