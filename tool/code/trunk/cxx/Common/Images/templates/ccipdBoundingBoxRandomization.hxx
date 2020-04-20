#ifndef __ccipdBoundingBoxRandomization_hxx
#define __ccipdBoundingBoxRandomization_hxx



//////////////////////////////////////////////////////////////////////////////////////////
// ccipd includes
#include "ccipdBoundingBoxRandomization.h"
#include "ccipdBoundingBoxCoordinates.h"
#include "ccipdBoundingBoxCreation.h"
#include "ccipdUnivariateDistribution.h"

#include "ccipdDisableWarningsMacro.h"
  // itk includes
  #include <itkImageBase.h>
#include "ccipdEnableWarningsMacro.h"
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
  const UnivariateDistribution< double > & distribution )
{

  typedef itk::Point< double, TDimension > PointType;

  // we extract the coordinates of the input bounding box
  PointType
    minCorner = GetMinBoundingBoxCoordinates< double, TDimension>( inputBoundingBox, image ),
    maxCorner = GetMaxBoundingBoxCoordinates< double, TDimension>( inputBoundingBox, image );

  // now we randomly expand or extract each corner
  for ( unsigned int dimension = 0; dimension < TDimension; ++dimension )
    {

    minCorner[ dimension ] -= distribution.GetRandomValue();
    maxCorner[ dimension ] += distribution.GetRandomValue();

    } // for each dimension

  // create a bounding box from the new,
  // randomly changed, corners
  auto outputBoundingBox =
    CreateBoundingBox( minCorner, maxCorner, image );

  // make sure we are fully inside the image
  outputBoundingBox.Crop( image.GetBufferedRegion() );

  // output
  return outputBoundingBox;

} // RandomizeBoundingBox
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace ccipd
#endif // __ccipdBoundingBoxRandomization_hxx
