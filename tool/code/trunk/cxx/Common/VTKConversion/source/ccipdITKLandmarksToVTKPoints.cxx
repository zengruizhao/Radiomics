


//////////////////////////////////////////////////////////////////////////////////////////
// ccipd includes
#include "ccipdITKLandmarksToVTKPoints.h"

#include "ccipdDisableWarningsMacro.h"
  //itk includes
  #include <itkPointSet.h>

  // vtk includes
  #include <vtkPoints.h>
  #include <vtkSmartPointer.h>
#include "ccipdEnableWarningsMacro.h"
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
using std::cerr;
using std::endl;
//////////////////////////////////////////////////////////////////////////////////////////



// based on http://bit.ly/HfVlDY


namespace ccipd
{


  

//////////////////////////////////////////////////////////////////////////////////////////
vtkPointsPointer ConvertLandmarksToVTKPoints( const LandmarkSetType * landmarks )
{

  // Get the number of points in the mesh
  typedef LandmarkSetType::PointIdentifier PointIdentifier;
  const PointIdentifier numberOfPoints = landmarks->GetNumberOfPoints();

  // check to make sure we have some points
  if( numberOfPoints == 0 )
    {
    landmarks->Print(std::cerr);
    cerr << "No landmarks!" << endl;
    return vtkPointsPointer();
    }

  // Create the vtkPoints object and set the number of points
  const vtkPointsPointer outputPoints = vtkPointsPointer::New();
  outputPoints->SetNumberOfPoints( (vtkIdType) numberOfPoints );

  // extract our mesh points
  const LandmarkSetContainerConstPointer inputPoints = landmarks->GetPoints();

  // iterate over all the points in the itk mesh filling in
  // the vtkPoints object as we go
  const LandmarkSetContainer::ConstIterator pointsEnd = inputPoints->End();
  for( LandmarkSetContainer::ConstIterator pointsIter = inputPoints->Begin();
      pointsIter != pointsEnd; ++pointsIter )
    {

    // Get the point index from the point container iterator
    const PointIdentifier index = pointsIter->Index();

    // Set the vtk point at the index with the the coord array from itk
    // itk returns a const pointer, but vtk is not const correct, so
    // we have to use a const cast to get rid of the const
    const LandmarkPointType & pointValue = pointsIter->Value();

    outputPoints->SetPoint( (vtkIdType) index, pointValue.GetDataPointer() );

    } // for each point

  // return the points
  return outputPoints;

} // MeshToUnstructuredGrid
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace
