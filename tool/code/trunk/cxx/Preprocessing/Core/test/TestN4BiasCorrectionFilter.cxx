


//////////////////////////////////////////////////////////////////////////////////////////
// ccipd includes
#include "ccipdMRITypes.h"
#include "ccipdLoadMRI.h"
#include "ccipdLoadVolume.h"
#include "ccipdSaveImage.h"
#include "ccipdN4MRIBiasCorrection.h"

// boost includes
#include <boost/lexical_cast.hpp>

// std includes
#include <string>
#include <iostream>

#include "ccipdDisableWarningsMacro.h"
  // itk includes
  #include <itkImage.h>
#include "ccipdEnableWarningsMacro.h"
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
// "using" shortcuts
using std::cout;
using std::endl;
using std::cerr;
using std::string;
using boost::lexical_cast;
//////////////////////////////////////////////////////////////////////////////////////////



int main( int argc, char *argv[] )
{


  //////////////////////////////////////////////////////////////////////////////////////////
  // initialize parameters
  ccipd::InternalPixelType
    addValue = 10;
  unsigned int
    resolutions             = 1,
    iterationsPerResolution = 50,
    histogramBins           = 200;
  //////////////////////////////////////////////////////////////////////////////////////////



  //////////////////////////////////////////////////////////////////////////////////////////
  if ( argc == 1 )
    {
    cout << endl << "Usage:" << endl <<
      argv[ 0 ] << " [inputFileOrDicomFolder = ./] " <<
      "[outputFile = biascorrected.mha] " <<
      "[mask = ] " << 
      "[addValue = 10] " <<
      "[resolutions = 1] " <<
      "[iterationsPerResolution = 50] " <<
      "[histogramBins = 200] " << endl << endl;
    return EXIT_SUCCESS;
    } // 1 argument
  //////////////////////////////////////////////////////////////////////////////////////////



  //////////////////////////////////////////////////////////////////////////////////////////
  /*
   * Which files will we use?
   *
   * If you've input at least one argument, use that as the prefix.
   * Otherwise, just use the current exe location.
   * Note that argv[0] just contains the name of the exe, not the actual first argument.
   *
   * In Visual Studio, right click on "BiasCorrectionFilter" and go to "Properties"
   * Click "Debugging" --> "Command Arguments"
   * Type in the location on your computer which contains "input.png"
   * You must surround your prefix with quotation marks, like "C:\MyFolder" (including quotes).
   *
   * The optional second input is the name of the file to use.
   * So your command arguments may look like: "C:\MyFolder" "input.dcm".
   * The default input file is "input.png"
   */
  const string
    inputFile  = argc > 1 ? argv[ 1 ] : "./",
    outputFile = argc > 2 ? argv[ 2 ] : "biascorrected.mha",
    maskFile   = argc > 3 ? argv[ 3 ] : "";

  cout << "Loading MRI image " << inputFile << ":" << endl;

  const ccipd::MRIImageConstPointer inputImage(
    ccipd::LoadMRI( inputFile ) );

  if ( !inputImage )
    {
    cerr << "Unable to read " << inputFile << endl;
    return EXIT_FAILURE;
    }

  cout << "Loading MRI image done." << endl;

  ccipd::MRIMaskImageConstPointer mask;

  if ( !maskFile.empty() )
    {
    cout << "Loading mask image " << maskFile << ":" << endl;

    mask = ccipd::LoadVolumeMask( maskFile ).GetPointer();
    
    if ( !mask )
      {
      cerr << "Unable to read " << maskFile << endl <<
        "No mask will be used for the N4 algorithm." << endl;
      }
    else
      {
      cout << "Loading mask image " << maskFile << " done." << endl;
      }

    } // empty mask file

  //////////////////////////////////////////////////////////////////////////////////////////



  //////////////////////////////////////////////////////////////////////////////////////////
  // try to load parameters from the command line

  if ( argc > 4 )
    {
    try
      {
      addValue = lexical_cast<
       ccipd::InternalPixelType >( argv[ 4 ] );
      }
    catch( ... ) {}
    }

  if ( argc > 5 ) 
    {
    try
      {
      resolutions = lexical_cast< unsigned int >( argv[ 5 ] );
      }
    catch( ... ) {}
    }

  if ( argc > 6 ) 
    {
    try
      {
      iterationsPerResolution =
        lexical_cast< unsigned int >( argv[ 6 ] );
      }
    catch( ... ) {}
    }

  if ( argc > 7 )
    {
    try
      {
      histogramBins = lexical_cast< unsigned int >( argv[ 7 ] );
      }
    catch( ... ) {}
    }
  //////////////////////////////////////////////////////////////////////////////////////////



  //////////////////////////////////////////////////////////////////////////////////////////
  // use the bias correction filter
  cout << "Correcting bias field with N4:" << endl;

  const bool verbose = true;

  const ccipd::MRIImageConstPointer correctedImage(
    ccipd::N4CorrectBiasField(
      inputImage,
      mask,
      addValue,
      resolutions,
      iterationsPerResolution,
      histogramBins,
      verbose ) );

  if ( !correctedImage )
    {
    cerr << "N4 bias correction failed." << endl;
    return EXIT_FAILURE;
    }

  cout << "Correcting bias field done." << endl;
  //////////////////////////////////////////////////////////////////////////////////////////



  //////////////////////////////////////////////////////////////////////////////////////////
  cout << "Saving:" << endl;
  
  const bool saved = ccipd::SaveImage( correctedImage.GetPointer(), outputFile, true );

  if ( !saved )
    {
    cerr << "Unable to save " << outputFile << endl;
    return EXIT_FAILURE;
    }

  cout << "Saving done." << endl;
  //////////////////////////////////////////////////////////////////////////////////////////

  
  return EXIT_SUCCESS;

} // main
