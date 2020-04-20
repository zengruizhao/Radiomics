#ifndef __ccipdPercentSpinBoxDelegate_h
#define __ccipdPercentSpinBoxDelegate_h



//////////////////////////////////////////////////////////////////////////////////////////

// ccipd includes
#include "ccipdMacros.h"
#include "ccipdSpinBoxDelegate.h"

//////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////
class QDoubleSpinBox;
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



/**
 * This spin box delegate
 * will only allow 0.0 to 1.0 percentages.
 */
class PercentSpinBoxDelegate : public SpinBoxDelegate< QDoubleSpinBox, double >
{

  

//////////////////////////////////////////////////////////////////////////////////////////
  Q_OBJECT
//////////////////////////////////////////////////////////////////////////////////////////



public: // typedefs
//////////////////////////////////////////////////////////////////////////////////////////

  typedef PercentSpinBoxDelegate                    Self;
  typedef SpinBoxDelegate< QDoubleSpinBox, double > Superclass;

//////////////////////////////////////////////////////////////////////////////////////////



public: // functions
//////////////////////////////////////////////////////////////////////////////////////////

  PercentSpinBoxDelegate( QObject * const parent = 0 ); ///< Constructor
  virtual ~PercentSpinBoxDelegate();                    ///< Destructor


  /**
   * Overriding this so that we can
   * set the precision to 3 decimals.
   */
  virtual QWidget * createEditor(
          QWidget              * parent,
    const QStyleOptionViewItem & option,
    const QModelIndex          & index) const;

//////////////////////////////////////////////////////////////////////////////////////////



protected: // typedefs
//////////////////////////////////////////////////////////////////////////////////////////

  /// What is the type of item we will insert into the Spin box?
  typedef Superclass::SpinValueType SpinValueType;

//////////////////////////////////////////////////////////////////////////////////////////



protected: // functions
//////////////////////////////////////////////////////////////////////////////////////////

  ccipdGetConstDeclareMacro( MinimumValue, SpinValueType )
  ccipdGetConstDeclareMacro( MaximumValue, SpinValueType )

//////////////////////////////////////////////////////////////////////////////////////////



}; // class PercentSpinBoxDelegate
} // namespace ccipd
#endif //__ccipdPercentSpinBoxDelegate_h
