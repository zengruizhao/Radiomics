#ifndef __ccipdMFAExtractorHasFeature_h
#define __ccipdMFAExtractorHasFeature_h



//////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdMFATypes.h" // for our forward declared MFAFeatureExtractorType
#include "ccipdFeatureCollectionTypes.h"
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////
/**
 * This function will check if
 * the extractor had
 * a given object added to it
 * in its list of feature objects.
 * 
 * \ingroup MFA
 * \author  Rob Toth
 * \date    2012-01-16
 */
bool HasFeature(
  const MFAFeatureExtractor & extractor,
  const FeatureID           & whichFeature );
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace ccipd
#endif // __ccipdMFAExtractorHasFeature_h
