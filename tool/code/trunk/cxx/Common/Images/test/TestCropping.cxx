


//////////////////////////////////////////////////////////////////////////////////////////
// ccipd includes
#include "ccipdSaveImage.h"      // for saving results
#include "ccipdCropImage.h"      // for actually cropping
#include "ccipdLoadVolume.h"     // for loading images
#include "ccipdVolumeTypes.h"    // for image types

// std includes
#include <iostream>             // for cout
#include <string>               // for filenames

#include "ccipdDisableWarningsMacro.h"
  //ITK includes
  #include <itkImage.h>
#include "ccipdEnableWarningsMacro.h"
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
      argv[ 0 ] << " fileName [output = cropped.mha] " <<
      "[crop in x = 0] [crop in y = 0] [crop in z = 0]" << endl;
    return EXIT_SUCCESS;
    }

  // extract the inputs
  const string
    imageFileName( argv[ 1 ] ),
    croppedFileName = ( argc > 2 ? string( argv[ 2 ] ) : "./cropped.mha" );

  const double
    cropX = ( argc > 3 ? strtod( argv[ 3 ], NULL ) : 0 ),
    cropY = ( argc > 4 ? strtod( argv[ 4 ], NULL ) : 0 ),
    cropZ = ( argc > 5 ? strtod( argv[ 5 ], NULL ) : 0 );

  // load the image
  cout << "Loading " << imageFileName << ":" <<endl;
  using ccipd::VolumeImageConstPointer;
  const VolumeImageConstPointer volume(
      ccipd::LoadVolume( imageFileName ) );

  if ( !volume )
    {
    cerr << "Loading " << imageFileName << " failed." <<endl;
    return EXIT_FAILURE;
    }
  cout << "Loading " << imageFileName << " done." <<endl;

  // how much will we crop?


  // crop image
  cout << "Cropping image (" <<
    cropX << ", " << cropY << ", " << cropZ << "):" << endl;
  const VolumeImageConstPointer croppedVolume(
      ccipd::CropVolume(
        volume,
        cropX,
        cropY,
        cropZ ) );
  cout << "Cropping image done." << endl;

  // save the result
  cout << "Saving cropped result:" << endl;
  const bool savedSuccessfully =
    ccipd::SaveImage( croppedVolume.GetPointer(), croppedFileName, true );
  if ( !savedSuccessfully )
    {
    cerr << "Saving cropped result failed." << endl;
    return EXIT_FAILURE;
    }
  cout << "Saving cropped result done." << endl;

  return EXIT_SUCCESS;

} // main
