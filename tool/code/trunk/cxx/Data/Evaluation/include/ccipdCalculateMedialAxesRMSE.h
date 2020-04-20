#ifndef __ccipdCalculateMedialAxesRMSE_h
#define __ccipdCalculateMedialAxesRMSE_h



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
 * \brief computes the mean ssd between two point sets
 *
 * \param fixed the points in the fixed image
 * \param moving the points in the moving image
 * This function checks if indexes are negative (data is missing);
 * if so that point is excluded from the calculation.
 */
EvaluationValueType CalculateMedialAxesRMSE( 
  const MedialAxisSetType * const fixed,
	const MedialAxisSetType * const moved);

/**
 * \brief computes the mean ssd between two point sets
 *
 * \param fixed the points in the fixed image
 * \param moving the points in the moving image
 * The number of points doesn't need to be the same 
 * between the two point set; it searches for the closet 
 * point in the other cloud and averages distances
 */
EvaluationValueType CalculateMedialAxesRMSE( 
  const LandmarkSetType * const fixed,
	const LandmarkSetType * const moved);
    
} // namespace ccipd

/** @{ */
#endif // __ccipdCalculateMedialAxesRMSE_h