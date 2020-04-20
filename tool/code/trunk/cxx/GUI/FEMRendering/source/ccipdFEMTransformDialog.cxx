



//////////////////////////////////////////////////////////////////////////////////////////

// ccipd includes
#include "ccipdFEMTransformDialog.h"
#include "ccipdRegisterFEMTransform.h"
#include "ccipdRegistrationFEMOptions.h"
#include "ccipdDisableWarningsMacro.h"


// Qt includes

#include "ui_ccipdFEMTransformDialog.h"
#include <QSpinBox>
#include "ccipdEnableWarningsMacro.h"

//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{
  


//////////////////////////////////////////////////////////////////////////////////////////
FEMTransformDialog::FEMTransformDialog(
  const RegisterFEMTransform & registration ) :
ui( new Ui_FEMTransformDialog ) // set up the ui
{

  Ui_FEMTransformDialog & interface = *this->ui;
  interface.setupUi( this ); // we need to do this first

  interface.m_ThresholdValueSpin->setValue(
    registration.GetThresholdValue() );
  interface.m_ThresholdMaskCheck->setChecked(
    registration.GetThresholdMask() );
  interface.m_DownsampleRateX->setValue(
    static_cast< int >( registration.GetDownsampleRateX() ) );
  interface.m_DownsampleRateY->setValue(
    static_cast< int >( registration.GetDownsampleRateY() ) );
  interface.m_DownsampleRateZ->setValue(
    static_cast< int >( registration.GetDownsampleRateZ() ) );
  interface.m_NumberOfSpheresSpin->setValue(
    static_cast< int >( registration.GetNumberOfSpheres() ) );
  interface.m_MinSphereRadiusSpin->setValue(
    registration.GetMinSphereRadius() );
  interface.m_MaxSphereRadiusSpin->setValue(
    registration.GetMaxSphereRadius() );
  interface.m_MinMagnitudeSpin->setValue(
    registration.GetMinShrinkMagnitude() );
  interface.m_MaxMagnitudeSpin->setValue(
    registration.GetMaxShrinkMagnitude() );
  interface.m_NumberOfSurfaceLoadsSpin->setValue(
    registration.GetNumberOfSurfaceLoads() );
  interface.m_MinScalingSpin->setValue(
    registration.GetMinScaling() );
  interface.m_MaxScalingSpin->setValue(
    registration.GetMaxScaling() );

  interface.m_NeverUseGPUCheck->setChecked(
#ifdef ENABLE_VIENNACL
    // with viennacl the user hsa the option
    registration.GetNeverUseGPU()
#else
    // we never allow the GPU without viennacl
    true
#endif // ENABLE_VIENNACL
  );

  // make sure this gets called
  this->SlotThresholdMask();

} // Constructor
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
FEMTransformDialog::~FEMTransformDialog()
{
} // Destructor
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void FEMTransformDialog::SlotThresholdMask()
{
  // disable the threshold value when we change the check box
  const bool thresholdMask = this->ui->m_ThresholdMaskCheck->isChecked();
  this->ui->m_ThresholdValueSpin->setEnabled( thresholdMask );
}
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
bool FEMTransformDialog::GetThresholdMask() const
{
  return this->ui->m_ThresholdMaskCheck->isChecked();
}
bool FEMTransformDialog::GetNeverUseGPU() const
{
  return this->ui->m_NeverUseGPUCheck->isChecked();
}
InternalPixelType FEMTransformDialog::GetThresholdValue() const
{
  return static_cast< InternalPixelType >(
    this->ui->m_ThresholdValueSpin->value() );
}
unsigned int FEMTransformDialog::GetDownsampleRateX() const
{
  return this->ui->m_DownsampleRateX->value();
}
unsigned int FEMTransformDialog::GetDownsampleRateY() const
{
  return this->ui->m_DownsampleRateY->value();
}
unsigned int FEMTransformDialog::GetDownsampleRateZ() const
{
  return this->ui->m_DownsampleRateZ->value();
}
unsigned int FEMTransformDialog::GetNumberOfSpheres() const
{
  return this->ui->m_NumberOfSpheresSpin->value();
}
unsigned int FEMTransformDialog::GetNumberOfSurfaceLoads() const
{
  return this->ui->m_NumberOfSurfaceLoadsSpin->value();
}
TransformValueType FEMTransformDialog::GetMinSphereRadius() const
{
  return this->ui->m_MinSphereRadiusSpin->value();
}
TransformValueType FEMTransformDialog::GetMaxSphereRadius() const
{
  return this->ui->m_MaxSphereRadiusSpin->value();
}
TransformValueType FEMTransformDialog::GetMinMagnitude() const
{
  return this->ui->m_MinMagnitudeSpin->value();
}
TransformValueType FEMTransformDialog::GetMaxMagnitude() const
{
  return this->ui->m_MaxMagnitudeSpin->value();
}
float FEMTransformDialog::GetMinScaling() const
{
  return static_cast< float >( this->ui->m_MinScalingSpin->value() );
}
float FEMTransformDialog::GetMaxScaling() const
{
  return static_cast< float >( this->ui->m_MaxScalingSpin->value() );
}
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace ccipd
