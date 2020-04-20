


/*
 * This program tests normalizing the levelsets.
 * When using levelsets in terms of signed distance,s
 * this may be a vastly different range than the
 * texture features, resulting in a bad PCA result.
 * Therefore, we want to normalize them.
 * However, we still want to be able to threshold the
 * levelsets at 0 to get a mask.
 */



//////////////////////////////////////////////////////////////////////////////////////////
// ccipd includes
#include "ccipdMaskToLevelset.h"     // for converting a mask to a levelset
#include "ccipdMRITypes.h"           // for our image types
#include "ccipdResizeImage.h"        // for resizing masks

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
  const std::string defaultOutputFilename( "normalizedLevelset.mhd" );
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
  typedef itk::ImageFileReader< ccipd::MRIMaskImageType > ReaderType;
  ReaderType::Pointer maskReader = ReaderType::New();
  maskReader->SetFileName( maskFileName );

  // load the mask
  try
    {
    maskReader->Update();
    } // try
  catch ( itk::ExceptionObject & err )
    {
    cout << "failed." << endl;
    cerr << "itk::ExceptionObject caught!" << endl;
    cerr << err << endl;
    return -2;
    }
  catch ( std::exception & err )
    {
    cout << "failed." << endl;
    cerr << "std::exception caught!" << endl;
    cerr << err.what() << endl;
    return -2;
    } 
  catch ( ... )
    {
    cout << "failed." << endl;
    cerr << "Unknown exception caught.  Exiting." << endl;
    return -3;
    }

  // get the loaded mask
  cout << "done in " << (time(NULL) - startTime) << " seconds." << endl; // loading mask
  const ccipd::MRIMaskImageConstPointer mask(
      maskReader->GetOutput() );

  // calculate the normalized levelset
  startTime = time(NULL);

  const bool
    normalize = true,
    sigmoid   = false,
	resizeIsotropically = false;
  const float
    sigmoidWidth      = 4,
    smoothingVariance = 0.0;
  const ccipd::VolumeImageConstPointer normalized(
    ccipd::MaskToLevelset< ccipd::InternalPixelType >( 
      mask.GetPointer(), 
      normalize, 
      sigmoid, 
      sigmoidWidth,
      smoothingVariance,
	  resizeIsotropically	  ) );

  cout << "done in " << difftime( time(NULL), startTime) << " seconds." << endl; // normalize

  // save the output
  cout << "Saving " << outputFileName << "...";
  startTime = time(NULL);
  typedef itk::ImageFileWriter< ccipd::MRIImageType > WriterType;
  WriterType::Pointer outputWriter = WriterType::New();
  outputWriter->SetFileName( outputFileName );
  outputWriter->SetInput( normalized );

  // save the result
  try
    {
    outputWriter->Update();
    } // try
  catch ( itk::ExceptionObject & err )
    {
    cout << "failed." << endl;
    cerr << "itk::ExceptionObject caught!" << endl;
    cerr << err << endl;
    return -2;
    }
  catch ( std::exception & err )
    {
    cout << "failed." << endl;
    cerr << "std::exception caught!" << endl;
    cerr << err.what() << endl;
    return -2;
    } 
  catch ( ... )
    {
    cout << "failed." << endl;
    cerr << "Unknown exception caught.  Exiting." << endl;
    return -3;
    }

  // exit
  return 0;

} // main
