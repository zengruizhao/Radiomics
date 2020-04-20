#ifndef __ccipdGetLargestContiguousRegion_h
#define __ccipdGetLargestContiguousRegion_h



//////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdCore.h"
#include "ccipdITKForwardDeclarations.h"
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////
/**
 * Output a mask containing
 * the largest contiguous region
 * in the input mask.
 * 
 * \param[in] numberOfLargestRegions
 *   How many of the largest regions should
 *   be retained? If 2, for example,
 *   then the 2 largest regions in the input
 *   mask will be retained in the output mask.
 * 
 * \note The output will have the same labels as the input.
 * 
 * \author  Rob Toth
 * \date    2013-04-03
 * \ingroup Segmentation
 */
template< typename TPixelType, unsigned int TDimension >
itk::SmartPointer< itk::Image< TPixelType, TDimension > >
GetLargestContiguousRegions(
  const itk::Image< TPixelType, TDimension > * const image );
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Largest non-zero region in the image
 */
template< typename TPixelType, unsigned int TDimension >
itk::SmartPointer< itk::Image< TPixelType, TDimension > >
GetLargestContiguousRegion(
  const itk::Image< TPixelType, TDimension > * const image );
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace ccipd
#endif // __ccipdGetLargestContiguousRegion_h
