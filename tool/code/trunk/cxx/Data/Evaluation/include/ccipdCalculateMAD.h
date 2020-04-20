#ifndef __ccipdCalculateMAD_h
#define __ccipdCalculateMAD_h



//////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdDataDLL.h"
#include "ccipdCore.h"
#include "ccipdVolumeTypes.h"
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Calculate MAD segmentation evaluation measure.
 * 
 * \see     itk::MeanAbsoluteDistanceImageFilter
 * \ingroup Evalulation
 */
Data_EXPORT
EvaluationValueType CalculateMAD(
  const VolumeMaskImageType * segmentation,
  const VolumeMaskImageType * groundTruth,
  const float                 segmentationBorderThickness = .01,
  const float                 smoothVariance = 0,
  const float                 increaseSizeInMM = 0 );
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace
#endif // __ccipdCalculateMAD_h