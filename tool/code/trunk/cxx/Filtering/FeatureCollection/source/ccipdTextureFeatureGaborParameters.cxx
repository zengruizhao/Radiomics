//////////////////////////////////////////////////////////////////////////////////////////
// ccipd includes
#include "ccipdTextureFeatureGaborParameters.h"
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
	ccipdFactoryImplementationMacro( TextureFeatureGaborParameters );

	template class Serializable< TextureFeatureGaborParameters >;
	//////////////////////////////////////////////////////////////////////////////////////////



	//////////////////////////////////////////////////////////////////////////////////////////
	class TextureFeatureGaborParameters::SecretData : public Hashable
	{
	public:

		/* Constructor */
		SecretData();

		/* Variables */
		float            m_Direction,
			m_Frequency;

		/* Functions */
		virtual bool operator==( const Hashable & other ) const;
		virtual size_t GetHash() const;
		virtual size_t ReHash () const;

	}; // TextureFeatureGaborParameters::SecretData
	//////////////////////////////////////////////////////////////////////////////////////////



	//////////////////////////////////////////////////////////////////////////////////////////
	TextureFeatureGaborParameters::TextureFeatureGaborParameters() : ccipdNewPIMPLMacro
	{
		this->SetFrequency(0.05); // frequency is initialized
	}

	TextureFeatureGaborParameters::Pointer
		TextureFeatureGaborParameters::New(const float frequency, const float direction)
	{
		const Pointer newObject = Self::New();
		newObject->SetFrequency(frequency);
		newObject->SetDirection(direction);
		return newObject;
	} // Constructor



	TextureFeatureGaborParameters::SecretData::SecretData():
		m_Direction ( 0 )
	{
	} // Constructor

	TextureFeatureGaborParameters::~TextureFeatureGaborParameters()
	{
	} //Destructor
	//////////////////////////////////////////////////////////////////////////////////////////



	//////////////////////////////////////////////////////////////////////////////////////////
	bool TextureFeatureGaborParameters::operator==(const Hashable & other) const
	{
		// cast and confirm
		const TextureFeatureGaborParameters & otherModel = dynamic_cast< const TextureFeatureGaborParameters & >( other );
		if ( &otherModel == this )
			return true;

		// check our variables
		return *( this->m_SecretData ) == *( otherModel.m_SecretData);

	} // operator==
	//////////////////////////////////////////////////////////////////////////////////////////



	//////////////////////////////////////////////////////////////////////////////////////////
	HashValueType TextureFeatureGaborParameters::ReHash() const
	{

		return m_SecretData->GetHash();

	} // Rehash()
	//////////////////////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////
	void TextureFeatureGaborParameters::SetFrequency(const float value )
	{
		this->m_SecretData->m_Frequency = value; 
		this->ClearHash(); 
	} // SetFequency()
	//////////////////////////////////////////////////////////////////////////////////////////
	void TextureFeatureGaborParameters::SetDirection(const float value )
	{
		this->m_SecretData->m_Direction = value; 
		this->ClearHash(); 
	} // SetDirection()


	//////////////////////////////////////////////////////////////////////////////////////////
	// implementation of all the getter functions
	ccipdGetConstImplementMacro(TextureFeatureGaborParameters, Frequency, float)
		ccipdGetConstImplementMacro(TextureFeatureGaborParameters, Direction, float)
		//////////////////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////////////
		bool TextureFeatureGaborParameters::SecretData::operator==( const Hashable & other ) const
	{

		// if cast is the same object return true
		const SecretData & otherModel = dynamic_cast< const SecretData & >( other );
		if ( &otherModel == this )
			return true;

		// check if the parameters are the same
		bool returnValue = m_Frequency     == otherModel.m_Frequency && 
			m_Direction     == otherModel.m_Direction &&
			this->GetHash() == otherModel.GetHash();

		return returnValue;

	} // SecretData::operator==
	//////////////////////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////////////////////
	HashValueType TextureFeatureGaborParameters::SecretData::GetHash() const
	{
		// never cache
		return this->ReHash();
	} // SecretData::GetHash
	//////////////////////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////////////////////
	HashValueType TextureFeatureGaborParameters::SecretData::ReHash() const
	{
		// initialize our hasher
		const HashValueType seed = 0;
		HashValueType hashValue = seed;

		// hash all simple variables of the object
		boost::hash_combine(hashValue, m_Frequency);
		boost::hash_combine(hashValue, m_Direction);

		// output
		return hashValue;

	} // SecretData::ReHash
	//////////////////////////////////////////////////////////////////////////////////////////



	//////////////////////////////////////////////////////////////////////////////////////////
	// Implemented directly to avoid ambiguous inheritance
	bool TextureFeatureGaborParameters::Load( const std::string & fileName)
	{ 
		return this->Serializable< Self >::Load(fileName);
	} // Load

	bool TextureFeatureGaborParameters::Save( const std::string & fileName) const
	{
		return this->Serializable< Self >::Save(fileName); 
	} // Save
	//////////////////////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////////////////////
#ifdef Boost_SERIALIZATION_FOUND

	ccipdSerializationImplementationHeadingMacro( TextureFeatureGaborParameters )
	{

		boost::serialization::void_cast_register( static_cast< ConcreteFactorySelf< Self > * >( NULL ), static_cast< Self * >( NULL ) );


		if ( version < 1)
			return;
		archive
			& boost::serialization::make_nvp ( "Frequency" , this->m_SecretData->m_Frequency)
			& boost::serialization::make_nvp ( "Direction" , this->m_SecretData->m_Direction)
			& BOOST_SERIALIZATION_BASE_OBJECT_NVP( ParameterBase );

	} // serialize

	ccipdSerializationXMLMacro( TextureFeatureGaborParameters )

		typedef ConcreteFactorySelf< TextureFeatureGaborParameters > TextureFeatureGaborParametersConcrete;
#endif // Boost_SERIALIZATION_FOUND
	//////////////////////////////////////////////////////////////////////////////////////////


} //namespace



#ifdef Boost_SERIALIZATION_FOUND
BOOST_CLASS_VERSION(ccipd::TextureFeatureGaborParametersConcrete, 1 )
	BOOST_CLASS_EXPORT_GUID(ccipd::TextureFeatureGaborParameters , "TextureFeatureGaborParameters" )
	BOOST_CLASS_EXPORT(ccipd::TextureFeatureGaborParametersConcrete )
	ccipdBoostVirtualBaseFalse(ccipd::TextureFeatureGaborParameters, ccipd::TextureFeatureGaborParametersConcrete)
#endif //Boost_SERIALIZATION_FOUND

