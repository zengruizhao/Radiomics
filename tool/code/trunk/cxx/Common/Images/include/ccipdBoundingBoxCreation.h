#ifndef __ccipdBoundingBoxCreation_h
#define __ccipdBoundingBoxCreation_h



//////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdCore.h"
#include "ccipdITKForwardDeclarations.h"
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{
/**
 * \brief 
 *   Create a bounding box
 *   from many points.
 */
template< unsigned int TDimension >
itk::ImageRegion< TDimension >
CreateBoundingBox(
  const itk::PointSet<
    LandmarkType,
    TDimension,
    itk::DefaultStaticMeshTraits<
      MeshPixelType,
      TDimension,
      TDimension,
      MeshPixelType,
      MeshPixelType,
      MeshPixelType
  > > & points,
  const itk::ImageBase< TDimension > & image );
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
/**
 * \brief 
 *   Create a bounding box
 *   from two corners.
 */
template< unsigned int TDimension >
itk::ImageRegion< TDimension >
CreateBoundingBox(
  const itk::Point< double, TDimension > & corner1,
  const itk::Point< double, TDimension > & corner2,
  const itk::ImageBase<     TDimension > & image );
//////////////////////////////////////////////////////////////////////////////////////////
/** \brief  : 
  * \Created: axa667 7/17/2013
  * \Updated:  
  * \ingroup:
  * \param  : 
  * \param  : 
  * \return : 
  * 
  * 
  * 
  * 
  */
template< unsigned int TDimension >
itk::ImageRegion< TDimension >
  CreateBoundingBox2(
  const itk::PointSet<
  LandmarkType,
  TDimension,
  itk::DefaultStaticMeshTraits<
  MeshPixelType,
  TDimension,
  TDimension,
  MeshPixelType,
  MeshPixelType,
  MeshPixelType
  > > & points,
  const itk::ImageBase< TDimension > & image );

} // namespace ccipd
#endif // __ccipdBoundingBoxCreation_h
