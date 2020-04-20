#ifndef __ccipdCropImage_h
#define __ccipdCropImage_h



////////////////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdCore.h"
#include "ccipdVolumeTypes.h"
#include "ccipdSliceTypes.h"
#include "ccipdCropImage.hxx"
////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////////
namespace itk
{
  template< typename TValueType, unsigned int VLength > class FixedArray;
}
////////////////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * This function will crop a given a bounding box.
 * 
 * \author  Rob Toth
 * \ingroup Common
 */
VolumeImagePointer CropVolume(
  const VolumeImageType  * inputVolume,
  const VolumeRegionType & boundingBox );
////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * This function will crop a mask given a bounding box.
 * 
 * \author  Rob Toth
 * \ingroup Common
 */
VolumeMaskImagePointer CropMask(
  const VolumeMaskImageType * inputMask,
  const VolumeRegionType    & boundingBox );
////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * This will crop a volume a certain # of mm in each direction,
 * where the # of mm to crop from the start and end of each dimension
 * can be specified separately.
 * 
 * \author  Rob Toth
 * \ingroup Common
 */
VolumeImagePointer CropVolume(
  const VolumeImageType * inputVolume,
  const double            cropInMMFromXStart,
  const double            cropInMMFromXEnd,
  const double            cropInMMFromYStart,
  const double            cropInMMFromYEnd,
  const double            cropInMMFromZStart,
  const double            cropInMMFromZEnd );
////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * This will crop a mask a certain # of mm in each direction,
 * where the # of mm to crop from the start and end of each dimension
 * can be specified separately.
 * 
 * \author  Rob Toth
 * \ingroup Common
 */
VolumeMaskImagePointer CropMask(
  const VolumeMaskImageType * inputMask,
  const double                cropInMMFromXStart,
  const double                cropInMMFromXEnd,
  const double                cropInMMFromYStart,
  const double                cropInMMFromYEnd,
  const double                cropInMMFromZStart,
  const double                cropInMMFromZEnd );
////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * This will crop a volume from both sides by a certain # of mm in each dimension.
 * 
 * \author  Rob Toth
 * \ingroup Common
 */
VolumeImagePointer CropVolume(
  const VolumeImageType * inputVolume,
  const double            cropInMMFromX,
  const double            cropInMMFromY,
  const double            cropInMMFromZ );
////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * This will crop a mask from both sides by a certain # of mm in each dimension.
 * 
 * \author  Rob Toth
 * \ingroup Common
 */
VolumeMaskImagePointer CropMask(
  const VolumeMaskImageType * inputMask,
  const double                cropInMMFromX,
  const double                cropInMMFromY,
  const double                cropInMMFromZ );
////////////////////////////////////////////////////////////////////////////////////////////////////



// Here begins the helper, template functions.



////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * This will crop a volume a certain # of mm in each direction,
 * where the # of mm to crop from the start and end of each dimension
 * can be specified separately.
 * 
 * \author  Rob Toth
 * \ingroup Common
 */
template< class PixelType >
typename itk::Image< PixelType, 3 >::Pointer CropVolume(
  const itk::Image< PixelType, 3 > * inputVolume,
  const double                       cropInMMFromXStart,
  const double                       cropInMMFromXEnd,
  const double                       cropInMMFromYStart,
  const double                       cropInMMFromYEnd,
  const double                       cropInMMFromZStart,
  const double                       cropInMMFromZEnd );
////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * It will crop a certain number of mm from either the beginning
 * or end of a given dimension of a bounding box.
 *
 * The output region can be passed to CropImage
 * 
 * \author  Rob Toth
 * \ingroup Common
 */
template< unsigned int ImageDimension >
itk::ImageRegion< ImageDimension > CropBoundingBox(
  const itk::ImageRegion< ImageDimension > & inputBoundingBox,
  const double                               imageSpacing,
  const unsigned int                         whichDimension,
  const double                               cropInMM,
  const bool                                 fromBeginning );
////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * It will crop a certain number of mm from either the beginning
 * or end of each given dimension of a bounding box.
 * 
 * \author  Rob Toth
 * \ingroup Common
 */
VolumeRegionType CropBoundingBox(
  const VolumeRegionType      & inputBoundingBox,
  const double                  cropInMMFromXStart,
  const double                  cropInMMFromXEnd,
  const double                  cropInMMFromYStart,
  const double                  cropInMMFromYEnd,
  const double                  cropInMMFromZStart,
  const double                  cropInMMFromZEnd,
  const itk::Vector<
    double, VolumeDimension > & imageSpacing );
////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * It will crop a certain number of pixels from the beginning
 * and the end of each given dimension of the SizeType bounding box.
 * 
 * \author  Ahmad Algohary
 * \ingroup Common
 */
template<class TImageType>
typename TImageType::Pointer
CropVolumeImage(  typename TImageType::Pointer  & pImage 
                , typename TImageType::SizeType & cropSize
               );

////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * It will crop a certain number of mm from either the beginning
 * or end of each given dimension of a bounding box.
 * 
 * \author  Rob Toth
 * \ingroup Common
 */
VolumeRegionType CropBoundingBox(
  const VolumeRegionType      & inputBoundingBox,
  const double                  cropInMMFromX,
  const double                  cropInMMFromY,
  const double                  cropInMMFromZ,
  const itk::Vector<
    double, VolumeDimension > & imageSpacing );
////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * It will crop a certain number of mm from either the beginning
 * or end of each dimension.
 *
 * The output region can be passed into an itk::ImageRegionFilter
 * 
 * \author  Rob Toth
 * \ingroup Common
 */
template< unsigned int ImageDimension >
itk::ImageRegion< ImageDimension > CropBoundingBox(
  const itk::ImageRegion<         ImageDimension >             & inputBoundingBox,
  const itk::FixedArray < double, ImageDimension >             & cropInMMFromBeginning,
  const itk::FixedArray < double, ImageDimension >             & cropInMMFromEnd,
  const typename itk::ImageBase< ImageDimension >::SpacingType & imageSpacing );
////////////////////////////////////////////////////////////////////////////////////////////////////



/**
 * Make sure TIMageType extends itk::Image.
 */

////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * Helper function for cropping.
 * 
 * \author  Rob Toth
 * \ingroup Common
 */
template< class PixelType, unsigned int ImageDimension >
typename itk::Image< PixelType, ImageDimension >::Pointer CropImage(
  const itk::Image< PixelType, ImageDimension >    * inputImage,
  const itk::FixedArray < double, ImageDimension > & cropInMMFromBeginning,
  const itk::FixedArray < double, ImageDimension > & cropInMMFromEnd );
////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * This function will actually perform the cropping of an image.
 * 
 * \author  Rob Toth
 * \ingroup Common
 */
template< class ImageType >
typename ImageType::Pointer CropImage(
  const ImageType                      * inputImage,
  const typename ImageType::RegionType & boundingBox );
////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Crop an itk::VectorImage
 * 
 * \author  Rob Toth
 * \ingroup Common
 */
VolumeFeaturesPointer CropFeatureImage(
  const VolumeFeaturesType * inputFeatures,
  const VolumeRegionType   & boundingBox );
////////////////////////////////////////////////////////////////////////////////////////////////////
/** \brief  : Crop Multidimensional Images in the First two dimensions only
  * \Created: Ahmad 7/16/2013
  * \Updated: Ahmad 7/16/2013 
  * \ingroup: Common/Images
  * \param  : pInputImage: Pointer to the templated Image (Volume)
  * \param  : pix1: Index of the first picked point
  * \param  : pix2: Index of the second picked point
  * \return : The cropped Image
  * \Notes  : Supports Time series volume data
  */
template <class TInputImageType>
typename TInputImageType::Pointer 
CropImageBy2PickedPoints(typename TInputImageType::Pointer pInputImage, 
                         typename TInputImageType::IndexType pix1, 
                         typename TInputImageType::IndexType pix2);
////////////////////////////////////////////////////////////////////////////////////////////////////
/** \brief  : Crop Multidimensional Images in the First two dimensions only
  * \Created: Ahmad 7/17/2013
  * \Updated: Ahmad 7/17/2013 
  * \ingroup: Common/Images
  * \param  : pInputImage: Pointer to the templated Image (Volume)
  * \param  : p1: Physical coordinates of the first picked point
  * \param  : p2: Physical coordinates of the second picked point
  * \return : A smart pointer to the cropped Image
  * \Notes  : Supports Time series volume data,
  *           This function does the same as the other overload, 
  *           except for that it takes points instead of indexes
  */
template <class TInputImageType>
typename TInputImageType::Pointer 
CropImageBy2PickedPoints(typename TInputImageType::Pointer pInputImage, 
                         typename TInputImageType::PointType p1, 
                         typename TInputImageType::PointType p2);

/** \brief  : 
  * \Created: axa667 7/17/2013
  * \Updated:  
  * \ingroup:
  * \param  : 
  * \param  : 
  * \return : 
  * 
  * 
  * 
  * 
  */

VolumeImagePointer CropVolume(const VolumeImageType  * inputVolume, const VolumeRegionType & boundingBox );



} // namespace
#endif // __ccipdCropImage_h

