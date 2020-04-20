


////////////////////////////////////////////////////////////////////////////////////////////////

// vtk includes
#include "ccipdBoundingBoxToActor.h"
#include "ccipdBoundingBox.h"
#include "ccipdIsosurfaceRendering.h"
#include "ccipdMaskToLevelset.h"
#include "ccipdImageToProp.h"

#include "ccipdDisableWarningsMacro.h"

// itk includes
#include <itkImage.h>

// vtk includes
#include <vtkSmartPointer.h>
#include <vtkActor.h>
#include <vtkProp.h>
#include <vtkProperty.h>

#include "ccipdEnableWarningsMacro.h"

////////////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{

  
////////////////////////////////////////////////////////////////////////////////////////////////
vtkActorPointer GenerateVTKActorFromBoundingBox(
  const VolumeRegionType &       boundingBox,
  const VolumeImageType * const image )
{

  // create a mask from the bounding box
  const VolumeMaskImageConstPointer boundingBoxMask(
    CreateMaskFromBoundingBox( boundingBox, image ) );

  // create a levelset from the mask
  const VolumeImageConstPointer boundingBoxLevelset(
    MaskToLevelset< InternalPixelType >( boundingBoxMask.GetPointer(), false, false, 4.0, 0.0, false ) );

  // create an actor from the levelset
  const vtkActorPointer levelsetActor =
    GenerateVTKActorFromIsosurface( boundingBoxLevelset, 0 );

  // show as a wire frame
  levelsetActor->GetProperty()->SetRepresentationToWireframe();
  levelsetActor->GetProperty()->SetOpacity( 0.1 );

  // output
  return levelsetActor;

} // GetActorFromBoundingBox
////////////////////////////////////////////////////////////////////////////////////////////////


} // namespace ccipd
