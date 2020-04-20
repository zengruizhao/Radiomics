#ifndef __ccipdCalculateCenterOfMassDistance
#define __ccipdCalculateCenterOfMassDistance



/**
 * \addtogroup Evaluation
 * @{
 */
 
 
 
//////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdVolumeTypes.h"
#include "ccipdDataDLL.h"
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



/** Calculate center of mass difference in mm.
  */
Data_EXPORT
EvaluationValueType CalculateCenterOfMassDistance(
  const VolumeMaskImageType * const mask1,
  const VolumeMaskImageType * const mask2 );
      
} // namespace ccipd

/** @{ */
#endif // __ccipdCalculateCenterOfMassDistance