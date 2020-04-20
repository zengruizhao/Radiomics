


//////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdDuplicateImage.hxx"
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////
template
VolumeMaskImagePointer DuplicateImage< VolumeMaskImageType >(
  const VolumeMaskImageType * const );

template
VolumeImagePointer DuplicateImage< VolumeImageType >(
  const VolumeImageType * const );

template
VolumeBinaryImagePointer DuplicateImage< VolumeBinaryImageType >(
  const VolumeBinaryImageType * const );
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace
