#ifndef __ccipdBlendImages_h
#define __ccipdBlendImages_h



//////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdCore.h"
#include "ccipdITKForwardDeclarations.h"
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Create a third image which is a blend of 2 images.
 * 
 * \author  Rob Toth
 * \date    2012-05-09
 * \ingroup Common
 */
template< class TPixelType, unsigned int Dimension >
itk::SmartPointer< itk::Image< TPixelType, Dimension > >
BlendImages(
  const itk::Image< TPixelType   , Dimension > * const image1,
  const itk::Image< MaskPixelType, Dimension > * const mask1,
  const itk::Image< TPixelType   , Dimension > * const image2,
  const itk::Image< MaskPixelType, Dimension > * const mask2
  );
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace ccipd
#endif // __ccipdBlendImages_h
