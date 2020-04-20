


//////////////////////////////////////////////////////////////////////////////////////////

// ccipd includes
#include "ccipdComboBoxDelegate.h"

#ifdef _MSC_VER
#pragma warning( push, 0 )
#endif // _MSC_VER

// qt includes
#include <QComboBox>

// boost includes
#include <boost/foreach.hpp>

#ifdef _MSC_VER
#pragma warning( pop )
#endif // _MSC_VER

//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////
ComboBoxDelegate::ComboBoxDelegate( QObject * const parent ) :
Superclass( parent )
{
} // constructor
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
ComboBoxDelegate::~ComboBoxDelegate()
{
} // destructor
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
QWidget * ComboBoxDelegate::createEditor(
        QWidget              * parent,
  const QStyleOptionViewItem & option,
  const QModelIndex          & index) const
{

  (void)index;
  (void)option;

  QComboBox * const comboEditor = new QComboBox( parent );

  // don't allow duplicate items
  comboEditor->setDuplicatesEnabled( false );

  // create the items to add to the list
  const UniqueItemsCollection comboItems = this->GetItemsForComboBox();
  const UniqueItemsSequence & comboItemsSequence = comboItems.get< sequence >();
  
  // iterate through the items in order
  BOOST_FOREACH( const ComboItemType & comboItem, comboItemsSequence )
    {

    // insert the item into the combo box
    comboEditor->addItem( tr( comboItem.c_str() ) );

    } // for each item

  return comboEditor;

} // createEditor
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void ComboBoxDelegate::setEditorData(
  QWidget * editor, const QModelIndex & index) const
{

  // get the transform string
  const QString currentTransformValue =
    index.model()->data( index, Qt::EditRole ).toString();

  // get the combo box
  QComboBox * comboBox = static_cast< QComboBox * >( editor );

  // update the combo box's text with
  // the index
  comboBox->setCurrentIndex( comboBox->findText( currentTransformValue ) );
  
} // setEditorData
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////

void ComboBoxDelegate::setModelData(
        QWidget            * editor,
        QAbstractItemModel * model,
  const QModelIndex        & index ) const
{

  // get the combo box
  QComboBox * comboBox = static_cast< QComboBox * >( editor );

  // get the selected text

  const QString currentTransformValue = comboBox->currentText();
  //const int currentSelectedIndex = comboBox->currentIndex();

  // update the model with the data
  model->setData( index, currentTransformValue, Qt::EditRole );

  /*
  comboBox->interpretText();
  const int value = spinBox->value();

  model->setData( index, value, Qt::EditRole );
  */

} // setModelData
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////

void ComboBoxDelegate::updateEditorGeometry(
        QWidget              * editor,
  const QStyleOptionViewItem & option,
  const QModelIndex          & index ) const
{

  (void)index;
  editor->setGeometry( option.rect );

} // updateEditorGeometry
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace ccipd
