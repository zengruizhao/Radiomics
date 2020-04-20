#ifndef __ccipdMFASegmenterOptionsDialog_h
#define __ccipdMFASegmenterOptionsDialog_h




//////////////////////////////////////////////////////////////////////////////////////////

// ccipd includes
#include "ccipdMacros.h"
#include "ccipdMFATypes.h"
#include "ccipdPatientTypes.h"
#include "ccipdFeatureCollectionTypes.h"

#ifdef _MSC_VER
#pragma warning( push, 0 )
#endif // _MSC_VER

// qt includes
#include <QDialog>

#ifdef _MSC_VER
#pragma warning( pop )
#endif // _MSC_VER

//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
// Forward Qt class declarations
class Ui_MFASegmenterOptionsDialog;
//////////////////////////////////////////////////////////////////////////////////////////
 


namespace ccipd
{


/**
 * \brief GUI window for specifying the MFA segmenter options.
 * 
 * \author  Rob Toth
 * \ingroup RenderMFA
 * \ingroup MFA
 */
class MFASegmenterOptionsDialog : public QDialog
{



//////////////////////////////////////////////////////////////////////////////////////////
  Q_OBJECT
//////////////////////////////////////////////////////////////////////////////////////////



public:
//////////////////////////////////////////////////////////////////////////////////////////
  
  /**
   * \brief Constructor for the MFA options dialog.
   */
  explicit MFASegmenterOptionsDialog( const MFASegmenter & segmenter );

  virtual ~MFASegmenterOptionsDialog(); ///< Destructor

  /// Return the levelset objects selected by the user.
  ccipdGetConstDeclareMacro( LevelsetObjects, MaskObjectCollectionType )

  /// Return the texture features selected by the user.
  ccipdGetConstDeclareMacro( TextureFeatures, TextureFeatureEnumCollection )

  /// Return the registration algorithms
  ccipdGetConstDeclareMacro( Algorithms, RegisterMFAAlgorithmCollection )

  /// Return the segmentation object
  ccipdGetConstDeclareMacro( SegmentationObject, MaskObjectType )

  /// Return the file name of the model
  ccipdGetConstDeclareMacro( TrainedMFAFile, std::string )

  /// Return the amount of extrapolation (in mm)
  ccipdGetConstDeclareMacro( Extrapolation, float )
  
  /// Returns the verbosity.
  ccipdGetConstDeclareMacro( Verbose, bool )

//////////////////////////////////////////////////////////////////////////////////////////



protected:
protected slots:
//////////////////////////////////////////////////////////////////////////////////////////

  /// Call this to insert a new levelset object
  virtual void SlotAddLevelset();

  /// Call this to remove the selected levelset object
  virtual void SlotRemoveLevelset();
  
  /// Call this to insert a new texture feature
  virtual void SlotAddTexture();

  /// Call this to remove the selected texture object
  virtual void SlotRemoveTexture();

  /// Remove the currently selected registration algorithm
  virtual void SlotAddAlgorithm();
    
  /// Remove the currently selected registration algorithm
  virtual void SlotRemoveAlgorithm();
    
  /**
   * \brief Call this to open a trained MFA model.
   * 
   * \note
   *   If the model is unable to be loaded,
   *   the "Trained MFA Model" text will
   *   remain unchanged.
   */
  virtual void SlotOpenModel();


//////////////////////////////////////////////////////////////////////////////////////////
 


private:
//////////////////////////////////////////////////////////////////////////////////////////
  /// Designer form
  Ui_MFASegmenterOptionsDialog * const ui;

  /// Just to say we're finished initializing.
  bool m_Initialized;

//////////////////////////////////////////////////////////////////////////////////////////

  

// PIMPL interface
//////////////////////////////////////////////////////////////////////////////////////////
  ccipdPIMPLMacro
//////////////////////////////////////////////////////////////////////////////////////////



}; // class
} // namespace
#endif // __ccipdMFASegmenterOptionsDialog_h
