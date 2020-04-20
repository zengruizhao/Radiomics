#ifndef __ccipdCalculateSimpleMedialAxesDistance_h
#define __ccipdCalculateSimpleMedialAxesDistance_h



//////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdDataDLL.h"
#include "ccipdCore.h"
#include "ccipdVolumeTypes.h"
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////
/**
 * compute distance between the central axes of two level set ( main axis is defined as the line 
 * with the min level set value with in the surface of the level set ); 
 * Useful to measure the deviation between urethras
 * 
 * \ingroup Validation
 */
Data_EXPORT
EvaluationValueType CalculateSimpleMedialAxesDistance(
  const VolumeImageType * levelSet1,
  const VolumeImageType * levelSet2);
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
/**
 * compute distance between the central axes of several level set as average of every two 
 * levelsets in the cohort 
 * 
 * \ingroup Validation
 */
Data_EXPORT
EvaluationValueType CalculateSimpleMedialAxesDistance(
  VolumeImagesContainer levelSets);
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace ccipd
#endif // __ccipdCalculateSimpleMedialAxesDistance_h
