#ifndef __ccipdMFAInterpolatorDelegate_h
#define __ccipdMFAInterpolatorDelegate_h




//////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdComboBoxDelegate.h"
#include "ccipdRegisterInterpolatorCombo.h"
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



class MFAInterpolatorDelegate : public ComboBoxDelegate
{



//////////////////////////////////////////////////////////////////////////////////////////
  Q_OBJECT
//////////////////////////////////////////////////////////////////////////////////////////



public: // typedefs
//////////////////////////////////////////////////////////////////////////////////////////

  typedef MFAInterpolatorDelegate   Self;
  typedef ComboBoxDelegate          Superclass;
  typedef Superclass::ComboItemType ComboItemType;

//////////////////////////////////////////////////////////////////////////////////////////



public: // functions
//////////////////////////////////////////////////////////////////////////////////////////

  MFAInterpolatorDelegate( QObject * const parent = 0 ); ///< Constructor
  virtual ~MFAInterpolatorDelegate();                    ///< Destructor

  /// Return the string for the given interpolator item.
  static ComboItemType GetComboItem(
    const RegisterInterpolatorCombo::WhichInterpolator whichInterpolator );

  /**
   * \brief Return the interpolator enum for the given string.
   * \note  This throws a std::out_of_range exception of the comboItem is unknown.
   */
  static RegisterInterpolatorCombo::WhichInterpolator GetWhichInterpolator(
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
   * of MFA Interpolator strings.
   */
  virtual UniqueItemsCollection GetItemsForComboBox() const;

//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
  class SecretData;
  friend class Self::SecretData;
//////////////////////////////////////////////////////////////////////////////////////////



}; // class MFAInterpolatorDelegate
} // namespace ccipd
#endif // __ccipdMFAInterpolatorDelegate_h
