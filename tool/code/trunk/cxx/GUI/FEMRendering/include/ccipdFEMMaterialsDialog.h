#ifndef __ccipdFEMMaterialsDialog_h
#define __ccipdFEMMaterialsDialog_h




//////////////////////////////////////////////////////////////////////////////////////////

// ccipd includes
#include "ccipdMacros.h"
#include "ccipdFEMTypes.h"    // for our material map type
#include "ccipdVolumeTypes.h" // for our mask

#include "ccipdDisableWarningsMacro.h"

// qt includes
#include <QDialog>

// itk includes
#include <itkFEMMaterialBase.h> // the std::unordered_map needs this include file :(

#include "ccipdEnableWarningsMacro.h"

// std includes
#include <memory> // for smart pointers

//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
// Forward Qt class declarations
class Ui_FEMMaterialsDialog;
//////////////////////////////////////////////////////////////////////////////////////////
 


namespace ccipd
{


/**
 * \brief GUI window for specifying the FEM materials for a mask.
 * 
 * \author  Rob Toth
 * \ingroup FEMRendering
 */
class FEMMaterialsDialog : public QDialog
{



//////////////////////////////////////////////////////////////////////////////////////////
  Q_OBJECT
//////////////////////////////////////////////////////////////////////////////////////////



public:
//////////////////////////////////////////////////////////////////////////////////////////
  
  /**
   * \brief Constructor for the FEM materials dialog.
   * 
   * Constructor in which you give it the mask to
   * calculate the unique values from.
   * 
   * You can also optionally create the
   * initial default material, and give
   * it some initial materials
   * for the mask's unique values.
   * 
   * If a given mask value
   * does not have an initial material,
   * or that material is of the
   * wrong type, then one will
   * be created.
   * 
   * Any materials which
   * aren't given automatically
   * have the checkbox checked
   * which specifies to use the default.
   *
   */
  FEMMaterialsDialog(
    const VolumeMaskImageType * const mask,
    const MaterialPointer     & initialDefaultMaterial = MaterialPointer(),
    const MaskMaterialMap     & initialMaterials       = MaskMaterialMap() );

  virtual ~FEMMaterialsDialog(); ///< Destructor

  /// The final output of which materials the user selected.
  ccipdGetConstDeclareMacro( Materials, MaskMaterialMap )

  /// The final output of the default material.
  ccipdGetConstDeclareMacro( DefaultMaterial, MaterialPointer )

//////////////////////////////////////////////////////////////////////////////////////////



public slots:
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////



protected:
protected slots:
//////////////////////////////////////////////////////////////////////////////////////////

  /// Call this when a value changes so the material can be updated.
  void SlotUpdateMaterial();
  
  /// Call this when you change the given mask value
  /// to update the properties and enabled state.
  void SlotMaskValueChanged();
  
  /// Call this when you change whether to use the default material or not.
  void SlotUseDefaultChanged( const bool useDefault );

//////////////////////////////////////////////////////////////////////////////////////////
 


private:
//////////////////////////////////////////////////////////////////////////////////////////
  /// Designer form
  Ui_FEMMaterialsDialog * const ui;

  /// Just to say we're finished initializing.
  bool m_Initialized;
//////////////////////////////////////////////////////////////////////////////////////////

  

// PIMPL interface
//////////////////////////////////////////////////////////////////////////////////////////
  ccipdPIMPLMacro
//////////////////////////////////////////////////////////////////////////////////////////



}; // class
} // namespace
#endif // __ccipdFEMMaterialsDialog_h
