#ifndef __ccipdMFATransformDelegate_h
#define __ccipdMFATransformDelegate_h




//////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdComboBoxDelegate.h"
#include "ccipdRegisterTransformLinearCombo.h"
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



class MFATransformDelegate : public ComboBoxDelegate
{



//////////////////////////////////////////////////////////////////////////////////////////
  Q_OBJECT
//////////////////////////////////////////////////////////////////////////////////////////



public: // typedefs
//////////////////////////////////////////////////////////////////////////////////////////

  typedef MFATransformDelegate         Self;
  typedef ComboBoxDelegate             Superclass;
  typedef RegisterTransformLinearCombo ComboTransformType;
  typedef Superclass::ComboItemType    ComboItemType;

//////////////////////////////////////////////////////////////////////////////////////////



public: // functions
//////////////////////////////////////////////////////////////////////////////////////////

  MFATransformDelegate( QObject * const parent = 0 ); ///< Constructor
  virtual ~MFATransformDelegate();                    ///< Destructor

  /**
   * \brief Return the string for the given transform item.
   * 
   * \note This throws a std::out_of_range exception for unsupported transforms.
   */
  static ComboItemType GetComboItem(
    const ComboTransformType::WhichTransform whichTransform );

  /**
   * \brief Return the transform enum for the given string.
   * \note  This throws a std::out_of_range exception of the comboItem is unknown.
   */
  static ComboTransformType::WhichTransform GetWhichTransform(
    const ComboItemType & comboItem );

//////////////////////////////////////////////////////////////////////////////////////////



protected: // typedefs
//////////////////////////////////////////////////////////////////////////////////////////
  typedef Superclass::UniqueItemsCollection UniqueItemsCollection;
  typedef Superclass::UniqueItemsSequence   UniqueItemsSequence;
//////////////////////////////////////////////////////////////////////////////////////////



protected: // functions
//////////////////////////////////////////////////////////////////////////////////////////

  /**
   * We will provide a collection
   * of MFA transform strings.
   */
  virtual UniqueItemsCollection GetItemsForComboBox() const;

//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
  class SecretData;
  friend class Self::SecretData;
//////////////////////////////////////////////////////////////////////////////////////////



}; // class MFATransformDelegate
} // namespace ccipd
#endif // __ccipdMFATransformDelegate_h
