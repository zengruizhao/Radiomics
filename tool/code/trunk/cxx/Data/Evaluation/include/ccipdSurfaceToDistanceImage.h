#ifndef __ccipdSurfaceMaskToDistanceImage_h
#define __ccipdSurfaceMaskToDistanceImage_h




//////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdITKForwardDeclarations.h"
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



/**
 * \brief From surface mask image calculate closest distance to ground truth
 *        
 *  Inputs to the function are (1) surfaceMask, (2) levelsetImage
 * 
 * \author  Rachel Sparks
 * \date    2013-01-29
 * \ingroup Evaluation
 */
template< class TImagePixelType, class TMaskPixelType, unsigned int TDimension >
itk::SmartPointer< itk::Image< TImagePixelType, TDimension > >
SurfaceToDistanceImage( 
 const itk::Image< TMaskPixelType, TDimension >  * const surfaceMask,
 const itk::Image< TImagePixelType, TDimension > * const levelSet
 );


} // namespace ccipd

#endif // __ccipdSurfaceMaskToDistanceImage_h
