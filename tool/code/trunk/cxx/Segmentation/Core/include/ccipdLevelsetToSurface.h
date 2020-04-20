#ifndef __ccipdLevelsetToSurface_h
#define __ccipdLevelsetToSurface_h



//////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdITKForwardDeclarations.h"
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Turn a levelset image into a surface image.
 * 
 * This will create a (hopefully contiguous) set of
 * pixels with foregroundValue at the desired
 * levelset value, and pixels with backgroundValue
 * everywhere else.
 * 
 * \author  Rob Toth
 * \date    2012-11-04
 * \ingroup Segmentation
 */
template< typename TLevelsetPixelType, typename TMaskPixelType, unsigned int TDimension >
itk::SmartPointer< itk::Image< TMaskPixelType, TDimension > >
GetSurfaceImage(
  const itk::Image< TLevelsetPixelType, TDimension > * const levelset,
  const TLevelsetPixelType                                   levelsetValue   = 0.0,
  const TMaskPixelType                                       backgroundValue = 0.0,
  const TMaskPixelType                                       foregroundValue = 1.0 );
//////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Merge a image with the surface image.
 * 
 * \date    2013-03-26
 * \ingroup Segmentation
 */
template< typename TPixelType, typename TLevelsetPixelType, unsigned int TDimension >
itk::SmartPointer< itk::Image< TPixelType, TDimension > >
MergeImageWithSurfaceImage(
  const itk::Image< TPixelType, TDimension > * const          image,
  const itk::Image< TLevelsetPixelType, TDimension > * const  levelset,
  const TLevelsetPixelType                                    levelsetValue   = 0.0);
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace ccipd
#endif // __ccipdLevelsetToSurface_h
