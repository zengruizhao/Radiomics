#ifndef __ccipdSliceTypes_h
#define __ccipdSliceTypes_h




////////////////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdCore.h"
#include "ccipdITKForwardDeclarations.h"
////////////////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



/**
 * \addtogroup Common
 * @{
 */



////////////////////////////////////////////////////////////////////////////////////////////////////

/// For 2D images
const unsigned int SliceDimension = 2;

////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////

/// 2D image type
typedef itk::Image<
  InternalPixelType,
  SliceDimension
> SliceImageType;

/// 2D transform image type
typedef itk::Image<
  TransformValueType,
  SliceDimension
> TransformImageType;

/// A smart pointer to a 2D image
typedef itk::SmartPointer<
  SliceImageType
> SliceImagePointer;

/// A smart pointer to a const 2D image
typedef itk::SmartPointer<
  const SliceImageType
> SliceImageConstPointer;

/// 2D image base
typedef itk::ImageBase<
  SliceDimension
> SliceImageBase;

/// 2D image region
typedef itk::ImageRegion<
  SliceDimension
> SliceRegionType;

////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////////

/// 2D mask type
typedef itk::Image<
  MaskPixelType,
  SliceDimension
> SliceMaskImageType;

/// A smart pointer to a 2D mask
typedef itk::SmartPointer<
  SliceMaskImageType
> SliceMaskImagePointer;

/// A smart pointer to a const 2D mask
typedef itk::SmartPointer<
  const SliceMaskImageType
> SliceMaskImageConstPointer;

/// 2D mask spatial object
typedef itk::ImageMaskSpatialObject<
  SliceDimension
> SliceMaskSpatialType;

////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////

/// Define a 2D transformation
typedef itk::Transform<
  TransformValueType,
  SliceDimension,
  SliceDimension
> SliceTransformType;

/// Smart pointer to 2D transformation
typedef itk::SmartPointer<
  SliceTransformType
> SliceTransformPointer;

/// Smart pointer to const 2D transformation
typedef itk::SmartPointer<
  const SliceTransformType
> SliceTransformConstPointer;

////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////

/// 2D warp field vector
typedef itk::Vector<
  WarpValueType,
  SliceDimension
> SliceWarpPixelType;

/// 2D warp field
typedef itk::Image<
  SliceWarpPixelType,
  SliceDimension
> SliceWarpFieldType;

/// 2D warp field smart pointer
typedef itk::SmartPointer<
  SliceWarpFieldType
> SliceWarpFieldPointer;

/// 2D warp field smart pointer
typedef itk::SmartPointer<
  const SliceWarpFieldType
> SliceWarpFieldConstPointer;

////////////////////////////////////////////////////////////////////////////////////////////////////




/** @} */
} // namespace ccipd
#endif // __ccipdRGBSliceTypes_h
