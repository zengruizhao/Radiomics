//////////////////////////////////////////////////////////////////////////////////////////
// ccipd includes
#include "ccipdTextureFeatureFirstOrderParameters.h"
#include "ccipdFactory.hxx"
#include "ccipdSerializable.hxx"
#include "ccipdDisableWarningsMacro.h"
// boost includes
#include <boost/functional/hash.hpp> // for hashing

// serialization includes
#ifdef Boost_SERIALIZATION_FOUND
  #include <boost/archive/xml_iarchive.hpp>
  #include <boost/archive/xml_oarchive.hpp> 
  #include <boost/serialization/export.hpp>
#endif // Boost_SERIALIZATION_FOUND
//////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdEnableWarningsMacro.h"



namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////
ccipdFactoryImplementationMacro( TextureFeatureFirstOrderParameters );

template
class Serializable< TextureFeatureFirstOrderParameters >;
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
class TextureFeatureFirstOrderParameters::SecretData : public Hashable
{
public: // public to the TextureFeatureFirstOrderParameters class

  /* Constructor */
  SecretData();

  /* Variables */
  VolumeRadiusType m_WindowSize;

  /* Functions */
  virtual bool operator==( const Hashable & other ) const;
  virtual size_t GetHash() const;
  virtual size_t ReHash () const;
//////////////////////////////////////////////////////////////////////////////////////////



}; // TextureFeatureFirstOrderParameters::SecretData
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
TextureFeatureFirstOrderParameters::TextureFeatureFirstOrderParameters():
ccipdNewPIMPLMacro
{
} // Constructor
TextureFeatureFirstOrderParameters::Pointer
TextureFeatureFirstOrderParameters::New( const VolumeRadiusType windowSize )
{
  const Pointer newObject = Self::New();
  newObject->SetWindowSize( windowSize );
  return newObject;
} //
TextureFeatureFirstOrderParameters::SecretData::SecretData():
m_WindowSize( 2 )
{
} // Constructor

TextureFeatureFirstOrderParameters::~TextureFeatureFirstOrderParameters()
{
} // Destructor
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
bool TextureFeatureFirstOrderParameters::operator==( const Hashable & other ) const
{

  // cast and confirm
  const TextureFeatureFirstOrderParameters & otherModel =
    dynamic_cast< const TextureFeatureFirstOrderParameters & >( other );
  if ( &otherModel == this )
    return true;

  // check our variables
  return *( this->m_SecretData ) == *( otherModel.m_SecretData );
  
} // operator==
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
HashValueType TextureFeatureFirstOrderParameters::ReHash() const
{

  return m_SecretData->GetHash();

} // ReHash()
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
// implementation of all the get/ set functions
  ccipdSetByvalImplementHashMacro( TextureFeatureFirstOrderParameters, WindowSize , VolumeRadiusType )
  ccipdGetConstImplementMacro( TextureFeatureFirstOrderParameters, WindowSize     , VolumeRadiusType )
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
bool TextureFeatureFirstOrderParameters::SecretData::operator==( const Hashable & other ) const
{

  // if cast is the same object return true
  const SecretData & otherModel =
    dynamic_cast< const SecretData & >( other );
  if ( &otherModel == this )
    return true;
  
  // check if the parameters are the same
  bool returnValue =
	  m_WindowSize        == otherModel.m_WindowSize          && 
	  this->GetHash()     == otherModel.GetHash();

  return returnValue;

} // SecretData::operator==
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
// Implemented directly to avoid ambiguous inheritance
bool TextureFeatureFirstOrderParameters::Load( const std::string & fileName)
{ 

  return this->Serializable< Self >::Load( fileName );

} // Load
  
bool TextureFeatureFirstOrderParameters::Save( const std::string & fileName) const
{

  return this->Serializable< Self >::Save( fileName ); 

} // Save
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
HashValueType TextureFeatureFirstOrderParameters::SecretData::GetHash() const
{
  // never cache
  return this->ReHash();

} // SecretData::GetHash
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
HashValueType TextureFeatureFirstOrderParameters::SecretData::ReHash() const
{
  // initialize our hasher

  const HashValueType seed = 0;
  HashValueType hashValue = seed;

  // initialize using the parameterbase hash
  // hash all simple variables of the object
  boost::hash_combine( hashValue, m_WindowSize     );

  // output
  return hashValue;

} // ReHash
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
#ifdef Boost_SERIALIZATION_FOUND

  ccipdSerializationImplementationHeadingMacro( TextureFeatureFirstOrderParameters )
  {

  boost::serialization::void_cast_register(
    static_cast< ConcreteFactorySelf< Self > * >( NULL ),
    static_cast< Self * >( NULL ) );

    if ( version < 1)
      return;

    archive
      & boost::serialization::make_nvp ( "WindowSize"     , this->m_SecretData->m_WindowSize      )
      & BOOST_SERIALIZATION_BASE_OBJECT_NVP( ParameterBase );

  } // serialize

  ccipdSerializationXMLMacro( TextureFeatureFirstOrderParameters )

  typedef ConcreteFactorySelf< TextureFeatureFirstOrderParameters > 
    TextureFeatureFirstOrderParametersConcrete;
#endif // Boost_SERIALIZATION_FOUND
//////////////////////////////////////////////////////////////////////////////////////////


} //namespace



#ifdef Boost_SERIALIZATION_FOUND
  BOOST_CLASS_VERSION( ccipd::TextureFeatureFirstOrderParametersConcrete, 1 )
  BOOST_CLASS_EXPORT_GUID( ccipd::TextureFeatureFirstOrderParameters , "TextureFeatureFirstOrderParameters" )
  BOOST_CLASS_EXPORT( ccipd::TextureFeatureFirstOrderParametersConcrete )
    ccipdBoostVirtualBaseFalse(
    ccipd::TextureFeatureFirstOrderParameters,
    ccipd::TextureFeatureFirstOrderParametersConcrete )
#endif // Boost_SERIALIZATION_FOUND
