#ifndef __ccipdMaskToDistanceImage_h
#define __ccipdMaskToDistanceImage_h



//////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdVolumeTypes.h"
#include "ccipdDataDLL.h"
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Calculate the closest distance between two masks for each surface pixel.
 * 
 * This function takes two masks, each pixel on the surface of the segmentation mask 
 * will be replace by the distance to the groundTruth.
 * \author  Rachel Sparks
 * \date    2013-01-29
 * \ingroup Evaluation
 */
template< class TImagePixelType, class TMaskPixelType, unsigned int TDimension >
itk::SmartPointer< itk::Image< TImagePixelType, TDimension > >
MaskToDistanceImage( 
 const itk::Image< TMaskPixelType, TDimension >  * const segmentation,
 const itk::Image< TMaskPixelType, TDimension > *  const groundTruth,
 const float                                       smoothVariance = 0,
 const float                                       increaseSizeInMM = 0 );
 //////////////////////////////////////////////////////////////////////////////////////////
 
 

} // namespace
#endif // __ccipdMaskToDistanceImage_h