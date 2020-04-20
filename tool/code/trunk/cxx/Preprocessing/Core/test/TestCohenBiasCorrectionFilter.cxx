


//////////////////////////////////////////////////////////////////////////////////////////
// ccipd includes
#include "ccipdMRITypes.h"
#include "ccipdLoadMRI.h"
#include "ccipdSaveImage.h"
#include "ccipdCohenMRIBiasCorrection.h"

// std includes
#include <string>
#include <iostream>

#include "ccipdDisableWarningsMacro.h"
  //ITK includes
  #include <itkImage.h>
#include "ccipdEnableWarningsMacro.h"
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
// "using" shortcuts
using std::cout;
using std::endl;
using std::cerr;
using std::string;
//////////////////////////////////////////////////////////////////////////////////////////



int main( int argc, char *argv[] )
{


  //////////////////////////////////////////////////////////////////////////////////////////
  if ( argc == 1 )
    {
    cout << endl << "Usage:" << endl <<
      argv[ 0 ] << " [inputFileOrDicomFolder = ./]" <<
      "[outputFile = biascorrected.mha " <<
      "[sigma = 6.0]" << endl << endl;
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
    inputFile  = argc > 1 ? argv[1] : "./",
    outputFile = argc > 2 ? argv[2] : "biascorrected.mha";

  cout << "Loading MRI image " << inputFile << ":" << endl;

  const ccipd::MRIImageConstPointer inputImage(
    ccipd::LoadMRI( inputFile ) );

  if ( !inputImage )
    {
    cerr << "Unable to read " << inputFile << endl;
    return EXIT_FAILURE;
    }

  cout << "Loading MRI image done." << endl;
  //////////////////////////////////////////////////////////////////////////////////////////



  //////////////////////////////////////////////////////////////////////////////////////////
  // use the bias correction filter

  float sigma = 6.0;
  if ( argc > 3 )
    sscanf( argv[ 3 ], "%f", &sigma );

  cout << "Correcting bias field with sigma = " << sigma << ":" << endl;

  const ccipd::MRIImageConstPointer correctedImage(
    ccipd::CohenCorrectBiasField( inputImage, sigma ) );

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
