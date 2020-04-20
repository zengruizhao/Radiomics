#ifndef __ccipdRegionCube_hxx
#define __ccipdRegionCube_hxx



//////////////////////////////////////////////////////////////////////////////////////////////////

// ccipd includes
#include "ccipdRegionCube.h"
#include "ccipdFactory.hxx"
#include "ccipdBoundingBoxCreation.h"

// itk includes
#include "ccipdDisableWarningsMacro.h"
#include <itkImageBase.h>
#include "ccipdEnableWarningsMacro.h"

//////////////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////////////
template< unsigned int TDimension >
class RegionCube< TDimension >::SecretData
{

public:

  typedef RegionCube< TDimension >                       EncapsulatingClass;
  typedef typename EncapsulatingClass::ImageRegionType   ImageRegionType;
  typedef typename EncapsulatingClass::LengthType        LengthType;
  typedef typename EncapsulatingClass::PointType         PointType;
  typedef typename EncapsulatingClass::ImageConstPointer ImageConstPointer;

  PointType
    m_Centroid;

  /// Displacement from the centroid in each dimension
  LengthType
    m_HalfCubeLength;

  /// Cache the calculated region
  ImageRegionType
    m_MaximumRegion;

  /// Recompute the maximum region
  void UpdateCachedRegion( const ImageConstPointer image );

}; // class SecretData
//////////////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////////////
template< unsigned int TDimension >
RegionCube< TDimension >::RegionCube() :
ccipdNewPIMPLMacro
{
  this->m_SecretData->m_Centroid.Fill( 0 );
}
//////////////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////////////
template< unsigned int TDimension >
RegionCube< TDimension >::~RegionCube()
{
}
//////////////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////////////
template< unsigned int TDimension >
void RegionCube< TDimension >::SecretData::UpdateCachedRegion(
  const ImageConstPointer image )
{

  // we will set the radius here in mm
  const LengthType  length   = this->m_HalfCubeLength;
  const PointType & centroid = this->m_Centroid;

  if ( !image )
    {
    this->m_MaximumRegion = ImageRegionType();
    return;
    }

  PointType
    minCorner = centroid,
    maxCorner = centroid;
  for ( unsigned int dimension = 0; dimension < TDimension; ++dimension )
    {
    minCorner[ dimension ] -= length;
    maxCorner[ dimension ] += length;
    } // dimension

  // create a bounding box from those coordinates
  ImageRegionType imageRegion =
    CreateBoundingBox( minCorner, maxCorner, *image );

  // store it
  this->m_MaximumRegion = imageRegion;

} // UpdateCachedRegion
//////////////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////////////
template< unsigned int TDimension >
bool RegionCube< TDimension >::IncludePointInShape( const PointType & point ) const
{

  const ImageConstPointer image = this->GetImage();
  if ( !image ) return false;

  // convert it to an index
  typename ImageType::IndexType index;
  image->TransformPhysicalPointToIndex( point, index );

  // see if it's inside our cached region
  const bool isInside = this->m_SecretData->m_MaximumRegion.IsInside( index );

  // output
  return isInside;

} // IncludePointInShape
//////////////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////////////
template< unsigned int TDimension >
void RegionCube< TDimension >::SetHalfCubeLength( const LengthType variable )
{
  ccipdSetBodyMacro( HalfCubeLength )
  this->m_SecretData->UpdateCachedRegion( this->GetImage() );
}
//////////////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////////////
template< unsigned int TDimension >
void RegionCube< TDimension >::SetImage( const ImageConstPointer image )
{
  this->Superclass::SetImage( image );
  this->m_SecretData->UpdateCachedRegion( this->GetImage() );
}
//////////////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////////////
template< unsigned int TDimension >
void RegionCube< TDimension >::SetCentroid( const PointType & variable )
{
  ccipdSetBodyMacro( Centroid )
  this->m_SecretData->UpdateCachedRegion( this->GetImage() );
}
//////////////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////////////
template< unsigned int TDimension >
ccipdGetConstImplementMacro( RegionCube< TDimension >, MaximumRegion, 
  typename RegionCube< TDimension >::ImageRegionType )
//////////////////////////////////////////////////////////////////////////////////////////////////



} // namespace ccipd
#endif // __ccipdRegionCube_hxx
