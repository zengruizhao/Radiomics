


//////////////////////////////////////////////////////////////////////////////////////////
// ccipd includes
#include "ccipdITKLandmarksToVTKPolyData.h"
#include "ccipdITKLandmarksToVTKPoints.h"


#include "ccipdDisableWarningsMacro.h"
  // vtk includes
  #include <vtkVersion.h>
  #include "vtkPolyData.h"
  #include "vtkCellArray.h"         // for polydata
  #include "vtkVertexGlyphFilter.h" // for converting points to cells
  #include "vtkSmartPointer.h"
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
vtkPolyDataPointer ConvertLandmarksToPolyData( const LandmarkSetType * landmarks )
{

  // first, convert the points to a vtk type
  const vtkPointsPointer points = ConvertLandmarksToVTKPoints( landmarks );

  // now have a poly data
  const vtkPolyDataPointer polyData = vtkPolyDataPointer::New();

  // give the points to the polydata
  polyData->SetPoints( points );

  // create a vertex filter
  typedef vtkSmartPointer< vtkVertexGlyphFilter > vtkVertexGlyphFilterPointer;
  vtkVertexGlyphFilterPointer vertexFilter = vtkVertexGlyphFilterPointer::New();

  // give it the existing points and calculate vertices
  vertexFilter->
#if VTK_MAJOR_VERSION >= 6
    AddInputData
#else
    AddInput
#endif
      ( polyData );

  vertexFilter->Update();

  return vertexFilter->GetOutput();

} // LandmarksToPolyData
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace
