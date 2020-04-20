


//////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdMaskToLevelset.hxx"
#include "ccipdCore.h"
#include "ccipdVolumeTypes.h"
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////
template
VolumeImagePointer
MaskToLevelset< InternalPixelType, MaskPixelType, VolumeDimension >(  const VolumeMaskImageType * const mask
																	, const bool  normalize
																	, const bool  sigmoid
																	, const float sigmoidWidth
																	, const float smoothingVariance
																	, const bool  resizeIsotropically
																   );

template
itk::SmartPointer< itk::Image< double, VolumeDimension > >
MaskToLevelset< double, MaskPixelType, VolumeDimension >(  const VolumeMaskImageType * const mask
														 , const bool  normalize
                                                         , const bool  sigmoid
                                                         , const float sigmoidWidth
                                                         , const float smoothingVariance
                                                         , const bool  resizeIsotropically
                                                        );
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace
