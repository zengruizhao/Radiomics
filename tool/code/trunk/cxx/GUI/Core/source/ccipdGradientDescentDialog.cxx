
// ccipd includes
#include "ccipdGradientDescentDialog.h"
#include "ccipdRegisterOptimizerGradientDescent.h"
#include "ccipdDisableWarningsMacro.h"
// Qt includes

#include "ui_ccipdGradientDescentDialog.h"
#include <QInputDialog>
#include <QSpinBox>
#include "ccipdEnableWarningsMacro.h"

//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{
  


//////////////////////////////////////////////////////////////////////////////////////////
GradientDescentDialog::GradientDescentDialog(
  const RegisterOptimizerGradientDescent & gradientDescent ) :
ui( new Ui_GradientDescentDialog ) // set up the ui
{

  Ui_GradientDescentDialog & interface = *this->ui;
  interface.setupUi( this ); // we need to do this first

  interface.m_IterationsSpin->setValue(
    static_cast< int >( gradientDescent.GetGradientDescentMaxIterations() ) );

  interface.m_GradientToleranceSpin->setValue( gradientDescent.GetGradientTolerance() );
  interface.m_RelaxationSpin       ->setValue( gradientDescent.GetRelaxation       () );
  interface.m_MinStepLengthSpin    ->setValue( gradientDescent.GetMinStepLength    () );
  interface.m_MaxStepLengthSpin    ->setValue( gradientDescent.GetMaxStepLength    () );

} // Constructor
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
GradientDescentDialog::~GradientDescentDialog()
{
} // Destructor
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
GradientDescentDialog::FloatingType GradientDescentDialog::GetRelaxation() const
{
  return this->ui->m_RelaxationSpin->value();
}
GradientDescentDialog::FloatingType GradientDescentDialog::GetMaxStepLength() const
{
  return this->ui->m_MaxStepLengthSpin->value();
}
GradientDescentDialog::FloatingType GradientDescentDialog::GetMinStepLength() const
{
  return this->ui->m_MinStepLengthSpin->value();
}
GradientDescentDialog::FloatingType GradientDescentDialog::GetTolerance() const
{
  return this->ui->m_GradientToleranceSpin->value();
}
unsigned int GradientDescentDialog::GetIterations() const
{
  return static_cast< unsigned int >( this->ui->m_IterationsSpin->value() );
}
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace ccipd
