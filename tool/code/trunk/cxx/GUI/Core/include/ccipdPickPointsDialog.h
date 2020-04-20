#ifndef __ccipdPickPointsDialog_h
#define __ccipdPickPointsDialog_h




//////////////////////////////////////////////////////////////////////////////////////////

// ccipd includes
#include "ccipdMacros.h"
#include "ccipdVolumeTypes.h"
#include "ccipdLandmarkTypes.h"

#include "ccipdDisableWarningsMacro.h"

// qt includes
#include <QDialog>

#include "ccipdEnableWarningsMacro.h"

// std includes
#include <memory>

//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
// Forward Qt class declarations
class Ui_PickPointsDialog;
//////////////////////////////////////////////////////////////////////////////////////////
 


namespace ccipd
{



/**
 * \brief GUI window for picking points.
 * 
 * \author  Rob Toth
 * \ingroup GUI
 */
class PickPointsDialog :
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
  explicit PickPointsDialog( const VolumeImageConstPointer & image );

  PickPointsDialog(
    const VolumeImageConstPointer & image,
    const LandmarkSetConstPointer & initialLandmarks );

  virtual ~PickPointsDialog(); ///< Destructor

  /// Return the landmarks selected by the user
  ccipdGetConstDeclareMacro( Landmarks, LandmarkSetPointer )

  /// Get the landmarks from a given slice
  virtual LandmarkSetConstPointer GetLandmarks(
    const unsigned int whichSlice ) const;

//////////////////////////////////////////////////////////////////////////////////////////
  


protected:
//////////////////////////////////////////////////////////////////////////////////////////

  /// called on construction
  virtual void Initialize(
    const VolumeImageConstPointer & image,
    const LandmarkSetConstPointer & initialLandmarks );

  /// capture all events
  virtual bool eventFilter( QObject * object, QEvent * event );

//////////////////////////////////////////////////////////////////////////////////////////



protected:
protected slots:
//////////////////////////////////////////////////////////////////////////////////////////

  /// update the displayed image slice
  virtual void SlotUpdateSlice();
  
  /// update the displayed image window/level
  virtual void SlotUpdateWindowLevel();

  /// update the landmark opacity
  virtual void SlotUpdateLandmarksOpacity();

  /// update landmarks size
  virtual void SlotUpdateLandmarksSize();

  /// update landmarks color
  virtual void SlotUpdateLandmarksColor();

//////////////////////////////////////////////////////////////////////////////////////////



private:
//////////////////////////////////////////////////////////////////////////////////////////
  /// Designer form
  Ui_PickPointsDialog * const ui;
//////////////////////////////////////////////////////////////////////////////////////////

  
  
private:
//////////////////////////////////////////////////////////////////////////////////////////
  ccipdPIMPLMacro
//////////////////////////////////////////////////////////////////////////////////////////



}; // class
} // namespace
#endif // __ccipdPickPointsDialog_h
