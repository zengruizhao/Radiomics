#ifndef __ccipdIsosurfaceRendering_h
#define __ccipdIsosurfaceRendering_h



//////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdVolumeTypes.h"
#include "ccipdRGBSliceTypes.h"
#include "ccipdVTKForwardDeclarations.h"
//////////////////////////////////////////////////////////////////////////////////////////
#include <itkRGBPixel.h>


namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////
/**
 * This will extract an actor representing
 * an isosurface at a specific value
 * in the volume (which most likely will
 * be a levelset).
 * 
 * The most common usage is to 
 * extract a levelset and create
 * an isosurface at the "0" levelset value.
 * 
 * \param[in] image
 *   An isosurface will be extracted
 *   at a specific intensity value
 *   in this image.
 * 
 * \param[in] isosurfaceValue
 *   Which intensity value should
 *   a surface be generated from.
 * 
 * \returns
 *   A vtkActor representing
 *   the surface.
 * 
 * \ingroup GUI
 */
vtkActorPointer GenerateVTKActorFromIsosurface(  const VolumeImageType * const image
											   , const InternalPixelType       isosurfaceValue 
											  );

// Overload
vtkActorPointer GenerateVTKActorFromIsosurface(  const RGBVolumeType    * const image
											   , const InternalPixelType        isosurfaceValue
											  );

/**
 * See GenerateVTKActorFromIsosurface( const VolumeImageType *, const InternalPixelType )
 * 
 * The only difference is that this function takes in
 * a vtk image, instead of an itk image.
 * 
 * \param[in] image This parameter is a VTK representation of the desired image.
 * 
 * \ingroup GUI
 */
vtkActorPointer GenerateVTKActorFromIsosurface(
  const vtkImageDataPointer & image,
  const InternalPixelType     isosurfaceValue );
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace ccipd
#endif // __ccipdIsosurfaceRendering_h
