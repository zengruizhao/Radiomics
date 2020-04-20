#ifndef __ccipdGetDiagonalDistance_hxx
#define __ccipdGetDiagonalDistance_hxx



//////////////////////////////////////////////////////////////////////////////////////////

// ccipd includes
#include "ccipdGetDiagonalDistance.h"

// itk includes
#include "ccipdDisableWarningsMacro.h"
#include <itkImageBase.h>
#include "ccipdEnableWarningsMacro.h"

//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////
template< unsigned int TDimension >
TransformValueType GetDiagonalDistance(
  const itk::ImageBase< TDimension > & image )
{

  typedef itk::ImageBase< TDimension > ImageType;

  // use the highest corner of the image
  const auto largestRegion = image.GetLargestPossibleRegion();
  const typename ImageType::IndexType
    minIndex = largestRegion.GetIndex(),
    maxIndex = largestRegion.GetUpperIndex();

  typename ImageType::PointType minPoint, maxPoint;
  image.TransformIndexToPhysicalPoint( minIndex, minPoint );
  image.TransformIndexToPhysicalPoint( maxIndex, maxPoint );

  // the max radius spans the entire image diagonal
  const TransformValueType maxDistance =
    static_cast< TransformValueType >(
      maxPoint.EuclideanDistanceTo( minPoint ) );

  // output
  return maxDistance;

}
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
template< unsigned int TDimension >
TransformValueType GetDiagonalDistance(
  const itk::ImageBase< TDimension > * const image )
{
  
  // by default, just a really large value'
  if ( image )
    return GetDiagonalDistance( *image );
  else
    return std::numeric_limits< TransformValueType >::max() / 4;

}
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace ccipd
#endif // __ccipdGetDiagonalDistance_hxx
