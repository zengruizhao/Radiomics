#ifndef __ccipdPreprocessVolume_h
#define __ccipdPreprocessVolume_h



/**
 * This file contains the preprocessing code for
 *  volumes in general. These funcions contain 
 * the actual implementation.
 */



////////////////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdVolumeTypes.h" // for image types
////////////////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{

  
  
////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * This function will input an volume image and normalize intensities (mean = 0, sd = 1).
 * \param inputImage
 * \return corrected map
 */
VolumeImagePointer NormalizeVolume(
  const VolumeImageType * inputImage );

/**
 * This function will input an volume image and will crop intensities
 * at user defined sigma. Note NormalizeVolume must always be called first.
 * \param inputImage
 * \param outputMinimum min intensity to be scaled to 
 * \param outputMaximum max intensity to be scaled to
 * \return corrected map
 */
VolumeImagePointer CropIntensitiesVolume(
  const VolumeImageType * inputImage,
  const InternalPixelType outputMinimum = -3,
  const InternalPixelType outputMaximum = +3 );

/**
 * This function will input a volume image and apply a bilater filter (to discard noise).
 * \param inputImage
 * \return corrected map
 */
VolumeImagePointer ApplyBilateralFilterVolume(
  const VolumeImageType * inputImage,
  const InternalPixelType domainSigma = 0.5f,
  const InternalPixelType rangeSigma  = 0.3f );
////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * This function will input an volume image and will rescale intensities between the given ranges
 * \param inputImage
 * \param minIntensity min intensity to be scaled to
 * \param maxIntensity max intensity to be scaled to 
 * \return corrected map
 */
VolumeImagePointer RescaleIntensitiesVolume(
  const VolumeImageType * inputImage,
  const InternalPixelType minIntensity = 0.0,
  const InternalPixelType maxIntensity = 255.0);
  
} // namespace
#endif // __ccipdPreprocessVolume_h
