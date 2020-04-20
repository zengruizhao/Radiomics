#ifndef __ccipdCalculateHausdorffDistance_h
#define __ccipdCalculateHausdorffDistance_h



/**
 * \addtogroup Evaluation
 * @{
 */
 
 
 
//////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdVolumeTypes.h"
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



/**
 * \brief  compute HausdorffDistance 
 * 
 * \param fixed an image
 * \param moving the other image
 * \return the distance
 */
 
EvaluationValueType CalculateHausdorffDistance(
  const VolumeImageType * const fixed,
  const VolumeImageType * const moving );
  
} // namespace ccipd

/** @{ */
#endif // __ccipdCalculateHausdorffDistance_h