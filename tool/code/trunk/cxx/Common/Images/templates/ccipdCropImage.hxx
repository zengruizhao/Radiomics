#ifndef __ccipdCropImage_hxx
#define __ccipdCropImage_hxx



////////////////////////////////////////////////////////////////////////////////////////////////////
// ccipd includes
#include "ccipdCropImage.h"

#include "ccipdDisableWarningsMacro.h"
  // itk includes
  #include <itkImageRegion.h>
  #include <itkImage.h>
  #include <itkFixedArray.h>
  #include <itkRegionOfInterestImageFilter.h>
  #include <itkCropImageFilter.h>
  #include <itkExceptionObject.h>
#include "ccipdEnableWarningsMacro.h"

// std includes
#include <cmath> // for floor and ceil
////////////////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



////////////////////////////////////////////////////////////////////////////////////////////////////
template< unsigned int ImageDimension >
itk::ImageRegion< ImageDimension > CropBoundingBox(
  const itk::ImageRegion< ImageDimension > & inputBoundingBox,
  const double                               imageSpacing,
  const unsigned int                         whichDimension,
  const double                               cropInMM,
  const bool                                 fromBeginning )

{

  // define our types
  typedef itk::ImageRegion<
    ImageDimension
  > RegionType;

  typedef typename RegionType::SizeValueType  SizeValueType;
  typedef typename RegionType::IndexValueType IndexValueType;
  typedef double                              FloatingType;

  const FloatingType
    // first, extract the size and starting index
    currentSize = static_cast< FloatingType >(
      inputBoundingBox.GetSize( whichDimension ) ),

    currentIndex = static_cast< FloatingType >(
      inputBoundingBox.GetIndex( whichDimension ) ),

    // how many pixels will we crop?
    cropInPixels1 =
      static_cast< FloatingType >( cropInMM     ) / 
      static_cast< FloatingType >( imageSpacing ),

    // if we are cropping too much, keep at least one pixel
    cropInPixels2 = ( cropInPixels1 >= currentSize ) ?
      currentSize - 1 : // keep at least one pixel
      cropInPixels1,    // otherwise, we are okay

    newIndexFloating = fromBeginning ? 
      currentIndex + cropInPixels2 : // add in some pixels to the index
      currentIndex,                  // otherwise, keep the index the same

    // our size is always decreasing the the # of pixels
    newSizeFloating = currentSize - cropInPixels2;

  // get them in the correct types
  const SizeValueType
    newSize = static_cast< SizeValueType >( ceil( newSizeFloating ) );

  const IndexValueType
    newIndex = static_cast< IndexValueType >( floor( newIndexFloating ) );

  // prepare our output
  RegionType outputBoundingBox( inputBoundingBox );

  // give it to our output
  outputBoundingBox.SetIndex( whichDimension, newIndex );
  outputBoundingBox.SetSize ( whichDimension, newSize  );

  // return
  return outputBoundingBox;

} // CropBoundingBox
////////////////////////////////////////////////////////////////////////////////////////////////////

template<class TImageType>
typename TImageType::Pointer
CropVolumeImage(  typename TImageType::Pointer  & pImage 
                , typename TImageType::SizeType & cropSize
               )
{
  // Check if pImage is valid
  // Check if sizes are less than the buffered region
  // Conditions will be caught in itk::ExceptionObject if not satisfied
  try
	{
     typedef itk::CropImageFilter <TImageType, TImageType> CropImageFilterType;
     typename CropImageFilterType::Pointer cropFilter = CropImageFilterType::New();
     cropFilter->SetInput(pImage);
     cropFilter->SetBoundaryCropSize(cropSize);
     cropFilter->Update();
     return cropFilter->GetOutput();
	}
  catch( itk::ExceptionObject & err )
    {
        std::cerr << "Error: Exception caught while cropping the image ...\nOriginal Image is returned." << std::endl
                  << __FILE__ << "  "<< __LINE__ << std::endl;
        std::cerr << err      << std::endl;
        return pImage; //typename TImageType::Pointer(NULL);
	}
}


////////////////////////////////////////////////////////////////////////////////////////////////////
template< unsigned int ImageDimension >
itk::ImageRegion< ImageDimension > CropBoundingBox(
  const itk::ImageRegion<         ImageDimension >             & inputBoundingBox,
  const itk::FixedArray < double, ImageDimension >             & cropInMMFromBeginning,
  const itk::FixedArray < double, ImageDimension >             & cropInMMFromEnd,
  const typename itk::ImageBase< ImageDimension >::SpacingType & imageSpacing )
{

  // define our types
  typedef itk::ImageRegion<
    ImageDimension
  > RegionType;

  RegionType outputBoundingBox = inputBoundingBox;

  // crop in each dimension
  for ( unsigned int dimension = 0; dimension < ImageDimension; ++dimension )
    {

    // crop current dimension from beginning
    outputBoundingBox = CropBoundingBox( outputBoundingBox,
      imageSpacing[ dimension ],
      dimension,
      cropInMMFromBeginning[ dimension ],
      true );

    // crop current dimension from end
    outputBoundingBox = CropBoundingBox( outputBoundingBox,
      imageSpacing[ dimension ],
      dimension,
      cropInMMFromEnd[ dimension ],
      false );

    } // dimension

  return outputBoundingBox;

} // CropBoundingBox ( with fixed arrays )
////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////
template< class ImageType >
typename ImageType::Pointer CropImage(
  const ImageType                      * inputImage,
  const typename ImageType::RegionType & boundingBox )
{

  // create a filter to crop our image
  typedef itk::RegionOfInterestImageFilter< 
    ImageType,
    ImageType 
  > FilterType;
  typename FilterType::Pointer filter = FilterType::New();

  // make sure that the bounding box
  // is not larger than the image
  // by taking the intersection
  // of the bounding box with
  // the image's buffered region
  typedef typename ImageType::RegionType RegionType;
  const RegionType bufferedRegion = inputImage->GetBufferedRegion();
  RegionType croppedRegion( boundingBox );
  croppedRegion.Crop( bufferedRegion  );

  // there's a bug with really large region sizes unable to be cropped
  if ( boundingBox.GetNumberOfPixels() > bufferedRegion.GetNumberOfPixels() )
    croppedRegion = bufferedRegion;

  // input into the filter
  filter->SetRegionOfInterest( croppedRegion );
  filter->SetInput           ( inputImage    );
 
  // calculate and return
  filter->Update();
  return filter->GetOutput();

} // CropImage
////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////
template< class PixelType, unsigned int ImageDimension >
typename itk::Image< PixelType, ImageDimension >::Pointer CropImage(
  const itk::Image< PixelType, ImageDimension >    * inputImage,
  const itk::FixedArray < double, ImageDimension > & cropInMMFromBeginning,
  const itk::FixedArray < double, ImageDimension > & cropInMMFromEnd )
{

  typedef itk::Image<
    PixelType,
    ImageDimension
  > ImageType;

  // extract the region of the image to crop
  VolumeRegionType cropRegion = CropBoundingBox(
    inputImage->GetBufferedRegion(),
    cropInMMFromBeginning,
    cropInMMFromEnd,
    inputImage->GetSpacing() );

  return CropImage< ImageType >( inputImage, cropRegion );

} // CropImage
////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////
template< class PixelType >
typename itk::Image< PixelType, VolumeDimension >::Pointer CropVolume( 
  const itk::Image< PixelType, VolumeDimension > * inputVolume,
  const double                                     cropInMMFromXStart,
  const double                                     cropInMMFromXEnd,
  const double                                     cropInMMFromYStart,
  const double                                     cropInMMFromYEnd,
  const double                                     cropInMMFromZStart,
  const double                                     cropInMMFromZEnd )
{

  typedef itk::Image<
    PixelType,
    VolumeDimension
  > ImageType;

  // extract the region of the image to crop
  VolumeRegionType cropRegion = CropBoundingBox(
    inputVolume->GetBufferedRegion(),
    cropInMMFromXStart,
    cropInMMFromXEnd,
    cropInMMFromYStart,
    cropInMMFromYEnd,
    cropInMMFromZStart,
    cropInMMFromZEnd,
    inputVolume->GetSpacing() );

  return CropImage< ImageType >( inputVolume, cropRegion );

} // CropVolume
////////////////////////////////////////////////////////////////////////////////////////////////////

template <class TInputImageType>
typename TInputImageType::Pointer 
CropImageBy2PickedPoints(typename TInputImageType::Pointer pInputImage, 
                         typename TInputImageType::IndexType pix1, 
                         typename TInputImageType::IndexType pix2)
{
  typedef typename TInputImageType::Pointer ImagePointerType;
  typedef typename TInputImageType::RegionType RegionType;
  typename TInputImageType::RegionType outRegion;

  typename RegionType::IndexType regionIndex;
  typename RegionType::SizeType  regionSize;

  regionIndex[0] = std::min(pix1[0], pix2[0]);
  regionIndex[1] = std::min(pix1[1], pix2[1]);

  regionSize[0] = std::abs(pix2[0] - pix1[0]) + 1;
  regionSize[1] = std::abs(pix2[1] - pix1[1]) + 1;

  for (unsigned int i = 2; i < pInputImage->ImageDimension; i++)
  {
    regionIndex[i] = pInputImage->GetLargestPossibleRegion().GetIndex(i);
    regionSize[i] = pInputImage->GetLargestPossibleRegion().GetSize(i);
  }

  outRegion.SetIndex(regionIndex);
  outRegion.SetSize(regionSize);

  typedef typename itk::RegionOfInterestImageFilter<TInputImageType, TInputImageType> FilterType;
  typename FilterType::Pointer pFilter = FilterType::New();
  pFilter->SetRegionOfInterest(outRegion);
  pFilter->SetInput(pInputImage);
  pFilter->Update();
  return pFilter->GetOutput();

} // CropImageBy2PickedPoints


template <class TInputImageType>
typename TInputImageType::Pointer 
CropImageBy2PickedPoints(typename TInputImageType::Pointer   pInputImage, 
                         typename TInputImageType::PointType p1, 
                         typename TInputImageType::PointType p2)
{
  typedef typename TInputImageType::Pointer ImagePointerType;
  typedef typename TInputImageType::RegionType RegionType;
  typedef typename TInputImageType::IndexType IndexType;

  IndexType pix1, pix2;
  pInputImage->TransformPhysicalPointToIndex(p1, pix1);
  pInputImage->TransformPhysicalPointToIndex(p2, pix2);

  return CropImageBy2PickedPoints< TInputImageType >(
    pInputImage, pix1, pix2);

} // CropImageBy2PickedPoints(Image, point, point)







} // namespace
#endif // __ccipdCropImage_hxx
