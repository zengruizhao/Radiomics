#ifndef __ccipdBoundingBoxRandomization_h
#define __ccipdBoundingBoxRandomization_h



//////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdDistributionTypes.h"
#include "ccipdITKForwardDeclarations.h"
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////
/**
 * This will randomly expand
 * or contract the corners of a bounding box.
 * 
 * Random values from distribution
 * will define exactly how
 * much to expand (>0) or contract (<0)
 * each corner.
 * 
 * The output bounding box
 * will be required to fit within
 * the image's buffered region.
 */
template< unsigned int TDimension >
itk::ImageRegion< TDimension >
RandomizeBoundingBox(
  const itk::ImageRegion< TDimension >   & inputBoundingBox,
  const itk::ImageBase  < TDimension >   & image,
  const UnivariateDistribution< double > & distribution );
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace ccipd
#endif // __ccipdBoundingBoxRandomization_h
