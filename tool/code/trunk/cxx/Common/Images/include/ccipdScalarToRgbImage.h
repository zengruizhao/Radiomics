#ifndef __ccipdccipdScalarToRGBImage_h
#define __ccipdccipdScalarToRGBImage_h


//////////////////////////////////////////////////////////////////////////////////////////
// ccipd includes
#include "ccipdScalarToRGBImage.hxx"
#include "ccipdITKForwardDeclarations.h"
//////////////////////////////////////////////////////////////////////////////////////////

namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////
/**
 * Converts Scalar Image to RGB Image
 * 
 * \author  Ahmad Algohary
 * \ingroup Common Images
 * \date    10/06/2013
 */
template< class TScalarImage, class TRGBImage >
typename TRGBImage::Pointer
ScalarToRGBImage(  typename TScalarImage::Pointer   pInputImage
                 , typename TScalarImage::PixelType ThresholdMin
                 , typename TScalarImage::PixelType ThresholdMax
                );
//////////////////////////////////////////////////////////////////////////////////////////

} // namespace ccipd
#endif 
