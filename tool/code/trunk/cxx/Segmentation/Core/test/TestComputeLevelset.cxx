


/*
 * This program computes a levelsets and counts the number of voxels on the surface
 */



//////////////////////////////////////////////////////////////////////////////////////////
// ccipd includes
#include "ccipdMaskToLevelset.h"     // for converting a mask to a levelset
#include "ccipdMRITypes.h"           // for our image types
#include "ccipdResizeImage.h"        // for resizing masks
#include "ccipdSaveImage.h"          // Save the images
#include "ccipdLoadVolume.h"          // Load images 

// std includes
#include <string>                   // for the file name
#include <ctime>                    // for timing things

#include "ccipdDisableWarningsMacro.h"
  // itk includes
  #include <itkImageFileReader.h>     // for loading a mask
  #include <itkImageFileWriter.h>     // for outputting
#include "ccipdEnableWarningsMacro.h"
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
using std::cout;
using std::endl;
using std::cerr;
//////////////////////////////////////////////////////////////////////////////////////////



int main(int argc, char * argv[])
{

  // check our inputs
  const std::string defaultOutputFilename( "Levelset.mhd" );
  if ( argc < 2 )
    {
    cerr << "At least 1 input is required." << endl;
    cout <<
      "The one required input is the " <<
      "file name of a mask.  A levelset will be " <<
      "calculated from this mask, and then normalized.  " <<
      "The normalized result will be turned into a mask " <<
      " and compared to the original mask." << endl << endl;
    cout << 
      "The second optional input is the " <<
      "output levelset file name.  " <<
      "Default = " << defaultOutputFilename << endl;
    return -1;
    }

  // prepare the output file anme
  const std::string outputFileName = 
    ( argc > 2 ) ?         // has the user input a file name?
    argv[2] :              // the user input a file name
    defaultOutputFilename; // nope, use the default

  // for timing things
  std::time_t startTime;

  // prepare to load the mask
  const std::string maskFileName(argv[1]);
  cout << "Loading mask " << maskFileName << "...";
  startTime = time(NULL);


  ccipd::VolumeMaskImagePointer mask = ccipd::LoadVolumeMask(maskFileName);

  // calculate the normalized levelset
  startTime = time(NULL);

  const bool
    normalize = false,
    sigmoid   = false,
	resizeIsotropically = false;
  const float
    sigmoidWidth      = 4,
    smoothingVariance = 0.0;
  const ccipd::VolumeImageConstPointer ls(
    ccipd::MaskToLevelset< ccipd::InternalPixelType >( 
      mask.GetPointer(), 
      normalize, 
      sigmoid, 
      sigmoidWidth,
      smoothingVariance,
	  resizeIsotropically	  ) );

  cout << "done in " << difftime( time(NULL), startTime) << " seconds." << endl; // normalize

  // save the output
  cout << "Saving " << outputFileName << "..." << endl;
  startTime = time(NULL);
  ccipd::SaveImage(ls.GetPointer(), outputFileName, false);

  // exit
  return 0;

} // main
