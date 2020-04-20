


//////////////////////////////////////////////////////////////////////////////////////////
// ccipd includes
#include "ccipdHashImage.hxx"

#include "ccipdDisableWarningsMacro.h"
  // itk includes
  #include <itkImage.h>
  #include <itkVectorImage.h>
#include "ccipdEnableWarningsMacro.h"
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////
template HashValueType
HashImage( const VolumeImageType * const volume );

template HashValueType
HashImage( const VolumeMaskImageType * const mask );

template HashValueType
HashImage( const VolumeFeaturesType * const features );

template HashValueType
HashImage( const VolumeWarpFieldType * const warpField );
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace ccipd
