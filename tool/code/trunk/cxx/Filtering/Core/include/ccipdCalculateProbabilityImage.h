#ifndef __ccipdCalculateProbabilityImage_h
#define __ccipdCalculateProbabilityImage_h



////////////////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdCore.h"
#include "ccipdVolumeTypes.h"

#include "ccipdDisableWarningsMacro.h"
  #include <itkMembershipFunctionBase.h>
#include "ccipdEnableWarningsMacro.h"
////////////////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * \ingroup Filtering
 */
typedef itk::Statistics::MembershipFunctionBase<
  itk::VariableLengthVector< ccipd::InternalPixelType > 
> MemberType;
////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Calculate a probability image.
 * 
 * \author  Rachel Sparks
 * \ingroup Filtering
 */
VolumeImageConstPointer CalculateProbabilityImage (
	const VolumeFeaturesType * featureImage,
	const MemberType         *  model);

/**
 * \brief Calculate a probability image.
 * 
 * \author  Rachel Sparks
 * \ingroup Filtering
 */
VolumeImageConstPointer CalculateProbabilityImage (
	const VolumeFeaturesType * featureImage,
	const MemberType         *  model,
	const VolumeImageType    * spatialPrior);

/**
 * \brief Calculate a probability image.
 * 
 * \author  Rachel Sparks
 * \ingroup Filtering
 */
VolumeImageConstPointer CalculateProbabilityImage (
	const VolumeFeaturesType * featureImage,
	const MemberType         *  model,
	const VolumeImageType    * spatialPrior,
	const double               spatialWeight );
////////////////////////////////////////////////////////////////////////////////////////////////////



} //namespace ccipd
#endif //__ccipdCalculateProbabilityImage_h
