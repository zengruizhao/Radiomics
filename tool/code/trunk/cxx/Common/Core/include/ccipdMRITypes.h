#ifndef __ccipdMRITypes_h
#define __ccipdMRITypes_h



//////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdVolumeTypes.h"
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{

  
  
//////////////////////////////////////////////////////////////////////////////////////////
// we are considering MRI images to be volumetric
const unsigned int                 MRIDimension = VolumeDimension;
typedef VolumeImageType            MRIImageType;
typedef VolumeImagePointer         MRIImagePointer;
typedef VolumeImageConstPointer    MRIImageConstPointer;
typedef VolumeRegionType           MRIRegionType;
typedef VolumeImagesContainer      MRIImagesContainer;
typedef VolumeConstImagesContainer MRIConstImagesContainer;
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
// elastic registrations
typedef VolumeWarpPixelType  MRIWarpPixelType;
typedef VolumeWarpFieldType  MRIWarpFieldType;
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
// linear transformations
typedef VolumeTransformType            MRITransformType;
typedef VolumeTransformPointer         MRITransformPointer;
typedef VolumeTransformConstPointer    MRITransformConstPointer;
typedef VolumeConstTransformsContainer MRIConstTransformsContainer;
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
// mask types
typedef VolumeMaskImageType            MRIMaskImageType;
typedef VolumeMaskImagePointer         MRIMaskImagePointer;
typedef VolumeMaskImageConstPointer    MRIMaskImageConstPointer;
typedef VolumeMaskImagesContainer      MRIMaskImagesContainer;
typedef VolumeConstMaskImagesContainer MRIConstMaskImagesContainer;
typedef VolumeMaskSpatialType          MRIMaskSpatialType;
//////////////////////////////////////////////////////////////////////////////////////////

  
  
} // namespace
#endif // __ccipdMRITypes_h
