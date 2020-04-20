



//////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdGetControlPointsFromBSpline.hxx"

#include "ccipdDisableWarningsMacro.h"
  #include <itkBSplineTransform.h>
#include "ccipdEnableWarningsMacro.h"
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{

LandmarkSetPointer GetControlPointsFromBSpline(
  const VolumeBSplineTransformType * const transform,
  bool const getMoved,
  bool const returnOnlyBorderPoints)
{

  return GetControlPointsFromBSpline
    <LandmarkType, TransformValueType, VolumeMeshTraits, splineOrder, VolumeDimension>
    (transform, getMoved, returnOnlyBorderPoints);
} //GetControlPointsFromBSpline

}//namespace
