


//////////////////////////////////////////////////////////////////////////////////////////

// ccipd includes
#include "ccipdRegisterMFAImages.h"
#include "ccipdExtractFeatureImage.h" // for getting the fixed image
#include "ccipdMFAModel.h"
#include "ccipdSIMStorage.h"
#include "ccipdMFADrivingFeaturePreparer.h"

// our own itk includes
#include "itkMFAReconstructionMetric.h"

// serialization includes
#ifdef Boost_SERIALIZATION_FOUND
  #include <boost/archive/xml_iarchive.hpp>
  #include <boost/archive/xml_oarchive.hpp>
#endif // Boost_SERIALIZATION_FOUND

#include "ccipdDisableWarningsMacro.h"
  // itk includes
  #include <itkVectorImage.hxx>
#include "ccipdEnableWarningsMacro.h"
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
using std::cerr;
using std::endl;
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////
template
class Copyable< RegisterMFAImages >;
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
class RegisterMFAImages::SecretData
{

public:
  SecretData();

  MFADrivingFeaturePreparerConstPointer
    m_DrivingFeatures;
 

}; // class SecretData
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
RegisterMFAImages::RegisterMFAImages() :
ccipdNewPIMPLMacro
{
  this->SetMetric( MetricType::New()           );
  this->SetFixedImage( FixedImageType::New()   );
  this->SetMovingImage( MovingImageType::New() );
} // constructor
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
RegisterMFAImages::SecretData::SecretData()
{
} // constructor
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
RegisterMFAImages::~RegisterMFAImages()
{
} // destructor
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void RegisterMFAImages::InitializeRegistration()
{
  this->Superclass::InitializeRegistration();

  // need to pass in objects to metric and initalize it.
  MetricPointer metric = std::dynamic_pointer_cast< MetricType > (this->GetMetric() );

  metric->SetFeaturePreparer ( this->GetFeaturePreparer()  );
  metric->SetTrainedMFA      ( this->GetTrainedMFA()       );
  metric->SetMovingStudy     ( this->GetMovingStudy()      );
  // Rob if you want something other than the buffered region we are going to implement it.
  metric->SetFixedImageRegion( 
    this->GetFixedFeatureImage()->GetBufferedRegion() ); 
  metric->InitializeRegistration();

} // InitializeRegistration
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
MFAVolumeBaseConstPointer RegisterMFAImages::GetTrainedMFA() const
{

  // if we have driving features, return its trained MFA
  const auto drivingFeatures = this->m_SecretData->m_DrivingFeatures;

  if ( drivingFeatures )
    {

    const auto trainedModel = drivingFeatures->GetMFAModel();

    if ( trainedModel )
      return trainedModel->GetTrainedSIM();

    } // drivingFeatures

  return MFAVolumeBaseConstPointer();

} // GetTrainedMFA
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
MFAFeaturePreparerConstPointer RegisterMFAImages::GetFeaturePreparer() const
{
  return this->m_SecretData->m_DrivingFeatures;
} // GetFeaturePreparer
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void RegisterMFAImages::SetDrivingFeatures(
  const MFADrivingFeaturePreparerConstPointer & drivingFeatures )
{

  // update the driving features
  this->m_SecretData->m_DrivingFeatures = drivingFeatures;

  // Rob: See note in the header
  this->SetFixedFeatureImage( this->GetFixedFeatureImage() );

  // delete our cache
  //this->ClearCache();

} // SetDrivingFeatures
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
StudyConstPointer
RegisterMFAImages::GetMovingStudy() const
{
  const MovingImageType::SuperclassFactory::Pointer 
    movingImage( std::dynamic_pointer_cast< MovingImageType > (this->GetMovingImage() ) );
  
  if ( !movingImage )
    std::cerr << "Error retrieving MovingImage. Not a RegisterStudyMaskAsImage" << std::endl;

    return movingImage->GetStudy();

} // GetMovingStudy
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void
RegisterMFAImages::SetMovingStudy( const StudyConstPointer & movingStudy )
{
  
  MovingImageType::SuperclassFactory::Pointer 
    movingImage( std::dynamic_pointer_cast< MovingImageType > (this->GetMovingImage() ) );
  
  if ( !movingImage )
    std::cerr << "Error retrieving MovingImage. Not a RegisterStudyMaskAsImage" << std::endl;

  movingImage->SetStudy( movingStudy );
} // SetMovingStudy
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
RegisterMFAImages::FeatureImageConstPointer
RegisterMFAImages::GetFixedFeatureImage() const
{

  // get our driving features
  const auto & drivingFeatures = this->m_SecretData->m_DrivingFeatures;

  // prepare some output
  const FeatureImageConstPointer badOutput;
  const bool verbose = this->GetVerbose();
  
  // no driving features, so return null
  if ( !drivingFeatures )
    {
    if ( verbose ) cerr << "Error: No driving features!" << endl;
    return badOutput;
    }

  const MFAModelConstPointer trainedModel =
    drivingFeatures->GetMFAModel();

  // no trained MFA, so return null
  if ( !trainedModel )
    {
    if ( verbose ) cerr << "Error: No trained model!" << endl;
    return badOutput;
    }

  const MFAVolumeBaseConstPointer trainedMFA =
    trainedModel->GetTrainedSIM();
  
  // no trained MFA, so return null
  if ( !trainedModel )
    {
    if ( verbose ) cerr << "Error: No trained MFA!" << endl;
    return badOutput;
    }

  // just extract a single feature's "mean" image as the fixed image
  return trainedMFA->GetMeanAndEigenvectors(
    *( trainedMFA->GetFeatureIDs().cbegin() ) );

} // GetFixedImage
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void
RegisterMFAImages::SetFixedFeatureImage( const FeatureImageConstPointer & fixedFeatureImage )
{
  
  FixedImageType::SuperclassFactory::Pointer 
    fixedImage( std::dynamic_pointer_cast< FixedImageType > (this->GetFixedImage() ) );
  
  if ( !fixedImage )
    std::cerr << "Error retrieving MovingImage. Not a RegisterStudyMaskAsImage" << std::endl;

  fixedImage->SetFeatureImage( fixedFeatureImage);
} // SetFixedFeatureImage
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void RegisterMFAImages::CopyParametersInternal( const Self & other )
{

  // copy the driving feature preparer
  this->SetDrivingFeatures( other.m_SecretData->m_DrivingFeatures );

} // CopyParametersInternal
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
#ifdef Boost_SERIALIZATION_FOUND

ccipdSerializationImplementationHeadingMacro( RegisterMFAImages )
{

  if ( version < 1 )
    return;

  archive
    & BOOST_SERIALIZATION_BASE_OBJECT_NVP( Superclass     );

} // serialize

ccipdSerializationXMLMacro( RegisterMFAImages )

#endif // Boost_SERIALIZATION_FOUND
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace ccipd
