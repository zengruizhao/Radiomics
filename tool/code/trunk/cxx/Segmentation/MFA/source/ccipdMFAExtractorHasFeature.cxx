


//////////////////////////////////////////////////////////////////////////////////////////

// ccipd includes
#include "ccipdMFAExtractorHasFeature.h"
#include "ccipdSIMFeatureExtractor.h"
#include "ccipdFeatureID.h"

// std includes
#include <algorithm>

// boost includes
#include <boost/foreach.hpp>

//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////
bool HasFeature(
  const MFAFeatureExtractor & extractor,
  const FeatureID           & whichFeature )
{

  // get our currenct objects
  const auto whichFeatures = extractor.GetWhichFeatures();
  
  // see if it exists

  // since this is a vector of pointers we
  // have to dereference them to actually
  // compare the feature objects instead of
  // the pointers
  
  BOOST_FOREACH( const FeatureIDConstPointer & testFeatureID, whichFeatures )
    {
    if ( testFeatureID && ( *testFeatureID == whichFeature ) )
      return true;
    }

  return false;
  
} // HasFeature
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace ccipd
