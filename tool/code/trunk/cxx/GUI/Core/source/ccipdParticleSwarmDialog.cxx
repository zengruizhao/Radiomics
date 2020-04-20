



//////////////////////////////////////////////////////////////////////////////////////////

// ccipd includes
#include "ccipdParticleSwarmDialog.h"
#include "ccipdRegisterOptimizerParticleSwarm.h"
#include "ccipdDisableWarningsMacro.h"
// Qt includes

  #include "ui_ccipdParticleSwarmDialog.h"
  #include <QInputDialog>
  #include <QSpinBox>
#include "ccipdEnableWarningsMacro.h"

#ifdef _MSC_VER
#pragma warning( disable: 4482 )
#endif
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{
  


//////////////////////////////////////////////////////////////////////////////////////////
ParticleSwarmDialog::ParticleSwarmDialog(
  const RegisterOptimizerParticleSwarm & particleSwarm ) :
ui( new Ui_ParticleSwarmDialog ) // set up the ui
{

  Ui_ParticleSwarmDialog & interface = *this->ui;
  interface.setupUi( this ); // we need to do this first

  interface.m_IterationsSpin->setValue(
    static_cast< int >( particleSwarm.GetParticleSwarmMaxIterations() ) );
  interface.m_NumberOfParticlesSpin->setValue( particleSwarm.GetNumberOfParticles() );

  // http://bit.ly/Xj5PII
  QString textToFind;
  switch ( particleSwarm.GetWhichParticleSwarm() )
    {
    case RegisterOptimizerParticleSwarm::WhichParticleSwarm::ParticleSwarm :
      textToFind = "Particle Swarm";
      break;
    case RegisterOptimizerParticleSwarm::WhichParticleSwarm::InitializationBiased :
      textToFind = "Initialization Biased";
      break;
    case RegisterOptimizerParticleSwarm::WhichParticleSwarm::MultiThreaded :
      textToFind = "MultiThreaded";
      std::cout << "Hello";
      break;
    } // switch
    
  const int index = interface.m_TypeCombo->findText( textToFind );
  if ( index >= 0 )
    interface.m_TypeCombo->setCurrentIndex( index );
  else
    std::cerr << "Error: Unable to find the particle swarm type." << std::endl;

} // Constructor
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
ParticleSwarmDialog::~ParticleSwarmDialog()
{
} // Destructor
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
bool ParticleSwarmDialog::GetInitializationBiased() const
{
  return this->ui->m_TypeCombo->currentText().compare( "Initialization Biased" ) == 0;
}
bool ParticleSwarmDialog::GetMultiThreaded() const
{
  return this->ui->m_TypeCombo->currentText().compare( "MultiThreaded" ) == 0;
}
unsigned int ParticleSwarmDialog::GetNumberOfParticles() const
{
  return static_cast< unsigned int >( this->ui->m_NumberOfParticlesSpin->value() );
}
unsigned int ParticleSwarmDialog::GetMaxIterations() const
{
  return static_cast< unsigned int >( this->ui->m_IterationsSpin->value() );
}
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace ccipd
