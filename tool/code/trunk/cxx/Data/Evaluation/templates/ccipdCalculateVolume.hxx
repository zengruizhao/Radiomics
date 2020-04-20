#ifndef __ccipdCalculateVolume_hxx
#define __ccipdCalculateVolume_hxx


 
//////////////////////////////////////////////////////////////////////////////////////////
// ccipd includes
#include "ccipdCalculateVolume.h"

#include "ccipdDisableWarningsMacro.h"
  // itk includes
  #include <itkImage.h>
  #include <itkImageRegionConstIterator.h>
#include "ccipdEnableWarningsMacro.h"
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{


  
//////////////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Get the volume of a single pixel in the image.
 */
template< unsigned int TDimension >
EvaluationValueType GetSinglePixelVolume(
  const itk::ImageBase< TDimension > & image )
{

  typedef itk::ImageBase< TDimension > ImageType;

  // get the size of each dimension in mm
  const typename ImageType::SpacingType
    pixelSpacing = image.GetSpacing();

  // get the size of each pixel in mm^3

  typename ImageType::SpacingValueType
    pixelVolume = 1;

  for ( unsigned int dimension = 0; dimension < TDimension; ++dimension )
    pixelVolume *= pixelSpacing[ dimension ];

  // output
  return static_cast< EvaluationValueType >( pixelVolume );

} // GetSinglePixelVolume
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Get the volume of the entire buffered image.
 */
template< unsigned int TDimension >
EvaluationValueType GetTotalImageVolume(
  const itk::ImageBase< TDimension > & image )
{

  // we need the # of pixels in the image
  const auto numberOfPixels = image.GetBufferedRegion().GetNumberOfPixels();

  // we need the size of each pixel
  // and then we need the volume
  const EvaluationValueType
    pixelVolume = GetSinglePixelVolume( image ),
    volume = static_cast< EvaluationValueType >(
      numberOfPixels * pixelVolume );

  // output
  return volume;

} // GetTotalImageVolume
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
template< typename TPixelType, unsigned int TDimension >
EvaluationValueType GetVolume(
  const itk::Image< TPixelType, TDimension > * const image,
  const TPixelType                                   pixelValue )
{

  if ( !image ) return 0;

  // typedefs
  typedef itk::Image< TPixelType, TDimension >       ImageType;
  typedef itk::ImageRegionConstIterator< ImageType > IteratorType;

  // create an iterator
  IteratorType iterator( image, image->GetBufferedRegion() );

  // initialize our count
  itk::SizeValueType numberOfPixelsWithValue = 0;
  
  // iterate through the image
  for ( iterator.GoToBegin(); !iterator.IsAtEnd(); ++iterator )
    {

    const TPixelType currentValue = iterator.Get();

    if ( currentValue == pixelValue )
      ++numberOfPixelsWithValue;

    } // for iterator

  // now that we know the # of pixels with the given value,
  // we need to know the size of each pixel
  // in mm^3
  const EvaluationValueType pixelVolume = GetSinglePixelVolume( *image );

  // output the number of pixels
  // multiplied by the volume of
  // each pixel
  const EvaluationValueType volume = static_cast< EvaluationValueType >(
    pixelVolume * numberOfPixelsWithValue );

  // output
  return volume;

} // GetVolume (pixelValue )
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
template< typename TPixelType, unsigned int TDimension >
EvaluationValueType GetVolume(
  const itk::Image< TPixelType, TDimension > * const image )
{

  if ( !image ) return 0;

  // simply subtract the 
  // volume of "0" pixels
  // from the volume of
  // the image as a whole
  const EvaluationValueType
    volumeZero       = GetVolume< TPixelType, TDimension >( image, 0 ),
    volumeTotal      = GetTotalImageVolume( *image ),
    volumeSubtracted = volumeTotal - volumeZero;

  // output
  return volumeSubtracted;

} // GetVolume
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace ccipd
#endif // __ccipdCalculateVolume_hxx
