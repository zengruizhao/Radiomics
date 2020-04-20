


//////////////////////////////////////////////////////////////////////////////////////////

// ccipd includes
#include "ccipdParticleSwarmOptionsGUI.h"
#include "ccipdParticleSwarmDialog.h"
#include "ccipdRegisterOptimizerParticleSwarm.h"

#ifdef _MSC_VER
#pragma warning( disable: 4482 )
#endif // _MSC_VER
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////
void ModifyParticleSwarmOptions( RegisterOptimizerParticleSwarm & particleSwarm )
{

  // create a new dialog to change the options
  ParticleSwarmDialog optionsDialog( particleSwarm );

  const auto returnValue = optionsDialog.exec();

  // return if the user pressed cancel
  if ( returnValue != QDialog::Accepted )
    return;

  // modify the type
  if ( optionsDialog.GetInitializationBiased() )
    particleSwarm.SetWhichParticleSwarm(
      RegisterOptimizerParticleSwarm::WhichParticleSwarm::InitializationBiased );
  else if ( optionsDialog.GetMultiThreaded() )
    particleSwarm.SetWhichParticleSwarm(
      RegisterOptimizerParticleSwarm::WhichParticleSwarm::MultiThreaded );
  else
    particleSwarm.SetWhichParticleSwarm(
      RegisterOptimizerParticleSwarm::WhichParticleSwarm::ParticleSwarm );
  
  // modify the values
  particleSwarm.SetNumberOfParticles   ( optionsDialog.GetNumberOfParticles   () );
  particleSwarm.SetParticleSwarmMaxIterations( optionsDialog.GetMaxIterations () );

} // ModifyParticleSwarmOptions
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace ccipd
