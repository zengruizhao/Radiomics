#ifndef __ccipdProstaFuseWindow_h
#define __ccipdProstaFuseWindow_h



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
class Ui_ProstaFuseWindow;
//////////////////////////////////////////////////////////////////////////////////////////
 


namespace ccipd
{


/**
 * \brief GUI window for ProstaFuse
 * 
 * \author  Rachel Sparks
 * \ingroup ProstaFuse
 */
class ProstaFuseWindow : public QMainWindow
{



//////////////////////////////////////////////////////////////////////////////////////////
  Q_OBJECT
//////////////////////////////////////////////////////////////////////////////////////////



public:
//////////////////////////////////////////////////////////////////////////////////////////
           ProstaFuseWindow(); ///< Constructor
  virtual ~ProstaFuseWindow(); ///< Destructor
//////////////////////////////////////////////////////////////////////////////////////////



public slots:
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////



protected:
protected slots:
//////////////////////////////////////////////////////////////////////////////////////////

  /// Exit the application.
  virtual void SlotExit();

  /// Open a fixed TRUS image (currently for the preprocessed TRUS).
  virtual void SlotOpenFixedTRUS();
  
  /// Change which slice is displayed for the fixed image
  virtual void SlotChangeFixedImageSlice( const int whichSlice ); 

  /// Save the fixed TRUS image
  virtual void SlotSaveFixedTRUS();

  /// Changing Fixed Image Appearance
  virtual void SlotChangeFixedWindow();
  virtual void SlotChangeFixedLevel();

  /// Open a moving MRI from DICOM.
  virtual void SlotOpenMovingMRI();

  /// Change which slice is displayed for the moving image
  virtual void SlotChangeMovingImageSlice( const int whichSlice ); 

  /// Save the MovingMRI Image.
  virtual void SlotSaveMovingMRI();
  
  /// Changing Moving Image Appearance
  virtual void SlotChangeMovingWindow();
  virtual void SlotChangeMovingLevel();

  /// LoadTransform 
  virtual void SlotLoadTransform();
  
  /// LoadTransform 
  virtual void SlotApplyTransform();

  /// Show the options dialog.
  virtual void SlotOptions();

  /// Show the options dialog.
  virtual void SlotShowAbout();

//////////////////////////////////////////////////////////////////////////////////////////
 


private:
//////////////////////////////////////////////////////////////////////////////////////////
  // Designer form
  Ui_ProstaFuseWindow * ui;
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
  ccipdPIMPLMacro
//////////////////////////////////////////////////////////////////////////////////////////



}; // class ProstaFuseWindow
} // namespace ccipd
#endif // __ccipdProstaFuseWindow_h
