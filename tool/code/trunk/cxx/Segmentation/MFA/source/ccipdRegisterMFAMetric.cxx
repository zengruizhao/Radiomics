


//////////////////////////////////////////////////////////////////////////////////////////

// ccipd includes
#include "ccipdRegisterMFAMetric.h"
#include "ccipdSIMFeaturePreparerInterface.h"
#include "ccipdFeatureCollection.h"


// itk includes
#include "itkMFAReconstructionMetric.h" // our metric

// serialization includes
#ifdef Boost_SERIALIZATION_FOUND
  #include <boost/archive/xml_iarchive.hpp>
  #include <boost/archive/xml_oarchive.hpp>
#endif // Boost_SERIALIZATION_FOUND

// bug in ITK requires this include
#include <itkVectorImage.hxx>

//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
using std::cerr;
using std::endl;
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////
template
class Copyable< RegisterMFAMetric >;
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
class RegisterMFAMetric::SecretData
{

public:
  SecretData();

  typedef RegisterMFAMetric::PercentType PercentType;

  static const PercentType
    m_DefaultPercentPixels;

  PercentType
    m_PercentPixels;

  unsigned int
    m_MaxEigenvectors;
  
  bool
    m_UseNCC,
    m_UseVarianceWeights,
    m_Verbose;

  RegisterMFAMetric::MFAMetricPointer
    m_InitializedMetric;

MFAFeaturePreparerConstPointer
    m_FeaturePreparer;

 StudyConstPointer
    m_MovingStudy;

VolumeRegionType
    m_FixedImageRegion;

MFAVolumeBaseConstPointer
    m_TrainedMFA;
}; // class SecretData
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
const RegisterMFAMetric::SecretData::PercentType
  RegisterMFAMetric::SecretData::m_DefaultPercentPixels = 0.50f;
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
RegisterMFAMetric::RegisterMFAMetric() :
ccipdNewPIMPLMacro
{
} // constructor
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
RegisterMFAMetric::SecretData::SecretData() :
m_PercentPixels( m_DefaultPercentPixels ),
m_MaxEigenvectors   ( 999   ),
m_UseNCC            ( true  ),
m_UseVarianceWeights( false ),
m_Verbose            (true  )
{
} // constructor
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
RegisterMFAMetric::~RegisterMFAMetric()
{
} // destructor
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
RegisterMFAMetric::MFAMetricPointer
RegisterMFAMetric::GetMFAMetric() const
{
  return this->m_SecretData->m_InitializedMetric;
}
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
RegisterMFAMetric::MetricPointer
RegisterMFAMetric::GetMetric() const
{
  return MetricPointer( this->GetMFAMetric() );
}
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void RegisterMFAMetric::InitializeRegistration()
{

  const bool verbose = this->GetVerbose();

  // clear the existing metric
  this->m_SecretData->m_InitializedMetric = MFAVolumeReconstructionMetricPointer();

  // create the metric
  const MFAMetricPointer metric =
    MFAVolumeReconstructionMetric::New();

  // set the parameters
  metric->SetUseNCC ( this->GetUseNCC () );
  metric->SetVerbose( verbose );

  // these parameters come from the child classes
  
  // a trained MFA
  const MFAVolumeBaseConstPointer trainedMFA = this->GetTrainedMFA();
  if ( !trainedMFA )
    {
    if ( verbose ) cerr << "Error: No trained MFA provided!" << endl;
    return;
    } // trainedMFA

  metric->SetTrainedMFA( *trainedMFA );
  
  // the features to use for the MFA segmentation
  const MFAFeaturePreparerConstPointer featurePreparer = this->GetFeaturePreparer();
  if ( !featurePreparer )
    {
    if ( verbose ) cerr << "Error: No features for the MFA provided!" << endl;
    return;
    } // featurePreparer

  const FeatureCollectionPointer featureImages =
    featurePreparer->GetFeatures( this->GetMovingStudy() );

  if ( !featureImages )
    {
    if ( verbose ) cerr << "Error: No feature images for the MFA provided!" << endl;
    return;
    } // featureImages

  const FeatureIDConstCollection whichFeatures = featurePreparer->GetWhichFeatures();

  // give the features to the metric
  metric->SetInputFeatures(
    featureImages->GetFeatures( whichFeatures ),
    whichFeatures );

  // set the % of pixels to use
  const PercentType percentPixels = this->GetPercentPixels();
  
  // multiply the fixed region size by
  // the percent of pixels to use
  const unsigned int numberOfSamples = static_cast< unsigned int >(
    this->GetFixedImageRegion().GetNumberOfPixels() * percentPixels );

  // update the metric with the # of pixels to use
  metric->SetNumberOfSpatialSamples( numberOfSamples );

  // whether or not to use variance weights for the reconstruction
  metric->SetUseVarianceWeights( this->GetUseVarianceWeights() );

  // the maximum # of eigenvectors
  metric->SetMaxEigenvectors( this->GetMaxEigenvectors() );
  
  // output
  this->m_SecretData->m_InitializedMetric = metric;

} // InitializeRegistration
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void RegisterMFAMetric::CopyParametersInternal( const Self & other )
{
  this->SetMaxEigenvectors( other.GetMaxEigenvectors() );
  this->SetPercentPixels      ( other.GetPercentPixels      () );
  this->SetUseNCC             ( other.GetUseNCC             () );
  this->SetUseVarianceWeights ( other.GetUseVarianceWeights () );
  this->SetVerbose            ( other.GetVerbose()             );
  this->SetFeaturePreparer    ( other.GetFeaturePreparer()     );
  this->SetMovingStudy        ( other.GetMovingStudy()         );
  this->SetFixedImageRegion   ( other.GetFixedImageRegion()    );
  this->SetTrainedMFA         ( other.GetTrainedMFA()          );
} // CopyParametersInternal
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
bool RegisterMFAMetric::GetMaximizeMetric() const
{

  // -NCC is minimized
  return false;

} // GetMaximizeMetric
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
RegisterMFAMetric::PercentType RegisterMFAMetric::GetPercentPixels() const
{
  const PercentType percentPixels = this->m_SecretData->m_PercentPixels;

  // do we have a valid percentage?
  const bool validPercentPixels =
    percentPixels >= 0.0f && percentPixels <= 1.0f;

  if ( validPercentPixels )
    return percentPixels;
  else
    return this->m_SecretData->m_DefaultPercentPixels; // definitely valid

} // GetPercentPixels
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
ccipdSetByvalImplementMacro( RegisterMFAMetric, UseNCC, bool )
ccipdGetConstImplementMacro( RegisterMFAMetric, UseNCC, bool )
ccipdSetByvalImplementMacro( RegisterMFAMetric, MaxEigenvectors, unsigned int )
ccipdGetConstImplementMacro( RegisterMFAMetric, MaxEigenvectors, unsigned int )
ccipdSetByvalImplementMacro( RegisterMFAMetric, UseVarianceWeights, bool )
ccipdGetConstImplementMacro( RegisterMFAMetric, UseVarianceWeights, bool )
ccipdSetByvalImplementMacro( RegisterMFAMetric, Verbose, bool )
ccipdGetConstImplementMacro( RegisterMFAMetric, Verbose, bool )
ccipdSetByvalImplementMacro( RegisterMFAMetric, PercentPixels, PercentType )
ccipdSetByrefImplementMacro(RegisterMFAMetric, FeaturePreparer, MFAFeaturePreparerConstPointer )
ccipdGetConstImplementMacro(RegisterMFAMetric, FeaturePreparer, MFAFeaturePreparerConstPointer )
ccipdSetByrefImplementMacro(RegisterMFAMetric, MovingStudy, StudyConstPointer )
ccipdGetConstImplementMacro(RegisterMFAMetric, MovingStudy, StudyConstPointer )
ccipdSetByrefImplementMacro(RegisterMFAMetric, FixedImageRegion, VolumeRegionType )
ccipdGetConstImplementMacro(RegisterMFAMetric, FixedImageRegion, VolumeRegionType )
ccipdSetByrefImplementMacro(RegisterMFAMetric, TrainedMFA, MFAVolumeBaseConstPointer )
ccipdGetConstImplementMacro(RegisterMFAMetric, TrainedMFA, MFAVolumeBaseConstPointer )
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
#ifdef Boost_SERIALIZATION_FOUND

ccipdSerializationImplementationHeadingMacro( RegisterMFAMetric )
{

  using boost::serialization::make_nvp;

  if ( version < 1 )
    return;

  archive
    & BOOST_SERIALIZATION_BASE_OBJECT_NVP( Superclass )
    & make_nvp( "UseNCC"            , this->m_SecretData->m_UseNCC             )
    & make_nvp( "UseVarianceWeights", this->m_SecretData->m_UseVarianceWeights )
    & make_nvp( "PercentPixels"     , this->m_SecretData->m_PercentPixels      );

  if ( version >= 2 )
    {
    archive & make_nvp( "MaxEigenvectors", this->m_SecretData->m_MaxEigenvectors );
    } // version 2

  if ( version >= 3 )
    {
    archive & make_nvp( "Verbose", this->m_SecretData->m_Verbose );
    } // version 2    
} // serialize

ccipdSerializationXMLMacro( RegisterMFAMetric )

#endif // Boost_SERIALIZATION_FOUND
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace ccipd
