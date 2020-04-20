


//////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdMaskToDistanceImage.hxx"
#include "ccipdVolumeTypes.h"
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd {

template
VolumeImagePointer
MaskToDistanceImage< InternalPixelType,  MaskPixelType, VolumeDimension>(
 const VolumeMaskImageType * const segmentation,
 const VolumeMaskImageType * const groundTruth,
 const float                 smoothVariance,
 const float                 increaseSizeInMM );

} // namespace