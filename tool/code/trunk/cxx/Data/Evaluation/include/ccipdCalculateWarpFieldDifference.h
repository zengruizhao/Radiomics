#ifndef __ccipdCalculateWarpFieldDifference_h
#define __ccipdCalculateWarpFieldDifference_h



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
 * \brief computes difference between two deformation fields
 *
 * \param field1 forward deformation field
 * \param field2 inverse deformation field
 */
 
VolumeWarpFieldPointer CalculateWarpFieldDifference(
  const VolumeWarpFieldType * const field1,
  const VolumeWarpFieldType * const field2
  );
  
} // namespace
/** @{ */
#endif // __ccipdCalculateWarpFieldDifference_h