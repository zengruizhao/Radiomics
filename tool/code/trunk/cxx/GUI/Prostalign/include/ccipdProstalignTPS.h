#ifndef __ccipdProstalignTPS_h
#define __ccipdProstalignTPS_h



//////////////////////////////////////////////////////////////////////////////////////////

// ccipd includes
#include "ccipdMacros.h"

#ifdef _MSC_VER
#pragma warning( push, 0 )
#endif // _MSC_VER

// qt includes
#include <QMainWindow>

// std includes
#include <memory> // for smart pointers

#ifdef _MSC_VER
#pragma warning( pop )
#endif // _MSC_VER

//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
// Forward Qt class declarations
class Ui_ProstalignTPS;
//////////////////////////////////////////////////////////////////////////////////////////
 

namespace ccipd
{


/**
 * \brief TPS GUI window for Prostalign.
 * 
 * \author  Rob Toth
 * \ingroup Prostalign
 */
class ProstalignTPS : public QMainWindow
{



//////////////////////////////////////////////////////////////////////////////////////////
  Q_OBJECT
//////////////////////////////////////////////////////////////////////////////////////////



public:
//////////////////////////////////////////////////////////////////////////////////////////
           ProstalignTPS(); ///< Constructor
  virtual ~ProstalignTPS(); ///< Destructor
//////////////////////////////////////////////////////////////////////////////////////////



public slots:
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////



protected:
protected slots:
//////////////////////////////////////////////////////////////////////////////////////////

  /// Exit the application.
  virtual void SlotExit();

  /// Open a fixed image.
  virtual void SlotOpenFixedImage();

  /// Open a moving image.
  virtual void SlotOpenMovingImage();

  /// Register moving to fixed image.
  virtual void SlotRegister();

  /// Remove all landmarks, and reload the image if one exists.
  virtual void SlotResetFixedImage();

  /// Remove all landmarks, and reload the image if one exists.
  virtual void SlotResetMovingImage();

  /// Show the options dialog.
  virtual void SlotOptions();

//////////////////////////////////////////////////////////////////////////////////////////
 


private:
//////////////////////////////////////////////////////////////////////////////////////////
  // Designer form
  Ui_ProstalignTPS * ui;
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
  ccipdPIMPLMacro
//////////////////////////////////////////////////////////////////////////////////////////



}; // class ProstalignTPS
} // namespace ccipd
#endif // __ccipdProstalignTPS_h
