#ifndef __ccipdCheckBoxDelegate_h
#define __ccipdCheckBoxDelegate_h



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
 */
class CheckBoxDelegate : public QItemDelegate
{



//////////////////////////////////////////////////////////////////////////////////////////
  Q_OBJECT
//////////////////////////////////////////////////////////////////////////////////////////



public: // typedefs
//////////////////////////////////////////////////////////////////////////////////////////

  typedef CheckBoxDelegate Self;
  typedef QItemDelegate    Superclass;

//////////////////////////////////////////////////////////////////////////////////////////



public: // functions
//////////////////////////////////////////////////////////////////////////////////////////

  CheckBoxDelegate( QObject * const parent = 0 ); ///< Constructor
  virtual ~CheckBoxDelegate();                    ///< Destructor

  QWidget * createEditor(
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



}; // class CheckBoxDelegate
} // namespace ccipd
#endif //__ccipdCheckBoxDelegate_h
