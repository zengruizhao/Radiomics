#ifndef __ccipdMFAPruneDrivingFeatures_hxx
#define __ccipdMFAPruneDrivingFeatures_hxx



//////////////////////////////////////////////////////////////////////////////////////////

// ccipd includes
#include "ccipdMFAPruneDrivingFeatures.h"
#include "ccipdMFAExtractorHasFeature.h"

// boost includes
#include <boost/foreach.hpp>

//////////////////////////////////////////////////////////////////////////////////////////




namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////
FeatureIDConstCollection PruneDrivingFeatures(
  const MFAFeatureExtractor      & extractor,
  const FeatureIDConstCollection & unprunedFeatures )
{

  // we will store the pruned collection

  // start as empty
  FeatureIDConstCollection prunedCollection;

  // loop through each object in the unpruned collection
  BOOST_FOREACH( const FeatureIDConstPointer & currentFeatureID,
    unprunedFeatures )
    {

    if ( currentFeatureID && HasFeature( extractor, *currentFeatureID ) )
      {

      // if it exists in the extractor
      // then we are allowed to add it
      // into our final collection
      prunedCollection.insert( prunedCollection.end(), currentFeatureID );

      } // hasfeature

    } // for each object

  // output
  return prunedCollection;

} // PruneDrivingFeatures
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace ccipd
#endif // __ccipdMFAPruneDrivingFeatures_hxx
