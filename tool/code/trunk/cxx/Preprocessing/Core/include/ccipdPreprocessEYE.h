#ifndef __ccipdPreprocessEYE_h
#define __ccipdPreprocessEYE_h



/**
 * This file contains the preprocessing code for
 * TRUS (EYE) images.
 */



////////////////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdTRUSTypes.h" // for TRUS image types
////////////////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{

  
 
////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * This function will input an TRUS image (Raw EYE data)
 * correct the attenuation, normalize the image( between 0 to 1),
 * crop the intensities, apply noise reduction, and then rescale
 *
 * \param inputImage
 * \param outputMinimum min intensity to be scaled to
 * \param outputMaximum max intensity to be scaled to 
 * \param intensityThreshold intensity threshold to crop 
 * (in terms of sigma). 
 * \return corrected image
 */
TRUSImagePointer PreprocessRawEYE(
  const TRUSImageType * const inputImage,
  const InternalPixelType intensityThreshold = 2.5,
  const InternalPixelType outputMinimum = 0,
  const InternalPixelType outputMaximum = 255  );

/**
* This function takes as input a TRUS image  (Raw EYE data), and corrects the attenuation
* by considering pixels within biasCorrectionSigma.
*
* \param inputImage
* \param biasCorrectionSigma width of attenuation window
* \return corrected image
*/
TRUSImagePointer CorrectAttenuationRawEye(
  const TRUSImageType * const inputImage,
  const float biasCorrectionSigma );
////////////////////////////////////////////////////////////////////////////////////////////////////



} // namespace
#endif // __ccipdPreprocessEYE_h
