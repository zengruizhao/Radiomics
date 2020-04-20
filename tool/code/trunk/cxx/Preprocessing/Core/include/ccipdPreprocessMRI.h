#ifndef __ccipdPreprocessMRI_h
#define __ccipdPreprocessMRI_h



/**
 * This file contains the preprocessing code for
 * MRI images. Note these are parameter specifics
 * calls to functions in ccipdPreprocessVolume.
 */



////////////////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdMRITypes.h" // for MRI image types
////////////////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{

  
  
////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * This function will input an MRI image
 * and correct the bias field, and normalize
 * the intensities.
 * \param inputImage
 * \param biasSigma sigma used for bias field correction
 * \param outputMinimum min intensity to be scaled to 
 * \param outputMaximum max intensity to be scaled to
 * \return corrected map
 */
MRIImagePointer PreprocessMRI(
  const MRIImageType * inputImage,
  const float biasSigma = 6.0f,
  const InternalPixelType intensityThreshold = 3 );

/**
 * This function will input an MRI image and apply a bilater filter (to discard noise).
 * \param inputImage
 * \return corrected map
 */
MRIImagePointer ApplyBilateralFilterMRI(
  const MRIImageType * inputImage );
////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * This function will input an MRI image and will rescale intensities between 0 and 255
 * \return corrected map
 */
MRIImagePointer RescaleIntensitiesMRI(
  const MRIImageType * inputImage);
  
} // namespace
#endif // __ccipdPreprocessMRI_h
