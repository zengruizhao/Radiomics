


//////////////////////////////////////////////////////////////////////////////////////////
// ccipd includes
#include "ccipdLoadMRI.h"
#include "ccipdPreprocessMRI.h"
#include "ccipdSaveImage.h"

// std includes
#include <string>

#include "ccipdDisableWarningsMacro.h"
  // itk includes
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

  // The first input is a dicom directory.
  // The (optional) second input is an output directory.
  if ( argc < 2 )
    {
    cerr << "Need to input a dicom directory." << endl;
    return EXIT_FAILURE;
    }
  const string
    inputFolder = argv[1],
    outputFolder = argc > 2 ? argv[2] : ".";

  // Load the MRI volume
  cout << "Loading image:" << endl;
  const ccipd::MRIImageConstPointer
    inputImage( ccipd::LoadMRI( inputFolder ) );
  cout << "Loading image done." << endl;

  // Preprocess it
  cout << "Preprocessing image:" << endl;
  const float biasSigma = 8;
  const ccipd::MRIImageConstPointer outputImage(
      ccipd::PreprocessMRI( inputImage, biasSigma ) );
  cout << "Preprocessing image done." << endl;

  // Save it
  cout << "Saving output:" << endl;
  const bool saved = ccipd::SaveImage(
    outputImage.GetPointer(), outputFolder + "/preprocessed.mha", true );

  if ( saved )
    {
    cout << "Saving output done." << endl;
    }
  else
    {
    cout << "Saving output failed." << endl;
    return EXIT_FAILURE;
    }

  // Exit
  return EXIT_SUCCESS;

} // main
