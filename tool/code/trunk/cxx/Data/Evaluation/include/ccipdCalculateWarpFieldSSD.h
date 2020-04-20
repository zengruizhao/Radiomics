#ifndef __ccipdCalculateWarpFieldSSD_h
#define __ccipdCalculateWarpFieldSSD_h



/**
 * \addtogroup Evaluate
 * @{
 */
 
 
 
//////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdVolumeTypes.h"
#include "ccipdDataDLL.h"
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



/**
 * \brief computes the mean ssd between two deformation fields
 *
 * \param field1 forward deformation field
 * \param field2 inverse deformation field
 */
 
Data_EXPORT
EvaluationValueType CalculateWarpFieldSSD(
  const VolumeWarpFieldType * const field1,
  const VolumeWarpFieldType * const filed2
  );
  
} // namespace
/** @{ */
#endif // __ccipdCalculateWarpFieldSSD_h