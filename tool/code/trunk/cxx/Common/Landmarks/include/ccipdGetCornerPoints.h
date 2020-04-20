#ifndef __ccipdGetCornerPoints_h
#define __ccipdGetCornerPoints_h



//////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdLandmarkTypes.h"
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{


  
//////////////////////////////////////////////////////////////////////////////////////////
/**
 * Output the points on
 * the corners of an image region.
 */
template< unsigned int TDimension >
itk::SmartPointer<
  itk::PointSet<
    LandmarkType, TDimension,
    itk::DefaultStaticMeshTraits<
      MeshPixelType, TDimension, TDimension,
      MeshPixelType, MeshPixelType, MeshPixelType >
  > >
GetCornerPoints(
  const itk::ImageBase  < TDimension > & image,
  const itk::ImageRegion< TDimension > & region );
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
/**
 * Output the points on
 * the corners of the buffered image.
 */
template< unsigned int TDimension >
itk::SmartPointer<
  itk::PointSet<
    LandmarkType, TDimension,
    itk::DefaultStaticMeshTraits<
      MeshPixelType, TDimension, TDimension,
      MeshPixelType, MeshPixelType, MeshPixelType >
  > >
GetCornerPoints( const itk::ImageBase< TDimension > & image );
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace ccipd
#endif // __ccipdGetCornerPoints_h
