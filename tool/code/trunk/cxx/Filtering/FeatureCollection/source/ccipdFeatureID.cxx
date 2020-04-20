
//////////////////////////////////////////////////////////////////////////////////////////
// ccipd includes
#include "ccipdFeatureID.h"
#include "ccipdTextureFeatureMapParametersType.h"
#include "ccipdTextureFeatureGaborParameters.h"
#include "ccipdTextureFeatureHaralickParameters.h"
#include "ccipdTextureFeatureSobelParameters.h"
#include "ccipdTextureFeatureFirstOrderParameters.h"
#include "ccipdTextureFeatureParser.h"
#include "ccipdCloneableFactory.hxx"
#include "ccipdSerializable.hxx"
#include "ccipdTextureFeatureGetParametersByVariant.h"
#include "ccipdDisableWarningsMacro.h"

// boost includes
#include <boost/functional/hash.hpp> // for hashing
#include <boost/variant/detail/hash_variant.hpp>

// std includes
#include <algorithm> // for equal
#include <string>


  // serialization includes
  #ifdef Boost_SERIALIZATION_FOUND
    #include <boost/archive/xml_iarchive.hpp>
    #include <boost/archive/xml_oarchive.hpp> 
    #include <boost/serialization/nvp.hpp>        // for xml tags
    #include "ccipdSharedPointerSerialization.hxx"
    #include <boost/serialization/variant.hpp>
    #include <boost/serialization/export.hpp>
    #include <boost/variant/apply_visitor.hpp>

	using boost::serialization::make_nvp;

  #endif // Boost_SERIALIZATION_FOUND
#include "ccipdEnableWarningsMacro.h"
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
using std::string;

//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////
template
class Factory< FeatureID >;

template
class CloneableFactory< FeatureID >;

template
class Cloneable< FeatureID >;

template
class Copyable< FeatureID >;

template
class Serializable< FeatureID >;
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
class FeatureID::SecretData
{
public: // public to the FeatureID class

  /* Constructor */
  SecretData() { }

  /* Variables */
  TextureFeatureEnumPointer   m_TextureFeature;
  FeatureDescriptionType      m_Description;
  AnyTextureFeatureParameters m_Parameters;

  /* To save/load the text key for m_TextureFeature */
  TextureFeatureParser        m_Parser;

  /* To determine default parameter type */
  void SetDefaultParameter();
}; // TextureFeatureExtractor::SecretData
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
FeatureID::FeatureID() : ccipdNewPIMPLMacro
{
}

FeatureID::Pointer FeatureID::New( const TextureFeatureEnum textureFeature )
{
  const Pointer newObject = Self::New();
  newObject->SetTextureFeature( textureFeature );
  newObject->m_SecretData->SetDefaultParameter();
  return newObject;
}

FeatureID::Pointer FeatureID::New( const TextureFeatureEnumPointer textureFeature )
{
  const Pointer newObject = Self::New();
  newObject->SetTextureFeature( textureFeature );
  newObject->m_SecretData->SetDefaultParameter();
  return newObject;
}

FeatureID::Pointer FeatureID::New( const FeatureDescriptionType & description )
{
  const Pointer newObject = Self::New();
  newObject->SetDescription( description );
  return newObject;
}

FeatureID::Pointer FeatureID::New( const AnyTextureFeatureParameters & parameters )
{
  const Pointer newObject = Self::New();
  newObject->SetParameters( parameters );
  return newObject;
}

FeatureID::Pointer FeatureID::New(  const TextureFeatureEnum       textureFeature
								  , const FeatureDescriptionType & description 
								 )
{
  const Pointer newObject = Self::New();
  newObject->SetTextureFeature( textureFeature );
  newObject->m_SecretData->SetDefaultParameter();
  newObject->SetDescription   ( description    );
  return newObject;
}

FeatureID::Pointer FeatureID::New(  const TextureFeatureEnumPointer textureFeature
								  , const FeatureDescriptionType  & description 
								 )
{
  const Pointer newObject = Self::New();
  newObject->SetTextureFeature( textureFeature );
  newObject->m_SecretData->SetDefaultParameter();
  newObject->SetDescription   ( description    );
  return newObject;
}

FeatureID::Pointer FeatureID::New(  const TextureFeatureEnum            textureFeature
								  , const AnyTextureFeatureParameters & parameters
								 )
{
  const Pointer newObject = Self::New();
  newObject->SetTextureFeature( textureFeature );
  newObject->SetParameters    ( parameters     );
  return newObject;
}

FeatureID::Pointer FeatureID::New(  const TextureFeatureEnumPointer     textureFeature
                                  , const AnyTextureFeatureParameters & parameters 
			                     )
{
  const Pointer newObject = Self::New();
  newObject->SetTextureFeature( textureFeature );
  newObject->SetParameters    ( parameters     );
  return newObject;
}

FeatureID::Pointer FeatureID::New(  const TextureFeatureEnum            textureFeature
								  , const FeatureDescriptionType      & description
								  , const AnyTextureFeatureParameters & parameters 
								 )
{
  const Pointer newObject = Self::New();
  newObject->SetTextureFeature( textureFeature );
  newObject->SetDescription   ( description    );
  newObject->SetParameters    ( parameters     );
  return newObject;
}

FeatureID::Pointer FeatureID::New(  const TextureFeatureEnumPointer     textureFeature
								  , const FeatureDescriptionType      & description
								  , const AnyTextureFeatureParameters & parameters 
								 )
{
  const Pointer newObject = Self::New();
  newObject->SetTextureFeature( textureFeature );
  newObject->SetDescription   ( description    );
  newObject->SetParameters    ( parameters     );
  return newObject;
}

FeatureID::~FeatureID()
{
}
//////////////////////////////////////////////////////////////////////////////////////////




//////////////////////////////////////////////////////////////////////////////////////////

#ifdef Boost_SERIALIZATION_FOUND

template< class Archive >                             
void FeatureID::load( Archive & archive, const unsigned int version )
{
  boost::serialization::void_cast_register(
    static_cast< ConcreteFactorySelf< Self > * >( NULL ),
    static_cast< Self * >( NULL ) ); 

  std::string description;

  archive & make_nvp( "Description", description );
  this->SetDescription( description );

  if ( version >= 1 )
	  {
	  bool hasTextureFeature;
    archive & make_nvp( "HasTextureFeature", hasTextureFeature );
	
	  if ( hasTextureFeature ) // if texture feature is present convert string to enum
	    {
	    std::string featureName;
	  
	    archive & make_nvp( "TextureFeature", featureName );
          this->SetTextureFeature(  featureName  );

		if ( version >= 2 )
		  {
      AnyTextureFeatureParameters parameters( this->GetParameters() ) ;
		  archive & make_nvp( "Parameters", parameters );
		  
        this->SetParameters( parameters );
		    } // version >= 2
      else // if a older version load default parameters for the given type
        {
        this->m_SecretData->SetDefaultParameter();
        } //version == 1
       
	    }
    } // version >= 1
  else // i think this is correct since no version was given for the base 
    {
    this->SerializationDepreciated( archive, version );
    }

} // load

// template boost serialization functions
template< class Archive >                             
void FeatureID::save(                                       
  Archive          & archive,                         
  const unsigned int version ) const
{

  archive & make_nvp( "Description", this->m_SecretData->m_Description );

  if ( version >= 1 )
    {
    bool hasTextureFeature = this->m_SecretData->m_TextureFeature;		
    archive & make_nvp( "HasTextureFeature", hasTextureFeature );
	
	  if ( hasTextureFeature ) // if texture feature is present convert enum to string
	    {
	    std::string featureName = this->GetFeatureName();
        archive & make_nvp( "TextureFeature", featureName );

	    if ( version >= 2 )
	      {
		  AnyTextureFeatureParameters parameters = this->GetParameters();
		  archive & make_nvp( "Parameters", parameters );
	      }
	    }
    } // version >= 1
  else // I think this is correct as there was version being set previously
    {
    
	  this->SerializationDepreciated( archive, version );

    }
} // save

template< class Archive >   
void FeatureID::SerializationDepreciated(
  Archive         & archive,                         
  const unsigned int version ) const
{
  (void) version;
  using boost::serialization::make_nvp;

  // do we actually have a texture feature?
  bool hasTextureFeature = this->m_SecretData->m_TextureFeature;
 
  // if so, extract it
  TextureFeatureEnum textureFeature = TextureFeatureIntensity; // start at a random value
  if ( hasTextureFeature )
      textureFeature = *this->m_SecretData->m_TextureFeature; // assign it

  archive & make_nvp( "HasTextureFeature", hasTextureFeature );

  if ( hasTextureFeature )
    {
    // save/load archive
    archive & make_nvp( "TextureFeature", textureFeature );

    // make sure our assigned variable is the loaded one
    this->m_SecretData->m_TextureFeature = TextureFeatureEnumPointer(
        new TextureFeatureEnum( textureFeature ) );

    } // hasTextureFeature
 
} // SerializationDepreciated

template void FeatureID::load(          
  boost::archive::xml_iarchive & archive,    
  const unsigned int version );              
                                               
  /* xml saving */                             
template void FeatureID::save(          
  boost::archive::xml_oarchive & archive,    
  const unsigned int version ) const;


  typedef ConcreteFactorySelf< FeatureID > FeatureIDConcrete;
#endif // Boost_SERIALIZATION_FOUND
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void FeatureID::CopyParametersInternal( const Self & other )
{

  // do the copy
  const TextureFeatureEnumPointer newTextureEnum(
   other.HasTextureFeature() ?
    TextureFeatureEnumPointer(
      new TextureFeatureEnum( *other.GetTextureFeature() ) ) :
    TextureFeatureEnumPointer() );
   
  this->SetTextureFeature( newTextureEnum );
  this->SetDescription   ( other.GetDescription() );
  this->SetParameters    ( other.GetParameters()  );
} // CopyParametersInternal
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
bool FeatureID::operator==( const Hashable & other ) const
{

  // cast and confirm
  const FeatureID & otherFeatureID =
    dynamic_cast< const FeatureID & >( other );
  if ( &otherFeatureID == this )
    return true;

  // check if we have a texture feature
  const bool hasTextureFeature = this->HasTextureFeature();
  if ( hasTextureFeature && !otherFeatureID.HasTextureFeature() )
    return false;

  // check our texture feature type
  // if a texture feature exists
  if ( hasTextureFeature &&
    *( this->GetTextureFeature() ) != *( otherFeatureID.GetTextureFeature() ) )
    return false;

  // check our description
  const FeatureDescriptionType
    thisDescription  = this->         GetDescription(),
    otherDescription = otherFeatureID.GetDescription();

  if ( thisDescription.compare( otherDescription ) != 0 )
    return false;

  // check if we have parameters
  const bool hasParameters = this->HasParameters();
  if ( hasParameters && !otherFeatureID.HasParameters() )
    return false;

  // check our parameters
  const ParameterPointer
    thisParameters = boost::apply_visitor( GetParametersByVariant(), this->GetParameters() ),
	  otherParameters = boost::apply_visitor( GetParametersByVariant(), otherFeatureID.GetParameters() );

  if  ( !( thisParameters == otherParameters ) ) // because boost variant has defined == but not !=.
	  return false;
  if ( !thisParameters&& !otherParameters )
    return true; // if both parameters are NULL we are the same

  if  ( !thisParameters || !otherParameters ||!thisParameters->operator==( *otherParameters) ) 
	  return false; // if only one of the parameters are null or the parameters are not the same

  // if we've gotten this far, all is equal
  return true;

} // operator==
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
HashValueType FeatureID::ReHash() const
{

  // shortcuts
  using boost::hash_combine;

  // initialize our hasher
  const HashValueType seed = 0;
  HashValueType hashValue = seed;

  // hashing the texture feature type
  // but only if we actually have one!
  if ( this->HasTextureFeature() )
    hash_combine( hashValue, *( this->m_SecretData->m_TextureFeature ) );
  else
    hash_combine( hashValue, 0 );

  if ( this->HasParameters() )
    hash_combine( hashValue, this->m_SecretData->m_Parameters );
  else
    hash_combine( hashValue, 0 );

  // hash the description
  const FeatureDescriptionType thisDescription = this->GetDescription();
  hash_combine( hashValue, thisDescription );


  // output our final combined hashed value
  return hashValue;

} // ReHash
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
bool FeatureID::HasTextureFeature() const
{

  // http://bit.ly/11z3KXk
  return !!this->m_SecretData->m_TextureFeature;

} // HasTextureFeature
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
bool FeatureID::HasParameters() const
{

  // make parameters into a pointer so I can check for null
	return ( this->m_SecretData->m_Parameters.empty() );

} // HasParameters
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void FeatureID::SetTextureFeature( TextureFeatureEnum textureFeature )
{

  this->SetTextureFeature(
    TextureFeatureEnumPointer(
      new TextureFeatureEnum( textureFeature ) ) );

} // SetTextureFeature
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
std::string FeatureID::GetFeatureName() const
{

  return this->m_SecretData->m_Parser.GetTextureFeatureString( 
	  *this->GetTextureFeature().get() );

} // GetFeatureName
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void FeatureID::SetTextureFeature( const std::string & featureName )
{
 // get the Enum and call SetTextureFeature( const TextureFeatureEnumPointer )
  this->SetTextureFeature( 
    this->m_SecretData->m_Parser.GetTextureFeatureEnum( featureName ) );
}
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
// set/get functions
ccipdGetConstImplementMacro    ( FeatureID, TextureFeature, TextureFeatureEnumPointer   )
ccipdSetByvalImplementHashMacro( FeatureID, TextureFeature, TextureFeatureEnumPointer   )
ccipdGetConstImplementMacro    ( FeatureID, Description   , FeatureDescriptionType      )
ccipdSetByrefImplementHashMacro( FeatureID, Description   , FeatureDescriptionType      )
ccipdGetConstImplementMacro    ( FeatureID, Parameters    , AnyTextureFeatureParameters & )
ccipdSetByrefImplementHashMacro( FeatureID, Parameters    , AnyTextureFeatureParameters )
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void FeatureID::SecretData::SetDefaultParameter()
{
  switch( *this->m_TextureFeature )
  {
    case TextureFeatureGabor:
      this->m_Parameters = MapParametersType<TextureFeatureGabor>::AllParametersType::New();
      break;
	case TextureFeatureHaralick:
		this->m_Parameters = MapParametersType<TextureFeatureHaralick>::AllParametersType::New();
		break;

	case TextureFeatureHaralickEnergy:
		this->m_Parameters = MapParametersType<TextureFeatureHaralick>::AllParametersType::New();
		break;
	case TextureFeatureHaralickInertia:
		this->m_Parameters = MapParametersType<TextureFeatureHaralick>::AllParametersType::New();
		break;
	case TextureFeatureHaralickSumEntropy:
		this->m_Parameters = MapParametersType<TextureFeatureHaralick>::AllParametersType::New();
		break;
	case TextureFeatureHaralickDifferenceEntropy:
		this->m_Parameters = MapParametersType<TextureFeatureHaralick>::AllParametersType::New();
		break;
	case TextureFeatureHaralickIMC1:
		this->m_Parameters = MapParametersType<TextureFeatureHaralick>::AllParametersType::New();
		break;
	case TextureFeatureHaralickIMC2:
		this->m_Parameters = MapParametersType<TextureFeatureHaralick>::AllParametersType::New();
		break;

	case TextureFeatureCorrelation:
		this->m_Parameters = MapParametersType<TextureFeatureHaralick>::AllParametersType::New();
		break;



    case TextureFeatureGradientMagnitude:
		this->m_Parameters = MapParametersType< TextureFeatureGradientMagnitude >::AllParametersType::New();
        break;
    case TextureFeatureIntensity:
		this->m_Parameters = MapParametersType< TextureFeatureIntensity >::AllParametersType::New();
        break;
	  case TextureFeatureMean:
		this->m_Parameters = MapParametersType< TextureFeatureMean >::AllParametersType::New();
        break;
      case TextureFeatureMedian:
		this->m_Parameters = MapParametersType< TextureFeatureMedian >::AllParametersType::New();
        break;
      case TextureFeatureNakagami:
		this->m_Parameters = MapParametersType< TextureFeatureNakagami >::AllParametersType::New();
        break;
	  case TextureFeatureRange:
		this->m_Parameters = MapParametersType< TextureFeatureRange >::AllParametersType::New();
        break;
	  case TextureFeatureRayleigh:
		this->m_Parameters = MapParametersType< TextureFeatureRayleigh >::AllParametersType::New();
        break;
      case TextureFeatureSkew:
        this->m_Parameters = MapParametersType< TextureFeatureSkew >::AllParametersType::New();
        break;
	  case TextureFeatureSobel:
	    this->m_Parameters = MapParametersType< TextureFeatureSobel >::AllParametersType::New();
        break;
	  case TextureFeatureStandardDeviation:
		this->m_Parameters = MapParametersType< TextureFeatureStandardDeviation >::AllParametersType::New();
        break;
      default:
      {
          TextureFeatureParser m_Parser;
          std::string featureName = m_Parser.GetTextureFeatureString( *this->m_TextureFeature );
          (void) m_Parser;
          std::cerr << "Unable to determine parameter type for " <<  featureName << "." << "Please add your feature in ccipdTextureFeatureParametersByEnum.cxx" << std::endl;
      } // default
   } //switch

}
//////////////////////////////////////////////////////////////////////////////////////////
} // namespace



//////////////////////////////////////////////////////////////////////////////////////////
// Allows boost to register ccipd::MRITRUSParametersConcrete
#ifdef Boost_SERIALIZATION_FOUND
  BOOST_CLASS_VERSION( ccipd::FeatureIDConcrete, 2 )
  BOOST_CLASS_EXPORT_GUID( ccipd::FeatureID , "FeatureID" )
  BOOST_CLASS_EXPORT( ccipd::FeatureIDConcrete )
  ccipdBoostVirtualBaseFalse(
    ccipd::FeatureID,
    ccipd::FeatureIDConcrete )
#endif // Boost_SERIALIZATION_FOUND
//////////////////////////////////////////////////////////////////////////////////////////
