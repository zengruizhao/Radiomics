


//////////////////////////////////////////////////////////////////////////////////////////

// ccipd includes
#include "ccipdMFAFeaturesFromLevelsets.h"
#include "ccipdMFALevelsetExtractor.h"

// boost includes
#include <boost/foreach.hpp>

//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////
FeatureIDConstCollection GetFeaturesFromLevelsetObjects(
  const MaskObjectCollectionType & whichLevelsets )
{

  FeatureIDConstCollection whichFeatures;

  // loop through all our Levelset features
  BOOST_FOREACH( const MaskObjectType & whichObject, whichLevelsets )
    {

    // create a new feature id with this Levelset feature
    const FeatureIDConstPointer whichFeature =
      MFALevelsetExtractor::CreateFeatureIDWithObject( whichObject );

    whichFeatures.push_back( whichFeature );
    
    } // for each Levelset feature

  // output
  return whichFeatures;

} // GetFeaturesFromLevelsetObjects
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace ccipd

