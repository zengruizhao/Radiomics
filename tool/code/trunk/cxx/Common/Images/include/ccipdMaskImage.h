#ifndef __ccipdMaskImage_h
#define __ccipdMaskImage_h



//////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdITKForwardDeclarations.h"
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////
/**
 * This will loop through both the image and
 * the mask. If the mask is 0, replace
 * the image with the given maskedValue.
 * 
 * If the mask is non-zero, keep the image value.
 * 
 * If the pixel is outside the bounds of the mask,
 * and maskPixelsOutsideOfMask is true, then
 * the pixel will be replaced with maskedValue.
 * 
 * If the pixel is outside the bounds of the mask,
 * and maskPixelsOutsideOfMask is false, then
 * the pixel will be kept.
 * 
 * \author  Rob Toth
 * \date    2012-11-03
 * \ingroup Common
 */
template< class TImagePixelType, class TMaskPixelType, unsigned int TDimension >
itk::SmartPointer< itk::Image< TImagePixelType, TDimension > >
MaskImage(
  const itk::Image< TImagePixelType, TDimension > * const imageToMask,
  const itk::Image< TMaskPixelType , TDimension > * const maskToApply,
  const TImagePixelType maskedValue = TImagePixelType( 0 ),
  const bool            maskPixelsOutsideOfMask = true );
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace ccipd
#endif // __ccipdMaskImage_h
