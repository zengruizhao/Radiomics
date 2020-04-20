


//////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdGetBSplineFromControlPoints.hxx"
#include "ccipdMeshTypes.h"

#include "ccipdDisableWarningsMacro.h"
  #include <itkBSplineTransform.h>
  #include <itkPointSet.h>
#include "ccipdEnableWarningsMacro.h"
//////////////////////////////////////////////////////////////////////////////////////////


namespace ccipd
{

VolumeBSplineTransformPointer GetBSplineFromControlPoints(
 const LandmarkSetType * const originalControlPoints, 
 const LandmarkSetType * const movedControlPoints,
 const itk::Size< VolumeDimension > & gridSize )
{

  return GetBSplineFromControlPoints<
    LandmarkType, 
    TransformValueType,
	VolumeMeshTraits,
    splineOrder,
    VolumeDimension
  > (originalControlPoints, movedControlPoints, gridSize);

} //GetControlPointsFromBSpline


}//namespace