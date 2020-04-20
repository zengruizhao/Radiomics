


//////////////////////////////////////////////////////////////////////////////////////////

// ccipd includes
#include "ccipdMFATextureExtractor.h"
#include "ccipdFeatureCollection.h"
#include "ccipdFeatureID.h"
#include "ccipdStudy.h"
#include "ccipdPreprocessMRI.h" // prior to extracint features
#include "ccipdFactory.hxx"

// boost includes
#include <boost/foreach.hpp>

// serialization includes
#ifdef Boost_SERIALIZATION_FOUND
  #include <boost/archive/xml_iarchive.hpp>
  #include <boost/archive/xml_oarchive.hpp>
  #include <boost/serialization/version.hpp>
  #include <boost/serialization/vector.hpp>
  #include "ccipdSharedPointerSerialization.hxx"
#endif // Boost_SERIALIZATION_FOUND

//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
using std::cout;
using std::cerr;
using std::endl;
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////
template
class Factory< MFATextureExtractor >;
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
class MFATextureExtractor::SecretData
{

public:
  SecretData(); ///< Constructor

  TextureFeatureEnumCollection
    m_WhichFeatureObjects;
  
}; // class SecretData
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
MFATextureExtractor::MFATextureExtractor() :
ccipdNewPIMPLMacro
{

  // start with just intensities
  this->AddTextureFeature( TextureFeatureIntensity );

} // Constructor
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
MFATextureExtractor::SecretData::SecretData()
{
} // Constructor
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
MFATextureExtractor::~MFATextureExtractor()
{
} // Destructor
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void MFATextureExtractor::ClearWhichFeatures()
{
  this->m_SecretData->m_WhichFeatureObjects.clear();
} // ClearWhichFeatures
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void MFATextureExtractor::AddTextureFeature(
  const TextureFeatureEnum whichTextureFeature )
{
  this->m_SecretData->m_WhichFeatureObjects.push_back( whichTextureFeature );
} // AddTextureFeature
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
FeatureIDConstCollection MFATextureExtractor::GetWhichFeatures() const
{

  FeatureIDConstCollection whichFeatures;

  // loop through all our texture features
  BOOST_FOREACH( const TextureFeatureEnum whichTextureFeature,
    this->m_SecretData->m_WhichFeatureObjects )
    {

    // create a new feature id with this texture feature
    const FeatureIDConstPointer whichFeature(
      FeatureID::New( whichTextureFeature ) );

    whichFeatures.push_back( whichFeature );

    } // for each texture feature

  // output
  return whichFeatures;

} // GetWhichFeatures
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void MFATextureExtractor::PreprocessFeature(
  const FeatureID & whichFeature ) const
{

  this->Superclass::PreprocessFeature( whichFeature );

  const bool verbose = this->GetVerbose();

  // extract the study
  const StudyConstPointer study = this->GetStudy();
  if ( !study )
    {
    if ( verbose )
      cerr << "Error: No study." << endl;
    return;
    }

  // extract the feature collection which will be used
  // as a cache
  FeatureCollectionPointer cachedFeatures = study->GetFeatureCollection();
  
  // see if the preprocessed version is already cached
  const FeatureIDPointer preprocessingFeatureID(
    FeatureID::New( FeatureDescriptionType( "Preprocessed" ) ) );
  /*
  const bool alreadyPreprocessed =
    cachedFeatures->HasFeature( preprocessingFeatureID );
  */

  // we will use the study xml file
  // to do any preprocessing
  // at load-time, instead of at
  // processing time.
  VolumeImageConstPointer preprocessed =
    study->GetVolume();

  /*
  if ( alreadyPreprocessed )
    {
    // use the cache
    if ( verbose ) cout << "Using cached preprocessed image." << endl;
    
    preprocessed = cachedFeatures->GetFeature(
      preprocessingFeatureID ).GetPointer();
    }
  else
    {

    // preprocess our volume
    if ( verbose ) cout << "Preprocessing volume:" << endl;

    preprocessed = PreprocessMRI( study->GetVolume() ).GetPointer();

    if ( verbose ) cout << "Preprocessing volume done." << endl;
    
    // store it in the cache
    cachedFeatures->SetFeature(
      preprocessingFeatureID,
      preprocessed );
    
    } // alreadyPreprocessed
  */

  // input the preprocessed volume into a feature collection
  cachedFeatures->SetVerbose( verbose      );
  cachedFeatures->SetVolume ( preprocessed );

} // PreprocessFeature
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
VolumeImageConstPointer MFATextureExtractor::ExtractFeature(
  const FeatureID & whichFeature ) const
{

  const bool verbose = this->GetVerbose();

  // extract the study
  const StudyConstPointer study = this->GetStudy();
  if ( !study )
    {
    if ( verbose ) cerr << "Error: No study." << endl;
    return VolumeImageConstPointer();
    }

  // this is straightforward because we've
  // already set up the feature cache
  // in the PreprocessFeature method
  // so that the GetVolume of the
  // cache will actually contain the
  // preprocessed volume
  if ( verbose ) cout << "Extracting texture:" << endl;
  const VolumeImageConstPointer texture =
    study->GetFeatureCollection()->GetFeature(
      whichFeature.shared_from_this() ).at(0); // we can create a shared pointer from our input
  if ( verbose ) cout << "Extracting texture done." << endl;

  return texture;

} // ExtractFeature
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
ccipdGetConstImplementMacro( MFATextureExtractor,
  WhichFeatureObjects, TextureFeatureEnumCollection )
ccipdSetByrefImplementMacro( MFATextureExtractor,
  WhichFeatureObjects, TextureFeatureEnumCollection )
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace ccipd



//////////////////////////////////////////////////////////////////////////////////////////
#ifdef Boost_SERIALIZATION_FOUND

namespace ccipd
{

ccipdSerializationImplementationHeadingMacro( MFATextureExtractor )
{

  using boost::serialization::make_nvp;

  if ( version < 1 )
    return;

  archive
    & BOOST_SERIALIZATION_BASE_OBJECT_NVP( SIMFeatureExtractor );
    
  if ( version < 2 )
    {
    archive
      & make_nvp( "WhichTextureFeatures", this->m_SecretData->m_WhichFeatureObjects );
    }
  else // version < 2
    {
    // get the texture features
    const FeatureIDConstCollection constWhichFeatures = this->GetWhichFeatures();
    
    // convert to non-const
    FeatureIDCollection unconstWhichFeatures;
    BOOST_FOREACH( const FeatureIDConstPointer constWhichFeature, constWhichFeatures )
      {
      const FeatureIDPointer unconstWhichFeature =
        std::const_pointer_cast< FeatureID >( constWhichFeature );
      
      unconstWhichFeatures.push_back( unconstWhichFeature );
      } // for each feature
      
    archive
      & make_nvp( "WhichTextureFeatures", unconstWhichFeatures );

    // go through and set the enums now
    TextureFeatureEnumCollection
      & whichTextures = this->m_SecretData->m_WhichFeatureObjects;
    
    // clear any old textures
    whichTextures.clear();
    
    BOOST_FOREACH( const FeatureIDConstPointer whichFeature, unconstWhichFeatures )
      {
      // insert the textures into our collection
      if ( whichFeature->HasTextureFeature() )
        whichTextures.push_back( *whichFeature->GetTextureFeature() );
      } // for each feature
    
    } // version < 2
  
} // serialize

ccipdSerializationXMLMacro( MFATextureExtractor )

} // namespace

#endif // Boost_SERIALIZATION_FOUND
//////////////////////////////////////////////////////////////////////////////////////////
