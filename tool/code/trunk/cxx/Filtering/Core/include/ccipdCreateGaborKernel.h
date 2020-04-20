#ifndef __ccipdCreateGaborKernel_h
#define __ccipdCreateGaborKernel_h



namespace ccipd
{

/**
 * \addtogroup Filtering
 * @{
 */

 /** \brief Create a Gabor Kernel with desired frequency and rotation. 
   * 
   * These functions will create a Gabor Kernel with desired frequency and rotation. 
   *
   *  \author Rachel Sparks	 
   *  Updated: Ahmad Algohary
   */

/** Funtion to create Gabor Filter. */
//template< class PixelType, unsigned int ImageDimensions >
//typename itk::Image< PixelType, ImageDimensions >::Pointer  CreateGaborKernel(
//  const itk::ImageBase< ImageDimensions > * const templateImage,
//  const float frequency,
//  const float rotation);
//
///** Helper function to rotate Gabor Filter. */
//template< class PixelType, unsigned int ImageDimensions >
//typename itk::Image< PixelType, ImageDimensions >::Pointer  TransformGaborKernel(
//  const itk::Image< PixelType, ImageDimensions > * const GaborKernel, 
//  const float rotation );
//
///** Helper function to  create an unroated Gabor Filter. */
//template< class PixelType, unsigned int ImageDimensions >
//typename itk::Image< PixelType, ImageDimensions >::Pointer  CreateGaborKernel(
//  const itk::ImageBase< ImageDimensions > * const templateImage,
//  const float frequency );


template<class TOutputImageType>
typename TOutputImageType::Pointer 
  CreateGaborKernel(  float frequency
                    , float ThetaZ = 0.0
                   );

} //namespace ccipd

/**
 * @}
 */
#endif