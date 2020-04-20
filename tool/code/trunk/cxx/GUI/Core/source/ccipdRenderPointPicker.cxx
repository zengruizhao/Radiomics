


////////////////////////////////////////////////////////////////////////////////////////////////////

// ccipd includes
#include "ccipdRenderPointPicker.hxx"
#include "ccipdVolumeTypes.h"

#include "ccipdDisableWarningsMacro.h"
  // itk includes
  #include <itkImage.h>
  #include <itkRGBPixel.h> 
  #include <itkPointSet.h>
#include "ccipdEnableWarningsMacro.h"

////////////////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



////////////////////////////////////////////////////////////////////////////////////////////////
// volume types
template
void RenderPointPicker< VolumeImageType >(
  const VolumeImageType * const image,
  const unsigned int            slice,
  const bool                    verbose,
  vtkRenderWindow       * const renderWindow );

template
void RenderPointPicker< VolumeImageType >(
  const VolumeImageType         * const image,
  const unsigned int                    slice,
  const bool                            verbose,
  vtkRenderWindow               * const renderWindow,
  const LandmarkSetConstPointer &       initialLandmarks );
////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////
// rgb types

template
void RenderPointPicker< RGBSliceType >(
  const RGBSliceType * const image,
  const unsigned int         slice,
  const bool                 verbose,
  vtkRenderWindow * const    renderWindow );

template
void RenderPointPicker< RGBSliceType >(
  const RGBSliceType            * const image,
  const unsigned int                    slice,
  const bool                            verbose,
  vtkRenderWindow               * const renderWindow,
  const LandmarkSetConstPointer &       initialLandmarks );
////////////////////////////////////////////////////////////////////////////////////////////////




} // namespace ccipd
