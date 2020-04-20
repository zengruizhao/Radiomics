//////////////////////////////////////////////////////////////////////////////////////////
// ccipd includes
#include "ccipdTextureFeatureHaralickParameters.h"
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
	ccipdFactoryImplementationMacro( TextureFeatureHaralickParameters );

	template class Serializable< TextureFeatureHaralickParameters >;
	//////////////////////////////////////////////////////////////////////////////////////////



	//////////////////////////////////////////////////////////////////////////////////////////
	class TextureFeatureHaralickParameters::SecretData : public Hashable
	{
	public: 

		/* Constructor */
		SecretData();

		/* Variables/Members*/
		VolRadiusType m_Radius               ;
		VolOffsetType m_Offset               ;
		unsigned int  m_NumberOfHistogramBins;

		/* Functions */
		virtual bool operator==( const Hashable & other ) const;
		virtual size_t GetHash() const;
		virtual size_t ReHash () const;

	}; // TextureFeatureHaralickParameters::SecretData
	//////////////////////////////////////////////////////////////////////////////////////////



	//////////////////////////////////////////////////////////////////////////////////////////
	TextureFeatureHaralickParameters::TextureFeatureHaralickParameters(): ccipdNewPIMPLMacro
	{
		this->SetNumberOfHistogramBins(16); // Number of Histogram Bins per axis is initialized
		
		VolRadiusType radius = {{1, 1, 1}};
		this->SetRadius(radius);	   
		
		VolOffsetType offset = {{0, 1, 0}};
		this->SetOffset(offset);
	}

	TextureFeatureHaralickParameters::Pointer 
	TextureFeatureHaralickParameters::New(  const VolRadiusType Radius
		                                  , const VolOffsetType Offset
		                                  , const unsigned int NumberOfHistogramBins
		                                 )
	{
		const Pointer newObject = Self::New();
		newObject->SetRadius(Radius);
		newObject->SetOffset(Offset);
		newObject->SetNumberOfHistogramBins(NumberOfHistogramBins);
		return newObject;
	} // Constructor

	TextureFeatureHaralickParameters::Pointer 
	TextureFeatureHaralickParameters::New(  const unsigned int RadiusX, const unsigned int RadiusY, const unsigned int RadiusZ
                                          , const unsigned int OffsetX, const unsigned int OffsetY, const unsigned int OffsetZ
                                          , const unsigned int NumberOfHistogramBins
                                         )
  {
    const Pointer newObject = Self::New();
    VolRadiusType radius = {{RadiusX, RadiusY, RadiusZ}};
    VolOffsetType offset = {{OffsetX, OffsetY, OffsetZ}};
    newObject->SetRadius(radius);
		newObject->SetOffset(offset);
		newObject->SetNumberOfHistogramBins(NumberOfHistogramBins);
		return newObject;
  }

	TextureFeatureHaralickParameters::SecretData::SecretData(): m_NumberOfHistogramBins(16)
	{

	} // Constructor

	TextureFeatureHaralickParameters::~TextureFeatureHaralickParameters()
	{
	} //Destructor
	//////////////////////////////////////////////////////////////////////////////////////////



	//////////////////////////////////////////////////////////////////////////////////////////
	bool TextureFeatureHaralickParameters::operator==(const Hashable & other) const
	{
		// cast and confirm
		const TextureFeatureHaralickParameters & otherModel = dynamic_cast< const TextureFeatureHaralickParameters & >( other );
		if ( &otherModel == this ) return true;

		// check our variables
		return *( this->m_SecretData ) == *( otherModel.m_SecretData);
	} // operator==
	//////////////////////////////////////////////////////////////////////////////////////////



	//////////////////////////////////////////////////////////////////////////////////////////
	HashValueType TextureFeatureHaralickParameters::ReHash() const
	{
		return m_SecretData->GetHash();
	} // Rehash()
	//////////////////////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////
	void TextureFeatureHaralickParameters::SetRadius(const VolRadiusType value )
	{
		this->m_SecretData->m_Radius = value; 
		this->ClearHash(); 
	} // SetRadius()
	//////////////////////////////////////////////////////////////////////////////////////////
	void TextureFeatureHaralickParameters::SetOffset(const VolOffsetType value )
	{
		this->m_SecretData->m_Offset = value; 
		this->ClearHash(); 
	} // SetOffset()
	//////////////////////////////////////////////////////////////////////////////////////////
	void TextureFeatureHaralickParameters::SetNumberOfHistogramBins(const unsigned int value )
	{
		this->m_SecretData->m_NumberOfHistogramBins = value; 
		this->ClearHash(); 
	} // SetNumberOfHistogramBins()

	//////////////////////////////////////////////////////////////////////////////////////////
	// implementation of all the getter functions
	ccipdGetConstImplementMacro(TextureFeatureHaralickParameters, Radius               , VolRadiusType)
	ccipdGetConstImplementMacro(TextureFeatureHaralickParameters, Offset               , VolOffsetType) 
	ccipdGetConstImplementMacro(TextureFeatureHaralickParameters, NumberOfHistogramBins, unsigned int )
		//////////////////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////////////
	bool TextureFeatureHaralickParameters::SecretData::operator==( const Hashable & other ) const
	{
		// if cast is the same object return true
		const SecretData & otherModel = dynamic_cast< const SecretData & >( other );
		if ( &otherModel == this )
			return true;

		// check if the parameters are the same
		bool returnValue =  m_Radius                == otherModel.m_Radius                &&
			                m_Offset                == otherModel.m_Offset                &&
			                m_NumberOfHistogramBins == otherModel.m_NumberOfHistogramBins &&
			                this->GetHash()         == otherModel.GetHash();
		return returnValue;

	} // SecretData::operator==
	////////////////////////////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////////////////////////////////
	HashValueType TextureFeatureHaralickParameters::SecretData::GetHash() const
	{
		// never cache
		return this->ReHash();
	} // SecretData::GetHash
	////////////////////////////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////////////////////////////////
	HashValueType TextureFeatureHaralickParameters::SecretData::ReHash() const
	{
		// initialize our hasher
		HashValueType hashValue = 0/*seed*/;

		// hash all simple variables of the object
		boost::hash_combine(hashValue, m_Radius[0]);
		boost::hash_combine(hashValue, m_Radius[1]);
		boost::hash_combine(hashValue, m_Radius[2]);
		boost::hash_combine(hashValue, m_Offset[0]);
		boost::hash_combine(hashValue, m_Offset[1]);
		boost::hash_combine(hashValue, m_Offset[2]);
		boost::hash_combine(hashValue, m_NumberOfHistogramBins);

		return hashValue;
	} // SecretData::ReHash
	////////////////////////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////////////////////
	// Implemented directly to avoid ambiguous inheritance
	bool TextureFeatureHaralickParameters::Load( const std::string & fileName)
	{ 
		return this->Serializable< Self >::Load(fileName);
	} // Load

	bool TextureFeatureHaralickParameters::Save( const std::string & fileName) const
	{
		return this->Serializable< Self >::Save(fileName); 
	} // Save
	//////////////////////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////////////////////
#ifdef Boost_SERIALIZATION_FOUND

	ccipdSerializationImplementationHeadingMacro( TextureFeatureHaralickParameters )
	{
		boost::serialization::void_cast_register( static_cast< ConcreteFactorySelf< Self > * >( NULL ), static_cast< Self * >( NULL ) );


		if ( version < 1)
			return;
		archive
			& boost::serialization::make_nvp ( "RadiusX" , this->m_SecretData->m_Radius[0])
			& boost::serialization::make_nvp ( "RadiusY" , this->m_SecretData->m_Radius[1])
			& boost::serialization::make_nvp ( "RadiusZ" , this->m_SecretData->m_Radius[2])
			& boost::serialization::make_nvp ( "OffsetX" , this->m_SecretData->m_Offset[0])
			& boost::serialization::make_nvp ( "OffsetY" , this->m_SecretData->m_Offset[1])
			& boost::serialization::make_nvp ( "OffsetZ" , this->m_SecretData->m_Offset[2])
			& boost::serialization::make_nvp ( "NumberOfHistogramBins" , this->m_SecretData->m_NumberOfHistogramBins)
			& BOOST_SERIALIZATION_BASE_OBJECT_NVP( ParameterBase );

	} // serialize

	ccipdSerializationXMLMacro( TextureFeatureHaralickParameters )

	typedef ConcreteFactorySelf< TextureFeatureHaralickParameters > TextureFeatureHaralickParametersConcrete;

#endif // Boost_SERIALIZATION_FOUND
	//////////////////////////////////////////////////////////////////////////////////////////


} //namespace



#ifdef Boost_SERIALIZATION_FOUND
    BOOST_CLASS_VERSION(ccipd::TextureFeatureHaralickParametersConcrete, 1 )
	BOOST_CLASS_EXPORT_GUID(ccipd::TextureFeatureHaralickParameters , "TextureFeatureHaralickParameters" )
	BOOST_CLASS_EXPORT(ccipd::TextureFeatureHaralickParametersConcrete )
	ccipdBoostVirtualBaseFalse(ccipd::TextureFeatureHaralickParameters, ccipd::TextureFeatureHaralickParametersConcrete)
#endif //Boost_SERIALIZATION_FOUND
