#ifndef __ccipdConcatenateFeatureImages_h
#define __ccipdConcatenateFeatureImages_h



////////////////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdVolumeTypes.h"
#include "ccipdFeatureCollectionTypes.h"
////////////////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Concatenate components of 2 feature images into one feature image.
 * 
 * This function will take two feature images
 * and concatenates them into a new feature image.
 * 
 * You also input the FeatureID's, and it will
 * give you the output concatenated ID's.
 * 
 * \note
 *   - The two feature must be in the same coordinate frame.
 *   - The feature id collections must have the same number
 *     of elements as their corresponding feature images.
 * 
 * \author  Rob Toth
 * \ingroup FeatureCollection
 */
VolumeFeaturesPointer Concatenate2FeatureImages(
  const VolumeFeaturesType  * featureImage1,
  const VolumeFeaturesType  * featureImage2,
  const FeatureIDCollection & featureIDs1, // ID's for featureImage1
  const FeatureIDCollection & featureIDs2, // ID's for featureImage2
        FeatureIDCollection & concatenatedFeatureIDs );
////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * You might not actually care about the feature id's,
 * and this function simply ignores them.
 * 
 * \see     Concatenate2FeatureImages()
 * \author  Rob Toth
 * \ingroup FeatureCollection
 */
VolumeFeaturesPointer Concatenate2FeatureImages(
  const VolumeFeaturesType  * featureImage1,
  const VolumeFeaturesType  * featureImage2 );
////////////////////////////////////////////////////////////////////////////////////////////////////



} // namespace
#endif // __ccipdConcatenateFeatureImages_h
