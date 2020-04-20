

//////////////////////////////////////////////////////////////////////////////////////////
// ccipd includes
#include "ccipdTexturesFromFeatures.h"
#include "ccipdFeatureID.h"
#include "ccipdDisableWarningsMacro.h"
// boost includes
#include <boost/foreach.hpp>
//////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdEnableWarningsMacro.h"


namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////
TextureFeatureEnumCollection GetTexturesFromFeatures(
  const FeatureIDConstCollection & whichFeatures )
{

  TextureFeatureEnumCollection textures;

  // loop through each feature
  BOOST_FOREACH( const FeatureIDConstPointer & whichFeature, whichFeatures )
    {

    // make sure there's a feature
    if ( whichFeature && whichFeature->HasTextureFeature() )
      textures.push_back( *whichFeature->GetTextureFeature() );

    } // for each feature

  // output
  return textures;

} // GetTexturesFromFeatures
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace ccipd

