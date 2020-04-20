#ifndef __ccipdPreprocessGIPL_h
#define __ccipdPreprocessGIPL_h



/**
 * This file contains the preprocessing code for
 * TRUS GIPL images.
 */



////////////////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdTRUSTypes.h" // for TRUS image types
////////////////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{

  
 
////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * This function will input an TRUS image  (GIPL format)
 * correct the bias field, normalize the image( between 0 to 1),
 * crop the intensities, and then apply noise reduction.
 *
 * \param inputImage
 * \param outputMinimum min intensity to be scaled to
 * \param outputMaximum max intensity to be scaled to 
 * \param intensityThreshold intensity threshold to crop 
 * (in terms of sigma). 
 * \return corrected image
 */
TRUSImagePointer PreprocessGIPL(
  const TRUSImageType * const inputImage,
  const InternalPixelType intensityThreshold = 2.5, 
  const InternalPixelType outputMinimum = 0,
  const InternalPixelType outputMaximum = 255 );

/**
* This function takes as input a TRUS image  (GIPL format)
* and corrects the attenuation by considering pixels within
* \biasCorrectionSigma.
*
* \param inputImage
* \param biasCorrectionSigma width of biasCorrection window
* \return corrected image
*/
TRUSImagePointer CorrectBiasFieldGIPL(
  const TRUSImageType * const inputImage,
  const float biasCorrectionSigma );
////////////////////////////////////////////////////////////////////////////////////////////////////



} // namespace
#endif // __ccipdPreprocessGIPL_h
