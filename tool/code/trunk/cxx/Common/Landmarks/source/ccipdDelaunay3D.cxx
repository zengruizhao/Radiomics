


//////////////////////////////////////////////////////////////////////////////////////////
// ccipd includes
#include "ccipdDelaunay3D.h"
#include "ccipdITKLandmarksToVTKPolyData.h" // for using actual itk landmarks

#include "ccipdDisableWarningsMacro.h"
  // vtk includes
  #include <vtkVersion.h>
  #include "vtkSmartPointer.h"
  #include "vtkPolyData.h"
  #include "vtkCleanPolyData.h"    // for removing duplicate points
  #include "vtkDelaunay3D.h"       // for actually computing
  #include "vtkUnstructuredGrid.h"
#include "ccipdEnableWarningsMacro.h"

#ifdef _MSC_VER
#pragma warning( disable: 4244 ) // possible loss of data
#endif // _MSC_VER
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////
vtkUnstructuredGridPointer Delaunay3D(
  const vtkPolyDataPointer & points,
  const bool                 removeDuplicatePoints )
{

  // based on http://bit.ly/HVs7I7

  // Generate a tetrahedral mesh from the input points. By
  // default, the generated volume is the convex hull of the points.
  typedef vtkSmartPointer< vtkDelaunay3D > vtkDelaunay3DPointer;
  vtkDelaunay3DPointer delaunayFilter = vtkDelaunay3DPointer::New();

  // remove duplicate points?
  if ( removeDuplicatePoints )
    {

    // Clean the polydata. This will remove duplicate points that may be
    // present in the input data.
    typedef vtkSmartPointer< vtkCleanPolyData > vtkCleanPolyDataPointer;

    vtkCleanPolyDataPointer cleaner = vtkCleanPolyDataPointer::New();
    cleaner->
#if VTK_MAJOR_VERSION >= 6
      SetInputData
#else
      SetInput
#endif
        ( points );

    delaunayFilter->SetInputConnection( cleaner->GetOutputPort() );

    } // removeDuplicates
  else
    {

    // just use the raw points themselves without cleaning
    delaunayFilter->
#if VTK_MAJOR_VERSION >= 6
      SetInputData
#else
      SetInput
#endif
        ( points );

    } // removeDuplicates

  // compute
  delaunayFilter->Update();
  return delaunayFilter->GetOutput();

} // Delaunay3D
//////////////////////////////////////////////////////////////////////////////////////////




//////////////////////////////////////////////////////////////////////////////////////////
vtkUnstructuredGridPointer Delaunay3D(
  const LandmarkSetType * landmarks,
  const bool              removeDuplicatePoints )
{

  return Delaunay3D( ConvertLandmarksToPolyData( landmarks ), removeDuplicatePoints );

} // Delaunay3D with landmarks
//////////////////////////////////////////////////////////////////////////////////////////


} // namespace ccipd
