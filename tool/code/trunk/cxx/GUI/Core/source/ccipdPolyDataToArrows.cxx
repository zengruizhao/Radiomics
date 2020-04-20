


//////////////////////////////////////////////////////////////////////////////////////////
// ccipd includes
#include "ccipdCore.h"
#include "ccipdPolyDataToArrows.h"

// vtk includes
#include <vtkVersion.h>
#include "vtkSmartPointer.h"     // for memory management
#include "vtkGlyph3D.h"          // to display arrows
#include "vtkArrowSource.h"      // to create arrows for the glyphs
#include "vtkPolyDataMapper.h"   // to map the arrows
#include "vtkActor.h"            // something to render
#include "vtkProperty.h"         // to change the colors
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////////////
vtkActorPointer GenerateArrowsFromPolyData(
  const vtkPolyDataPointer & polyData,
  const bool                 pointAway,
  const double               scaleFactor )
{

  // create a glyph to display arrows
  typedef vtkSmartPointer< vtkGlyph3D > vtkGlyph3DPointer;
  const vtkGlyph3DPointer glyphs = vtkGlyph3DPointer::New();

  // give the poly data points to the glyph
  glyphs->
#if VTK_MAJOR_VERSION >= 6
    SetInputData
#else
    SetInput
#endif
      ( polyData );

  // create arrows
  typedef vtkSmartPointer< vtkArrowSource > vtkArrowSourcePointer;
  const vtkArrowSourcePointer arrowSource = vtkArrowSourcePointer::New();
  arrowSource->SetTipResolution( 20 ); // high resolution
  arrowSource->SetInvert( !pointAway ); // if invert is on, point towards the point

  // give the data to the glyphs
  glyphs->SetSourceConnection( arrowSource->GetOutputPort() );
  glyphs->
#if VTK_MAJOR_VERSION >= 6
    SetInputData
#else
    SetInput
#endif
      ( polyData );

  // set some glyph properties
  glyphs->SetVectorModeToUseVector   ();
  glyphs->SetScaleModeToScaleByVector(); // scale arrow length by the magnitude
  glyphs->SetScaleFactor( scaleFactor ); // multiplied by a scaleFactor

  // create a mapper
  const vtkPolyDataMapperPointer mapper = vtkPolyDataMapperPointer::New();
  mapper->SetInputConnection( glyphs->GetOutputPort() );

  // create an actor
  const vtkActorPointer arrowsActor = vtkActorPointer::New();
  arrowsActor->SetMapper( mapper );
  
  // change the colors of the arrows
  const double
    arrowsR       = 0.0,
    arrowsG       = 1.0,
    arrowsB       = 0.0,
    arrowsOpacity = 1.0;
  arrowsActor->GetProperty()->SetColor( arrowsR, arrowsG, arrowsB );
  arrowsActor->GetProperty()->SetOpacity( arrowsOpacity );

  return arrowsActor;

} // GenerateVTKActorsFromFEM
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace
