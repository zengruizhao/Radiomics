#ifndef __ccipdGetBSplineFromControlPoints_h
#define __ccipdGetBSplineFromControlPoints_h



//////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdVolumeTypes.h"
#include "ccipdLandmarkTypes.h"
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



/**
 * \addtogroup Common
 * @{
 */

/** A function to take in a pair of
  * landmark sets which
  * contain the BSpline control points
  * prior to registration and after registration
  * in real world coordinates
  *
  * \author  Rachel Sparks
  * \date    2012-06-27
  * \ingroup Common
  */

/** Functions to take in landmark sets and 
  * results a BSpline Transformation
  */ 
VolumeBSplineTransformPointer GetBSplineFromControlPoints(
 const LandmarkSetType              * const originalControlPoints, 
 const LandmarkSetType              * const movedControlPoints,
 const itk::Size< VolumeDimension > & gridSize );


/** Templated function. */
template <class LandmarkType, 
          class TransformValueType, 
		  class MeshTraits,
          unsigned int splineOrder, 
          unsigned int ImageDimensions>

 itk::SmartPointer <itk::BSplineTransform<
    TransformValueType,
    ImageDimensions,
    splineOrder
  > >

GetBSplineFromControlPoints(

  const itk::PointSet< 
    LandmarkType, 
    ImageDimensions, 
	MeshTraits
  > * const originalControlPoints,

  const itk::PointSet< 
    LandmarkType, 
    ImageDimensions,
	MeshTraits
  > * const movedControlPoints,

  const itk::Size<
    ImageDimensions
  > &  gridSize
);
}//namespace
/** @} */
#endif // __ccipdGetControlPointsFromBSpline_h  