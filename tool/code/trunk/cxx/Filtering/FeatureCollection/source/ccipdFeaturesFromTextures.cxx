

//////////////////////////////////////////////////////////////////////////////////////////
// ccipd includes
#include "ccipdFeaturesFromTextures.h"
#include "ccipdFeatureID.h"
#include "ccipdDisableWarningsMacro.h"
// boost includes
#include <boost/foreach.hpp>
//////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdEnableWarningsMacro.h"


namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////
FeatureIDConstCollection GetFeaturesFromTextures(
  const TextureFeatureEnumCollection  & whichTextures )
{

  FeatureIDConstCollection features;

  // loop through each feature
  BOOST_FOREACH( const TextureFeatureEnum whichTexture, whichTextures )
    {

    // create a new feature with that
    features.push_back( FeatureIDPointer( FeatureID::New( whichTexture ) ) );

    } // for each feature

  // output
  return features;

} // GetFeaturesFromTextures
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace ccipd
