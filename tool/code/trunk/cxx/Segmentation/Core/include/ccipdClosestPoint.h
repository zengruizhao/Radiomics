#ifndef __ccipdClosestPoint_h
#define __ccipdClosestPoint_h



//////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdITKForwardDeclarations.h"
#include "ccipdSegmentationTypes.h"
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Finds the closest point on the surface of a mask.
 * 
 * You need to provide an existing
 * calculator which already has
 * the mask set, and has been computed.
 * 
 * If any errors, occur, then the
 * output point will have NaN's for
 * its components.
 * 
 * \author Rob Toth
 * \date    2013-03-25
 * \ingroup Segmentation
 */
template< typename TPrecision, unsigned int TDimension >
itk::Point< TPrecision, TDimension >
GetClosestSurfacePoint(
  const itk::Point< TPrecision, TDimension >    & point,
  const NormalDirectionCalculator< TDimension > & calculator );
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace ccipd
#endif // __ccipdClosestPoint_h
