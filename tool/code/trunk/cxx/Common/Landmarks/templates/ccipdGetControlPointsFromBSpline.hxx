#ifndef __ccipdGetControlPointsFromBSpline_hxx
#define __ccipdGetControlPointsFromBSpline_hxx



/**
 * \addtogroup Common
 * @{
 */



//////////////////////////////////////////////////////////////////////////////////////////  
#include "ccipdGetControlPointsFromBSpline.h"

#include "ccipdDisableWarningsMacro.h"
  #include <itkPointSet.h>
#include "ccipdEnableWarningsMacro.h"
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{

template <class LandmarkType, 
          class TransformValueType, 
		  class MeshTraits,
          unsigned int splineOrder, 
          unsigned int ImageDimensions>
typename itk::SmartPointer< 
  itk::PointSet< 
    LandmarkType, 
    ImageDimensions,
    MeshTraits > > 
GetControlPointsFromBSpline(

  const itk::BSplineTransform<
    TransformValueType,
    ImageDimensions,
    splineOrder
  > * const transform,

  bool const getMoved,

  bool const returnOnlyBorderPoints)
{

typedef itk::BSplineTransform<
  TransformValueType,
  ImageDimensions,
  splineOrder
> BSplineType;

typedef itk::PointSet<
  LandmarkType,
  ImageDimensions, 
  MeshTraits
> LandmarkSetType;

//Retreive the necessary BSpline information
typename BSplineType::OriginType 
  origin = transform->GetTransformDomainOrigin();

typename BSplineType::DirectionType 
  direction = transform->GetTransformDomainDirection();

typename BSplineType::PhysicalDimensionsType
  gridROI = transform->GetTransformDomainPhysicalDimensions(),
  gridSpacing;

typename BSplineType::MeshSizeType
  paramsPerDim = transform->GetTransformDomainMeshSize();

typename BSplineType::ParametersType
	parameters = transform->GetParameters();

//create the initial mesh
unsigned int totalPoints = 1;

//get the grid spacing and total number of points
for (unsigned int j=0;j<ImageDimensions; ++j)
{
  gridSpacing[j]= gridROI[j]/(paramsPerDim[j]);
  totalPoints*= static_cast< unsigned int >(
    paramsPerDim[j]+splineOrder );
}

//if we do not want the moved points just fill the parameters as empty
if (!getMoved)
	parameters.Fill(0);

// prepare my landmark type
typename LandmarkSetType::Pointer landmarks = LandmarkSetType::New();

bool 
  isOnTheBorder = false,
  isOutsideTheBorder = false;
unsigned int borderI  = 0;
for (unsigned int i = 0 ;i < totalPoints ; ++i)
  {
  typename LandmarkSetType::PointType point;
  unsigned int pointsForDim = 1;
 
  isOnTheBorder = false;
  isOutsideTheBorder = false; 
  for (unsigned int j = 0; j<VolumeDimension;++j)
    {
	  signed int gridLocation = ( (i)/ (pointsForDim)) % (paramsPerDim[j]+splineOrder)-1;
	  
	  if (returnOnlyBorderPoints && (gridLocation == 0 || gridLocation == int(paramsPerDim[j])))// on the border
	    isOnTheBorder = true;
  
	  if (returnOnlyBorderPoints && (gridLocation < 0 || gridLocation > int(paramsPerDim[j])))// on the border
	    isOutsideTheBorder = true;
          
    unsigned int parameterInd = (totalPoints*j)+ i;
   
	  point[j] = static_cast< typename LandmarkSetType::PointType::ValueType >(
      (gridLocation * gridSpacing[j]) + origin[j] + parameters[parameterInd] ); 
	 
	  pointsForDim*= static_cast< unsigned int >(
      paramsPerDim[j] + splineOrder );

    } // for unsigned int j
  if (!returnOnlyBorderPoints )
  {	
    landmarks->SetPoint(i,point);
  }else
  { 
     if (returnOnlyBorderPoints && isOnTheBorder && !isOutsideTheBorder)
     {
       landmarks->SetPoint(borderI,point);
       borderI++;	
      }
  }

  } // for unsigned int i

return landmarks;
} //GetControlPointsFromBSpline
//////////////////////////////////////////////////////////////////////////////////////////



} //namespace ccipd
/** @} */
#endif // __ccipdGetControlPointsFromBSpline_hxx
