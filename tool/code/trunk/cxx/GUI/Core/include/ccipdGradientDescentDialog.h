#ifndef __ccipdGradientDescentDialog_h
#define __ccipdGradientDescentDialog_h




//////////////////////////////////////////////////////////////////////////////////////////


// ccipd includes
#include "ccipdMacros.h"
#include "ccipdRegisterAlgorithmsForwardDeclarations.h"
#include "ccipdDisableWarningsMacro.h"
// qt includes
#include <QDialog>

// std includes
#include <memory>

#include "ccipdEnableWarningsMacro.h"


#ifdef _MSC_VER
#pragma warning( disable: 4127 )
#endif // _MSC_VER//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
// Forward Qt class declarations
class Ui_GradientDescentDialog;
//////////////////////////////////////////////////////////////////////////////////////////
 


namespace ccipd
{


/**
 * \brief GUI window for specifying gradient descent options.
 * 
 * \author  Rob Toth
 * \ingroup GUI
 */
class GradientDescentDialog :
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
  explicit GradientDescentDialog( const RegisterOptimizerGradientDescent & gradientDescent );

  virtual ~GradientDescentDialog(); ///< Destructor

  typedef double FloatingType;

  /// Return the properties selected by the user
  ccipdGetConstDeclareMacro( Relaxation   , FloatingType  )
  ccipdGetConstDeclareMacro( MaxStepLength, FloatingType  )
  ccipdGetConstDeclareMacro( MinStepLength, FloatingType  )
  ccipdGetConstDeclareMacro( Tolerance    , FloatingType  )
  ccipdGetConstDeclareMacro( Iterations   , unsigned int  )

//////////////////////////////////////////////////////////////////////////////////////////




private:
//////////////////////////////////////////////////////////////////////////////////////////
  /// Designer form
  Ui_GradientDescentDialog * const ui;
//////////////////////////////////////////////////////////////////////////////////////////

  


}; // class
} // namespace
#endif // __ccipdGradientDescentDialog_h
