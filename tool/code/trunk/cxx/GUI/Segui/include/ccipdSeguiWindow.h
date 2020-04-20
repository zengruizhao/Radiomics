#ifndef __ccipdSeguiWindow_h
#define __ccipdSeguiWindow_h



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
class Ui_SeguiWindow;
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



/**
 * \brief GUI window for MFA segmentation.
 * 
 * \author  Rob Toth
 * \ingroup Segui
 * \ingroup MFA
 */
class SeguiWindow: public QMainWindow
{



//////////////////////////////////////////////////////////////////////////////////////////
  Q_OBJECT
//////////////////////////////////////////////////////////////////////////////////////////



public:
//////////////////////////////////////////////////////////////////////////////////////////
           SeguiWindow(); ///< Constructor
  virtual ~SeguiWindow(); ///< Destructor
//////////////////////////////////////////////////////////////////////////////////////////
  


protected:
protected slots:
//////////////////////////////////////////////////////////////////////////////////////////

  /// Exit the application.
  virtual void SlotExit();
  
  /// About the application.
  virtual void SlotAbout();

  /// Create a new MFA segmenter
  virtual void SlotNewMFASegmenter();

  /// Open an MFA segmenter
  virtual void SlotOpenMFASegmenter();
  
  /// Edit an MFA segmenter
  virtual void SlotEditMFASegmenter();
  
  /// Save an MFA segmenter using the last file name
  virtual void SlotSaveMFASegmenter();

  /// Save an MFA segmenter with a file selection
  virtual void SlotSaveMFASegmenterAs();

  /// Close any open MFA segmenters.
  virtual void SlotCloseMFASegmenter();
  
  /// Load a study to segment.
  virtual void SlotOpenStudy();
  
  /// Load a study to segment.
  virtual void SlotOpenDICOM();

  /// Segment the study
  virtual void SlotSegment();

  /// Update the transparency
  virtual void SlotUpdateTransparency();

  /// Update the MFA & study rendering
  virtual void SlotRerender();

  /// Make the levelset colors random
  virtual void SlotRandomizeColors();

  /// Display a given slice of the image.
  virtual void SlotUpdateSlice();

  /// Change the "windowing" and "level" of the image
  virtual void SlotUpdateImageWindowLevel();

  /// Update which image is displayed
  virtual void SlotUpdateWhichImage();

  /// Clear the currently displayed levelset
  virtual void SlotClearLevelset();

  /// Save the image
  virtual void SlotSaveCurrentImage();
  
  /// Edit the segmentation with control points
  virtual void SlotCorrectSegmentation();

  /// CLear known points
  virtual void SlotClearCorrectionPoints();

  /// Clear existing correction and points
  virtual void SlotClearCorrectionAndPoints();
  
  /// Clear existing correction but keep points
  virtual void SlotClearCorrection();

  /// Regenerate existing correction
  virtual void SlotRegenerateCorrection();
  
  /// How many random fixed points to use.
  virtual void SlotSetNumberOfFixedPoints();

  /// Set a bounding box
  virtual void SlotSetBoundingBox();

  /// Clear any manually-set bounding box
  virtual void SlotClearBoundingBox();

  /// Display the bounding box if it exists
  virtual void SlotShowBoundingBox();

  /// Hide the bounding box if it exists
  virtual void SlotHideBoundingBox();

  /// Show fixed points if they exist
  virtual void SlotShowFixedPoints();

  /// Hide fixed points if they exist
  virtual void SlotHideFixedPoints();
  
  /// No surface points within the radius of each manually selected points will be allowed.
  virtual void SlotSetForbiddenRadius();

  /// # of random anchor points on the surface
  virtual void SlotSetNumberOfSurfacePoints();

  /// Save a joined label map
  virtual void SlotSaveSummedLabels();

//////////////////////////////////////////////////////////////////////////////////////////
  


private:
//////////////////////////////////////////////////////////////////////////////////////////
  // Designer form
  Ui_SeguiWindow * ui;
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
  ccipdPIMPLMacro
//////////////////////////////////////////////////////////////////////////////////////////



}; // class Segui
} // namespace ccipd
#endif // __ccipdSeguiWindow_h
