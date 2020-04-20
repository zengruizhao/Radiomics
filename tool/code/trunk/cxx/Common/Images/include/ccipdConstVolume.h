/**
 * This file aims to convert Volume types to const Volume types.
 */
#ifndef __ccipdConstVolume_h
#define __ccipdConstVolume_h



//////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdVolumeTypes.h"

#include "ccipdDisableWarningsMacro.h"
  // itk includes
  #include <itkImage.h>
  #include <itkTransform.h>
#include "ccipdEnableWarningsMacro.h"
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////

/**
 * \brief Create a const pointer of a single image.
 * 
 * \deprecated
 *   To create a const pointer, you simply
 *   have to do the following:
 *   \code
 *     const VolumeImagePointer unconstPointer = VolumeImageType::New();
 *     const VolumeImageConstPointer constPointer1 = unconstPointer.GetPointer();
 *     const VolumeImageConstPointer constPointer2( unconstPointer );
 *   \endcode
 * 
 * \ingroup Common
 */

/**
 * \brief Create a const pointers of a collection of images.
 * 
 * \ingroup Common
 */
VolumeConstImagesContainer ConvertVolumeImagesToConst(
  const VolumeImagesContainer images );

/**
 * \brief Create a const pointers of a collection of masks.
 * 
 * \ingroup Common
 */
VolumeConstMaskImagesContainer ConvertVolumeMasksToConst(
  const VolumeMaskImagesContainer masks );


/**
 * \brief Create a const pointers of a collection of transforms.
 * 
 * \ingroup Common
 */
VolumeConstTransformsContainer ConvertVolumeTransformsToConst(
  const VolumeTransformsContainer transforms);
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace
#endif // __ccipdConstVolume_h
