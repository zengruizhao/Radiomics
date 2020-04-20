#ifndef __ccipdCalculateSimpleMedialAxesRMSE_h
#define __ccipdCalculateSimpleMedialAxesRMSE_h



/**
 * \addtogroup Evaluation
 * @{
 */
 
 
 
//////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdLandmarkTypes.h"
#include "ccipdVolumeTypes.h"
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



/**
 * \brief computes the root mean squared error between two medial axes.
 *
 * \param fixed medial axis point set
 * \param moving medial axis point set
 * This function checks if indexes are negative (data is missing);
 * if so that point is excluded from the calculation.
 */
EvaluationValueType CalculateSimpleMedialAxesRMSE( 
  const MedialAxisSetType * const fixed,
	const MedialAxisSetType * const moved);

} // namespace ccipd

/** @{ */
#endif // __ccipdCalculateSimpleMedialAxesRMSE_h