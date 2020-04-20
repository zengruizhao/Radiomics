


//////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdMFALevelsetsFromFeatures.h"
#include "ccipdMFALevelsetExtractor.h"

#include <boost/foreach.hpp>

//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////
MaskObjectCollectionType GetLevelsetObjectsFromFeatures(
  const FeatureIDConstCollection & whichFeatures )
{

  MaskObjectCollectionType levelsetObjects;

  // loop through each feature
  BOOST_FOREACH( const FeatureIDConstPointer & whichFeature, whichFeatures )
    {

    // make sure there's a feature
    if ( !whichFeature )
      continue;

    // get the associated levelset object name
    const MaskObjectType levelsetObject =
      MFALevelsetExtractor::ExtractObjectNameFromFeatureID( *whichFeature );

    // if there were no errors, insert it
    if ( !levelsetObject.empty() )
      levelsetObjects.insert( levelsetObject );

    } // for each feature

  // output
  return levelsetObjects;

} // GetLevelsetObjectsFromFeatures
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace ccipd

