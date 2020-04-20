



//////////////////////////////////////////////////////////////////////////////////////////
// ccipd includes
#include "ccipdMRITypes.h"             // for mask types
#include "ccipdCalculateDICE.h"
#include "ccipdCalculateMAD.h" 
#include "ccipdExtractSubMask.h"  // for midgland extraction

// std includes
#include <cstdlib>
#include <cstdio> // for sscanf
#include <iostream>

#include "ccipdDisableWarningsMacro.h"
  // itk includes
  #include <itkImageRegion.h>
  #include <itkImageFileReader.h>       // for loading masks
#include "ccipdEnableWarningsMacro.h"
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
using std::cout;
using std::endl;
using std::cerr;
using std::string;
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
ccipd::MRIMaskImageConstPointer LoadMask( const string & fileName )
{

  // load our masks
  typedef itk::ImageFileReader< ccipd::MRIMaskImageType > MaskReaderType;
  auto maskReader = MaskReaderType::New();
  maskReader->SetFileName( fileName );

  bool error = false;
  string errorDescription;

  try
    {
    maskReader->Update();
    }
  catch ( itk::ExceptionObject & exception )
    {
    error = true;
    errorDescription = exception.GetDescription();
    }
  catch ( std::exception & exception )
    {
    error = true;
    errorDescription = exception.what();
    }
  catch ( ... )
    {
    error = true;
    errorDescription = "Unknown error.";
    }

  if ( error )
    {
    cerr << "Unable to load " << fileName << endl <<
      errorDescription << endl;
    return NULL;
    }
  else
    {
    return maskReader->GetOutput();
    }

} // LoadMask
//////////////////////////////////////////////////////////////////////////////////////////




//////////////////////////////////////////////////////////////////////////////////////////
int main( int argc, char *argv[] )
{

  if ( argc == 1 )
    {

    // display help message
    cout << endl << endl <<
      "Usage: " << argv[ 0 ] <<
        " segmentation ground_truth prostate_ground_truth " <<
        "[optional smoothing variance] " <<
        "[optional expand size in mm ] " <<
      endl << endl << "This will calculate the Dice " <<
      "value between 2 masks.  There must be " <<
      "3 parameters, where each parameter " <<
      "is a 3D mask file." << endl << endl;

    return EXIT_SUCCESS;
    }
  else if ( argc < 4 )
    {
    cerr << "You must supply 3 mask files." << endl << endl;
    return EXIT_FAILURE;
    }
  else if ( argc > 5 )
    {
    cerr << "Warning: Arguments 5 through " << argc << " ignored." << endl;
    }

  // optionally smooth the levelsets
  // and threshold differently
  float
    smoothingVariance = 0,
    increaseSizeInMM  = 0;
  if ( argc > 4 )
    sscanf( argv[ 4 ], "%f", &smoothingVariance );
  if ( argc > 4 )
    sscanf( argv[ 5 ], "%f", &increaseSizeInMM );

  // input our mask files
  const string
    segmentationFile( argv[ 1 ] ),
    groundTruthFile ( argv[ 2 ] ),
    prostateFile    ( argv[ 3 ] );

  // load our masks
  const auto
    segmentationMask = LoadMask( segmentationFile ),
    groundTruthMask  = LoadMask( groundTruthFile  ),
    prostateMask     = LoadMask( prostateFile     );

  if ( !segmentationMask || !groundTruthMask || !prostateMask )
    {
    cerr << "A mask wasn't loaded!" << endl;
    return EXIT_FAILURE;
    }

  // display some progress
  cout << "  Segmentation = " << segmentationFile << endl;
  cout << "  Ground Truth = " << groundTruthFile  << endl;
  if ( smoothingVariance > 0 )
    cout << "         Smoothing Variance = " << smoothingVariance << endl;
  if ( increaseSizeInMM != 0 )
    cout << "Expanding Segmentation (mm) = " << increaseSizeInMM << endl;


  // extract the apex
  cout << "Extracting apex:" << endl;

  const auto
    segmentationApex = ccipd::ExtractApexMask(
      segmentationMask, prostateMask ),
    groundTruthApex = ccipd::ExtractApexMask(
      groundTruthMask, prostateMask );
  
  cout << "Extracting apex done." << endl;

  cout << "Calculating Dice:" << endl;

  const auto diceApex = ccipd::CalculateDice(
    segmentationApex, groundTruthApex, smoothingVariance, increaseSizeInMM );

  cout << endl << "Apex Dice = " << diceApex << endl << endl;

  cout << "Calculating MAD:" << endl;

  const auto MADApex = ccipd::CalculateMAD(
    segmentationApex, groundTruthApex, 1e-3, smoothingVariance, increaseSizeInMM );

  cout << "Apex MAD = " << MADApex << endl;

  // extract the midgland
  cout << "Extracting midgland:" << endl;

  const auto
    segmentationMidgland = ccipd::ExtractMidglandMask(
      segmentationMask, prostateMask ),
    groundTruthMidgland = ccipd::ExtractMidglandMask(
      groundTruthMask, prostateMask );
  
  cout << "Extracting midgland done." << endl;

  cout << "Calculating Dice:" << endl;

  const auto diceMidgland = ccipd::CalculateDice(
    segmentationMidgland, groundTruthMidgland, smoothingVariance, increaseSizeInMM );

  cout << endl << "Midgland Dice = " << diceMidgland << endl << endl;

  cout << "Calculating MAD:" << endl;

  const auto MADMidgland = ccipd::CalculateMAD(
    segmentationMidgland, groundTruthMidgland, 1e-3, smoothingVariance, increaseSizeInMM );

  cout << "Midgland MAD = " << MADMidgland << endl;


  // extract the base
  cout << "Extracting base:" << endl;

  const auto
    segmentationBase = ccipd::ExtractBaseMask(
      segmentationMask, prostateMask ),
    groundTruthBase = ccipd::ExtractBaseMask(
      groundTruthMask, prostateMask );
  
  cout << "Extracting base done." << endl;

  cout << "Calculating Dice:" << endl;

  const auto diceBase = ccipd::CalculateDice(
    segmentationBase, groundTruthBase, smoothingVariance, increaseSizeInMM );

  cout << endl << "Base Dice = " << diceBase << endl << endl;

  cout << "Calculating MAD:" << endl;

  const auto MADBase = ccipd::CalculateMAD(
    segmentationBase, groundTruthBase, 1e-3, smoothingVariance, increaseSizeInMM );

  cout << "Base MAD = " << MADBase << endl;
  return EXIT_SUCCESS;

} // main
//////////////////////////////////////////////////////////////////////////////////////////
