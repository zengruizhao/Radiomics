


//////////////////////////////////////////////////////////////////////////////////////////

// ccipd includes
#include "ccipdSIMFeaturePreparerUsingExtractors.h"
#include "ccipdSIMFeatureExtractor.h"
#include "ccipdSIMFeatureExtractorInsertion.h"
#include "ccipdFeatureCollection.h"
#include "ccipdSIMStudiesMaskAligner.h"
#include "ccipdFactory.hxx"

// boost includes
#include <boost/foreach.hpp>

// serialization includes
#ifdef Boost_SERIALIZATION_FOUND
  #include <boost/archive/xml_iarchive.hpp>
  #include <boost/archive/xml_oarchive.hpp>
  #include <boost/serialization/export.hpp>
#endif // Boost_SERIALIZATION_FOUND

//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{


//////////////////////////////////////////////////////////////////////////////////////////
template
class Factory< SIMFeaturePreparerUsingExtractors >;
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
class SIMFeaturePreparerUsingExtractors::SecretData
{

public:

  SecretData();

  bool
    m_Verbose;
  StudyAlignerPointer
    m_StudyAligner;
  SIMBoundingBoxExtractorPointer
    m_BoundingBoxExtractor;
  SIMFeatureExtractorCollection
    m_Extractors;

}; // class SecretData
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
using std::cout;
using std::endl;
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
SIMFeaturePreparerUsingExtractors::SecretData::SecretData() :
m_Verbose( true )
{
} // Constructor
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
SIMFeaturePreparerUsingExtractors::SIMFeaturePreparerUsingExtractors() :
ccipdNewPIMPLMacro
{
} // Constructor
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
SIMFeaturePreparerUsingExtractors::~SIMFeaturePreparerUsingExtractors()
{
} // Destructor
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void SIMFeaturePreparerUsingExtractors::ClearExtractors()
{
  this->m_SecretData->m_Extractors.clear();
} // ClearExtractors
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void SIMFeaturePreparerUsingExtractors::AddExtractor(
  const SIMFeatureExtractorPointer & extractor )
{
  this->m_SecretData->m_Extractors.push_back( extractor );
} // AddExtractor
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void SIMFeaturePreparerUsingExtractors::AddExtractors(
  const SIMFeatureExtractorCollection & extractors )
{
  BOOST_FOREACH( const SIMFeatureExtractorPointer & extractor,
    extractors )
  {
    this->AddExtractor( extractor );
  }
} // AddExtractor
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
FeatureIDConstCollection SIMFeaturePreparerUsingExtractors::GetWhichFeatures() const
{

  // start out empty
  FeatureIDConstCollection whichFeatures;

  // we concatenate the features from each extractor
  BOOST_FOREACH( const SIMFeatureExtractorPointer & extractor,
    this->m_SecretData->m_Extractors )
    {

    // get the features for the current extractor
    const FeatureIDConstCollection currentExtractorWhichFeatures =
      extractor->GetWhichFeatures();

    // add it to the collection
    whichFeatures.insert(
      whichFeatures.end(),
      currentExtractorWhichFeatures.cbegin(),
      currentExtractorWhichFeatures.cend() );

    } // for each extractor

  return whichFeatures;

} // GetWhichFeatures
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
FeatureCollectionPointer SIMFeaturePreparerUsingExtractors::GetFeatures(
  const StudyConstPointer & study ) const
{

  const bool verbose = this->m_SecretData->m_Verbose;

  // start with a blank feature collection
  FeatureCollectionPointer allFeatures( FeatureCollection::New() );

  if ( verbose )
    {
    cout << "Has study aligner = " <<
      !!this->m_SecretData->m_StudyAligner << endl;
    cout << "Has bounding box aligner = " <<
      !!this->m_SecretData->m_BoundingBoxExtractor << endl;    
    }
  
  // loop through each extractor
  BOOST_FOREACH( const SIMFeatureExtractorPointer & extractor,
    this->m_SecretData->m_Extractors )
    {

    extractor->SetVerbose( verbose );

    // set the study for this extractor
    extractor->SetStudy( study );

    // set the mask aligner
    extractor->SetAligner             ( this->m_SecretData->m_StudyAligner );
    extractor->SetBoundingBoxExtractor( this->m_SecretData->m_BoundingBoxExtractor );

    // insert the extractor's features into our collection
    if ( verbose ) cout << "Inserting features:" << endl;
    SIMInsertFeatures( *extractor, *allFeatures );
    if ( verbose ) cout << "Inserting features done." << endl;

    } // for each extractor

  // output
  return allFeatures;

} // GetFeatures
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
ccipdSetByvalImplementMacro( SIMFeaturePreparerUsingExtractors,
  Verbose, bool )
ccipdSetByrefImplementMacro( SIMFeaturePreparerUsingExtractors,
  StudyAligner, StudyAlignerPointer )
ccipdSetByrefImplementMacro( SIMFeaturePreparerUsingExtractors,
  BoundingBoxExtractor, SIMBoundingBoxExtractorPointer )
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
#ifdef Boost_SERIALIZATION_FOUND

ccipdSerializationImplementationHeadingMacro( SIMFeaturePreparerUsingExtractors )
{
  boost::serialization::void_cast_register(
    static_cast< ConcreteFactorySelf< Self > * >( NULL ),
    static_cast< Self * >( NULL ) );

  using boost::serialization::make_nvp;

  if ( version < 1 )
    return;

  archive
    & make_nvp( "Verbose", this->m_SecretData->m_Verbose );

} // serialize

ccipdSerializationXMLMacro( SIMFeaturePreparerUsingExtractors )

typedef ConcreteFactorySelf< SIMFeaturePreparerUsingExtractors > 
  SIMFeaturePreparerUsingExtractorsConcrete;

#endif // Boost_SERIALIZATION_FOUND
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace ccipd
 


#ifdef Boost_SERIALIZATION_FOUND

  BOOST_CLASS_VERSION( ccipd::SIMFeaturePreparerUsingExtractorsConcrete, 1 )
  BOOST_CLASS_EXPORT_GUID( ccipd::SIMFeaturePreparerUsingExtractors , "SIMFeaturePreparerUsingExtractors" )
  BOOST_CLASS_EXPORT( ccipd::SIMFeaturePreparerUsingExtractorsConcrete )
      ccipdBoostVirtualBaseFalse(
    ccipd::SIMFeaturePreparerUsingExtractors,
    ccipd::SIMFeaturePreparerUsingExtractorsConcrete )

#endif // Boost_SERIALIZATION_FOUND
