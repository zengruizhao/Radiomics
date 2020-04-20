#ifndef __ccipdGetCornerPoints_hxx
#define __ccipdGetCornerPoints_hxx




//////////////////////////////////////////////////////////////////////////////////////////

// ccipd includes
#include "ccipdGetCornerPoints.h"
#include "ccipdBoundingBoxCoordinates.h"

#include "ccipdDisableWarningsMacro.h"
  // itk includes
  #include <itkImage.h>
  #include <itkPointSet.h>
#include "ccipdEnableWarningsMacro.h"

// std includes
#include <bitset>

//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////
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
  const itk::ImageRegion< TDimension > & region )
{

  typedef itk::Point< LandmarkType, TDimension > PointType;

  const PointType
    minCorner = GetMinBoundingBoxCoordinates< LandmarkType, TDimension >( region, image ),
    maxCorner = GetMaxBoundingBoxCoordinates< LandmarkType, TDimension >( region, image );

  typedef itk::PointSet<
    LandmarkType, TDimension,
    itk::DefaultStaticMeshTraits<
      MeshPixelType, TDimension, TDimension,
      MeshPixelType, MeshPixelType, MeshPixelType >
  > PointSetType;
  typedef itk::SmartPointer< PointSetType > PointSetPointer;

  // create a collection of landmarks with the corners
  const PointSetPointer cornerPoints = PointSetType::New();

  // we will add each corner

  // We need to know whether each element
  // of each corner uses the min
  // or max corner value for that
  // dimension.
  // A trick here is to use a bitset
  // and count from 0 to # of corners
  // in binary. So in 3D, 000 through 111 for
  // example, represents whether
  // each dimension of corners
  // 1 through 8 should contain the
  // min or max value.

  const unsigned int numberOfCorners = static_cast < unsigned int > (
    std::pow(2.0, static_cast< int >( TDimension ) ) );

  for ( unsigned int cornerIndex = 0; cornerIndex < numberOfCorners; ++cornerIndex )
    {

    // this will contain the binary representation
    // of that corner
    typedef std::bitset< TDimension > BinaryType;
    const BinaryType binary( static_cast< unsigned long long> (cornerIndex ) );

    // prepare the points for the given corner
    PointType cornerPoint;

    // loop through each dimension
    for ( unsigned int dimension = 0; dimension < TDimension; ++dimension )
      {

      // extract the boolean representing
      // whether or not to use the min
      // or max value for this corner
      const bool useMax = binary[ dimension ];

      cornerPoint[ dimension ] =
        useMax ? maxCorner[ dimension ] : minCorner[ dimension ];

      } // for each dimension

    // insert the points into the collections
    cornerPoints->SetPoint( cornerIndex, cornerPoint );

    } // cornerIndex

  // output
  return cornerPoints;

} // GetCornerPoints
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
template< unsigned int TDimension >
itk::SmartPointer<
  itk::PointSet<
    LandmarkType, TDimension,
    itk::DefaultStaticMeshTraits<
      MeshPixelType, TDimension, TDimension,
      MeshPixelType, MeshPixelType, MeshPixelType >
  > >
GetCornerPoints( const itk::ImageBase< TDimension > & image )
{
  return GetCornerPoints( image, image.GetBufferedRegion() );
} // GetCornerPoints
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace ccipd
#endif // __ccipdGetCornerPoints_h
