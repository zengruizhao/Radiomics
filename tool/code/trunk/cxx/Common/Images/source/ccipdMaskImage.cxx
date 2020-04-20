


//////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdMaskImage.hxx"
#include "ccipdVolumeTypes.h"
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////
template
VolumeImagePointer MaskImage< InternalPixelType, MaskPixelType, VolumeDimension >(
  const VolumeImageType     * const imageToMask,
  const VolumeMaskImageType * const maskToApply,
  const InternalPixelType           maskedValue,
  const bool                        maskPixelsOutsideOfMask );
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
template
VolumeMaskImagePointer MaskImage< MaskPixelType, MaskPixelType, VolumeDimension >(
  const VolumeMaskImageType * const imageToMask,
  const VolumeMaskImageType * const maskToApply,
  const MaskPixelType               maskedValue,
  const bool                        maskPixelsOutsideOfMask );
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
template
VolumeMaskImagePointer MaskImage< MaskPixelType, BinaryPixelType, VolumeDimension >(
  const VolumeMaskImageType   * const imageToMask,
  const VolumeBinaryImageType * const maskToApply,
  const MaskPixelType                 maskedValue,
  const bool                          maskPixelsOutsideOfMask );
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
template
VolumeBinaryImagePointer MaskImage< BinaryPixelType, BinaryPixelType, VolumeDimension >(
  const VolumeBinaryImageType * const imageToMask,
  const VolumeBinaryImageType * const maskToApply,
  const BinaryPixelType               maskedValue,
  const bool                          maskPixelsOutsideOfMask );
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace ccipd
