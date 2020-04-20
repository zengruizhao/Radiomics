


//////////////////////////////////////////////////////////////////////////////////////////

// ccipd includes
#include "ccipdSIMFeatureExtractorInsertion.h"
#include "ccipdSIMFeatureExtractor.h"
#include "ccipdFeatureCollection.h"

// boost includes
#include <boost/foreach.hpp>

#include "ccipdDisableWarningsMacro.h"
  // itk includes
  #include <itkImage.h>
#include "ccipdEnableWarningsMacro.h"
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////
void SIMInsertFeatures(
  SIMFeatureExtractor & extractor,
  FeatureCollection   & existingCollection )
{

  // first get all the feature ID's
  const FeatureIDConstCollection whichFeatures =
    extractor.GetWhichFeatures();

  // now loop through
  BOOST_FOREACH( const FeatureIDConstPointer & whichFeature, whichFeatures )
    {

    // extract the feature
    const VolumeImageConstPointer
      currentFeature = extractor.GetFeature( *whichFeature );

    // insert it into our collection
    existingCollection.SetFeature( whichFeature, currentFeature );

    } // for each feature

} // SIMInsertFeatures
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace ccipd
