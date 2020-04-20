


//////////////////////////////////////////////////////////////////////////////////////////
// ccipd includes
#include "ccipdCore.h"
#include "ccipdSaveImage.h"      // for saving results
#include "ccipdLoadVolume.h"     // for loading images
#include "ccipdVolumeTypes.h"    // for image types
#include "ccipdExtractMedialAxis.h"      // for extract medial axis
#include "ccipdConstVolume.h"    // for image types

// std includes
#include <iostream>             // for cout
#include <string>               // for filenames
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
using std::cout;
using std::endl;
using std::cerr;
using std::string;
//////////////////////////////////////////////////////////////////////////////////////////



int main( int argc, char * argv[] )
{

  if ( argc == 1 )
    {
    cout << endl << "Usage:" << 
      argv[ 0 ] << " fileName [outputVolume = axis.mha] [outputLandmark = axis.vtk] " <<
		 "[sigma = 0.5] [threshold = 0.0]" << 	endl;
    return EXIT_SUCCESS;
    }

  // extract the inputs
  const string
    imageFileName( argv[ 1 ] ),
    outputFileName = ( argc > 2 ? string( argv[ 2 ] ) : "./axis.mha" ),
    outputLandmarkFileName = ( argc > 3 ? string( argv[ 3 ] ) : "./axis.vtk" );

 const float
    sigma			= ( argc > 4 ? strtod( argv[ 4 ], NULL ) : 0.5 ),
    threshold = ( argc > 5 ? strtod( argv[ 5 ], NULL ) : 0.0 );


  // load the image
  cout << "Loading " << imageFileName << ":" <<endl;
  using ccipd::VolumeImageConstPointer;
  const VolumeImageConstPointer volume = 
    ccipd::ConvertVolumeImageToConst(
      ccipd::LoadVolume( imageFileName ) );

  if ( !volume )
    {
    cerr << "Loading " << imageFileName << " failed." <<endl;
    return EXIT_FAILURE;
    }
  cout << "Loading " << imageFileName << " done." <<endl;

	auto 	axisVol = ccipd::ExtractMedialAxis( volume, sigma, threshold );
	auto 	axis = ccipd::ExtractMedialAxisIntoLandmark( volume, sigma, threshold );

  // save the result
  cout << "Savine Landmark:" << endl;
  bool savedSuccessfully =
    ccipd::SaveLandmarks( static_cast< ccipd::LandmarkSetConstPointer >(axis), outputLandmarkFileName, true );
  if ( !savedSuccessfully )
    {
    cerr << "Saving medial axis failed." << endl;
    return EXIT_FAILURE;
    }
  cout << "Saving medial axis done." << endl;

 	cout << "Savine volume:" << endl;
  savedSuccessfully =
    ccipd::SaveImage( axisVol, outputFileName, true );
  if ( !savedSuccessfully )
    {
    cerr << "Saving medial axis failed." << endl;
    return EXIT_FAILURE;
    }
  cout << "Saving medial axis done." << endl;

  return EXIT_SUCCESS;

} // main
