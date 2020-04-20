#ifndef __ccipdBoundingBoxCoordinates_hxx
#define __ccipdBoundingBoxCoordinates_hxx



//////////////////////////////////////////////////////////////////////////////////////////
// ccipd includes
#include "ccipdCore.h"
#include "ccipdBoundingBoxCoordinates.h"

#include "ccipdDisableWarningsMacro.h"
  #include <itkImageBase.h>
#include "ccipdEnableWarningsMacro.h"
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{
    
    

//////////////////////////////////////////////////////////////////////////////////////////
template< typename TScalar, unsigned int TDimension >
itk::Point< TScalar, TDimension >
GetMinBoundingBoxCoordinates(
  const itk::ImageRegion< TDimension > & boundingBox,
  const itk::ImageBase  < TDimension > & image )
{

  // get the index of the min corner
  const auto minIndex = boundingBox.GetIndex();

  // convert it to real coordiantes
  itk::Point< TScalar, TDimension > minCoordinates;

  image.TransformIndexToPhysicalPoint( minIndex, minCoordinates );

  // output
  return minCoordinates;
  
} // GetMinBoundingBoxCoordinates
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
template< typename TScalar, unsigned int TDimension >
itk::Point< TScalar, TDimension >
GetMaxBoundingBoxCoordinates(
  const itk::ImageRegion< TDimension > & boundingBox,
  const itk::ImageBase  < TDimension > & image )
{

  // get the index of the min corner
  const auto maxIndex = boundingBox.GetUpperIndex();

  // convert it to real coordiantes
  itk::Point< TScalar, TDimension > maxCoordinates;

  image.TransformIndexToPhysicalPoint( maxIndex, maxCoordinates );

  // output
  return maxCoordinates;

} // GetMaxBoundingBoxCoordinates
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace ccipd
#endif // __ccipdBoundingBoxCoordinates_hxx
