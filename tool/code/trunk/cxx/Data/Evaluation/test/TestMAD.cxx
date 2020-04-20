


//////////////////////////////////////////////////////////////////////////////////////////
// ccipd includes
#include "ccipdMRITypes.h"             // for mask types
#include "ccipdCalculateMAD.h" // for MAD calculation

//std includes
#include <cstdlib>
#include <cstdio> // for sscanf
#include <iostream>

#include "ccipdDisableWarningsMacro.h"
  // itk includes
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
      "Usage: " << argv[ 0 ] << " mask1 mask2 " <<
        "[optional smoothing variance] " <<
        "[optional expand size in mm ] " <<
      endl << endl << "This will calculate the Dice " <<
      "value between 2 masks.  There must be " <<
      "exactly 2 parameters, where each parameter " <<
      "is a 3D mask file." << endl << endl;

    return EXIT_SUCCESS;
    }
  else if ( argc < 3 )
    {
    cerr << "You must supply at least 2 mask files." << endl << endl;
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
  if ( argc > 3 )
    sscanf( argv[ 3 ], "%f", &smoothingVariance );
  if ( argc > 4 )
    sscanf( argv[ 4 ], "%f", &increaseSizeInMM );
    
  // input our mask files
  const string
    maskFile1( argv[ 1 ] ),
    maskFile2( argv[ 2 ] );

  // load our masks
  auto
    mask1 = LoadMask( maskFile1 ),
    mask2 = LoadMask( maskFile2 );

  if ( !mask1 || !mask2 )
    {
    return EXIT_FAILURE;
    }

  // display some progress
  cout << "Calculating MAD:" << endl;
  cout << "  Mask 1 = " << maskFile1 << endl;
  cout << "  Mask 2 = " << maskFile2 << endl;
  if ( smoothingVariance > 0 )
    cout << "         Smoothing Variance = " << smoothingVariance << endl;
  if ( increaseSizeInMM != 0 )
    cout << "Expanding Segmentation (mm) = " << increaseSizeInMM << endl;
  
  const auto MAD = ccipd::CalculateMAD(
    mask1, mask2, 0.01, smoothingVariance, increaseSizeInMM );
  cout << endl << "MAD = " << MAD << endl << endl;

  return EXIT_SUCCESS;

} // main
//////////////////////////////////////////////////////////////////////////////////////////
