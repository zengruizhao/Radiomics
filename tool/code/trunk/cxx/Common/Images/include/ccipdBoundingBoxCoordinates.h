#ifndef __ccipdBoundingBoxCoordinates_h
#define __ccipdBoundingBoxCoordinates_h


//////////////////////////////////////////////////////////////////////////////////////////
// ccipd includes
#include "ccipdITKForwardDeclarations.h"
#include "ccipdCore.h"
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////
/**
 * Extract the coordinates
 * of the "minimum" corner of the
 * bounding box.
 * 
 * \author  Rob Toth
 * \ingroup Common
 * \date    2013-02-03
 */
template< typename TScalar,unsigned int TDimension >
itk::Point< TScalar, TDimension >
GetMinBoundingBoxCoordinates(
  const itk::ImageRegion< TDimension > & boundingBox,
  const itk::ImageBase  < TDimension > & image );
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
/**
 * Extract the coordinates
 * of the "maximum" corner of the
 * bounding box.
 * 
 * \author  Rob Toth
 * \ingroup Common
 * \date    2013-02-03
 */
template< typename TScalar, unsigned int TDimension >
itk::Point< TScalar, TDimension >
GetMaxBoundingBoxCoordinates(
  const itk::ImageRegion< TDimension > & boundingBox,
  const itk::ImageBase  < TDimension > & image );
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace ccipd
#endif // __ccipdBoundingBoxCoordinates_h
