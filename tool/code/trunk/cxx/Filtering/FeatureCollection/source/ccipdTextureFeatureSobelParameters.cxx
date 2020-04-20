//////////////////////////////////////////////////////////////////////////////////////////
// ccipd includes
#include "ccipdTextureFeatureSobelParameters.h"
#include "ccipdSerializable.hxx"
#include "ccipdFactory.hxx"
#include "ccipdDisableWarningsMacro.h"	  

// boost includes
#include <boost/functional/hash.hpp> // for hashing

// boost serialization includes
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
ccipdFactoryImplementationMacro( TextureFeatureSobelParameters );

template class Serializable< TextureFeatureSobelParameters >;
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
class TextureFeatureSobelParameters::SecretData : public Hashable
{
public: // public to the TextureFeatureFirstOrderParameters class

  /* Constructor */
  SecretData();

 /* Variables */
  VolumeRadiusType m_WindowSize;
  unsigned int m_Direction;

  /* Functions */
  virtual bool operator==( const Hashable & other ) const;
  virtual size_t GetHash() const;
  virtual size_t ReHash () const;
//////////////////////////////////////////////////////////////////////////////////////////



}; // TextureFeatureFirstOrderParameters::SecretData
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
TextureFeatureSobelParameters::TextureFeatureSobelParameters():
ccipdNewPIMPLMacro
{
}

TextureFeatureSobelParameters::Pointer
TextureFeatureSobelParameters::New( const VolumeRadiusType windowSize, const unsigned int direction )
{
  const Pointer newObject = Self::New();
  newObject->SetWindowSize( windowSize ); 
  newObject->SetDirection( direction );

  return newObject;
} // Constructor

TextureFeatureSobelParameters::SecretData::SecretData():
m_WindowSize( 2 ),
m_Direction( 1 )
{
} // Constructor

TextureFeatureSobelParameters::~TextureFeatureSobelParameters()
{
} //Desctructor
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
bool TextureFeatureSobelParameters::operator==( const Hashable & other ) const
{

  // cast and confirm
  const TextureFeatureSobelParameters & otherModel =
    dynamic_cast< const TextureFeatureSobelParameters & >( other );
  if ( &otherModel == this )
    return true;

  // check our variables
  return *( this->m_SecretData ) == *( otherModel.m_SecretData );
  
} // operator==
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
HashValueType TextureFeatureSobelParameters::ReHash() const
{

  return m_SecretData->GetHash();

} // Rehash()
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
// implementation of all the get/ set functions
  ccipdSetByvalImplementHashMacro( TextureFeatureSobelParameters, WindowSize , VolumeRadiusType )
  ccipdGetConstImplementMacro( TextureFeatureSobelParameters, WindowSize     , VolumeRadiusType )
  ccipdSetByvalImplementHashMacro( TextureFeatureSobelParameters, Direction , unsigned int      )
  ccipdGetConstImplementMacro( TextureFeatureSobelParameters, Direction     , unsigned int      )
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
bool TextureFeatureSobelParameters::SecretData::operator==( const Hashable & other ) const
{
  
  // if cast is the same object return true
  const SecretData & otherModel =
    dynamic_cast< const SecretData & >( other );
  if ( &otherModel == this )
    return true;
  
  // check if the parameters are the same
  bool returnValue =
	  m_WindowSize        == otherModel.m_WindowSize  && 
	  m_Direction         == otherModel.m_Direction   &&
	  this->GetHash()     == otherModel.GetHash();

  return returnValue;

} // SecretData::operator==
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
HashValueType TextureFeatureSobelParameters::SecretData::GetHash() const
{
  // never cache
  return this->ReHash();
} // SecretData::GetHash
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
HashValueType TextureFeatureSobelParameters::SecretData::ReHash() const
{
  // initialize our hasher
  const HashValueType seed = 0;
  HashValueType hashValue = seed;

  // hash all simple variables of the object
  boost::hash_combine( hashValue, m_WindowSize     );
  boost::hash_combine(hashValue, m_Direction       );

  // output
  return hashValue;

} // SecretData::ReHash
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
// Implemented directly to avoid ambiguous inheritance
bool TextureFeatureSobelParameters::Load( const std::string & fileName)
{ 

  return this->Serializable< Self >::Load( fileName );

} // Load
  
bool TextureFeatureSobelParameters::Save( const std::string & fileName) const
{

  return this->Serializable< Self >::Save( fileName ); 

} // Save
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
#ifdef Boost_SERIALIZATION_FOUND

  ccipdSerializationImplementationHeadingMacro( TextureFeatureSobelParameters )
  {

  boost::serialization::void_cast_register(
    static_cast< ConcreteFactorySelf< Self > * >( NULL ),
    static_cast< Self * >( NULL ) );


    if ( version < 1)
      return;
    archive
      & boost::serialization::make_nvp ( "WindowSize" ,this->m_SecretData->m_WindowSize      )
	    & boost::serialization::make_nvp ( "Direction"  , this->m_SecretData->m_Direction      )
      & BOOST_SERIALIZATION_BASE_OBJECT_NVP( ParameterBase );

  } // serialize

  ccipdSerializationXMLMacro( TextureFeatureSobelParameters )

  typedef ConcreteFactorySelf< TextureFeatureSobelParameters > TextureFeatureSobelParametersConcrete;
#endif // Boost_SERIALIZATION_FOUND
//////////////////////////////////////////////////////////////////////////////////////////


} //namespace



#ifdef Boost_SERIALIZATION_FOUND
  BOOST_CLASS_VERSION( ccipd::TextureFeatureSobelParametersConcrete, 1 )
  BOOST_CLASS_EXPORT_GUID( ccipd::TextureFeatureSobelParameters , "TextureFeatureSobelParameters" )
  BOOST_CLASS_EXPORT( ccipd::TextureFeatureSobelParametersConcrete )
      ccipdBoostVirtualBaseFalse(
    ccipd::TextureFeatureSobelParameters,
    ccipd::TextureFeatureSobelParametersConcrete )
#endif //Boost_SERIALIZATION_FOUND
