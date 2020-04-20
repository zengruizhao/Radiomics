


//////////////////////////////////////////////////////////////////////////////////////////

// ccipd includes
#include "ccipdSIMFeatureExtractor.h"
#include "ccipdFeatureCollection.h"
#include "ccipdSIMStudiesMaskAligner.h"  // for the transform and bounding box
#include "ccipdApplyTransformation.h"    // for transforming
#include "ccipdCropImage.h"              // for cropping
#include "ccipdSIMBoundingBoxExtractor.h"
#include "ccipdPadImage.h" // for expanding

#include "ccipdDisableWarningsMacro.h"
  // itk includes 
  #include <itkImage.h>
  #include <itkTransform.h>
#include "ccipdEnableWarningsMacro.h"

// serialization includes
#ifdef Boost_SERIALIZATION_FOUND
  #include <boost/archive/xml_iarchive.hpp>
  #include <boost/archive/xml_oarchive.hpp>
  #include <boost/serialization/export.hpp>
  #include "ccipdSharedPointerSerialization.hxx" // for serializing smart pointers
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
class SIMFeatureExtractor::SecretData
{

public:
  SecretData(); ///< Constructor

  StudyConstPointer
    m_Study;
  /*
  FeatureCollection::Pointer
    m_FeatureCache;
  */
  SIMFeatureExtractor::AlignerPointer
    m_Aligner;
  SIMFeatureExtractor::BoundingBoxPointer
    m_BoundingBoxExtractor;
  float
    m_ExtrapolateInMM;
  bool
    m_Verbose;

}; // class SecretData
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
SIMFeatureExtractor::SIMFeatureExtractor() :
ccipdNewPIMPLMacro
{
} // Constructor
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
SIMFeatureExtractor::SecretData::SecretData() :
//m_FeatureCache( FeatureCollection::New() ),
m_ExtrapolateInMM( 0 ),
m_Verbose( true )
{
} // Constructor
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
SIMFeatureExtractor::~SIMFeatureExtractor()
{
} // Destructor
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void SIMFeatureExtractor::PreprocessFeature(
  const FeatureID & itkNotUsed( whichFeature ) ) const
{
  // by default, no preprocessing
} // PreprocessFeature
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
VolumeImageConstPointer SIMFeatureExtractor::PostprocessFeature(
  const VolumeImageConstPointer & inputFeature ) const
{

  // here, we apply a transform and cropping to the feature
  // if we have a mask aligner

  // extract variables

  const bool verbose = this->GetVerbose();

  // we will store the output feature
  // (start out with just the input feature)
  VolumeImageConstPointer outputFeature = inputFeature;
  
  SIMFeatureExtractor::AlignerPointer
    aligner ( this->m_SecretData->m_Aligner );
  const auto & 
    boundingBoxExtractor = this->m_SecretData->m_BoundingBoxExtractor;

  const StudyConstPointer study = this->GetStudy();

  // what do we have?
  const bool
    hasStudy                = !!study,
    hasAligner              = !!aligner,
    hasBoundingBoxExtractor = !!boundingBoxExtractor;
    
  if ( hasStudy && hasAligner )
    {
      
    // extract our transform
    const VolumeTransformConstPointer transformation =
      aligner->GetTransform( study );
    
    // extract the template volume
    // to apply the transform to
    VolumeImageBaseConstPointer templateVolume;
    
    if ( hasBoundingBoxExtractor )
      templateVolume = boundingBoxExtractor->GetTemplateVolume();
      
    if ( templateVolume.IsNull() )
      templateVolume = outputFeature;
      
    // if we have everything, transform
    if ( templateVolume.IsNotNull() && transformation.IsNotNull() )
      {          
      // transform our texture
      if ( verbose ) cout << "Transforming feature:" << endl;

      const bool extrapolate = true;
      const VolumeImageConstPointer transformedFeature(
        ApplyTransform(
          transformation, inputFeature, templateVolume, extrapolate ) );

      if ( verbose ) cout << "Transforming feature done." << endl;

      // set it if successful
      if ( transformedFeature.IsNotNull() )
        outputFeature = transformedFeature;
      
      } // template and transformation
      
    } // study and aligner
    
  if ( hasBoundingBoxExtractor )
    {
    
    // get the bounding box
    const VolumeRegionType boundingBox =
      boundingBoxExtractor->GetTemplateBoundingBox();

    // is its size positive?
    const bool hasBoundingBox = boundingBox.GetNumberOfPixels() > 0;
    
    if ( hasBoundingBox )
      {      
      
      // crop our feature
      if ( verbose ) cout << "Cropping feature:" << endl;
      
      // crop the feature
      const VolumeImageConstPointer croppedFeature(
        CropVolume( outputFeature, boundingBox ) );

      if ( verbose ) cout << "Cropping feature done." << endl;

      if ( croppedFeature.IsNotNull() )
        outputFeature = croppedFeature;
      
      } // hasBoundingBox
      
    } // bounding box extractor
    
  // now extrapolate the image
  const auto extrapolateInMM = this->GetExtrapolateInMM();
  if ( extrapolateInMM != 0 )
    {
    
    if ( verbose ) cout << "Extrapolating image " <<
      extrapolateInMM << " mm:" << endl;
      
    const VolumeImageConstPointer extrapolatedFeature(
      ExtrapolateImage( outputFeature.GetPointer(), extrapolateInMM ) );
      
    if ( verbose ) cout << "Extrapolating image done." << endl;
      
    if ( extrapolatedFeature.IsNotNull() )
      {
      outputFeature = extrapolatedFeature;
      }
    
    } // extrapolate
    
  // output
  return outputFeature;

} // PostprocessFeature
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
VolumeImageConstPointer SIMFeatureExtractor::GetFeature(
  const FeatureID & whichFeature )
{

  const bool verbose = this->GetVerbose();

  if ( verbose ) cout << "Preprocessing feature:" << endl;
  this->PreprocessFeature( whichFeature );
  if ( verbose ) cout << "Preprocessing feature done." << endl;

  if ( verbose ) cout << "Extracting feature:" << endl;
  const VolumeImageConstPointer
    initialFeature = this->ExtractFeature( whichFeature );
  if ( verbose ) cout << "Extracting feature done." << endl;

  if ( verbose ) cout << "Postprocessing feature:" << endl;
  const VolumeImageConstPointer postProcessedFeature =
    this->PostprocessFeature( initialFeature );
  if ( verbose ) cout << "Postprocessing feature done." << endl;

  // if no post-processing was performed, return the
  // initial feature
  const VolumeImageConstPointer finalFeature =
    postProcessedFeature ?
    postProcessedFeature :
    initialFeature;

  // output
  return finalFeature;

} // GetFeature
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
ccipdSetByvalImplementMacro( SIMFeatureExtractor, Verbose        , bool              )
ccipdGetConstImplementMacro( SIMFeatureExtractor, Verbose        , bool              )
ccipdSetByvalImplementMacro( SIMFeatureExtractor, ExtrapolateInMM, float             )
ccipdGetConstImplementMacro( SIMFeatureExtractor, ExtrapolateInMM, float             )
ccipdSetByrefImplementMacro( SIMFeatureExtractor, Study          , StudyConstPointer )
ccipdGetConstImplementMacro( SIMFeatureExtractor, Study          , StudyConstPointer )

ccipdSetByrefImplementMacro( SIMFeatureExtractor,
  Aligner, StudyAlignerPointer )
ccipdGetConstImplementMacro( SIMFeatureExtractor,
  Aligner, SIMFeatureExtractor::AlignerPointer )
ccipdSetByrefImplementMacro( SIMFeatureExtractor,
  BoundingBoxExtractor, SIMBoundingBoxExtractorPointer )
ccipdGetConstImplementMacro( SIMFeatureExtractor,
  BoundingBoxExtractor, SIMFeatureExtractor::BoundingBoxPointer )
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace ccipd



//////////////////////////////////////////////////////////////////////////////////////////

#ifdef Boost_SERIALIZATION_FOUND

namespace ccipd
{

ccipdSerializationImplementationHeadingMacro( SIMFeatureExtractor )
{

  using boost::serialization::make_nvp;

  if ( version < 1 )
    return;

  archive
      //& make_nvp( "Aligner"             , this->m_SecretData->m_Aligner )
      & make_nvp( "BoundingBoxExtractor", this->m_SecretData->m_BoundingBoxExtractor );

  if ( version >= 2 )
    {
    archive & make_nvp( "ExtrapolateInMM", this->m_SecretData->m_ExtrapolateInMM );
    } // version 2
    
} // serialize

ccipdSerializationXMLMacro( SIMFeatureExtractor )

} // namespace

#endif // Boost_SERIALIZATION_FOUND

//////////////////////////////////////////////////////////////////////////////////////////
