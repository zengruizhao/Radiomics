#ifndef __ccipdProstaFuseResults_h
#define __ccipdProstaFuseResults_h



//////////////////////////////////////////////////////////////////////////////////////////

// ccipd includes
#include "ccipdMacros.h"
#include "ccipdVolumeTypes.h"

#ifdef _MSC_VER
#pragma warning( push, 0 )
#endif // _MSC_VER

// qt includes
#include <QDialog>

// std includes
#include <memory> // for smart pointers

#ifdef _MSC_VER
#pragma warning( pop )
#endif // _MSC_VER

//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
// Forward Qt class declarations
class Ui_ProstaFuseResults;
class QSlider;
//////////////////////////////////////////////////////////////////////////////////////////
 


namespace ccipd
{


/**
 * \brief GUI window for displaying the results of ProstaFuse
 * 
 * \author  Rachel Sparks
 * \ingroup ProstaFuse
 */
class ProstaFuseResults : public QDialog
{



//////////////////////////////////////////////////////////////////////////////////////////
  Q_OBJECT
//////////////////////////////////////////////////////////////////////////////////////////



public:
//////////////////////////////////////////////////////////////////////////////////////////
  ProstaFuseResults(
    const VolumeImageType    * const fixedImage,
    QSlider                  &       fixedWindowSlider,
    QSlider                  &       fixedLevelSlider,
    const VolumeImageType    * const movedImage,
    QSlider                  &       movedWindowSlider,
    QSlider                  &       movedLevelSlider,
    QSlider                  &       movedSliceSlider,
    const float                      slideFactor       ); ///< Constructor
	
  virtual ~ProstaFuseResults(); ///< Destructor
//////////////////////////////////////////////////////////////////////////////////////////



public slots:
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////



protected:
protected slots:
//////////////////////////////////////////////////////////////////////////////////////////
  
  /// Change which slice is displayed for the images
  virtual void SlotChangeImageSlice( const int whichSlice ); 

  /// For changing imaging charactersitcs of Fixed Image
  virtual void SlotChangeFixedLevel();
  virtual void SlotChangeFixedWindow();


  /// Save the moved image
  virtual void SlotSaveMovedImage();

  /// For changing imaging charactersitcs of Moved Image
  virtual void SlotChangeMovedLevel();
  virtual void SlotChangeMovedWindow();

  /// Show the options dialog.
  virtual void SlotOptions();
//////////////////////////////////////////////////////////////////////////////////////////
 


private:
//////////////////////////////////////////////////////////////////////////////////////////
  // Designer form
  Ui_ProstaFuseResults * ui;
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
  /// PIMPL macro
  ccipdPIMPLMacro
//////////////////////////////////////////////////////////////////////////////////////////



}; // class ProstaFuseWindow
} // namespace ccipd
#endif // __ccipdProstaFuseResults_h