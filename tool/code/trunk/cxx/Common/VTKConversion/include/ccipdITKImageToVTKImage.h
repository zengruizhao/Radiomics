#ifndef __ccipdITKImageToVTKImage_h
#define __ccipdITKImageToVTKImage_h



//////////////////////////////////////////////////////////////////////////////////////////
//#include "ccipdITKImageToVTKImage.hxx"
#include "ccipdVTKForwardDeclarations.h"
#include "ccipdVolumeTypes.h"
#include "ccipdRGBSliceTypes.h"

//////////////////////////////////////////////////////////////////////////////////////////

namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Convert ITK image to VTK image type.
 * 
 * This is a helper function for converting
 * our ITK images to VTK images.
 * 
 * The VTK image type is called "vtkImageData".
 * 
 * \param[in] image
 *   The ITK image
 *   to convert to a VTK image.
 * 
 * \returns
 *   A smart pointer to a
 *   vtkImageData object.
 * 
 * \warning
 *   It appears that the ITK image
 *   direction is completely ignored in the
 *   VTK image.
 * 
 * \note
 *   It may be possible to convert
 *   other image types (such as a
 *   mask image, or even a 2D slice)
 *   to a VTK image but currently
 *   only volumetric images are
 *   supported.
 * 
 * \date    2012-04-14
 * \author  Rob Toth
 * \ingroup VTKConversion
 */
template< class TImageType >
vtkImageDataPointer ConvertITKImageToVTKImage(const TImageType * const image);
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace ccipd
#endif // __ccipdITKImageToVTKImage_h
