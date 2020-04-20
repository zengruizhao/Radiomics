


//////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdLoadVolume.h"
#include "ccipdPreprocessMRI.h"
#include "ccipdTextureFeatureExtractor.h"
#include "ccipdSaveImage.h"

//std includes
#include <vector>
#include <string>

#include "ccipdDisableWarningsMacro.h"
  #include <itkVectorImage.h> // our output type
  #include <itkImage.h>
#include "ccipdEnableWarningsMacro.h"
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
using std::cout;
using std::cerr;
using std::endl;
using std::string;
//////////////////////////////////////////////////////////////////////////////////////////



int main( int argc, char *argv[] )
{

  // The first input is an MRI file or folder
  // The (optional) second input is an output directory.
  if ( argc == 1 )
    {
    cout << endl << "Usage: " <<
      argv[ 0 ] << " VolumeFileOrFolder [outputFile = features.mha]" << endl;
    return EXIT_SUCCESS;
    }

  const string
    inputFolder = argv[1],
    outputFile  = argc > 2 ? argv[2] : "features.mha";

  // The first input as an MRI

  // Load the Volume volume
  cout << "Loading image:" << endl;

  const ccipd::VolumeImageConstPointer inputImage =
    ccipd::LoadConstVolume( inputFolder );
  if ( !inputImage )
    {
    cerr << "Unable to load " << inputFolder <<
      endl << "Exiting." << endl;
    return EXIT_FAILURE;
    }
  cout << "Loading image done." << endl;

  // Preprocess it
  cout << "Preprocessing image:" << endl;
  const float biasSigma = 8;
  const ccipd::VolumeImageConstPointer preprocessedImage (
      ccipd::PreprocessMRI( inputImage, biasSigma ) );
  cout << "Preprocessing image done." << endl;

  // Extract features
  cout << "Extracting features:" << endl;

  ccipd::TextureFeatureExtractor extractor;
  extractor.SetVolume( preprocessedImage );

  const ccipd::VolumeFeaturesConstPointer
    extractedFeatures( extractor.GetAllFeatures() );

  cout << "Feature extraction done." << endl;

  // Save
  ccipd::SaveImage( extractedFeatures.GetPointer(), outputFile, true );

  // Exit
  return EXIT_SUCCESS;

} // main
