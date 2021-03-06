



//////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdLevelsetToSurface.hxx"
#include "ccipdVolumeTypes.h"
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{


//////////////////////////////////////////////////////////////////////////////////////////
template
VolumeMaskImagePointer GetSurfaceImage< InternalPixelType, MaskPixelType, VolumeDimension >(
  const VolumeImageType * const levelset,
  const InternalPixelType       levelsetValue,
  const MaskPixelType           backgroundValue,
  const MaskPixelType           foregroundValue );
//////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////
template
VolumeImagePointer MergeImageWithSurfaceImage< InternalPixelType, InternalPixelType, VolumeDimension >(
  const VolumeImageType * const image,
  const VolumeImageType * const levelset,
  const InternalPixelType       levelsetValue);
//////////////////////////////////////////////////////////////////////////////////////////


} // namespace ccipd
