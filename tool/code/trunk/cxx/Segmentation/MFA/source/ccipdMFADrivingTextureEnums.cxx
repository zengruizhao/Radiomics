


//////////////////////////////////////////////////////////////////////////////////////////

// ccipd includes
#include "ccipdMFADrivingTextureEnums.h"
#include "ccipdMFADrivingFeaturePreparer.h"
#include "ccipdMFAModel.h"
#include "ccipdMFAPruneDrivingFeatures.h"
#include "ccipdMFATextureExtractor.h"
#include "ccipdTexturesFromFeatures.h"

//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////
TextureFeatureEnumCollection GetDrivingTextureEnums(
  const MFADrivingFeaturePreparer & drivingFeatures )
{

  // extract the input model
  const MFAModelConstPointer
    model =  drivingFeatures.GetMFAModel();

  if ( !model )
    return TextureFeatureEnumCollection();

  // get the texture extractor
  const MFATextureExtractorConstPointer textureExtractor =
    model->GetTextureExtractor();

  if ( !textureExtractor )
    return TextureFeatureEnumCollection();

  // extract the input features and prune them
  const FeatureIDConstCollection
    inputTextureFeatures   = drivingFeatures.GetInputTextureFeatures(),
    drivingTextureFeatures = PruneDrivingFeatures(
      *textureExtractor, inputTextureFeatures );

  // now, just get the texture enums from them
  return GetTexturesFromFeatures( drivingTextureFeatures );

} // GetDrivingTextureEnums
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace ccipd
