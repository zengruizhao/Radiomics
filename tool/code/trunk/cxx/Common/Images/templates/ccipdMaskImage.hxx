


//////////////////////////////////////////////////////////////////////////////////////////
// ccipd includes
#include "ccipdMaskImage.h"
#include "ccipdDuplicateImage.h" // in case no mask is input

#include "ccipdDisableWarningsMacro.h"
  // itk includes
  #include <itkImageRegionIterator.h>
  #include <itkImageRegionConstIteratorWithIndex.h>
  #include <itkNearestNeighborInterpolateImageFunction.h> // for getting mask values
#include "ccipdEnableWarningsMacro.h"
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////
template< class TImagePixelType, class TMaskPixelType, unsigned int TDimension >
itk::SmartPointer< itk::Image< TImagePixelType, TDimension > >
MaskImage(
  const itk::Image< TImagePixelType, TDimension > * const imageToMask,
  const itk::Image< TMaskPixelType , TDimension > * const maskToApply,
  const TImagePixelType maskedValue,
  const bool            maskPixelsOutsideOfMask )
{

  typedef itk::Image< TImagePixelType, TDimension > ImageType;
  typedef itk::Image< TMaskPixelType , TDimension > MaskType;
  typedef itk::SmartPointer< ImageType > ImagePointer;

  // make sure we have everything
  if ( !imageToMask )
    return ImagePointer();

  if ( !maskToApply )
    return DuplicateImage( imageToMask );

  // set up iterators

  // what is the image region?
  const typename ImageType::RegionType region =
    imageToMask->GetBufferedRegion();

  // prepare the output
  const ImagePointer maskedImage = ImageType::New();
  maskedImage->SetOrigin   ( imageToMask->GetOrigin   () );
  maskedImage->SetSpacing  ( imageToMask->GetSpacing  () );
  maskedImage->SetDirection( imageToMask->GetDirection() );
  maskedImage->SetRegions  ( region );
  maskedImage->Allocate();

  // prepare the iterators
  typedef itk::ImageRegionConstIteratorWithIndex<
    ImageType
  > InputIteratorType;
  typedef itk::ImageRegionIterator<
    ImageType
  > OutputIteratorType;

  InputIteratorType  inputIterator ( imageToMask, region );
  OutputIteratorType outputIterator( maskedImage, region );

  // we will need to interpolate to get the mask locations
  typedef itk::NearestNeighborInterpolateImageFunction<
    MaskType
  > InterpolatorType;

  const typename InterpolatorType::Pointer
    interpolator = InterpolatorType::New();

  interpolator->SetInputImage( maskToApply );

  // iterate through each pixel
  for ( inputIterator.GoToBegin(), outputIterator.GoToBegin();
    !inputIterator.IsAtEnd(); ++inputIterator, ++outputIterator )
    {

    // get the point in real space
    const typename InputIteratorType::IndexType currentIndex =
      inputIterator.GetIndex();

    typename ImageType::PointType currentPoint;
    imageToMask->TransformIndexToPhysicalPoint( currentIndex, currentPoint );

    // see if the point is inside the mask
    const bool isInsideMask =
      interpolator->IsInsideBuffer( currentPoint );

    if ( isInsideMask )
      {

      const MaskPixelType maskValue = 
        static_cast< MaskPixelType >(
          interpolator->Evaluate( currentPoint ) );

      // either keep the image or give it a new value
      const TImagePixelType newPixelValue = 
        ( maskValue == 0 ) ?
          maskedValue :
          inputIterator.Get();

      outputIterator.Set( newPixelValue );

      } 
    else // outside mask
      {

      // the user chooses what to do
      // when we fall outside the mask
      const TImagePixelType newPixelValue =
        ( maskPixelsOutsideOfMask ) ?
          maskedValue :
          inputIterator.Get();

      outputIterator.Set( newPixelValue );

      } // outside mask

    } // for iterators

  // output
  return maskedImage;

} // MaskImage
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace ccipd
