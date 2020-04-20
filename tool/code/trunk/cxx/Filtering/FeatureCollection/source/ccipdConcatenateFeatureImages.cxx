


////////////////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdConcatenateFeatureImages.h"
#include "ccipdFeatureID.h"
#include "ccipdExtractFeatureImage.h" // for extracting an individual component
#include "ccipdVolumeContainerToVolumeFeatures.h" // for concatenating

#include "ccipdDisableWarningsMacro.h"
  #include <itkImage.h>
  #include <itkVectorImage.h>
#include "ccipdEnableWarningsMacro.h"
////////////////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



////////////////////////////////////////////////////////////////////////////////////////////////////
VolumeFeaturesPointer Concatenate2FeatureImages(
  const VolumeFeaturesType  * featureImage1,
  const VolumeFeaturesType  * featureImage2,
  const FeatureIDCollection & featureIDs1, // ID's for featureImage1
  const FeatureIDCollection & featureIDs2,
        FeatureIDCollection & concatenatedFeatureIDs )
{

  // safety checks
  const unsigned int
    numberOfComponents1 = featureImage1->GetNumberOfComponentsPerPixel(),
    numberOfComponents2 = featureImage2->GetNumberOfComponentsPerPixel();

  // return empty if we have bad inputs
  if (
    featureIDs1.size() != numberOfComponents1 ||
    featureIDs2.size() != numberOfComponents2 )
    return VolumeFeaturesPointer();

  // now, concatenate our feature id's
  concatenatedFeatureIDs.clear();

  concatenatedFeatureIDs.insert(
    concatenatedFeatureIDs.begin(),
    featureIDs1.cbegin(),
    featureIDs1.cend() );

  concatenatedFeatureIDs.insert(
    concatenatedFeatureIDs.end(),
    featureIDs2.cbegin(),
    featureIDs2.cend() );

  // we are going to split up each component and
  // recombine them

  VolumeConstImagesContainer extractedFeatures;

  // go through image 1
  for ( unsigned int componentIndex = 0;
    componentIndex < numberOfComponents1; ++componentIndex )
    {

    // extract the given component
    const VolumeImageConstPointer extractedFeature(
      ExtractFeatureImage( featureImage1, componentIndex ) );

    // give it to our collection
    extractedFeatures.push_back( extractedFeature );

    } // for componentIndex

  // go through image 2
  for ( unsigned int componentIndex = 0;
    componentIndex < numberOfComponents2; ++componentIndex )
    {

    // extract the given component
    const VolumeImageConstPointer extractedFeature(
      ExtractFeatureImage( featureImage2, componentIndex ) );

    // give it to our collection
    extractedFeatures.push_back( extractedFeature );

    } // for componentIndex

  // concatenate and return
  return VolumeContainerToVolumeFeatures( extractedFeatures );

} // Concatenate2FeatureImages
////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////
VolumeFeaturesPointer Concatenate2FeatureImages(
  const VolumeFeaturesType  * featureImage1,
  const VolumeFeaturesType  * featureImage2 )
{

  // just create dummy collections
  const FeatureIDCollection
    featureIDs1( featureImage1->GetNumberOfComponentsPerPixel() ),
    featureIDs2( featureImage2->GetNumberOfComponentsPerPixel() );

  FeatureIDCollection concatenatedFeatureIDs;

  return Concatenate2FeatureImages(
    featureImage1, featureImage2,
    featureIDs1  , featureIDs2  ,
    concatenatedFeatureIDs );

} // Concatenate2FeatureImages ignoring feature id's
////////////////////////////////////////////////////////////////////////////////////////////////////



} // namespace
