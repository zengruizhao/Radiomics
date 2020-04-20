

////////////////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdCropImage.hxx"

#include "ccipdDisableWarningsMacro.h"
  // itk includes
  #include <itkImage.h>
  #include <itkVectorImage.h>
#include "ccipdEnableWarningsMacro.h"
////////////////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{
////////////////////////////////////////////////////////////////////////////////////////////////////
SliceImagePointer CropImage(
  const SliceImageType  * inputVolume,
  const SliceRegionType & boundingBox )
{
  return CropImage< SliceImageType >( inputVolume, boundingBox );
}
////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////
VolumeImagePointer CropVolume(
  const VolumeImageType  * inputVolume,
  const VolumeRegionType & boundingBox )
{
  return CropImage< VolumeImageType >( inputVolume, boundingBox );
}
////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * This function will crop a mask given a bounding box.
 */
VolumeMaskImagePointer CropMask(
  const VolumeMaskImageType * inputMask,
  const VolumeRegionType    & boundingBox )
{
  return CropImage< VolumeMaskImageType >( inputMask, boundingBox );
}
////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////
VolumeImagePointer CropVolume(
  const VolumeImageType * inputVolume,
  const double            cropInMMFromXStart,
  const double            cropInMMFromXEnd,
  const double            cropInMMFromYStart,
  const double            cropInMMFromYEnd,
  const double            cropInMMFromZStart,
  const double            cropInMMFromZEnd )
{

  return CropVolume< InternalPixelType >(
    inputVolume,
    cropInMMFromXStart,
    cropInMMFromXEnd,
    cropInMMFromYStart,
    cropInMMFromYEnd,
    cropInMMFromZStart,
    cropInMMFromZEnd );

} // CropVolume
////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////
VolumeImagePointer CropVolume(
  const VolumeImageType * inputVolume,
  const double            cropInMMFromX,
  const double            cropInMMFromY,
  const double            cropInMMFromZ )
{

  return CropVolume< InternalPixelType >(
    inputVolume,
    cropInMMFromX,
    cropInMMFromX,
    cropInMMFromY,
    cropInMMFromY,
    cropInMMFromZ,
    cropInMMFromZ );

} // CropVolume
////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////
VolumeMaskImagePointer CropMask(
  const VolumeMaskImageType * inputMask,
  const double                cropInMMFromXStart,
  const double                cropInMMFromXEnd,
  const double                cropInMMFromYStart,
  const double                cropInMMFromYEnd,
  const double                cropInMMFromZStart,
  const double                cropInMMFromZEnd )
{

  return CropVolume< MaskPixelType >(
    inputMask,
    cropInMMFromXStart,
    cropInMMFromXEnd,
    cropInMMFromYStart,
    cropInMMFromYEnd,
    cropInMMFromZStart,
    cropInMMFromZEnd );

} // CropMask
////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////
VolumeMaskImagePointer CropMask(
  const VolumeMaskImageType * inputMask,
  const double                cropInMMFromX,
  const double                cropInMMFromY,
  const double                cropInMMFromZ )
{

  return CropVolume< MaskPixelType >(
    inputMask,
    cropInMMFromX,
    cropInMMFromX,
    cropInMMFromY,
    cropInMMFromY,
    cropInMMFromZ,
    cropInMMFromZ );

} // CropMask
////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////
VolumeRegionType CropBoundingBox(
  const VolumeRegionType      & inputBoundingBox,
  const double                  cropInMMFromX,
  const double                  cropInMMFromY,
  const double                  cropInMMFromZ,
  const itk::Vector<
    double, VolumeDimension > & imageSpacing )
{

  return CropBoundingBox( inputBoundingBox,
    cropInMMFromX, cropInMMFromX,
    cropInMMFromY, cropInMMFromY,
    cropInMMFromZ, cropInMMFromZ,
    imageSpacing );

} // CropBoundingBox
////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////
VolumeRegionType CropBoundingBox(
  const VolumeRegionType      & inputBoundingBox,
  const double                  cropInMMFromXStart,
  const double                  cropInMMFromXEnd,
  const double                  cropInMMFromYStart,
  const double                  cropInMMFromYEnd,
  const double                  cropInMMFromZStart,
  const double                  cropInMMFromZEnd,
  const itk::Vector<
    double, VolumeDimension > & imageSpacing )
{

  // crop in each dimension
  itk::FixedArray< double, VolumeDimension >
    cropInMMFromStart, cropInMMFromEnd;

  cropInMMFromStart[ 0 ] = cropInMMFromXStart;
  cropInMMFromStart[ 1 ] = cropInMMFromYStart;
  cropInMMFromStart[ 2 ] = cropInMMFromZStart;
  cropInMMFromEnd  [ 0 ] = cropInMMFromXEnd;
  cropInMMFromEnd  [ 1 ] = cropInMMFromYEnd;
  cropInMMFromEnd  [ 2 ] = cropInMMFromZEnd;

  return CropBoundingBox< VolumeDimension >(
    inputBoundingBox, cropInMMFromStart, cropInMMFromEnd, imageSpacing );

} // CropBoundingBox
////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////
VolumeFeaturesPointer CropFeatureImage(
  const VolumeFeaturesType * inputFeatures,
  const VolumeRegionType   & boundingBox )
{

  return CropImage< VolumeFeaturesType >( inputFeatures, boundingBox );

} // CropFeatureImage
////////////////////////////////////////////////////////////////////////////////////////////////////




template
VolumeImagePointer
CropImageBy2PickedPoints< VolumeImageType >(
  VolumeImagePointer pInputImage, 
  VolumeImageBase::IndexType p1,
  VolumeImageBase::IndexType p2 );



template
VolumeImagePointer
CropImageBy2PickedPoints< VolumeImageType >(
  VolumeImagePointer pInputImage, 
  VolumePointType p1, 
  VolumePointType p2);


} // namespace
