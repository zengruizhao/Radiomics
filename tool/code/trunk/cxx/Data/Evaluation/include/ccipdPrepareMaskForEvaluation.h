#ifndef __ccipdPrepareMaskForEvaluation_h
#define __ccipdPrepareMaskForEvaluation_h



//////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdDataDLL.h"
#include "ccipdCore.h"
#include "ccipdVolumeTypes.h"
#include "ccipdLandmarkTypes.h"
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Helper function to smooth and increase a mask.
 * 
 * \ingroup Evaluation
 */
Data_EXPORT
VolumeMaskImageConstPointer PrepareMaskForEvaluation(
  const VolumeMaskImageType * mask,
  const VolumeMaskImageType * templateMask,
  const float                 smoothVariance   = 0,
  const float                 increaseSizeInMM = 0 );
//////////////////////////////////////////////////////////////////////////////////////////


} // namespace ccipd
#endif // __ccipdPrepareMaskForEvaluation_h