#ifndef __ccipdGetBSplineFromControlPoints_hxx
#define __ccipdGetBSplineFromControlPoints_hxx




/**
 * \addtogroup Common
 * @{
 */



//////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdGetBSplineFromControlPoints.h"

#include "ccipdDisableWarningsMacro.h"
#include <itkVectorContainer.h>
#include "ccipdEnableWarningsMacro.h"

#include <vector>

//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{
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
  > & gridSize
)
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

typename BSplineType::Pointer transform = BSplineType::New();
 std::vector<unsigned int> pointsPerDim; 
 pointsPerDim.resize(ImageDimensions+1);

//generate the number of points per each dimension
unsigned int totalPoints = 1; 
pointsPerDim[0] = totalPoints;
 
for(unsigned int i = 0; i < ImageDimensions; i++ )
  {
  totalPoints *= static_cast< unsigned int >(
    gridSize.GetElement(i) + splineOrder );
  pointsPerDim[i+1] = totalPoints;
  } // for(unsigned int i = 0; i < ImageDimensions; i++ )

// check to make the moved and fixed control points are of the same size
if(movedControlPoints->GetNumberOfPoints()!=originalControlPoints->GetNumberOfPoints() )
  throw itk::ExceptionObject( __FILE__, __LINE__,
    "originalControlPoints and movedControlPoints must be the same length." );

// check to make sure the gridSize and the number of control points are the same size
if ( totalPoints != movedControlPoints->GetNumberOfPoints() )
  throw itk::ExceptionObject( __FILE__, __LINE__,
    "movedControlPoints and gridSize must refer to the same number of points." );


// generate the fixedParameters
typename BSplineType::DirectionType          transformDirection;
typename BSplineType::PhysicalDimensionsType gridROI, 
	                                           gridSpacing;
typename BSplineType::OriginType             origin;
typename LandmarkSetType::PointType          firstPoint = originalControlPoints->GetPoint(0);


for (unsigned int i = 0; i< ImageDimensions;i++)
  { 
  typename LandmarkSetType::PointType pointAlongDim = originalControlPoints->GetPoint(pointsPerDim[i]);
  typename BSplineType::ScalarType dimStep = 0;
  
  //calculate the gridSpacing
  for (unsigned int j = 0; j < ImageDimensions; j++ )
    {	
	dimStep += pow ( (pointAlongDim[j] - firstPoint[j]), 2);
    } //  for (unsigned int j = 0; j < ImageDimensions; j++ )

  gridSpacing[i] = sqrt(dimStep);
  gridROI[i] = gridSpacing[i]*gridSize[i];
  origin[i]= firstPoint[i]+gridSpacing[i];
  
  //calculate the direction vector
  for (unsigned int j=0; j<ImageDimensions;j++)
    {
    transformDirection[i][j] = (pointAlongDim[j] - firstPoint[j]) /gridSpacing[i];
    }//  for (unsigned int j = 0; j < ImageDimensions; j++ )

  } // for (unsigned int i = 0; i< ImageDimensions;i++)

transform->SetTransformDomainOrigin(origin);
transform->SetTransformDomainDirection(transformDirection);
transform->SetTransformDomainPhysicalDimensions(gridROI);
transform->SetTransformDomainMeshSize(gridSize);

//generate the movingParameters
typename BSplineType::ParametersType parameters(totalPoints*ImageDimensions);

for( unsigned int i = 0; i < movedControlPoints->GetNumberOfPoints(); i++ )
  {
  typename LandmarkSetType::PointType movedPoint = movedControlPoints->GetPoint( i );

  for (unsigned int j = 0; j< ImageDimensions; j++)
    {
	  signed int gridLocation = ( (i)/ (pointsPerDim[j])) % (gridSize[j]+splineOrder)-1;
    parameters[i+j*totalPoints] = movedPoint[j] - (gridLocation * gridSpacing[j]) - origin[j];
    } // for (unsigned int j = 0; j< ImageDimensions; j++)
  } // for( unsigned int i = 0; i < movedControlPoints->GetNumberOfPoints(); i++ )


transform->SetParametersByValue( parameters );

return transform;
}//GetControlPointsFromBSpline
//////////////////////////////////////////////////////////////////////////////////////////



} //namespace ccipd
/** @} */
#endif // __ccipdGetBSplineFromControlPoints_hxx
