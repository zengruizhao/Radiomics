/**
 * This file aims to take a mask (or set of masks)
 * and calculate a set of signed distance maps
 * to be used as levelsets.
 */
#ifndef __ccipdlevelsets_h
#define __ccipdlevelsets_h



//////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdVolumeTypes.h"
#include "ccipdLandmarkTypes.h"
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////

/**
 * \brief Calculate a set of levelsets from a set of masks.
 * 
 * \ingroup Segmentation
 *
 */
VolumeImagesContainer CalculateLevelsetsFromMasks(
  const VolumeConstMaskImagesContainer masks,
  const bool  normalize         = false,
  const bool  sigmoid           = false,
  const float sigmoidWidth      = 4,
  const float smoothingVariance = 0 );
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Threshold at 0 to get a mask.
 * 
 * \ingroup Segmentation
 *
 */
VolumeMaskImagePointer CalculateMaskFromLevelset(
  const VolumeImageType * levelset,
  const float threshold = 0 );
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
/**
 * \brief
 *   Recomputed the levelset after a transformation
 *   was applied (that changed the surface).
 * 
 * \note Slow code as it uses the isotropic map to reduce interpolation artifacts
 * 
 * \ingroup Segmentation
 */
VolumeImagePointer UpdateLevelset(
  const VolumeImageType * levelset,
  const float threshold         = 0 ,
  const bool  normalize         = false,
  const bool  sigmoid           = false,
  const float sigmoidWidth      = 4,
  const float smoothingVariance = 0);
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
/*
 * Compute the median axes of a level set - i.e. central axis within the negative size of
 * a level set.
 * This is a dumb version of the medial Axis that is computed by iterating on the z axis
 * 
 * \ingroup Segmentation
 */
MedialAxisSetPointer CalculateSimpleMedialAxisFromLevelset (
  const VolumeImageType * levelset );
//////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////
/*
 * Compute the simple median axes of a level set - i.e. central axis within the negative size of
 * a level set.
 * This is a dumb version of the medial Axis that is computed by iterating on the z axis
 * 
 * \ingroup Segmentation
 */
LandmarkSetPointer ExtractSimpleMedialAxisIntoLandmark(
  const VolumeImageType * levelset);
} // namespace
#endif // __ccipdlevelsets_h
