


/////////////////////////////////////////////////////////////////////////////////////////////////
// ccipd includes
#include "ccipdCore.h"
#include "ccipdSurfacePointsFromGrid.h"

#include "ccipdDisableWarningsMacro.h"
  // vtk includes
  #include <vtkVersion.h>
  #include <vtkSmartPointer.h>
  #include <vtkDataSetSurfaceFilter.h> // for extracting surface triangles
  #include <vtkUnstructuredGrid.h>     // our input type
  #include <vtkPolyData.h>             // the output of the surface extractor
  #include <vtkPointData.h>            // this stores the old point ids
  #include <vtkIdTypeArray.h>          // the actual collection of surface point ids

  // std includes
  #include <string> // for naming the array
#include "ccipdDisableWarningsMacro.h"
/////////////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



/////////////////////////////////////////////////////////////////////////////////////////////////
VectorIndexContainer ExtractSurfacePointsIndices( const vtkUnstructuredGridPointer & grid )
{

  // we first extract only the triangle faces on the surface
  typedef vtkSmartPointer< vtkDataSetSurfaceFilter > vtkDataSetSurfaceFilterPointer;
  const vtkDataSetSurfaceFilterPointer surfaceExtractor = vtkDataSetSurfaceFilterPointer::New();

  // const vtkIdType numberOfGridPoints = grid->GetNumberOfPoints();
  surfaceExtractor->
#if VTK_MAJOR_VERSION >= 6
    SetInputData
#else
    SetInput
#endif
      ( grid );

  // we want to retain the original point indices
  surfaceExtractor->PassThroughPointIdsOn();
  const std::string pointIDNames = "Original IDs";
  surfaceExtractor->SetOriginalPointIdsName( pointIDNames.c_str() );

  // calculate
  surfaceExtractor->Update();

  // get the surface output
  const vtkPolyDataPointer surfacePolygons = surfaceExtractor->GetOutput();

  // const vtkIdType numberOfSurfacePoints = surfacePolygons->GetNumberOfPoints();

  // get the original point ids
  vtkIdTypeArray * originalPointIDs = static_cast< vtkIdTypeArray * >(
    surfacePolygons->GetPointData()->GetArray( pointIDNames.c_str() ) );

  // how many do we have?
  const vtkIdType numberOfOriginalPointIds = originalPointIDs->GetSize();

  // prepare the container of the indices
  VectorIndexContainer surfaceIndices;

  // iterate through
  for ( vtkIdType originalPointIDsIndex = 0;
    originalPointIDsIndex < numberOfOriginalPointIds; ++originalPointIDsIndex )
    {

    // get the value
    const vtkIdType originalPointID = originalPointIDs->GetValue( originalPointIDsIndex );

    // cast it
    const VectorIndexType pointIndex = static_cast< VectorIndexType >( originalPointID );

    // insert it into our list
    surfaceIndices.insert( pointIndex );

    } // originalPointIDsIndex 

  return surfaceIndices;

} // ExtractSurfacePointsIndices
/////////////////////////////////////////////////////////////////////////////////////////////////


} // namespace ccipd
