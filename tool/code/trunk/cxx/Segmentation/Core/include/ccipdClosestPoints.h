#ifndef __ccipdClosestPoints_h
#define __ccipdClosestPoints_h



//////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdITKForwardDeclarations.h"
#include "ccipdSegmentationTypes.h"
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{

  

//////////////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Finds closest points on a mask.
 * 
 * You give it a collection of points.
 * It will also output a collection of points.
 * 
 * For each point in the input space, the distance
 * to the inputMaskImage is calculated.
 * 
 * The closest point on the outputMaskImage
 * which has the same distance is then
 * computed.
 * 
 * The output is the closest point on 
 * the outputMaskImage.
 * 
 * This is related to the Iterative Closest
 * Points algorithm.
 * 
 * \author  Rob Toth
 * \date    2013-03-25
 * \ingroup Segmentation
 */
template< typename TLandmarksType >
itk::SmartPointer< TLandmarksType >
GetClosestSurfacePoints(
  const TLandmarksType & inputPoints,
  const NormalDirectionCalculator<
    TLandmarksType::PointDimension
  > & calculator );
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Finds closest points on a mask.
 * 
 * \see the other GetClosestSurfacePoints()
 * 
 * \author  Rob Toth
 * \date    2013-03-25
 * \ingroup Segmentation
 */
template< typename TLandmarksType >
itk::SmartPointer< TLandmarksType >
GetClosestSurfacePoints(
  const TLandmarksType & inputPoints,
  const itk::SmartPointer< const
    itk::Image< MaskPixelType, TLandmarksType::PointDimension >
  > & mask );
//////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Finds closest points on a mask.
 * 
 * You give it a collection of points.
 * It will also output a collection of points.
 * 
 * For each point in the input space, the distance
 * to the inputMaskImage is calculated.
 * 
 * The closest point on the outputMaskImage
 * which has the same distance is then
 * computed.
 * 
 * The output is the closest point on 
 * the outputMaskImage.
 * 
 * This is related to the Iterative Closest
 * Points algorithm.
 * 
 * \author  Rob Toth
 * \date    2012-11-04
 * \ingroup Segmentation
 */
template< typename TLandmarkType, typename TMaskPixelType, unsigned int TDimension >
itk::SmartPointer< itk::PointSet<
  TLandmarkType,
  TDimension,
  itk::DefaultStaticMeshTraits<
    TLandmarkType,
    TDimension,
    TDimension, 
    TLandmarkType,
    TLandmarkType,
    TLandmarkType
> > >
GetClosestPoints(
  const itk::PointSet<
    TLandmarkType,
    TDimension,
    itk::DefaultStaticMeshTraits<
      TLandmarkType,
      TDimension,
      TDimension,
      TLandmarkType,
      TLandmarkType,
      TLandmarkType > >                            &       inputPoints,
  const itk::Image< TMaskPixelType, TDimension >   * const inputMask,
  const itk::Image< TMaskPixelType, TDimension >   * const outputMask );
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Finds all points some distance from a mask.
 * 
 * This will find all pixels some distance
 * (in mm) from the mask.
 * 
 * The output will be a collection of points
 * You give it a collection of points.
 * It will also output a collection of points.
 * 
 * For each point in the input space, the distance
 * to the inputMaskImage is calculated.
 * 
 * The closest point on the outputMaskImage
 * which has the same distance is then
 * computed.
 * 
 * The output is the closest point on 
 * the outputMaskImage.
 * 
 * This is related to the Iterative Closest
 * Points algorithm.
 * 
 * \author  Rob Toth
 * \date    2012-11-04
 * \ingroup Common
 */
template< typename TLandmarkType, typename TMaskPixelType, unsigned int TDimension >
itk::SmartPointer< itk::PointSet<
  TLandmarkType,
  TDimension,
  itk::DefaultStaticMeshTraits<
    TLandmarkType,
    TDimension,
    TDimension, 
    TLandmarkType,
    TLandmarkType,
    TLandmarkType
> > >
GetClosestPoints(
  const itk::PointSet<
    TLandmarkType,
    TDimension,
    itk::DefaultStaticMeshTraits<
      TLandmarkType,
      TDimension,
      TDimension,
      TLandmarkType,
      TLandmarkType,
      TLandmarkType > >                            &       inputPoints,
  const itk::Image< TMaskPixelType, TDimension >   * const inputMask,
  const itk::Image< TMaskPixelType, TDimension >   * const outputMask );
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace ccipd
#endif // __ccipdClosestPoints_h
