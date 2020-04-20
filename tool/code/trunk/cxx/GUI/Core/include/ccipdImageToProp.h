#ifndef __ccipdImageToProp_h
#define __ccipdImageToProp_h


//////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdVTKForwardDeclarations.h"
#include "ccipdRGBSliceTypes.h"
#include "ccipdVolumeTypes.h"
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Creates an image from an image to add to a vtk renderer.
 * 
 * \param[in] image
 *   The image to create a prop from.
 * 
 * \param[in] slice
 *   If the image is 3D, this will determine
 *   which slice to display. If the image
 *   isn't 3D, this is ignored.
 * 
 * \date    2012-05-08
 * \author  Rob Toth
 * \ingroup GUI
 */
template< class TImageType >
vtkPropPointer CreatePropFromImage(
  const TImageType * const image,
  const unsigned int       slice,
  const bool               makeZeroTransparent = false );
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
/// Create a vtk prop from an RGB Slice.
vtkPropPointer CreatePropFromImage(
  const RGBSliceType * const image,
  const bool                 makeZeroTransparent = false );

/// Create a vtk prop from a volume.
vtkPropPointer CreatePropFromImage(
  const VolumeImageType * const image,
  const unsigned int            slice,
  const bool                    makeZeroTransparent = false );
//////////////////////////////////////////////////////////////////////////////////////////

 // Create a vtk prop from an RGB volume.
   vtkPropPointer CreatePropFromImage(
     const RGBVolumeType* const image,
     const unsigned int         slice,
     const bool                 makeZeroTransparent = false );

    vtkPropPointer CreatePropFromImage(
     const RGBAVolumeType* const image,
     const unsigned int         slice,
     const bool                 makeZeroTransparent = false );
}

#endif // __ccipdImageToProp_h
