#ifndef __ccipdMFAFeaturesFromLevelsets_h
#define __ccipdMFAFeaturesFromLevelsets_h



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
FeatureIDConstCollection GetFeaturesFromLevelsetObjects(
  const MaskObjectCollectionType & whichLevelsets );
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace ccipd
#endif // __ccipdMFAFeaturesFromLevelsets_h
