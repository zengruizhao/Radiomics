


//////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdMFADrivingTextureExtractor.h"
#include "ccipdMFADrivingTextureEnums.h"
#include "ccipdMFATextureExtractor.h"
#include "ccipdMFATextureExtractorClone.h"
#include "ccipdMFADrivingFeaturePreparer.h"
#include "ccipdMFAModel.h"
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////
MFATextureExtractorPointer CreateDrivingTextureExtractor(
  const MFADrivingFeaturePreparer & drivingFeatures )
{

  // first extract the model
  const MFAModelConstPointer model = drivingFeatures.GetMFAModel();

  if ( !model )
    return MFATextureExtractorPointer();

  // now, extract the texture extractor to clone
  const MFATextureExtractorConstPointer trainedTextureExtractor =
    model->GetTextureExtractor();

  if ( !trainedTextureExtractor )
    return MFATextureExtractorPointer();

  // clone it so we can modify a new one
  const MFATextureExtractorPointer newTextureExtractor =
    Clone( *trainedTextureExtractor );

  // now get the driving texture features
  const TextureFeatureEnumCollection drivingTextureFeatures =
    GetDrivingTextureEnums( drivingFeatures );

  // give them to the new extractor
  newTextureExtractor->SetWhichFeatureObjects( drivingTextureFeatures );

  // output
  return newTextureExtractor;

} // CreateDrivingTextureExtractor
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace ccipd
