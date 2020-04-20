


//////////////////////////////////////////////////////////////////////////////////////////

// ccipd includes
#include "ccipdFEMTransformOptionsGUI.h"
#include "ccipdFEMTransformDialog.h"
#include "ccipdRegisterFEMTransform.h"

//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////
void ModifyFEMTransformOptions( RegisterFEMTransform & registration )
{

  // create a new dialog to change the options
  FEMTransformDialog optionsDialog( registration );

  const auto returnValue = optionsDialog.exec();

  // return if the user pressed cancel
  if ( returnValue != QDialog::Accepted )
    return;

  // modify the values
  registration.SetNeverUseGPU         ( optionsDialog.GetNeverUseGPU         () );
  registration.SetThresholdMask       ( optionsDialog.GetThresholdMask       () );
  registration.SetThresholdValue      ( optionsDialog.GetThresholdValue      () );
  registration.SetDownsampleRateX     ( optionsDialog.GetDownsampleRateX     () );
  registration.SetDownsampleRateY     ( optionsDialog.GetDownsampleRateY     () );
  registration.SetDownsampleRateZ     ( optionsDialog.GetDownsampleRateZ     () );
  registration.SetNumberOfSpheres     ( optionsDialog.GetNumberOfSpheres     () );
  registration.SetNumberOfSurfaceLoads( optionsDialog.GetNumberOfSurfaceLoads() );
  registration.SetMinSphereRadius     ( optionsDialog.GetMinSphereRadius     () );
  registration.SetMaxSphereRadius     ( optionsDialog.GetMaxSphereRadius     () );
  registration.SetMinShrinkMagnitude  ( optionsDialog.GetMinMagnitude        () );
  registration.SetMaxShrinkMagnitude  ( optionsDialog.GetMaxMagnitude        () );
  registration.SetMinScaling          ( optionsDialog.GetMinScaling          () );
  registration.SetMaxScaling          ( optionsDialog.GetMaxScaling          () );

} // ModifyFEMTransformOptions
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace ccipd
