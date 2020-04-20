#ifndef __ccipdGetControlPointsFromBSpline_h
#define __ccipdGetControlPointsFromBSpline_h



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

/** Functions to take in a BSplineTransformationType
  * and return a set of spatial coordinates (landmarks)
  */ 


//////////////////////////////////////////////////////////////////////////////////////////
/** A function to take in a BSplineTransformation
  * and return a landmark set which
  * contains the BSpline control points
  * in real world coordinates.
  *
  * \author  Rachel Sparks
  * \date    2012-06-27
  * \ingroup Common
  */
LandmarkSetPointer GetControlPointsFromBSpline(
  const VolumeBSplineTransformType * const transform,
  bool const getMoved,
  bool const returnOnlyBorderPoints = false);
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
/* Function where the core code of the BSplineTransformation 
 * and return a set of points. */
template <
  class LandmarkType, 
  class TransformValueType,
  class MeshTraits,
  unsigned int splineOrder, 
  unsigned int ImageDimensions
>
typename itk::SmartPointer< 
  itk::PointSet< LandmarkType, ImageDimensions, MeshTraits > 
> GetControlPointsFromBSpline(

  const itk::BSplineTransform<
    TransformValueType,
    ImageDimensions,
    splineOrder
  > * const transform,

  bool const getMoved,

  bool const returnOnlyBorderPoints = false
);

}//namespace
/** @} */
#endif // __ccipdGetControlPointsFromBSpline_h  
