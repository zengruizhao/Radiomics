#ifndef __ccipdN4MRIBiasCorrection_h
#define __ccipdN4MRIBiasCorrection_h



////////////////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdVolumeTypes.h"
////////////////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * This function will input an image
 * and correct the bias field, returning
 * a corrected result.
 * 
 * \param[in] maskImage
 *   An optional mask for where in the image
 *   to perform the bias correction.
 *   Any non-zero mask value will be considered!
 * 
 * \returns NULL if any errors occur.
 *  
 * \ingroup BiasCorrection
 * \author  Rob Toth
 * \date    2012-10-11
 */
VolumeImagePointer N4CorrectBiasField(
  const VolumeImageType     * const inputImage,
  const VolumeMaskImageType * const maskImage               = 0,
  const InternalPixelType           addValue                = 10,
  const unsigned int                resolutions             = 1,
  const unsigned int                iterationsPerResolution = 50,
  const unsigned int                histogramBins           = 200,
  const bool                        verbose                 = false );
////////////////////////////////////////////////////////////////////////////////////////////////////


  
} // namespace ccipd
#endif // __ccipdN4MRIBiasCorrection_h
