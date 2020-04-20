#ifndef __ccipdSpinBoxDelegate_hxx
#define __ccipdSpinBoxDelegate_hxx



//////////////////////////////////////////////////////////////////////////////////////////

// ccipd includes
#include "ccipdSpinBoxDelegate.h"

#ifdef _MSC_VER
#pragma warning( push, 0 )
#endif // _MSC_VER

// qt includes
#include <QAbstractSpinBox>

// boost includes
#include <boost/static_assert.hpp>
#include <boost/type_traits/is_base_of.hpp>

#ifdef _MSC_VER
#pragma warning( pop )
#endif // _MSC_VER

//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////
template< class TSpinBoxType, typename TValueType >
SpinBoxDelegate< TSpinBoxType, TValueType >::
SpinBoxDelegate( QObject * const parent ) :
Superclass( parent )
{

  BOOST_STATIC_ASSERT_MSG(
    ( boost::is_base_of< QAbstractSpinBox, TSpinBoxType >::value ),
    "You must derive from QAbstractSpinBox!" );

} // constructor
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
template< class TSpinBoxType, typename TValueType >
SpinBoxDelegate< TSpinBoxType, TValueType >::~SpinBoxDelegate()
{
} // destructor
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
template< class TSpinBoxType, typename TValueType >
QWidget * SpinBoxDelegate< TSpinBoxType, TValueType >::createEditor(
        QWidget              * parent,
  const QStyleOptionViewItem & option,
  const QModelIndex          & index) const
{

  (void)index;
  (void)option;

  TSpinBoxType * const spinEditor = new TSpinBoxType( parent );

  const TValueType
    minimum = this->GetMinimumValue(),
    maximum = this->GetMaximumValue(),
    step    = this->GetStep        ();

  spinEditor->setMinimum( minimum );
  spinEditor->setMaximum( maximum );
  spinEditor->setSingleStep( step );

  // output
  return spinEditor;

} // createEditor
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
template< class TSpinBoxType, typename TValueType >
void SpinBoxDelegate< TSpinBoxType, TValueType >::setEditorData(
  QWidget * editor, const QModelIndex & index) const
{

  // get the value from the QVariant
  const TValueType currentValue =
    index.model()->data( index, Qt::EditRole ).value< TValueType >();

  // get the spin box
  TSpinBoxType * spinBox = static_cast< TSpinBoxType * >( editor );

  // update the spin box's text with
  // the value
  spinBox->setValue( currentValue );

} // setEditorData
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
template< class TSpinBoxType, typename TValueType >
void SpinBoxDelegate< TSpinBoxType, TValueType >::setModelData(
        QWidget            * editor,
        QAbstractItemModel * model,
  const QModelIndex        & index ) const
{

  // get the Spin box
  TSpinBoxType * spinBox = static_cast< TSpinBoxType * >( editor );

  // get the selected value
  const TValueType currentValue = spinBox->value();

  // update the model with the data
  model->setData( index, currentValue, Qt::EditRole );

} // setModelData
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
template< class TSpinBoxType, typename TValueType >
void SpinBoxDelegate< TSpinBoxType, TValueType >::updateEditorGeometry(
        QWidget              * editor,
  const QStyleOptionViewItem & option,
  const QModelIndex          & index ) const
{

  (void)index;
  editor->setGeometry( option.rect );

} // updateEditorGeometry
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
template< class TSpinBoxType, typename TValueType >
typename
SpinBoxDelegate< TSpinBoxType, TValueType >
::SpinValueType
SpinBoxDelegate< TSpinBoxType, TValueType >
::GetStep() const
{

  // just make 10 steps
  return ( this->GetMaximumValue() - this->GetMinimumValue() ) / 10; 

} // GetStep
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace ccipd
#endif // __ccipdSpinBoxDelegate_hxx
