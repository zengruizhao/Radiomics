#ifndef __ccipdSpinBoxDelegate_h
#define __ccipdSpinBoxDelegate_h



//////////////////////////////////////////////////////////////////////////////////////////

#ifdef _MSC_VER
#pragma warning( push, 0 )
#endif // _MSC_VER

// qt includes
#include <QItemDelegate>
#ifdef _MSC_VER
#pragma warning( pop )
#endif // _MSC_VER

//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



/**
 * Based on http://bit.ly/VhdO6h
 * 
 * You must provide the spin box type
 * and associated "value" type
 * for min and max values.
 * 
 * The spin box type must inherit
 * the abstract spin box.
 */
template< class TSpinBoxType, typename TValueType >
class SpinBoxDelegate : public QItemDelegate
{



//////////////////////////////////////////////////////////////////////////////////////////
  // http://bit.ly/14n4anR - signal/slot not necessary in my class
  //Q_OBJECT
//////////////////////////////////////////////////////////////////////////////////////////



public: // typedefs
//////////////////////////////////////////////////////////////////////////////////////////

  typedef SpinBoxDelegate Self;
  typedef QItemDelegate   Superclass;

//////////////////////////////////////////////////////////////////////////////////////////



public: // functions
//////////////////////////////////////////////////////////////////////////////////////////

  SpinBoxDelegate( QObject * const parent = 0 ); ///< Constructor
  virtual ~SpinBoxDelegate();                    ///< Destructor

  virtual QWidget * createEditor(
          QWidget              * parent,
    const QStyleOptionViewItem & option,
    const QModelIndex          & index) const;

  void setEditorData( QWidget * editor, const QModelIndex & index) const;

  void setModelData(
          QWidget            * editor,
          QAbstractItemModel * model,
    const QModelIndex        & index ) const;

  void updateEditorGeometry(
          QWidget              * editor,
    const QStyleOptionViewItem & option,
    const QModelIndex          & index ) const;

//////////////////////////////////////////////////////////////////////////////////////////



protected: // typedefs
//////////////////////////////////////////////////////////////////////////////////////////

  /// What is the type of item we will insert into the Spin box?
  typedef TValueType SpinValueType;

  typedef TSpinBoxType SpinBoxType;

//////////////////////////////////////////////////////////////////////////////////////////



protected: // functions
//////////////////////////////////////////////////////////////////////////////////////////

  /**
   * Child classes must provide the
   * minimum spin box value.
   */
  virtual SpinValueType GetMinimumValue() const = 0;

  /**
   * Child classes must provide the
   * minimum spin box value.
   */
  virtual SpinValueType GetMaximumValue() const = 0;

  /**
   * Return a single step.
   * 
   * Be default, it returns
   * 10% of the range so
   * there are 10 steps.
   * 
   * Override this to return a step size.
   */
  virtual SpinValueType GetStep() const;

//////////////////////////////////////////////////////////////////////////////////////////



}; // class SpinBoxDelegate
} // namespace ccipd
#endif //__ccipdSpinBoxDelegate_h
