#ifndef __ccipdCalculateLandmarkRMSE_h
#define __ccipdCalculateLandmarkRMSE_h



/**
 * \addtogroup Evaluate
 * @{
 */
 
 
 
//////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdLandmarkTypes.h"
#include "ccipdDataDLL.h"
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



/**
 * \brief computes the root mean squared error between two point sets
 *
 * \param[in] fixed the points in the fixed image
 * \param[in] moving the points in the moving image
 * \param[in] verbose If true, display each landmark.
 * by default don't check; only check in the case of the urethra
 */
Data_EXPORT
EvaluationValueType CalculateLandmarkRMSE( 
  const LandmarkSetType * const fixed,
	const LandmarkSetType * const moving,
  const bool                    verbose = false );

  
  
} // namespace
/** @{ */
#endif // __ccipdCalculateLandmarkRMSE_h