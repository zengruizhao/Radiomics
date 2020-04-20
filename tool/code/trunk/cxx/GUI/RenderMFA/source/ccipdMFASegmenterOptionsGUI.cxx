


//////////////////////////////////////////////////////////////////////////////////////////

// ccipd includes
#include "ccipdMFASegmenterOptionsGUI.h"
#include "ccipdMFASegmenterOptionsDialog.h"
#include "ccipdMFASegmenter.h"
#include "ccipdMFADrivingFeaturePreparer.h"
#include "ccipdMFAFeaturesFromLevelsets.h"
#include "ccipdFeaturesFromTextures.h"

//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////
void ModifyMFASegmenterOptions( MFASegmenter & segmenter )
{

  // create a new dialog to change the options
  MFASegmenterOptionsDialog optionsDialog( segmenter );

  const auto returnValue = optionsDialog.exec();

  // return if the user pressed cancel
  if ( returnValue != QDialog::Accepted )
    return;

  // the user didn't press cancel, so update the MFA

  const auto drivingFeatures = segmenter.GetDrivingFeaturePreparer();
  
  // update the levelset objects
  drivingFeatures->SetInputLevelsetFeatures(
    GetFeaturesFromLevelsetObjects( optionsDialog.GetLevelsetObjects() ) );

  // update the texture features
  drivingFeatures->SetInputTextureFeatures(
    GetFeaturesFromTextures( optionsDialog.GetTextureFeatures() ) );

  // update the extrapolation
  drivingFeatures->SetExtrapolateInMM( optionsDialog.GetExtrapolation() );
  
  // update the algorithms
  segmenter.SetRegistrationAlgorithms( optionsDialog.GetAlgorithms() );

  // update the segmentation object
  segmenter.SetSegmentationObject( optionsDialog.GetSegmentationObject() );

  // update the trained MFA file
  segmenter.SetTrainedMFAFile( optionsDialog.GetTrainedMFAFile() );

  // update the verbosity
  segmenter.SetVerbose( optionsDialog.GetVerbose() );

} // ModifyMFASegmenterOptions
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace ccipd
