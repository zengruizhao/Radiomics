#ifndef __ccipdProstalignFEM_h
#define __ccipdProstalignFEM_h



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
class Ui_ProstalignFEM;
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{


/**
 * \brief FEM GUI window for Prostalign.
 * 
 * \author  Rob Toth
 * \ingroup Prostalign
 * \ingroup FEM
 */
class ProstalignFEM : public QMainWindow
{



//////////////////////////////////////////////////////////////////////////////////////////
  Q_OBJECT
//////////////////////////////////////////////////////////////////////////////////////////



public:
//////////////////////////////////////////////////////////////////////////////////////////
           ProstalignFEM(); ///< Constructor
  virtual ~ProstalignFEM(); ///< Destructor
//////////////////////////////////////////////////////////////////////////////////////////



public slots:
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////



protected:
protected slots:
//////////////////////////////////////////////////////////////////////////////////////////

  /// Exit the application.
  virtual void SlotExit();

  /// Load a patient to register pre and post.
  virtual void SlotLoadPatient();

  /// Register moving to fixed image.
  virtual void SlotRegister();

  /// Gets called when the user selects a different fixed study.
  virtual void SlotChangeFixedStudy( const int whichIndex );

  /// Gets called when the user selects a different moving study.
  virtual void SlotChangeMovingStudy( const int whichIndex );

  /// Change the combo box of the first image.
  virtual void SlotChangeImage1( const QString & value );

  /// This will re-update given the current index.
  virtual void SlotChangeImage1();

  /// Change the combo box of the second image.
  virtual void SlotChangeImage2( const QString & value );

  /// This will re-update given the current index.
  virtual void SlotChangeImage2();

  /// Gets called when the user "slides" to a new slice.
  virtual void SlotChangeImage1Slice( const int whichSlice );

  /// Gets called when the user "slides" to a new slice.
  virtual void SlotChangeImage2Slice( const int whichSlice );

  /// Ranges from 0 to 100
  
  /// Change the window/level of the first image based on the slider values.
  virtual void SlotChangeImage1WindowLevel();

  /// Change the window/level of the second image based on the slider values.
  virtual void SlotChangeImage2WindowLevel();

  /// What type of blinds are we showing for image 1?
  virtual void SlotChangeImage1Blinds();

  /// What type of blinds are we showing for image 2?
  virtual void SlotChangeImage2Blinds();

  /// Reset the camera
  virtual void SlotResetGUI();

  /// Show a popup menu to save the results.
  virtual void SlotSaveResults();// const QPoint & position );

  /// Show a dialog box to set up the materials.
  virtual void SlotEditMaterials();

  /// Ranges from 0 to 100
  virtual void SlotChangeImage1Transparency       ( const int opacity );
  virtual void SlotChangeImage2Transparency       ( const int opacity );
  virtual void SlotChangeLandmarksTransparency    ();
  virtual void SlotChangeTrianglesTransparency    ();
  virtual void SlotChangeLoadsSpheresTransparency ();
  virtual void SlotChangeLoadsArrowsTransparency  ();

  virtual void SlotChangeLandmarksColor    ();
  virtual void SlotChangeTrianglesFaceColor();
  virtual void SlotChangeTrianglesEdgeColor();
  virtual void SlotChangeLoadsSpheresColor ();
  virtual void SlotChangeLoadsArrowsColor  ();

  virtual void SlotChangeArrowsScale();

  virtual void SlotFEMTransformOptions       ();
  virtual void SlotGradientDescentOptions    ();
  virtual void SlotParticleSwarmLinearOptions();
  virtual void SlotParticleSwarmFEMOptions   ();

//////////////////////////////////////////////////////////////////////////////////////////
 


private:
//////////////////////////////////////////////////////////////////////////////////////////
  // Designer form
  Ui_ProstalignFEM * ui;
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
  ccipdPIMPLMacro
//////////////////////////////////////////////////////////////////////////////////////////



}; // class ProstalignFEM
} // namespace ccipd
#endif // __ccipdProstalignFEM_h
