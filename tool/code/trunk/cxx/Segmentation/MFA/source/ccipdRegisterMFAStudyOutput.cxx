


//////////////////////////////////////////////////////////////////////////////////////////

// ccipd includes
#include "ccipdRegisterMFAStudyOutput.h"
#include "ccipdFeatureID.h"
#include "ccipdFeatureCollection.h"
#include "ccipdSIMStorage.h"
#include "ccipdExtractFeatureImage.h"
#include "ccipdStudy.h"
#include "ccipdMFASegmenter.h"
#include "ccipdTextureFeatureParser.h"
#include "ccipdCloneableFactory.hxx"

// serialization includes
#ifdef Boost_SERIALIZATION_FOUND
  #include <boost/archive/xml_iarchive.hpp>
  #include <boost/archive/xml_oarchive.hpp>
  #include <boost/serialization/export.hpp>
#endif // Boost_SERIALIZATION_FOUND

// boost includes
#include <boost/foreach.hpp>
#include <itkVectorImage.h>

//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
using std::cerr;
using std::cout;
using std::endl;
using std::string;
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////
template
class Copyable< RegisterMFAStudyOutput >;

template
class Cloneable< RegisterMFAStudyOutput >;

template
class CloneableFactory< RegisterMFAStudyOutput >;

template
class Factory< RegisterMFAStudyOutput >;
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
class RegisterMFAStudyOutput::SecretData
{
public:

  SecretData(); ///< Constructor
  
  bool
    m_ReconstructAllFeatures,
    m_UpdateMovedStudy,
    m_CropMovedOutput;

  MFAVolumeBaseConstPointer
    m_TrainedMFA;

}; // class SecretData
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
RegisterMFAStudyOutput::RegisterMFAStudyOutput() :
ccipdNewPIMPLMacro
{
} // Constructor
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
RegisterMFAStudyOutput::SecretData::SecretData() :
m_ReconstructAllFeatures( true ),
m_UpdateMovedStudy      ( true )
{
} // Constructor
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
RegisterMFAStudyOutput::~RegisterMFAStudyOutput()
{
} // Constructor
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void RegisterMFAStudyOutput::UpdateMovedStudy( const MFASegmenter & segmenter )
{

  const bool verbose = this->GetVerbose();

  // extract the trained MFA
  const MFAVolumeBaseConstPointer trainedMFA = this->GetTrainedMFA();
  if ( !trainedMFA )
    {
    if ( verbose ) cout << "Error: No trained MFA." << endl;
    return;
    } // trainedMFA

  // extract the moved study
  this->Superclass::FinalizeRegistration();
  const StudyPointer movedStudy = this->GetMovedStudy();

  if ( !movedStudy )
    {
    if ( verbose ) cout << "Error: No moved study." << endl;
    return;
    } // movedStudy

  // extract the feature collection
  const FeatureCollectionPointer movedFeatures = movedStudy->GetFeatureCollection();
  if ( !movedFeatures )
    {
    if ( verbose ) cout << "Error: No moved features." << endl;
    return;
    } // movedFeatures

  // set the moved volume to the study
  movedFeatures->SetFeature(
    FeatureIDConstPointer( FeatureID::New(
      TextureFeatureIntensity, FeatureDescriptionType( "Moved Volume" ) )),
    movedStudy->GetVolume() );

  if ( verbose && !movedStudy->HasVolume() )
    {
    cout << "Warning: No moved volume!" << endl;
    }
    
  // do nothing if we don't want to
  const bool reconstructAllFeatures = 
    this->m_SecretData->m_ReconstructAllFeatures;

  // loop through each existing feature
  const FeatureIDConstCollection whichFeatures =
    trainedMFA->GetFeatureIDs();
  BOOST_FOREACH( const FeatureIDConstPointer & whichFeature, whichFeatures )
    {

    if ( !whichFeature ) continue;

    // extract the mean
    const HashValueType featureHash = whichFeature->GetHash();

    if ( verbose )
      cout << "Extracting mean for feature " <<
        featureHash << ":" << endl;

    const unsigned int meanIndex = 0;
    const VolumeImageConstPointer meanImage( ExtractFeatureImage(
        trainedMFA->GetMeanAndEigenvectors( whichFeature ), meanIndex ) );

    if ( verbose ) cout << "Extracting mean done." << endl;

    // get a string for this feature
    const string
      existingDescription = whichFeature->GetDescription(),
      textureString = whichFeature->HasTextureFeature() ?
        TextureFeatureParser::GetTextureFeatureString(
          *whichFeature->GetTextureFeature() ) : "",
      meanString = string( "Mean " ) +
        existingDescription + " " + textureString;

    // give the mean to the feature collection
    const FeatureIDPointer meanFeatureID( FeatureID::New( *whichFeature ) );
    meanFeatureID->SetDescription( meanString );
    movedFeatures->SetFeature( meanFeatureID, meanImage );

    // optionally recosntruct the featuers
    if ( reconstructAllFeatures )
      {

      // extract the images
      if ( verbose )
        cout << "Calculated reconstruction for feature " <<
          featureHash << ":" << endl;
      const VolumeImageConstPointer reconstructedImage(
          segmenter.GetReconstruction( whichFeature ) );
      if ( verbose ) cout << "Calculated reconstruction done." << endl;

      if ( verbose )
        cout << "Calculated transformed reconstruction for feature" <<
          featureHash << ":" << endl;
      const VolumeImageConstPointer transformedReconstructedImage(
          segmenter.GetTransformedReconstruction( whichFeature ) );
      if ( verbose ) cout << "Calculated transformed reconstruction done." << endl;

      // create some feature id's
      const FeatureIDPointer
        reconstructedFeatureID           ( FeatureID::New( *whichFeature ) ),
        transformedReconstructedFeatureID( FeatureID::New( *whichFeature ) );

      const string
        transformedString = string( "Transformed Reconstructed " ) +
          existingDescription + " " + textureString,
        reconstructedString = string( "Reconstructed " ) +
          existingDescription + " " + textureString;

      reconstructedFeatureID           ->SetDescription( reconstructedString );
      transformedReconstructedFeatureID->SetDescription( transformedString   );

      // give the features to the collection
      movedFeatures->SetFeature(
        reconstructedFeatureID,
        reconstructedImage );
      movedFeatures->SetFeature(
        transformedReconstructedFeatureID,
        transformedReconstructedImage );

      } // reconstructAllFeatures

    } // for each feature

} // UpdateMovedStudy
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void RegisterMFAStudyOutput::CopyParametersInternal( const Self & other )
{
  this->SetReconstructAllFeatures( other.m_SecretData->m_ReconstructAllFeatures );
} // CopyParametersInternal
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
ccipdSetByvalImplementMacro( RegisterMFAStudyOutput, ReconstructAllFeatures, bool )
ccipdSetByvalImplementMacro( RegisterMFAStudyOutput, CropMovedOutput,        bool )
ccipdSetByrefImplementMacro( RegisterMFAStudyOutput, TrainedMFA, MFAVolumeBaseConstPointer )
ccipdGetConstImplementMacro( RegisterMFAStudyOutput, TrainedMFA, MFAVolumeBaseConstPointer )
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
#ifdef Boost_SERIALIZATION_FOUND

ccipdSerializationImplementationHeadingMacro( RegisterMFAStudyOutput )
{

  using boost::serialization::make_nvp;

    boost::serialization::void_cast_register(
    static_cast< ConcreteFactorySelf< Self > * >( NULL ),
    static_cast< Self * >( NULL ) );

  if ( version < 1 )
    return;

  archive
    & BOOST_SERIALIZATION_BASE_OBJECT_NVP( RegisterOutputStudyImage )
    & make_nvp( "ReconstructAllFeatures", this->m_SecretData->m_ReconstructAllFeatures );

} // serialize

ccipdSerializationXMLMacro( RegisterMFAStudyOutput )


typedef ConcreteFactorySelf< RegisterMFAStudyOutput > 
  RegisterMFAStudyOutputConcrete;

#endif // Boost_SERIALIZATION_FOUND
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace ccipd
 


#ifdef Boost_SERIALIZATION_FOUND

  BOOST_CLASS_VERSION( ccipd::RegisterMFAStudyOutputConcrete, 1 )
  BOOST_CLASS_EXPORT_GUID( ccipd::RegisterMFAStudyOutput , "RegisterMFAStudyOutput" )
  BOOST_CLASS_EXPORT( ccipd::RegisterMFAStudyOutputConcrete )
      ccipdBoostVirtualBaseFalse(
    ccipd::RegisterMFAStudyOutput,
    ccipd::RegisterMFAStudyOutputConcrete )

#endif // Boost_SERIALIZATION_FOUND

