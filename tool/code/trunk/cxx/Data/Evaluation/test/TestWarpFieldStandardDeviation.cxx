// This test program calculates standard deviation of vector
// image in a masked region.

//////////////////////////////////////////////////////////////////////////////////////////

// ccipd includes
#include "ccipdLoadVolume.h"
#include "ccipdVolumeTypes.h"
#include "ccipdSaveImage.h"
#include "ccipdApplyWarpField.h"

// itk includes
#include "ccipdDisableWarningsMacro.h"
  //#include <itkImage.h>
  #include "itkMaskStandardDeviationImageFilter.h"
  #include "itkMeanAbsoluteDistanceImageFilter.h"
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

  if ( argc == 2 )
    {

    // display help message
    cout << endl << endl <<
      "Usage: " << argv[ 0 ] << " mask " << endl
                             << " deformation field " << endl;
    return EXIT_SUCCESS;
    }

  // input mask
  const string maskFile( argv[ 1 ] );
  const ccipd::VolumeImagePointer
    mask( ccipd::LoadVolume( maskFile ) );

  // input displacement field
  const string dispFieldFile( argv[ 2 ] );
  const ccipd::VolumeWarpFieldPointer
    dispField( ccipd::LoadVolumeWarpField( dispFieldFile ) );

  // warp the mask
  ccipd::VolumeImagePointer warpedMask = 
    ccipd::ApplyWarpField( dispField, mask.GetPointer(), mask.GetPointer() );

  typedef itk::MaskStandardDeviationImageFilter< ccipd::VolumeWarpFieldType, ccipd::VolumeImageType > StandardDeviationFilterType;
  StandardDeviationFilterType::Pointer standardDeviationImageFilter = StandardDeviationFilterType::New();
  standardDeviationImageFilter->SetDisplacementField( dispField );
  standardDeviationImageFilter->SetMask( warpedMask );
  standardDeviationImageFilter->SetRadius( 1 );
  standardDeviationImageFilter->SetNumberOfThreads( 1 );
  standardDeviationImageFilter->Update();
  std::cout << standardDeviationImageFilter->GetMaskStandardDeviation() << endl;
  ccipd::SaveImage< ccipd::VolumeImageType >( standardDeviationImageFilter->GetOutput(), "stdDevImage.mha", true );

  return EXIT_SUCCESS;

} // main
//////////////////////////////////////////////////////////////////////////////////////////
