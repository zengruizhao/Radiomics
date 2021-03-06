#ifndef __ccipdParticleSwarmDialog_h
#define __ccipdParticleSwarmDialog_h
 



//////////////////////////////////////////////////////////////////////////////////////////

#ifdef _MSC_VER
#pragma warning( disable: 4127 )
#endif // _MSC_VER

// ccipd includes
#include "ccipdMacros.h"
#include "ccipdRegisterAlgorithmsForwardDeclarations.h"
#include "ccipdDisableWarningsMacro.h"


// qt includes
#include <QDialog>

// std includes
#include <memory>

//////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdEnableWarningsMacro.h"


//////////////////////////////////////////////////////////////////////////////////////////
// Forward Qt class declarations
class Ui_ParticleSwarmDialog;
//////////////////////////////////////////////////////////////////////////////////////////
 


namespace ccipd
{


/**
 * \brief GUI window for specifying gradient descent options.
 * 
 * \author  Rob Toth
 * \ingroup GUI
 */
class ParticleSwarmDialog :
  public QDialog
{



//////////////////////////////////////////////////////////////////////////////////////////
  Q_OBJECT
//////////////////////////////////////////////////////////////////////////////////////////



public:
//////////////////////////////////////////////////////////////////////////////////////////
  
  /**
   * \brief Constructor for the MFA options dialog.
   */
  explicit ParticleSwarmDialog( const RegisterOptimizerParticleSwarm & ParticleSwarm );

  virtual ~ParticleSwarmDialog(); ///< Destructor

  /// Return the properties selected by the user
  ccipdGetConstDeclareMacro( InitializationBiased, bool )
  ccipdGetConstDeclareMacro( MultiThreaded       , bool )
  ccipdGetConstDeclareMacro( NumberOfParticles   , unsigned int )
  ccipdGetConstDeclareMacro( MaxIterations       , unsigned int  )

//////////////////////////////////////////////////////////////////////////////////////////




private:
//////////////////////////////////////////////////////////////////////////////////////////
  /// Designer form
  Ui_ParticleSwarmDialog * const ui;
//////////////////////////////////////////////////////////////////////////////////////////

  


}; // class
} // namespace
#endif // __ccipdParticleSwarmDialog_h
