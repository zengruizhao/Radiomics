#ifndef __ccipdVolumeFeaturesToGaussianModels_h
#define __ccipdVolumeFeaturesToGaussianModels_h



//////////////////////////////////////////////////////////////////////////////////////////
// ccipd includes
#include "ccipdVolumeTypes.h"

#include "ccipdDisableWarningsMacro.h"
  // itk includes
  #include <itkGaussianMembershipFunction.h>
  #include <itkVectorImage.h>
#include "ccipdEnableWarningsMacro.h"
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////
/** \ingroup Filtering */
typedef itk::Statistics::GaussianMembershipFunction< 
  itk::VariableLengthVector< ccipd::InternalPixelType > 
> VolumeFeaturesGaussianFunctionType;

/** \ingroup Filtering */
typedef std::vector<
  VolumeFeaturesGaussianFunctionType::Pointer
> VolumeFeaturesGuassianFunctionContainer;
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Calculate a Gaussian Model for a VolumeFeaturesImage
 * 
 * \author  Rachel Sparks
 * \ingroup Filtering
 */
const VolumeFeaturesGuassianFunctionContainer	
  VolumeFeaturesToGaussianModels (
  VolumeFeaturesType  * const features,
  VolumeMaskImageType * const mask,
  const unsigned int    numberOfClasses );
//////////////////////////////////////////////////////////////////////////////////////////



}

#endif