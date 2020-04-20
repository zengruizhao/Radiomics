


////////////////////////////////////////////////////////////////////////////////////////////////////

// ccipd includes
#include "ccipdLoadVolume.h"        // loading our image
#include "ccipdPreprocessMRI.h"     // bias field correction, etc.
#include "ccipdPickPointsOnImage.h" // for picking points

// std includes
#include <string>
#include <cstdlib>
#include <iostream>

// itk includes
#include "ccipdDisableWarningsMacro.h"
#include <itkImage.h>
#include <itkPointSet.h>
#include "ccipdEnableWarningsMacro.h"

////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////
using std::cout;
using std::endl;
using std::cerr;
using std::string;
using ccipd::InternalPixelType;
using ccipd::VolumeImageConstPointer;
using ccipd::LandmarkSetConstPointer;
using ccipd::PickPointsOnImage;
////////////////////////////////////////////////////////////////////////////////////////////////




int main( int argc, char * argv[] )
{

  if ( argc == 1 )
    {
    cout << "Usage: " << argv[ 0 ] << " VolumeImage" << endl;
    return EXIT_SUCCESS;
    }

  // load a file
  const string volumeFileName = argv[ 1 ];

  cout << "Loading " << volumeFileName << ":" << endl;

  const bool ignoreOrientation = true;
  VolumeImageConstPointer volume( ccipd::LoadVolume(
    volumeFileName, ignoreOrientation ) );

  if ( !volume )
    {
    cerr << "Error: unable to load " << volumeFileName << endl;
    return EXIT_FAILURE;
    }

  cout << "Loading " << volumeFileName << " done." << endl;

#ifdef NDEBUG
  // preprocess
  cout<< "Preprocessing:" << endl; 

  const InternalPixelType
    biasCorrection   = 6,
    intensityMinimum = 0,
    intensityMaximum = 255;
  volume = ccipd::PreprocessMRI(
    volume,
    biasCorrection,
    intensityMinimum,
    intensityMaximum );
  cout<< "Preprocessing done." << endl; 
#endif // NDEBUG

  // now, get some points
  const bool verbose = true;
  const unsigned int slice = static_cast< unsigned int >(
    volume->GetBufferedRegion().GetSize()[ 2 ] / 2 );
  const LandmarkSetConstPointer landmarks(
    PickPointsOnImage( volume.GetPointer(), slice, verbose ) );

  // the verbose variable will also display
  // the final points once you are done picking

  // exit
  return EXIT_SUCCESS;

} // main
