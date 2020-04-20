


//////////////////////////////////////////////////////////////////////////////////////////
// ccipd includes
#include "ccipdLevelsets.h"
#include "ccipdMaskToLevelset.h"
#include "ccipdVolumeTypes.h"

// std includes
#include <iostream>
#include <string>

#include "ccipdDisableWarningsMacro.h"
  // itk includes
  #include <itkImageFileReader.h> // for loading masks
  #include <itkImageFileWriter.h>
#include "ccipdEnableWarningsMacro.h"
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
using std::cout;
using std::endl;
using std::string;
//////////////////////////////////////////////////////////////////////////////////////////



int main( int argc, char * argv[] )
{

  if ( argc != 4 )
    {
    cout << "Usage: " << argv[ 0 ] <<
      " inputMaskFile enlargeInMM outputMaskFile "
      << endl << endl;

    return EXIT_FAILURE;
    }

  const string
    inputMaskFile  = argv[ 1 ],
    outputMaskFile = argv[ 3 ];

  cout << "Loading mask " << inputMaskFile << ":" << endl;

  // load our mask
  typedef itk::ImageFileReader< ccipd::VolumeMaskImageType > MaskReaderType;
  auto maskReader = MaskReaderType::New();
  maskReader->SetFileName( inputMaskFile );
  try
    {
    maskReader->Update();
    }
  catch( itk::ExceptionObject & e )
    {
    cout << "Error: " << e << endl;
    return EXIT_FAILURE;
    }

  cout << "Loading mask " << inputMaskFile << " done." << endl;

  float expandInMM = 0;
  sscanf( argv[ 2 ], "%f", &expandInMM );

  cout << "Expanding mask by " << expandInMM << " mm:" << endl;

  const auto expanded = ccipd::CalculateMaskFromLevelset(
    ccipd::MaskToLevelset< ccipd::InternalPixelType >(
      maskReader->GetOutput(), false, false, 0, 0, false ),
    expandInMM );

  cout << "Expanding mask by " << expandInMM << " mm done." << endl;

  cout << "Saving " << outputMaskFile << ":" << endl;

  // sav eour mask
  typedef itk::ImageFileWriter< ccipd::VolumeMaskImageType > MaskWriterType;
  auto maskWriter = MaskWriterType::New();
  maskWriter->SetFileName( outputMaskFile );
  maskWriter->SetInput   ( expanded       );
  maskWriter->UseCompressionOn();

  try
    {
    maskWriter->Update();
    }
  catch( itk::ExceptionObject & e )
    {
    cout << "Error: " << e << endl;
    return EXIT_FAILURE;
    }

  cout << "Saving " << outputMaskFile << " done." << endl;

  return EXIT_SUCCESS;

} // main