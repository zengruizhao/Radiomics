


/**
 * This program will train an MFA with small,
 * random images.
 *
 * It will then attempt to save itself to a folder,
 * and finally reload itself.
 */



//////////////////////////////////////////////////////////////////////////////////////////
// ccipd includes
#include "ccipdVolumeTypes.h"
#include "ccipdSIMEstimator.h"

// std includes
#include <iostream>

#include "ccipdDisableWarningsMacro.h"
  // itk includes
  #include <itkRandomImageSource.h>
#include "ccipdEnableWarningsMacro.h"
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
using std::cout;
using std::endl;
using std::cerr;
using std::string;
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
namespace ccipd
{

// http://www.itk.org/Wiki/ITK/Examples/SimpleOperations/RandomImageSource
VolumeImageConstPointer CreateRandomImage()
{

  // set the size
  const unsigned int imageSize = 10; // should be very small to run fast
  itk::Size< VolumeDimension > size;
  size.Fill( imageSize );

  // prepare a random image generator
  typedef itk::RandomImageSource< ccipd::VolumeImageType > RandomImageType;
  RandomImageType::Pointer randomImageSource = RandomImageType::New();
  randomImageSource->SetSize( size );

  // create a random image
  randomImageSource->Update();
  return randomImageSource->GetOutput();

} // CreateRandomImage
} // namespace
//////////////////////////////////////////////////////////////////////////////////////////



int main( int argc, char *argv[] )
{

  // get our directory
  if ( argc < 2 )
    {
    cerr << "Please input a directory to save the MFA." << endl;
    return EXIT_FAILURE;
    }

  const string directory( argv[1] );

  /*

  // how much test data will we create?
  const unsigned int
    numberOfLevelsets      = 2,
    numberOfTextures       = 3,
    numberOfTrainingImages = 4,
    numberOfComponents     = numberOfTrainingImages,
    imageSize              = 10; // pixels

  // prepare our MFA trainer
  typedef itk::MFAEstimator< ccipd::VolumeImageType > MFAType;
  MFAType::Pointer MFA = MFAType::New();
  MFA->SetNumberOfLevelsets                  ( numberOfLevelsets      );
  MFA->SetNumberOfTextures                   ( numberOfTextures       );
  MFA->SetNumberOfPrincipalComponentsRequired( numberOfComponents     );
  MFA->SetNumberOfTrainingImages             ( numberOfTrainingImages );

  // create some images to train an MFA
  cout << "Creating some random images...";

  // go through each artificial training image
  for ( unsigned int trainingIndex = 0;
    trainingIndex < numberOfTrainingImages;
    ++trainingIndex )
    {

    // create a random levelset
    for ( unsigned int levelsetIndex = 0;
      levelsetIndex < numberOfLevelsets;
      ++levelsetIndex )
        {
        MFA->SetInputLevelset( trainingIndex, ccipd::CreateRandomImage(), levelsetIndex );
        } // levelsetIndex

    // create a random texture
    for ( unsigned int textureIndex = 0;
      textureIndex < numberOfTextures;
      ++textureIndex )
        {
        MFA->SetInputTexture( trainingIndex, ccipd::CreateRandomImage(), textureIndex );
        } // textureIndex

    } // trainingIndex

  cout << "done." << endl; // random images

  // train the MFA
  cout << "Training MFA...";
  MFA->VerboseOff();
  MFA->Update();
  cout << "done." << endl;

  // save the trained MFA
  cout << "Saving MFA...";
  const bool saved = MFA->SaveToDisk( directory );
  if ( !saved )
    {
    cout << "failed." << endl;
    return EXIT_FAILURE;
    }
  cout << "done." << endl;

  // reload the saved MFA
  cout << "Loading saved MFA...";
  MFA = MFAType::New();
  const bool loaded = MFA->LoadFromDisk( directory );
  if ( !loaded )
    {
    cout << "failed." << endl;
    return EXIT_FAILURE;
    }
  cout << "done." << endl;
  */

  return EXIT_SUCCESS;

} // main
