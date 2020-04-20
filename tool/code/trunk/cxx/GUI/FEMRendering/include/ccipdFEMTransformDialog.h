#ifndef __ccipdFEMTransformDialog_h
#define __ccipdFEMTransformDialog_h



 
//////////////////////////////////////////////////////////////////////////////////////////

//#ifdef _MSC_VER
//#pragma warning( disable: 4127 )
//#endif // _MSC_VER

// ccipd includes
#include "ccipdCore.h"
#include "ccipdMacros.h"
#include "ccipdDisableWarningsMacro.h"

// qt includes
#include <QDialog>

// std includes
#include <memory>
#include "ccipdEnableWarningsMacro.h"
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
// Forward Qt class declarations
class Ui_FEMTransformDialog;
//////////////////////////////////////////////////////////////////////////////////////////
 


namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////
  class RegisterFEMTransform;
//////////////////////////////////////////////////////////////////////////////////////////



/**
 * \brief GUI window for specifying gradient descent options.
 * 
 * \author  Rob Toth
 * \ingroup GUI
 */
class FEMTransformDialog :
  public QDialog
{



//////////////////////////////////////////////////////////////////////////////////////////
  Q_OBJECT
//////////////////////////////////////////////////////////////////////////////////////////



public:
//////////////////////////////////////////////////////////////////////////////////////////
  
  /**
   * \brief Constructor for the FEM transform dialog
   */
  explicit FEMTransformDialog( const RegisterFEMTransform & registration );

  virtual ~FEMTransformDialog(); ///< Destructor

  /// Return the properties selected by the user
  ccipdGetConstDeclareMacro( NeverUseGPU         , bool              )
  ccipdGetConstDeclareMacro( ThresholdMask       , bool              )
  ccipdGetConstDeclareMacro( ThresholdValue      , InternalPixelType )
  ccipdGetConstDeclareMacro( DownsampleRateX     , unsigned int      )
  ccipdGetConstDeclareMacro( DownsampleRateY     , unsigned int      )
  ccipdGetConstDeclareMacro( DownsampleRateZ     , unsigned int      )
  ccipdGetConstDeclareMacro( NumberOfSpheres     , unsigned int      )
  ccipdGetConstDeclareMacro( NumberOfSurfaceLoads, unsigned int      )
  ccipdGetConstDeclareMacro( MinSphereRadius     , TransformValueType )
  ccipdGetConstDeclareMacro( MaxSphereRadius     , TransformValueType )
  ccipdGetConstDeclareMacro( MinMagnitude        , TransformValueType )
  ccipdGetConstDeclareMacro( MaxMagnitude        , TransformValueType )
  ccipdGetConstDeclareMacro( MinScaling          , float              )
  ccipdGetConstDeclareMacro( MaxScaling          , float              )

  void SlotThresholdMask();

//////////////////////////////////////////////////////////////////////////////////////////




private:
//////////////////////////////////////////////////////////////////////////////////////////
  /// Designer form
  Ui_FEMTransformDialog * const ui;
//////////////////////////////////////////////////////////////////////////////////////////

  


}; // class
} // namespace
#endif // __ccipdFEMTransformDialog_h
