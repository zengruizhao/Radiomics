#ifndef __ccipdRGBSliceTypes_h
#define __ccipdRGBSliceTypes_h




////////////////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdSliceTypes.h"
#include "ccipdVolumeTypes.h"
////////////////////////////////////////////////////////////////////////////////////////////////////

namespace ccipd
{



/**
 * \addtogroup Common
 * @{
 */


 
////////////////////////////////////////////////////////////////////////////////////////////////////

/// RGB image pixel type

typedef itk::RGBPixel <unsigned char>   RGBPixelType ;
typedef itk::RGBAPixel<unsigned char>   RGBAPixelType; // Alpha controls transparency of the pixel

/// 2D RGB image slice

typedef itk::Image<RGBPixelType, SliceDimension> RGBSliceType;
typedef itk::Image<RGBAPixelType, SliceDimension> RGBASliceType;

/// Smart pointer to 2D RGB slice
typedef itk::SmartPointer<RGBSliceType> RGBSlicePointer;
typedef itk::SmartPointer<RGBASliceType> RGBASlicePointer;

/// Smart pointer to const 2D RGB slice
typedef itk::SmartPointer<const RGBSliceType> RGBSliceConstPointer;
typedef itk::SmartPointer<const RGBASliceType> RGBASliceConstPointer;
////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////

/// 2D RGB warp field vector
typedef itk::Vector<WarpValueType, SliceDimension> RGBWarpPixelType;

/// 2D RGB warp field
typedef itk::Image<SliceWarpPixelType, SliceDimension> RGBWarpFieldType;

/// 2D RGB warp field smart pointer
typedef itk::SmartPointer<SliceWarpFieldType> RGBWarpFieldPointer;

/// 2D RGB warp field smart pointer
typedef itk::SmartPointer<const SliceWarpFieldType> RGBWarpFieldConstPointer;
////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////

/// 3D RGB image slice
typedef itk::Image<RGBPixelType, VolumeDimension> RGBVolumeType;
typedef itk::Image<RGBAPixelType, VolumeDimension> RGBAVolumeType;

typedef itk::ImageBase<VolumeDimension> VolumeImageBaseType;

/// Smart pointer to 2D RGB slice
typedef itk::SmartPointer< RGBVolumeType > RGBVolumePointer   ;
typedef std::vector<RGBVolumePointer>      RGBVolumeContainer ;

typedef itk::SmartPointer< RGBAVolumeType> RGBAVolumePointer  ;
typedef std::vector<RGBAVolumePointer>     RGBAVolumeContainer;

/// Smart pointer to const 2D RGB slice
typedef itk::SmartPointer<const RGBVolumeType>  RGBVolumeConstPointer  ;
typedef std::vector<RGBVolumeConstPointer>      RGBConstVolumeContainer;

typedef itk::SmartPointer<const RGBAVolumeType> RGBAVolumeConstPointer;
typedef std::vector<RGBAVolumeConstPointer>     RGBAConstVolumeContainer;





////////////////////////////////////////////////////////////////////////////////////////////////////
/** @} */
} // namespace ccipd
#endif // __ccipdRGBSliceTypes_h
