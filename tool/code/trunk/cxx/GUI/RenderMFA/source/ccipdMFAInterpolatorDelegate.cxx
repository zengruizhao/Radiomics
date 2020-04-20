


//////////////////////////////////////////////////////////////////////////////////////////

// ccipd includes
#include "ccipdMFAInterpolatorDelegate.h"

#ifdef _MSC_VER
#pragma warning( disable: 4512 )
#endif

// boost includes
#include <boost/assign/list_of.hpp>
#include <boost/bimap.hpp>
#include <boost/bimap/unordered_set_of.hpp>

//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////
class MFAInterpolatorDelegate::SecretData
{

public:

  typedef MFAInterpolatorDelegate::UniqueItemsCollection UniqueItemsCollection;
  typedef MFAInterpolatorDelegate::ComboItemType         ComboItemType;

  /// Map the interpolator enum values with the strings.
  typedef boost::bimap<
    boost::bimaps::unordered_set_of<
      RegisterInterpolatorCombo::WhichInterpolator >,
    boost::bimaps::unordered_set_of<
      ComboItemType >
  > InterpolatorItemMap;

  static const UniqueItemsCollection
    m_ComboItemCollection;

  /// For each combo item, what's the combo item?
  static const InterpolatorItemMap
    m_ComboItemMap;

}; // class SecretData
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////

// Define the mapping between the strings and enums
// http://bit.ly/10vAu82
const MFAInterpolatorDelegate::SecretData::InterpolatorItemMap
  MFAInterpolatorDelegate::SecretData::m_ComboItemMap =
    boost::assign::list_of<
      MFAInterpolatorDelegate::SecretData::InterpolatorItemMap::relation >
        ( RegisterInterpolatorCombo::LinearInterpolator         , "Linear"  )
        ( RegisterInterpolatorCombo::NearestNeighborInterpolator, "Nearest" );

// Define the sequence of interpolator strings for the combo box
const MFAInterpolatorDelegate::SecretData::UniqueItemsCollection
  MFAInterpolatorDelegate::SecretData::m_ComboItemCollection =
    boost::assign::list_of
      ( MFAInterpolatorDelegate::ComboItemType(
        m_ComboItemMap.left.at(
          RegisterInterpolatorCombo::LinearInterpolator ) ) )
      ( MFAInterpolatorDelegate::ComboItemType(
        m_ComboItemMap.left.at(
          RegisterInterpolatorCombo::NearestNeighborInterpolator ) ) );

//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
MFAInterpolatorDelegate::MFAInterpolatorDelegate( QObject * const parent ) :
Superclass( parent )
{
} // constructor
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
MFAInterpolatorDelegate::~MFAInterpolatorDelegate()
{
} // destructor
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
MFAInterpolatorDelegate::ComboItemType
MFAInterpolatorDelegate::GetComboItem(
  const RegisterInterpolatorCombo::WhichInterpolator whichInterpolator )
{

  return Self::SecretData::m_ComboItemMap.left.at( whichInterpolator );

} // GetComboItem
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
RegisterInterpolatorCombo::WhichInterpolator
MFAInterpolatorDelegate::GetWhichInterpolator(
  const ComboItemType & comboItem )
{

  return Self::SecretData::m_ComboItemMap.right.at( comboItem );

} // GetWhichInterpolator
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
MFAInterpolatorDelegate::UniqueItemsCollection
MFAInterpolatorDelegate::GetItemsForComboBox() const
{

  return Self::SecretData::m_ComboItemCollection;

} // GetItemsForComboBox
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace ccipd
