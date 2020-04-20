#ifndef __ccipdMasksToLandmarks_h
#define __ccipdMasksToLandmarks_h



////////////////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdLandmarkTypes.h"    // for our landmark output types
#include "ccipdVolumeTypes.h"      // for our mask types
#include "ccipdVectorIndexTypes.h" // for saying which are the surface landmarks
////////////////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * This overloaded version will also output which landmarks
 * came from the surface of the object (as opposed to inside).
 * 
 * \ingroup Segmentation
 */
LandmarkSetContainerPointer ExtractLandmarkContainerFromMask(
  const VolumeMaskImageType  * const mask,
        VectorIndexContainer &       borderLandmarkIndices,
  const itk::SizeValueType           numberOfBorderLandmarks   = 100,
  const itk::SizeValueType           numberOfInternalLandmarks = 1000,
  const InternalPixelType            borderWidth               = 1.0f /* in mm */);
////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * This will extract a bunch of landmarks from a mask
 * where the user defines the number of landmarks to lie
 * on the surface, and the number of
 * landmarks to lie inside the object.
 * 
 * \param borderWidth
 *   A border can't be 0 width, and this will define
 *   how wide of a border to sample points from, in mm.
 * 
 * \ingroup Segmentation
 */
LandmarkSetContainerPointer ExtractLandmarkContainerFromMask(
  const VolumeMaskImageType * const mask,
  const itk::SizeValueType          numberOfBorderLandmarks   = 100,
  const itk::SizeValueType          numberOfInternalLandmarks = 1000,
  const InternalPixelType           borderWidth               = 1.0 /* in mm */ );
////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * Just a different output type.
 * 
 * \ingroup Segmentation
 */
LandmarkSetPointer ExtractLandmarkSetFromMask(
  const VolumeMaskImageType * const mask,
  const itk::SizeValueType          numberOfBorderLandmarks   = 100,
  const itk::SizeValueType          numberOfInternalLandmarks = 1000,
  const InternalPixelType           borderWidth               = 1.0 /* in mm */ );
////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * Just a different output type.
 * 
 * \ingroup Segmentation
 */
LandmarkSetPointer ExtractLandmarkSetFromMask(
  const VolumeMaskImageType  * const mask,
        VectorIndexContainer &       borderLandmarkIndices,
  const itk::SizeValueType           numberOfBorderLandmarks   = 100,
  const itk::SizeValueType           numberOfInternalLandmarks = 1000,
  const InternalPixelType            borderWidth               = 1.0 /* in mm */ );
////////////////////////////////////////////////////////////////////////////////////////////////////



} // namespace ccipd



#endif // __ccipdMasksToLandmarks_h
