#ifndef __ccipdTRUSTypes_h
#define __ccipdTRUSTypes_h



//////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdVolumeTypes.h"
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



/**
 * \addtogroup Common
 * @{
 */

  
  
//////////////////////////////////////////////////////////////////////////////////////////
// we are considering TRUS images to be volumetric
const unsigned int                 TRUSDimension = VolumeDimension;
typedef VolumeImageType            TRUSImageType;
typedef VolumeImagePointer         TRUSImagePointer;
typedef VolumeImageConstPointer    TRUSImageConstPointer;
typedef VolumeRegionType           TRUSRegionType;
typedef VolumeImagesContainer      TRUSImagesContainer;
typedef VolumeConstImagesContainer TRUSConstImagesContainer;
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
// elastic registrations
typedef VolumeWarpPixelType  TRUSWarpPixelType;
typedef VolumeWarpFieldType  TRUSWarpFieldType;
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
// linear transformations
typedef VolumeTransformType            TRUSTransformType;
typedef VolumeTransformPointer         TRUSTransformPointer;
typedef VolumeTransformConstPointer    TRUSTransformConstPointer;
typedef VolumeConstTransformsContainer TRUSConstTransformsContainer;
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
// mask types
typedef VolumeMaskImageType            TRUSMaskImageType;
typedef VolumeMaskImagePointer         TRUSMaskImagePointer;
typedef VolumeMaskImageConstPointer    TRUSMaskImageConstPointer;
typedef VolumeMaskImagesContainer      TRUSMaskImagesContainer;
typedef VolumeConstMaskImagesContainer TRUSConstMaskImagesContainer;
typedef VolumeMaskSpatialType          TRUSMaskSpatialType;
//////////////////////////////////////////////////////////////////////////////////////////

  
  
/** @} */
} // namespace
#endif // __ccipdTRUSTypes_h
