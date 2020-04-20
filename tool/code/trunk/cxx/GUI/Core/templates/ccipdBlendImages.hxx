#ifndef __ccipdBlendImages_hxx
#define __ccipdBlendImages_hxx



//////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdBlendImages.h"
#include "ccipdResizeImage.h"

// std includes
#include <limits>

#include "ccipdDisableWarningsMacro.h"
  // itk includes
  #include <itkImageRegionIterator.h>      // for creating a new image
  #include <itkImageRegionConstIterator.h> // for reading existing images
#include "ccipdEnableWarningsMacro.h"
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////
template< class TPixelType, unsigned int Dimension >
itk::SmartPointer< itk::Image< TPixelType, Dimension > >
BlendImages(
  const itk::Image< TPixelType   , Dimension > * const image1,
  const itk::Image< MaskPixelType, Dimension > * const mask1,
  const itk::Image< TPixelType   , Dimension > * const image2,
  const itk::Image< MaskPixelType, Dimension > * const mask2
  )
{

  // typedefs
  typedef itk::Image< TPixelType   , Dimension > ImageType;
  typedef itk::Image< MaskPixelType, Dimension > MaskType;
  typedef itk::SmartPointer<       ImageType >   ImagePointer;
  typedef itk::SmartPointer< const ImageType >   ImageConstPointer;
  typedef itk::SmartPointer< const MaskType  >   MaskConstPointer;

  // set up the extrapolate default value
  const TPixelType defaultPixel( (unsigned char)0 );

  const ImageConstPointer templateImage = image1;

  // first, we must make sure everything is in the reference frame of image1
  const ImageConstPointer
    resizedImage1( ResizeImage< TPixelType, Dimension >(
      image1, templateImage, false, defaultPixel ) ),
    resizedImage2( ResizeImage< TPixelType, Dimension >(
      image2, templateImage, false, defaultPixel ) );
  const MaskConstPointer
    resizedMask1( ResizeMask< Dimension >(
      mask1, templateImage, false, 0 ) ),
    resizedMask2( ResizeMask< Dimension >(
      mask2, templateImage, false, 0 ) );

  // prepare the region to iterate
  const typename ImageType::RegionType region = templateImage->GetBufferedRegion();

  // prepare the output
  const ImagePointer blendedImage = ImageType::New();
  blendedImage->SetOrigin   ( templateImage->GetOrigin   () );
  blendedImage->SetDirection( templateImage->GetDirection() );
  blendedImage->SetSpacing  ( templateImage->GetSpacing  () );
  blendedImage->SetRegions  ( region );
  blendedImage->Allocate();
  blendedImage->FillBuffer( defaultPixel );

  // prepare the iterators
  typedef itk::ImageRegionConstIterator< ImageType > ImageInputIterator;
  typedef itk::ImageRegionConstIterator< MaskType  > MaskInputIterator;
  typedef itk::ImageRegionIterator     < ImageType > ImageOutputIterator;
  
  ImageInputIterator
    image1Iterator( resizedImage1, region ),
    image2Iterator( resizedImage2, region );
  MaskInputIterator
    mask1Iterator( resizedMask1, region ),
    mask2Iterator( resizedMask2, region );
  ImageOutputIterator
    blendedIterator( blendedImage, region );

  // how will we define opacity?
  typedef float OpacityType;
  const OpacityType maxMaskValue = std::numeric_limits< MaskPixelType >::max();

  // iterate through the same pixels in all images
  for (
    image1Iterator .GoToBegin(),
    image2Iterator .GoToBegin(),
    mask1Iterator  .GoToBegin(),
    mask2Iterator  .GoToBegin(),
    blendedIterator.GoToBegin();
    !blendedIterator.IsAtEnd();
    ++image1Iterator,
    ++image2Iterator,
    ++mask1Iterator,
    ++mask2Iterator,
    ++blendedIterator )
    {

    // what's the opacity of each image pixel?
    const OpacityType
      image1Opacity = static_cast< OpacityType >( mask1Iterator.Get() ) / maxMaskValue,
      image2Opacity = static_cast< OpacityType >( mask2Iterator.Get() ) / maxMaskValue;

    // what are the pixel values?
    const TPixelType
      image1Value = image1Iterator.Get(),
      image2Value = image2Iterator.Get();

    const bool image1OnTop = image1Opacity < image2Opacity;

    // see http://en.wikipedia.org/wiki/Alpha_compositing

    // the more opaque one will go in back

    const OpacityType
      topOpacity    = image1OnTop ? image1Opacity : image2Opacity,
      bottomOpacity = image1OnTop ? image2Opacity : image1Opacity,
      topTransparency = 1.0f - topOpacity,
      combinedOpacity = topOpacity + bottomOpacity * topTransparency;
    
    const TPixelType
      topValue    = image1OnTop ? image1Value : image2Value,
      bottomValue = image1OnTop ? image2Value : image1Value;

    // now calculate the blended values
    const TPixelType blendedValue =
      ( ( topValue * (unsigned char)topOpacity ) +
      ( bottomValue * (unsigned char)bottomOpacity * (unsigned char)topTransparency ) ) *
      (unsigned char)combinedOpacity;

    // assign it
    blendedIterator.Set( blendedValue );

    } // iterators

  return blendedImage;

} // BlendImages
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace ccipd
#endif // __ccipdBlendImages_hxx
