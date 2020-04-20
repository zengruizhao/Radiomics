


//////////////////////////////////////////////////////////////////////////////////////////

// ccipd includes
#include "ccipdMFATransformDelegate.h"

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
class MFATransformDelegate::SecretData
{

public:
  
  typedef MFATransformDelegate::UniqueItemsCollection UniqueItemsCollection;
  typedef MFATransformDelegate::ComboItemType         ComboItemType;
  typedef MFATransformDelegate::ComboTransformType    ComboTransformType;

  /// Map the interpolator enum values with the strings.
  typedef boost::bimap<
    boost::bimaps::unordered_set_of<
      ComboTransformType::WhichTransform >,
    boost::bimaps::unordered_set_of<
      ComboItemType >
  > TransformItemMap;

  static const UniqueItemsCollection
    m_ComboItemCollection;

  /// For each combo item, what's the combo item?
  static const TransformItemMap
    m_ComboItemMap;

}; // class SecretData
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////

// Define the mapping between the strings and enums
// http://bit.ly/10vAu82
const MFATransformDelegate::SecretData::TransformItemMap
  MFATransformDelegate::SecretData::m_ComboItemMap =
    boost::assign::list_of<
      MFATransformDelegate::SecretData::TransformItemMap::relation >
        ( MFATransformDelegate::ComboTransformType::RigidTransform , "Rigid"  )
        ( MFATransformDelegate::ComboTransformType::AffineTransform, "Affine" );

// Define the sequence of interpolator strings for the combo box
const MFATransformDelegate::SecretData::UniqueItemsCollection
  MFATransformDelegate::SecretData::m_ComboItemCollection =
    boost::assign::list_of
      ( MFATransformDelegate::ComboItemType(
        m_ComboItemMap.left.at(
          MFATransformDelegate::ComboTransformType::RigidTransform ) ) )
      ( MFATransformDelegate::ComboItemType(
        m_ComboItemMap.left.at(
          MFATransformDelegate::ComboTransformType::AffineTransform ) ) )
      ( MFATransformDelegate::ComboItemType(
        m_ComboItemMap.left.at(
          MFATransformDelegate::ComboTransformType::RigidTransform ) ) ); // this one should never get added

//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
MFATransformDelegate::MFATransformDelegate( QObject * const parent ) :
Superclass( parent )
{
} // constructor
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
MFATransformDelegate::~MFATransformDelegate()
{
} // destructor
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
MFATransformDelegate::ComboItemType
MFATransformDelegate::GetComboItem(
  const ComboTransformType::WhichTransform whichTransform )
{

  return Self::SecretData::m_ComboItemMap.left.at( whichTransform );

} // GetComboItem
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
MFATransformDelegate::ComboTransformType::WhichTransform
MFATransformDelegate::GetWhichTransform(
  const ComboItemType & comboItem )
{

  return Self::SecretData::m_ComboItemMap.right.at( comboItem );

} // GetWhichTransform
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
MFATransformDelegate::UniqueItemsCollection
MFATransformDelegate::GetItemsForComboBox() const
{

  return Self::SecretData::m_ComboItemCollection;

} // GetItemsForComboBox
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace ccipd
