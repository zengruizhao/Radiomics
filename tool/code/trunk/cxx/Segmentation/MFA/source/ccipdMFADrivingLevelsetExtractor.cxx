


//////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdMFADrivingLevelsetExtractor.h"
#include "ccipdMFADrivingLevelsetObjects.h"
#include "ccipdMFALevelsetExtractor.h"
#include "ccipdMFALevelsetExtractorClone.h"
#include "ccipdMFADrivingFeaturePreparer.h"
#include "ccipdMFAModel.h"
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////
MFALevelsetExtractorPointer CreateDrivingLevelsetExtractor(
  const MFADrivingFeaturePreparer & drivingFeatures )
{

  // first extract the model
  const MFAModelConstPointer model = drivingFeatures.GetMFAModel();

  if ( !model )
    return MFALevelsetExtractorPointer();

  // now, extract the Levelset extractor to clone
  const MFALevelsetExtractorConstPointer trainedLevelsetExtractor =
    model->GetLevelsetExtractor();

  if ( !trainedLevelsetExtractor )
    return MFALevelsetExtractorPointer();

  // clone it so we can modify a new one
  const MFALevelsetExtractorPointer newLevelsetExtractor =
    Clone( *trainedLevelsetExtractor );

  // now get the driving Levelset features
  const MaskObjectCollectionType drivingLevelsetFeatures =
    GetDrivingLevelsetObjects( drivingFeatures );

  // give them to the new extractor
  newLevelsetExtractor->SetWhichLevelsetObjects( drivingLevelsetFeatures );

  // output
  return newLevelsetExtractor;

} // CreateDrivingLevelsetExtractor
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace ccipd
