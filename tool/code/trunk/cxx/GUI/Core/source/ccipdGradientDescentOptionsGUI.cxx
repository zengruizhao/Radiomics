

//////////////////////////////////////////////////////////////////////////////////////////

// ccipd includes
#include "ccipdGradientDescentOptionsGUI.h"
#include "ccipdGradientDescentDialog.h"
#include "ccipdRegisterOptimizerGradientDescent.h"

//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////
void ModifyGradientDescentOptions( RegisterOptimizerGradientDescent & gradientDescent )
{

  // create a new dialog to change the options
  GradientDescentDialog optionsDialog( gradientDescent );

  const auto returnValue = optionsDialog.exec();

  // return if the user pressed cancel
  if ( returnValue != QDialog::Accepted )
    return;

  // modify the values
  typedef RegisterOptimizerGradientDescent::FloatingType FloatingType;
  gradientDescent.SetGradientTolerance(
    static_cast< FloatingType >( optionsDialog.GetTolerance() ) );
  gradientDescent.SetMinStepLength(
    static_cast< FloatingType >( optionsDialog.GetMinStepLength() ) );
  gradientDescent.SetMaxStepLength(
    static_cast< FloatingType >( optionsDialog.GetMaxStepLength() ) );
  gradientDescent.SetRelaxation(
    static_cast< FloatingType >( optionsDialog.GetRelaxation() ) );
  gradientDescent.SetGradientDescentMaxIterations(
    optionsDialog.GetIterations() );

} // ModifyGradientDescentOptions
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace ccipd
