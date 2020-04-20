


//////////////////////////////////////////////////////////////////////////////////////////

// ccipd includes
#include "ccipdMFARegularizerTypeDelegate.h"

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
class MFARegularizerTypeDelegate::SecretData
{

public:
  
  typedef MFARegularizerTypeDelegate::UniqueItemsCollection UniqueItemsCollection;
  typedef MFARegularizerTypeDelegate::ComboItemType         ComboItemType;
  typedef MFARegularizerTypeDelegate::RegularizerType       RegularizerType;

  /// Map the interpolator enum values with the strings.
  typedef boost::bimap<
    boost::bimaps::unordered_set_of<
      RegularizerType::LikelihoodType >,
    boost::bimaps::unordered_set_of<
      ComboItemType >
  > LikelihoodItemMap;

  static const UniqueItemsCollection
    m_ComboItemCollection;

  /// For each combo item, what's the combo item?
  static const LikelihoodItemMap
    m_ComboItemMap;

}; // class SecretData
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////

// Define the mapping between the strings and enums
// http://bit.ly/10vAu82
const MFARegularizerTypeDelegate::SecretData::LikelihoodItemMap
  MFARegularizerTypeDelegate::SecretData::m_ComboItemMap =
    boost::assign::list_of<
      MFARegularizerTypeDelegate::SecretData::LikelihoodItemMap::relation >
        ( MFARegularizerTypeDelegate::RegularizerType::LikelihoodPDF        , "PDF"          )
        ( MFARegularizerTypeDelegate::RegularizerType::LikelihoodLogPDF     , "log( PDF )"   )
        ( MFARegularizerTypeDelegate::RegularizerType::LikelihoodRelativePDF, "Relative PDF" );

// Define the sequence of interpolator strings for the combo box
const MFARegularizerTypeDelegate::SecretData::UniqueItemsCollection
  MFARegularizerTypeDelegate::SecretData::m_ComboItemCollection =
    boost::assign::list_of
      ( MFARegularizerTypeDelegate::ComboItemType(
        m_ComboItemMap.left.at(
          MFARegularizerTypeDelegate::RegularizerType::LikelihoodPDF ) ) )
      ( MFARegularizerTypeDelegate::ComboItemType(
        m_ComboItemMap.left.at(
          MFARegularizerTypeDelegate::RegularizerType::LikelihoodLogPDF ) ) )
      ( MFARegularizerTypeDelegate::ComboItemType(
        m_ComboItemMap.left.at(
          MFARegularizerTypeDelegate::RegularizerType::LikelihoodRelativePDF ) ) )
      ( MFARegularizerTypeDelegate::ComboItemType(
        m_ComboItemMap.left.at(
          MFARegularizerTypeDelegate::RegularizerType::LikelihoodLogPDF ) ) ); // this one should never get added

//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
MFARegularizerTypeDelegate::MFARegularizerTypeDelegate( QObject * const parent ) :
Superclass( parent )
{
} // constructor
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
MFARegularizerTypeDelegate::~MFARegularizerTypeDelegate()
{
} // destructor
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
MFARegularizerTypeDelegate::ComboItemType
MFARegularizerTypeDelegate::GetComboItem(
  const RegularizerType::LikelihoodType likelihoodType )
{

  return Self::SecretData::m_ComboItemMap.left.at( likelihoodType );

} // GetComboItem
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
MFARegularizerTypeDelegate::RegularizerType::LikelihoodType
MFARegularizerTypeDelegate::GetLikelihoodType(
  const ComboItemType & comboItem )
{

  return Self::SecretData::m_ComboItemMap.right.at( comboItem );

} // GetLikelihoodType
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
MFARegularizerTypeDelegate::UniqueItemsCollection
MFARegularizerTypeDelegate::GetItemsForComboBox() const
{

  return Self::SecretData::m_ComboItemCollection;

} // GetItemsForComboBox
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace ccipd
