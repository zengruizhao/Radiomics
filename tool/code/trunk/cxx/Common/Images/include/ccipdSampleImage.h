#ifndef __ccipdSampleImage_h
#define __ccipdSampleImage_h



////////////////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdDisableWarningsMacro.h"
  #include <itkInterpolateImageFunction.h>       // you can specify an interpolator
  #include <itkLinearInterpolateImageFunction.h> // default interpolator
  #include <itkVectorImageToImageAdaptor.h>
  #include <vector>
#include "ccipdEnableWarningsMacro.h"
////////////////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * You input the physical locations, the output iterator, and the interpolator.
 *
 * You can input your own interpolator.
 * 
 * Your output iterator type must provide the "+" operator
 * for incrementing, and the "*" for dereferencing and assigning.
 *
 * Also, you can choose if you want to extrapolate pixels (nearest neighbor)
 * outside the image.
 * If you choose not to extrapolate, then they will be filled in with
 * the value specified.
 * 
 * \author  Rob Toth
 * \ingroup Common
 */
template < class TImage, class TInterpolator, class TExtrapolator, class TOutputIter >
void SampleImage(

  const std::vector< typename TImage::PointType > locations,
  const TImage *                                  inputImage,
  TOutputIter                                     outputIter,
  TInterpolator *                                 interpolator,
  const bool extrapolatePixelsOutsideImage = false,
  const typename TImage::PixelType valueForPixelsOutsideImage = 0 );
////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *  \brief This version uses a default linear interpolator
 * 
 * \ingroup Common
 */
template < class TImage, class TOutputIter >
void SampleImage(

  const std::vector< typename TImage::PointType > locations,
  const TImage *                                  inputImage,
  TOutputIter                                     outputIter,
  const bool extrapolatePixelsOutsideImage = false,
  const typename TImage::PixelType valueForPixelsOutsideImage = 0 );
////////////////////////////////////////////////////////////////////////////////////////////////////




////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * For sampling an itk::VectorImage.
 * 
 * An example of TOutputIter is std::vector< std::vector >::iterator
 * 
 * TOutputIter needs to have the [] operator defined.
 * 
 * (*outputIter) is a container to store the sampled
 * pixels of the first pixel
 * 
 * *(outputIter + 1) is a reference to an output container
 * for the second pixel.
 * 
 * *(outputIter + 1)[2] will contain the second pixel
 * of the third component
 *
 * Additionally we assume that TOutputIter has already been allocated
 * as no range checking is performed.
 * 
 * \ingroup Common
 */
template < class TImage, class TOutputIter >
void SampleVectorImageRowWise(

  const std::vector< typename TImage::PointType > locations,
  const TImage                                   * inputImage,
  TOutputIter                                      outputIter,

  itk::InterpolateImageFunction < 
    itk::VectorImageToImageAdaptor <
      typename TImage::InternalPixelType,
      TImage::ImageDimension
	> >                                       *  interpolator,

  const bool extrapolatePixelsOutsideImage = false,

  const typename TImage::InternalPixelType valueForPixelsOutsideImage = 0 );
////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * (*outputIter) defines a container to store the sampled pixels
 * from the first component.
 * 
 * (*outputIter) needs to have the begin() function defined.
 * 
 * ( *(outputIter + 1) ) defines a container to store the
 * sampled pixels from the second component.
 * 
 * This begin() function is actually passed to SampleImage
 * to store the output for a given component of the VectorImage.
 * 
 * (outputIter + 1)->begin() would represent a pointer
 * to a container of sampled values for component
 * index 1, for example.
 * 
 * \ingroup Common
 */
template < class TImage, class TOutputIter >
void SampleVectorImageColWise(

  const std::vector< typename TImage::PointType > locations,
  const TImage                                   * inputImage,
  TOutputIter                                      outputIter,

  itk::InterpolateImageFunction < 
    itk::VectorImageToImageAdaptor <
      typename TImage::InternalPixelType,
      TImage::ImageDimension
	> >                                       *  interpolator,

  const bool extrapolatePixelsOutsideImage = false,

  const typename TImage::InternalPixelType valueForPixelsOutsideImage = 0 );
////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * \brief This version uses a default linear interpolator
 * 
 * \ingroup Common
 */
template < class TImage, class TOutputIter >
void SampleVectorImageRowWise(

  const std::vector< typename TImage::PointType >  locations,
  const TImage                                   * inputImage,
  TOutputIter                                      outputIter,
  const bool                                       extrapolatePixelsOutsideImage = false,
  const typename TImage::InternalPixelType         valueForPixelsOutsideImage = 0 );

/**
 * This version uses a default linear interpolator
 * 
 * \ingroup Common
 */
template < class TImage, class TOutputIter >
void SampleVectorImageColWise(
  const std::vector< typename TImage::PointType >  locations,
  const TImage                                   * inputImage,
  TOutputIter                                      outputIter,
  const bool                                       extrapolatePixelsOutsideImage = false,
  const typename TImage::InternalPixelType         valueForPixelsOutsideImage = 0 );
////////////////////////////////////////////////////////////////////////////////////////////////////



} // namespace



////////////////////////////////////////////////////////////////////////////////////////////////////
// this is where the implementation is actually included in the header
#include "ccipdSampleImage.hxx"
////////////////////////////////////////////////////////////////////////////////////////////////////



#endif // __ccipdSampleImage_h
