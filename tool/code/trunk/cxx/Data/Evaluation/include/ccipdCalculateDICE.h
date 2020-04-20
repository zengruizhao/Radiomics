#ifndef __ccipdCalculateDICE_h
#define __ccipdCalculateDICE_h



//////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdDataDLL.h"
#include "ccipdCore.h"
#include "ccipdVolumeTypes.h"
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////
/**
 * This function will take in 2 masks, and
 * calculate the Dice Similarity Coefficient (DSC)
 * between them, as
 * DSC = 2 * TP / (2 * TP + FP + FN)
 * 
 * Note that it is symmetric, so
 * CalculateDice(a, b) == CalculateDice(b, a).
 * Denotion of segmentation and ground truth
 * is simply for readability.
 * 
 * If smoothVariance > 0, then the masks will be converted
 * to a levelset, and smoothed, prior to Dice calculation.
 * 
 * \ingroup Evaluation
 */
Data_EXPORT
EvaluationValueType CalculateDice(
  const VolumeMaskImageType * segmentation,
  const VolumeMaskImageType * groundTruth,
  const float                 smoothVariance   = 0,
  const float                 increaseSizeInMM = 0 );
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
/**
 * 
 * compute dice coefficient between n masks in a cohort as
 * n*|N1 and N2 and ... and Nn|/(|N1| + |N2| + ... + |Nn|)
 * 
 * \param masks the cohort 
 * 
 * \todo
 *   - highly unoptimized should derive class from ImageToImageFilter and use thread
 *   - not essential in this case as this will only be run rarely 
 * 
 * \author  Mirabela Rusu
 * \ingroup Segmentation
 */
Data_EXPORT
EvaluationValueType CalculateDice(
 VolumeMaskImagesContainer masks);
//////////////////////////////////////////////////////////////////////////////////////////

}

#endif // __ccipdCalculateDICE_h