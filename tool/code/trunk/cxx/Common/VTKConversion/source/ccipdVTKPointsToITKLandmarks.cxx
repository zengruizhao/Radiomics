


//////////////////////////////////////////////////////////////////////////////////////////
// ccipd includes
#include "ccipdVTKPointsToITKLandmarks.h"

#include "ccipdDisableWarningsMacro.h"
  //itk includes
  #include <itkPointSet.h>

  // vtk includes
  #include <vtkSmartPointer.h>
  #include <vtkPoints.h>
#include "ccipdEnableWarningsMacro.h"
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////
LandmarkSetPointer ConvertVTKPointsToLandmarks( const vtkPointsPointer & points )
{

  // based on http://bit.ly/HfVlDY

  // how many points do we have?
  typedef LandmarkSetType::PointIdentifier PointIdentifier;
  const PointIdentifier numberOfPoints = static_cast< PointIdentifier > ( points->GetNumberOfPoints() );

  // Create a compatible point container
  LandmarkSetPointer landmarkSet = LandmarkSetType::New();

  // Resize the point container to be able to fit the vtk points
  LandmarkSetContainerPointer landmarkContainer = LandmarkSetContainer::New();
  landmarkContainer->Reserve( numberOfPoints );

  // give it to the set
  landmarkSet->SetPoints( landmarkContainer );

  // go through each point
  for( PointIdentifier pointIndex = 0; pointIndex < numberOfPoints; ++pointIndex )
    {

    // extract the input point
    const double * const currentInputPoint = points->GetPoint( (vtkIdType) pointIndex );

    // create an output point
    LandmarkPointType currentOutputPoint;
    currentOutputPoint[0] = static_cast< LandmarkType >( currentInputPoint[0] );
    currentOutputPoint[1] = static_cast< LandmarkType >( currentInputPoint[1] );
    currentOutputPoint[2] = static_cast< LandmarkType >( currentInputPoint[2] );

    // give it to our landmarks
    landmarkSet->SetPoint( pointIndex, currentOutputPoint );

    } // for each pointIndex

  return landmarkSet;

} // ConvertVTKPointsToLandmarks
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace ccipd

