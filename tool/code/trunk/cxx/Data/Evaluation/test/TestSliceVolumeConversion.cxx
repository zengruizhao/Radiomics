// This test program loads a volume, extracts the first slice
// and converts the slice to a volume.

//////////////////////////////////////////////////////////////////////////////////////////

// ccipd includes
#include "ccipdCalculateVolume.h"
#include "ccipdLoadVolume.h"
#include "ccipdSliceVolumeConversion.hxx"

// itk includes
#include "ccipdDisableWarningsMacro.h"
#include <itkImage.h>
#include "ccipdEnableWarningsMacro.h"

//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
using std::cout;
using std::endl;
using std::cerr;
using std::string;
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
int main( int argc, char *argv[] )
{

  if ( argc == 1 )
    {

    // display help message
    cout << endl << endl <<
      "Usage: " << argv[ 0 ] << " mask " << endl;
    return EXIT_SUCCESS;
    }
    
  // input our mask files
  const string maskFile( argv[ 1 ] );

  // load our masks
  const ccipd::VolumeMaskImageConstPointer
    mask( ccipd::LoadVolumeMask( maskFile ) );

  if ( !mask )
    {
    cerr << "Error: No mask!" << endl;
    return EXIT_FAILURE;
    }

  cout << "Mask = " << endl << mask << endl;

  // display some progress
  cout << "Calculating volume:" << endl;
  const ccipd::EvaluationValueType
    volume = ccipd::GetVolume( mask.GetPointer() );
  cout << "Calculating volume done." << endl;

  cout << "Volume = " << volume << endl;

  ccipd::SliceMaskImagePointer slice = ccipd::GetSliceFromVolume< ccipd::MaskPixelType >( mask.GetPointer(), 0 );
  ccipd::VolumeMaskImagePointer newVolume = ccipd::GetVolumeFromSlice< ccipd::MaskPixelType >( slice );

  return EXIT_SUCCESS;

} // main
//////////////////////////////////////////////////////////////////////////////////////////
