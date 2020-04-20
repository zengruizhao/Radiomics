


//////////////////////////////////////////////////////////////////////////////////////////

// ccipd includes
#include "ccipdCheckBoxDelegate.h"

#ifdef _MSC_VER
#pragma warning( push, 0 )
#endif // _MSC_VER

// qt includes
#include <QCheckBox>

#ifdef _MSC_VER
#pragma warning( pop )
#endif // _MSC_VER

//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////
CheckBoxDelegate::CheckBoxDelegate( QObject * const parent ) :
Superclass( parent )
{
} // constructor
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
CheckBoxDelegate::~CheckBoxDelegate()
{
} // destructor
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
QWidget * CheckBoxDelegate::createEditor(
        QWidget              * parent,
  const QStyleOptionViewItem & option,
  const QModelIndex          & index) const
{

  (void)index;
  (void)option;

  QCheckBox * const checkBox = new QCheckBox( parent );

  return checkBox;

} // createEditor
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void CheckBoxDelegate::setEditorData(
  QWidget * editor, const QModelIndex & index) const
{

  // get the transform string
  const bool currentValue =
    index.model()->data( index, Qt::EditRole ).toBool();

  // get the Check box
  QCheckBox * checkBox = static_cast< QCheckBox * >( editor );

  // update the check box's text with
  checkBox->setChecked( currentValue );
  
} // setEditorData
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////

void CheckBoxDelegate::setModelData(
        QWidget            * editor,
        QAbstractItemModel * model,
  const QModelIndex        & index ) const
{

  // get the Check box
  QCheckBox * checkBox = static_cast< QCheckBox * >( editor );

  // get the selected text

  const bool currentValue = checkBox->isChecked();

  // update the model with the data
  model->setData( index, int( currentValue ), Qt::EditRole );

} // setModelData
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////

void CheckBoxDelegate::updateEditorGeometry(
        QWidget              * editor,
  const QStyleOptionViewItem & option,
  const QModelIndex          & index ) const
{

  (void)index;
  editor->setGeometry( option.rect );

} // updateEditorGeometry
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace ccipd
