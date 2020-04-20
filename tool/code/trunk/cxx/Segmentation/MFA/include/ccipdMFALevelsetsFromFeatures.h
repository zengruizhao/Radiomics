#ifndef __ccipdMFALevelsetsFromFeatures_h
#define __ccipdMFALevelsetsFromFeatures_h



//////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdPatientTypes.h"
#include "ccipdFeatureCollectionTypes.h"
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////
/**
 * Return the associated mask object
 * for each feature that represents
 * a levelset.
 * 
 * It's therefore possible that
 * the size of the output is
 * less than the size of the input.
 * 
 * \ingroup MFA
 * \author  Rob Toth
 * \date    2013-01-21
 */
MaskObjectCollectionType GetLevelsetObjectsFromFeatures(
  const FeatureIDConstCollection & whichFeatures );
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace ccipd
#endif // __ccipdMFALevelsetsFromFeatures_h
