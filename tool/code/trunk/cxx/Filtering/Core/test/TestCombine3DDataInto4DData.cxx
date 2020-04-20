//////////////////////////////////////////////////////////////////////////////////////////
// ccipd includes
#include "ccipdSaveImage.h"      // for saving results
#include "ccipdLoadVolume.h"     // for loading images
#include "ccipdVolumeTypes.h"    // for image types

// std includes
#include <iostream>             // for cout
#include <string>               // for filenames
//////////////////////////////////////////////////////////////////////////////////////////

// itk includes
#include "ccipdDisableWarningsMacro.h"
  #include <itkImage.h>   // for the concatenated result
  #include <itkJoinSeriesImageFilter.h>   // for the concatenated result
  #include <itkComposeImageFilter.h>
#include "ccipdEnableWarningsMacro.h"

//////////////////////////////////////////////////////////////////////////////////////////
using std::cout;
using std::cerr;
using std::endl;
using std::string; 

//////////////////////////////////////////////////////////////////////////////////////////



int main( int argc, char * argv[] )
{

  if ( argc < 3 )
  {
    cout << endl << "Usage:" << 
      argv[ 0 ] << " inIm1 inIm2 ... outIm" << endl;
    cout << endl << "!!!!Assumes that the volumes are already projected onto the same space!" << endl;

    return EXIT_SUCCESS;
  }

   typedef itk::JoinSeriesImageFilter< 
    ccipd::VolumeImageType, 
    ccipd::Volume4DType > JoinSeriesImageFilterType;

  JoinSeriesImageFilterType::Pointer JSIFilter =  JoinSeriesImageFilterType::New();
  ccipd::VolumeImagePointer vol;

  for (int i = 1; i< argc - 1; i++ )
  {
    std::cout << "Reading file " << i << " " << argv[ i ] << std::endl;  
    vol  = ccipd::LoadVolume( argv[ i ] );
    JSIFilter->SetInput( i-1, vol );
  }

  const string
    outputFileName( argv[ argc-1 ] );

  ccipd::Volume4DPointer outputVol = JSIFilter->GetOutput(); 

  // save the result
  cout << "Saving extraction result:" << endl;
  const bool savedSuccessfully =
    ccipd::SaveImage( outputVol.GetPointer(), outputFileName, true );
  if ( !savedSuccessfully )
    {
    std::cerr << "Saving extraction result failed." << endl;
    return EXIT_FAILURE;
    }
  cout << "Saving extraction result done." << endl;

  return EXIT_SUCCESS;

} // main
