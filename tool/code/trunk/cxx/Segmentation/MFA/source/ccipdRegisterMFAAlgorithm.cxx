


//////////////////////////////////////////////////////////////////////////////////////////

// ccipd includes
#include "ccipdRegisterMFAAlgorithm.h"
#include "ccipdRegisterInterpolatorCombo.h"
#include "ccipdRegisterTransformLinearCombo.h"
#include "ccipdFactory.hxx"

// serialization includes
#ifdef Boost_SERIALIZATION_FOUND
  #include <boost/archive/xml_iarchive.hpp>
  #include <boost/archive/xml_oarchive.hpp>
  #include <boost/serialization/export.hpp>
#endif // Boost_SERIALIZATION_FOUND

//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////
template
class Copyable< RegisterMFAAlgorithm >;

template
class Factory< RegisterMFAAlgorithm >;
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
class RegisterMFAAlgorithm::SecretData
{

public:
  SecretData();

  OutputPointer
    m_StudyOutput;
 

}; // class SecretData
//////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////
RegisterMFAAlgorithm::RegisterMFAAlgorithm() :
ccipdNewPIMPLMacro
{
  this->SetInterpolator( InterpolatorType::New() );
 
  this->SetTransform   ( TransformType::New()    ); 

  this->SetOptimizer   ( OptimizerType::New()    );

  this->AddInitializer( this->m_SecretData->m_StudyOutput   );
} // constructor
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
RegisterMFAAlgorithm::SecretData::SecretData()
{
  this->m_StudyOutput  = OutputType::New();
} // Constructor
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
RegisterMFAAlgorithm::~RegisterMFAAlgorithm()
{
} // constructor
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void RegisterMFAAlgorithm::InitializeRegistration()
{
  this->SuperclassMetric::InitializeRegistration();

  OptimizerType::Pointer optimizer( std::dynamic_pointer_cast< OptimizerType >( this->GetOptimizer() ) );

  if ( !optimizer )
    std::cerr << "Error retrieving Optimizer. Not a RegisterMFAAlgorithm::OptimizerType" << std::endl;

  optimizer->SetTransform( this->GetTransform() );

  // set up the output study appropriately
  this->m_SecretData->m_StudyOutput->SetTrainedMFA( this->GetTrainedMFA() );
  this->m_SecretData->m_StudyOutput->SetFixedImage( this->GetFixedImage() );
} // InitializeRegistration
//////////////////////////////////////////////////////////////////////////////////////////


  
//////////////////////////////////////////////////////////////////////////////////////////
void RegisterMFAAlgorithm::CopyParameters( const CopyableBase & other )
{

  // for if we ever want to be copyable ourselves:
  //this->Copyable< Self >::CopyParameters( other );

  // call "CopyParameters" for each child class
  this->SuperclassMetric      ::CopyParameters( other );
  this->SuperclassResolution  ::CopyParameters( other );
  this->m_SecretData->m_StudyOutput->CopyParameters( other );
} // CopyParameters
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
MFAVolumeBaseConstPointer RegisterMFAAlgorithm::GetTrainedMFA() const
{
  return this->SuperclassMetric::GetTrainedMFA();
}
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void RegisterMFAAlgorithm::SetMovingStudy( const StudyConstPointer & movingStudy) 
{
  this->m_SecretData->m_StudyOutput->SetMovingStudy( movingStudy );
  this->SuperclassMetric::SetMovingStudy( movingStudy );
}
//////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////
ccipdGetConstImplementMacro( RegisterMFAAlgorithm, StudyOutput, 
  RegisterMFAAlgorithm::OutputPointer )
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
#ifdef Boost_SERIALIZATION_FOUND

ccipdSerializationImplementationHeadingMacro( RegisterMFAAlgorithm )
{

  boost::serialization::void_cast_register(
    static_cast< ConcreteFactorySelf< Self > * >( NULL ),
    static_cast< Self * >( NULL ) );

  if ( version < 1 )
    return;
  
  archive
    & BOOST_SERIALIZATION_BASE_OBJECT_NVP( SuperclassMetric     )
    & BOOST_SERIALIZATION_BASE_OBJECT_NVP( SuperclassResolution );

} // serialize

ccipdSerializationXMLMacro( RegisterMFAAlgorithm )

typedef ConcreteFactorySelf< RegisterMFAAlgorithm > 
  RegisterMFAAlgorithmConcrete;

#endif // Boost_SERIALIZATION_FOUND
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace ccipd
 


#ifdef Boost_SERIALIZATION_FOUND

  BOOST_CLASS_VERSION( ccipd::RegisterMFAAlgorithmConcrete, 1 )
  BOOST_CLASS_EXPORT_GUID( ccipd::RegisterMFAAlgorithm , "RegisterMFAAlgorithm" )
  BOOST_CLASS_EXPORT( ccipd::RegisterMFAAlgorithmConcrete )
      ccipdBoostVirtualBaseFalse(
    ccipd::RegisterMFAAlgorithm,
    ccipd::RegisterMFAAlgorithmConcrete )

#endif // Boost_SERIALIZATION_FOUND