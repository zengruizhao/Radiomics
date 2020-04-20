


//////////////////////////////////////////////////////////////////////////////////////////

// ccipd includes
#include "ccipdMFALevelsetExtractor.h"
#include "ccipdFeatureID.h"
#include "ccipdStudy.h"
#include "ccipdLevelsets.h"
#include "ccipdMaskToLevelset.h"
#include "ccipdMFAFeaturesFromLevelsets.h"
#include "ccipdFactory.hxx"

// boost includes
#include <boost/foreach.hpp>

// serialization includes
#ifdef Boost_SERIALIZATION_FOUND
  #include <boost/archive/xml_iarchive.hpp>
  #include <boost/archive/xml_oarchive.hpp>
  #include <boost/serialization/version.hpp>
  #include <boost/serialization/set.hpp>
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
class Factory< MFALevelsetExtractor >;
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
class MFALevelsetExtractor::SecretData
{

public:
  SecretData(); ///< Constructor

  /**
   * We will store the
   * levelset object
   * in the description
   * of the feature
   * id. However, to prevent
   * others from inadvertently
   * reusing the description
   * name, we will prepend
   * a random prefix.
   */
  static const MaskObjectType
    m_DescriptionPrefix;

  MaskObjectCollectionType
    m_WhichLevelsetObjects;
  
  float
    m_SigmoidWidth,
    m_SmoothingVariance;
  bool
    m_SigmoidLevelset,
    m_NormalizeLevelset;

}; // class SecretData
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
const MaskObjectType
  MFALevelsetExtractor::SecretData::m_DescriptionPrefix = "Levelset_";
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
MFALevelsetExtractor::MFALevelsetExtractor() :
ccipdNewPIMPLMacro
{
} // Constructor
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
MFALevelsetExtractor::SecretData::SecretData() :
m_SigmoidWidth     ( 4.0    ),
m_SmoothingVariance( 0.0    ),
m_SigmoidLevelset  ( false  ),
m_NormalizeLevelset( false  )
{
} // Constructor
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
MFALevelsetExtractor::~MFALevelsetExtractor()
{
} // Destructor
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
FeatureIDConstCollection MFALevelsetExtractor::GetWhichFeatures() const
{

  return GetFeaturesFromLevelsetObjects(
    this->m_SecretData->m_WhichLevelsetObjects );

} // GetWhichFeatures
//////////////////////////////////////////////////////////////////////////////////////////




//////////////////////////////////////////////////////////////////////////////////////////
MaskObjectType MFALevelsetExtractor::ExtractObjectNameFromFeatureID(
    const FeatureID & whichFeature )
{

  // get the description
  const MaskObjectType
    & prefix    = SecretData::m_DescriptionPrefix,
    description = whichFeature.GetDescription();

  const auto
    prefixLength      = prefix.length(),
    descriptionLength = description.length();

  // if we are not long enough, we can't extract the subset
  if ( descriptionLength <= prefixLength )
    return MaskObjectType();

  // check if the first few characters are the prefix
  if ( description.find( prefix ) != 0 )
    return MaskObjectType();

  // okay the remaining portion of the string
  // will contain our object name
  const MaskObjectType
    objectName = description.substr( prefixLength );

  // output
  return objectName;

} // ExtractObjectNameFromFeatureID
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
FeatureIDConstPointer MFALevelsetExtractor::CreateFeatureIDWithObject(
  const MaskObjectType & objectName )
{

  // concatenate the prefix and the object name
  const MaskObjectType
    & prefix = SecretData::m_DescriptionPrefix,
    concatenated = prefix + objectName;

  // create a feature id with that description
  return FeatureIDConstPointer( FeatureID::New( concatenated ) );

} // CreateFeatureIDWithObject
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
VolumeImageConstPointer MFALevelsetExtractor::ExtractFeature(
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
  
  // Extract the object name from the feature.
  // We stored the mask object inside
  // the description.
  const MaskObjectType object =
    this->ExtractObjectNameFromFeatureID( whichFeature );

  // we extract the mask from the study
  const VolumeMaskImageConstPointer mask = study->GetMask( object );

  if ( verbose )
    cout << "Mask object = " << object << endl;

  if ( !mask )
    {
    if ( verbose )
      cerr << "Error: No mask for object " << object << "." << endl;
    return VolumeImageConstPointer();
    }

  // calculate the levelset
  if ( verbose ) cout << "Calculating levelset for " << object << ":" << endl;
  const VolumeImageConstPointer levelset(
    MaskToLevelset< InternalPixelType >(
      mask.GetPointer(),
      this->m_SecretData->m_NormalizeLevelset,
      this->m_SecretData->m_SigmoidLevelset,
      this->m_SecretData->m_SigmoidWidth,
      this->m_SecretData->m_SmoothingVariance,
	  /*ResizeIsitropically*/false) );
  if ( verbose ) cout << "Calculating levelset for " << object << " done." << endl;

  // output
  return levelset;

} // ExtractFeature
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
VolumeImageConstPointer MFALevelsetExtractor::PostprocessFeature(
  const VolumeImageConstPointer & inputLevelset ) const
{

  const bool verbose = this->GetVerbose();

  if ( verbose ) cout << "Calling superclass's post-processing feature:" << endl;
  const VolumeImageConstPointer currentLevelset =
    this->Superclass::PostprocessFeature( inputLevelset );
  if ( verbose ) cout << "Calling superclass's post-processing feature done." << endl;

  if ( !inputLevelset )
    return currentLevelset;

#ifdef NDEBUG

  // we recompute it once it's been transformed
  const float threshold = 0; // surface is at 0

  if ( verbose ) cout << "Updating levelset:" << endl;
  const VolumeImageConstPointer updatedLevelset(
    UpdateLevelset( currentLevelset, threshold,
      this->m_SecretData->m_NormalizeLevelset,
      this->m_SecretData->m_SigmoidLevelset,
      this->m_SecretData->m_SigmoidWidth,
      this->m_SecretData->m_SmoothingVariance ) );
  if ( verbose ) cout << "Updating levelset done." << endl;

  return updatedLevelset;

#else

  // in debug mode this takes too long
  return currentLevelset;

#endif // NDEBUG

} // PostprocessFeature
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
ccipdSetByvalImplementMacro( MFALevelsetExtractor, SigmoidWidth     , float )
ccipdGetConstImplementMacro( MFALevelsetExtractor, SigmoidWidth     , float )
ccipdSetByvalImplementMacro( MFALevelsetExtractor, SmoothingVariance, float )
ccipdGetConstImplementMacro( MFALevelsetExtractor, SmoothingVariance, float )
ccipdSetByvalImplementMacro( MFALevelsetExtractor, SigmoidLevelset  , bool  )
ccipdGetConstImplementMacro( MFALevelsetExtractor, SigmoidLevelset  , bool  )
ccipdSetByvalImplementMacro( MFALevelsetExtractor, NormalizeLevelset, bool  )
ccipdGetConstImplementMacro( MFALevelsetExtractor, NormalizeLevelset, bool  )

ccipdSetByrefImplementMacro( MFALevelsetExtractor,
  WhichLevelsetObjects, MaskObjectCollectionType )
ccipdGetConstImplementMacro( MFALevelsetExtractor,
  WhichLevelsetObjects, MaskObjectCollectionType )
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace ccipd



//////////////////////////////////////////////////////////////////////////////////////////
#ifdef Boost_SERIALIZATION_FOUND

namespace ccipd
{

ccipdSerializationImplementationHeadingMacro( MFALevelsetExtractor )
{

  using boost::serialization::make_nvp;

  if ( version < 1 )
    return;

  archive
    & BOOST_SERIALIZATION_BASE_OBJECT_NVP( MFAFeatureExtractor )
    & make_nvp( "WhichLevelsetObjects", this->m_SecretData->m_WhichLevelsetObjects )
    & make_nvp( "SigmoidWidth"        , this->m_SecretData->m_SigmoidWidth        )
    & make_nvp( "SmoothingVariance"   , this->m_SecretData->m_SmoothingVariance   )
    & make_nvp( "SigmoidLevelset"     , this->m_SecretData->m_SigmoidLevelset     )
    & make_nvp( "NormalizeLevelset"   , this->m_SecretData->m_NormalizeLevelset   );

} // serialize

ccipdSerializationXMLMacro( MFALevelsetExtractor )

} // namespace

#endif // Boost_SERIALIZATION_FOUND
//////////////////////////////////////////////////////////////////////////////////////////
