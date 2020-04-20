


/////////////////////////////////////////////////////////////////////////////////////////////////
// ccipd includes
#include "ccipdCore.h"
#include "ccipdInternalPointsFromGrid.h"
#include "ccipdSurfacePointsFromGrid.h"

#include "ccipdDisableWarningsMacro.h"
  // vtk includes
  #include <vtkSmartPointer.h>
  #include <vtkUnstructuredGrid.h>     // our input type
#include "ccipdEnableWarningsMacro.h"
/////////////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



/////////////////////////////////////////////////////////////////////////////////////////////////
VectorIndexContainer ExtractInternalPointsIndices(
  const vtkUnstructuredGridPointer & grid,
  const VectorIndexContainer       & surfacePointIndices )
{

  // how many nodes do we have?
  const VectorIndexType numberOfGridPoints =
    static_cast< VectorIndexType >( grid->GetNumberOfPoints() );

  VectorIndexContainer internalPointIndices; // start out empty

  // cache for efficiency
  const VectorIndexContainer::const_iterator
    surfacePointsEnd = surfacePointIndices.cend();

  // loop through
  for ( VectorIndexType gridPointIndex = 0;
    gridPointIndex < numberOfGridPoints; ++gridPointIndex )
    {
    
    // if it's not a surface point, then add it as an internal point
    const bool isSurfacePoint =
      surfacePointIndices.find( gridPointIndex ) != surfacePointsEnd;

    if ( !isSurfacePoint )
      internalPointIndices.insert( gridPointIndex );

    } // gridPointIndex 

  // output
  return internalPointIndices;

} // ExtractInternalPointsIndices
/////////////////////////////////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////////////////////////////////
VectorIndexContainer ExtractInternalPointsIndices( const vtkUnstructuredGridPointer & grid )
{

  // calculate the surface points first
  return ExtractInternalPointsIndices( grid, ExtractSurfacePointsIndices( grid ) );

} // ExtractInternalPointIndices
/////////////////////////////////////////////////////////////////////////////////////////////////



} // namespace ccipd
