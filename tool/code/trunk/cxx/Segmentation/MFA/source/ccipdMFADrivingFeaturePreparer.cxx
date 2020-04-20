


//////////////////////////////////////////////////////////////////////////////////////////

// ccipd includes
#include "ccipdFeatureID.h"
#include "ccipdMFADrivingFeaturePreparer.h"
#include "ccipdSIMFeaturePreparerUsingExtractors.h"
#include "ccipdMFALevelsetExtractor.h"      // for setting the driving features
#include "ccipdMFATextureExtractor.h"       // for setting the driving features
#include "ccipdMFADrivingLevelsetExtractor.h"
#include "ccipdMFADrivingTextureExtractor.h"
#include "ccipdMFATypes.h"
#include "ccipdFactory.hxx"

// boost includes
#include <boost/foreach.hpp>

// serialization includes
#ifdef Boost_SERIALIZATION_FOUND
  #include <boost/archive/xml_iarchive.hpp>
  #include <boost/archive/xml_oarchive.hpp>
  #include <boost/serialization/vector.hpp>
  #include <boost/serialization/export.hpp>
  #include "ccipdSharedPointerSerialization.hxx" // for serialization
#endif // Boost_SERIALIZATION_FOUND

#ifdef _MSC_VER
#pragma warning( disable: 4355 )
#endif // _MSC_VER

//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////
template
class Factory< MFADrivingFeaturePreparer >;
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
class MFADrivingFeaturePreparer::SecretData : public boost::noncopyable
{

public:
  typedef MFADrivingFeaturePreparer EncapsulatingClass;

  SecretData( EncapsulatingClass * const encapsulatingClass );

  EncapsulatingClass
    * const m_EncapsulatingClass;

  // this variable will use extractors
  MFAFeaturePreparerUsingExtractors::Pointer
    m_InternalPreparer;
    
  FeatureIDConstCollection
    m_InputLevelsetFeatures,
    m_InputTextureFeatures;

  float
    m_ExtrapolateInMM;
  
  MFAModelConstPointer
    m_MFAModel;

  /**
   * Call this function
   * whenever anything changes
   * so that the superclass's
   * extractors can be
   * updated.
   */
  void UpdateExtractors();

}; // class SecretData
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
MFADrivingFeaturePreparer::MFADrivingFeaturePreparer() :
m_SecretData( new SecretData( this ) )
{
} // constructor
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
MFADrivingFeaturePreparer::~MFADrivingFeaturePreparer()
{
} // destructor
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
MFADrivingFeaturePreparer::SecretData::SecretData(
  MFADrivingFeaturePreparer * const encapsulatingClass ) :
m_EncapsulatingClass( encapsulatingClass ),
m_InternalPreparer( MFAFeaturePreparerUsingExtractors::New() ),
m_ExtrapolateInMM( 0 )
{

  // start with an empty texture feature
  // so that serializing a new driving preparer
  // actually has a template FeatureID to use in the XML file
  this->m_InputTextureFeatures.push_back( 
    FeatureIDConstPointer( FeatureID::New( TextureFeatureIntensity ) ) );

} // constructor
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
FeatureIDConstCollection MFADrivingFeaturePreparer::GetWhichFeatures() const
{
  return this->m_SecretData->m_InternalPreparer->GetWhichFeatures();
} // GetWhichFeatures
//////////////////////////////////////////////////////////////////////////////////////////


  
//////////////////////////////////////////////////////////////////////////////////////////
FeatureCollectionPointer MFADrivingFeaturePreparer::GetFeatures(
  const StudyConstPointer & study ) const
{
  return this->m_SecretData->m_InternalPreparer->GetFeatures( study );
} // GetFeatures
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void MFADrivingFeaturePreparer::SecretData::UpdateExtractors()
{

  // get the class
  EncapsulatingClass & encapsulating = *( this->m_EncapsulatingClass );

  // create new extractors with only the driving features
  const MFALevelsetExtractorPointer
    drivingLevelsetExtractor = CreateDrivingLevelsetExtractor( encapsulating );
  const MFATextureExtractorPointer
    drivingTextureExtractor = CreateDrivingTextureExtractor( encapsulating );

  // add in the extrapolation value
  const float extrapolationInMM = this->m_ExtrapolateInMM;
  if ( drivingLevelsetExtractor )
    drivingLevelsetExtractor->SetExtrapolateInMM( extrapolationInMM );
  if ( drivingTextureExtractor )
    drivingTextureExtractor->SetExtrapolateInMM( extrapolationInMM );
  
  // clear the old extractors
  this->m_InternalPreparer->ClearExtractors();

  // add in the new ones
  this->m_InternalPreparer->AddExtractor( drivingLevelsetExtractor );
  this->m_InternalPreparer->AddExtractor( drivingTextureExtractor  );

} // UpdateExtractors
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
// We must update the extractors whenever anything changes.
#define ccipdDrivingPreparerSetByrefImplementMacro(  \
  VariableName, VariableType )                       \
  void MFADrivingFeaturePreparer::Set##VariableName( \
    const VariableType & variable )                  \
  {                                                  \
    ccipdSetBodyMacro( VariableName )                \
    this->m_SecretData->UpdateExtractors();          \
  }
  
#define ccipdDrivingPreparerSetByvalImplementMacro(  \
  VariableName, VariableType )                       \
  void MFADrivingFeaturePreparer::Set##VariableName( \
    const VariableType variable )                    \
  {                                                  \
    ccipdSetBodyMacro( VariableName )                \
    this->m_SecretData->UpdateExtractors();          \
  }  
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
ccipdDrivingPreparerSetByrefImplementMacro(
  MFAModel, MFAModelConstPointer )
ccipdGetConstImplementMacro( MFADrivingFeaturePreparer,
  MFAModel, MFAModelConstPointer )
ccipdDrivingPreparerSetByvalImplementMacro(
  ExtrapolateInMM, float )
ccipdGetConstImplementMacro( MFADrivingFeaturePreparer,
  ExtrapolateInMM, float )
ccipdDrivingPreparerSetByrefImplementMacro(
  InputLevelsetFeatures, FeatureIDConstCollection )
ccipdDrivingPreparerSetByrefImplementMacro(
  InputTextureFeatures, FeatureIDConstCollection )
ccipdGetConstImplementMacro( MFADrivingFeaturePreparer,
  InputLevelsetFeatures, FeatureIDConstCollection )
ccipdGetConstImplementMacro( MFADrivingFeaturePreparer,
  InputTextureFeatures, FeatureIDConstCollection )
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
#ifdef Boost_SERIALIZATION_FOUND

ccipdSerializationImplementationHeadingMacro( MFADrivingFeaturePreparer )
{
  boost::serialization::void_cast_register(
    static_cast< ConcreteFactorySelf< Self > * >( NULL ),
    static_cast< Self * >( NULL ) );

  using boost::serialization::make_nvp;

  if ( version < 1 )
    return;

  // make non-const (temporarily) so we can serialize 
  FeatureIDCollection
    unconstLevelsetFeatures,
    unconstTextureFeatures;

  BOOST_FOREACH( const FeatureIDConstPointer & featureID,
    this->m_SecretData->m_InputLevelsetFeatures )
    {
    unconstLevelsetFeatures.push_back(
      std::const_pointer_cast< FeatureID >( featureID ) );
    }

  BOOST_FOREACH( const FeatureIDConstPointer & featureID,
    this->m_SecretData->m_InputTextureFeatures )
    {
    unconstTextureFeatures.push_back(
      std::const_pointer_cast< FeatureID >( featureID ) );
    }

  archive
    & make_nvp( "MFAFeaturePreparer", this->m_SecretData->m_InternalPreparer )
    & make_nvp( "LevelsetFeatures", unconstLevelsetFeatures )
    & make_nvp( "TextureFeatures" , unconstTextureFeatures  );

  if ( version >= 2 )
    {
    archive & make_nvp( "ExtrapolateInMM", this->m_SecretData->m_ExtrapolateInMM );
    } // version 2
    
  // copy back in case they have changed from serialization
  this->m_SecretData->m_InputLevelsetFeatures.clear();
  this->m_SecretData->m_InputLevelsetFeatures.insert(
    this->m_SecretData->m_InputLevelsetFeatures.begin(),
    unconstLevelsetFeatures.cbegin(),
    unconstLevelsetFeatures.cend  () );

  // copy back in case they have changed from serialization
  this->m_SecretData->m_InputTextureFeatures.clear();
  this->m_SecretData->m_InputTextureFeatures.insert(
    this->m_SecretData->m_InputTextureFeatures.begin(),
    unconstTextureFeatures.cbegin(),
    unconstTextureFeatures.cend  () );

  // update the extractors as the features may have changed
  this->m_SecretData->UpdateExtractors();

} // serialize

ccipdSerializationXMLMacro( MFADrivingFeaturePreparer )

typedef ConcreteFactorySelf< MFADrivingFeaturePreparer > 
  MFADrivingFeaturePreparerConcrete;

#endif // Boost_SERIALIZATION_FOUND
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace ccipd
 


#ifdef Boost_SERIALIZATION_FOUND

  BOOST_CLASS_VERSION( ccipd::MFADrivingFeaturePreparerConcrete, 2 )
  BOOST_CLASS_EXPORT_GUID( ccipd::MFADrivingFeaturePreparer , "MFADrivingFeaturePreparer" )
  BOOST_CLASS_EXPORT( ccipd::MFADrivingFeaturePreparerConcrete )
      ccipdBoostVirtualBaseFalse(
    ccipd::MFADrivingFeaturePreparer,
    ccipd::MFADrivingFeaturePreparerConcrete )

#endif // Boost_SERIALIZATION_FOUND

