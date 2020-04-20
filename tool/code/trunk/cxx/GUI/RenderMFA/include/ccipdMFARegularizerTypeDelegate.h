#ifndef __ccipdMFARegularizerTypeDelegate_h
#define __ccipdMFARegularizerTypeDelegate_h




//////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdComboBoxDelegate.h"
#include "ccipdRegisterMFARegularizedMetric.h"
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



class MFARegularizerTypeDelegate : public ComboBoxDelegate
{



//////////////////////////////////////////////////////////////////////////////////////////
  Q_OBJECT
//////////////////////////////////////////////////////////////////////////////////////////



public: // typedefs
//////////////////////////////////////////////////////////////////////////////////////////

  typedef MFARegularizerTypeDelegate   Self;
  typedef ComboBoxDelegate             Superclass;
  typedef RegisterMFARegularizedMetric RegularizerType;
  typedef Superclass::ComboItemType    ComboItemType;

//////////////////////////////////////////////////////////////////////////////////////////



public: // functions
//////////////////////////////////////////////////////////////////////////////////////////

  MFARegularizerTypeDelegate( QObject * const parent = 0 ); ///< Constructor
  virtual ~MFARegularizerTypeDelegate();                    ///< Destructor

  /**
   * \brief Return the string for the given likelihood type
   * 
   * \note This throws a std::out_of_range exception for unsupported transforms.
   */
  static ComboItemType GetComboItem(
    const RegularizerType::LikelihoodType likelihoodType );

  /**
   * \brief Return the likelihood enum for the given string.
   * \note  This throws a std::out_of_range exception of the comboItem is unknown.
   */
  static RegularizerType::LikelihoodType GetLikelihoodType(
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



}; // class MFARegularizerTypeDelegate
} // namespace ccipd
#endif // __ccipdMFARegularizerTypeDelegate_h