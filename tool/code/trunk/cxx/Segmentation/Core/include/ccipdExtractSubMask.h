#ifndef __ccipdExtractSubMask_h
#define __ccipdExtractSubMask_h



//////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdVolumeTypes.h"
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{


//////////////////////////////////////////////////////////////////////////////////////////
/**
 * This function will extract a subportion of a given mask.
 * 
 * \param templateMask The template mask is used to estimate 
 * the submask
 * \param firstSlicePercentage  what slice to start from in percentage
 * relative to the mask size; 
 * \param slicesPercentage (default 1) is the percent
 * of slices to keep
 * 
 * \ingroup Segmentation
 */
VolumeMaskImageConstPointer ExtractSubMask(
  const VolumeMaskImageType * const inputMask,
  const VolumeMaskImageType * const templateMask,
  const float firstSlicePercentage = 0.0f,
  const float slicesPercentage     = 1.0f);


//////////////////////////////////////////////////////////////////////////////////////////
/**
 * This function will extract
 * only the midgland portion of a given mask.
 * 
 * The template mask is used to estimate
 * the midgland, as the middle 1/3 of slices
 * containing the prostate.
 * 
 * slicesPercentage (default 1/3) is the percent
 * of slices to keep, radially outwards from
 * the center slice.
 * 
 * \ingroup Segmentation
 */
VolumeMaskImageConstPointer ExtractMidglandMask(
  const VolumeMaskImageType * const inputMask,
  const VolumeMaskImageType * const templateMask,
  const float                       slicesPercentage = 1.0f/3.0f );
//////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////
/**
 * This function will extract only the prostate base portion of a given mask.
 * \see ExtractSubMask, ExtractMidglandMasky
 */
VolumeMaskImageConstPointer ExtractBaseMask(
  const VolumeMaskImageType * const inputMask,
  const VolumeMaskImageType * const templateMask,
  const float                       slicesPercentage = 1.0f/3.0f );

  
//////////////////////////////////////////////////////////////////////////////////////////
/**
 * This function will extract only the prostate base portion of a given mask.
 * \see ExtractSubMask, ExtractMidglandMask
 */
VolumeMaskImageConstPointer ExtractApexMask(
  const VolumeMaskImageType * const inputMask,
  const VolumeMaskImageType * const templateMask,
  const float                       slicesPercentage = 1.0f/3.0f );

} // namespace



#endif // __ccipdExtractSubMask_h
