#ifndef __ccipdDilateImage_h
#define __ccipdDilateImage_h



//////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdVolumeTypes.h"
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Dilate an image by a certain amount
 * 
 * This function takes in an image and dilates it by a certain size.
 *
 * \author  Rachel Sparks
 * \date    2013-01-31
 * \ingroup Common
 */
template< class TPixelType, unsigned int TDimension >
itk::SmartPointer< itk::Image< TPixelType, TDimension > >
DilateImage( 
 const itk::Image< TPixelType, TDimension >  * const image );
 //////////////////////////////////////////////////////////////////////////////////////////
 
 

} // namespace
#endif // __ccipdDilateImage_h