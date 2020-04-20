#ifndef __ccipdRegisterMFAAlgorithm_h
#define __ccipdRegisterMFAAlgorithm_h



//////////////////////////////////////////////////////////////////////////////////////////

// ccipd includes
#include "ccipdRegisterMFAImages.h"
#include "ccipdRegisterMFAStudyOutput.h"
#include "ccipdRegisterTransformLinearCombo.h"
#include "ccipdRegisterOptimizerParticleSwarm.h"
#include "ccipdRegisterInterpolatorCombo.h"
#include "ccipdRegisterResolutionSingle.h"
#include "ccipdFactory.h"

//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



/**
 * This class combines all the
 * desired MFA registration parameters
 * to use in the registration
 * algorithms framework.
 * 
 * \author  Rob Toth
 * \date    2013-01-15
 * \ingroup MFA
 * \ingroup RegistrationAlgorithms
 */
class RegisterMFAAlgorithm :
  public RegisterResolutionSingleConcrete,
  virtual public RegisterMFAImages,
  public Factory< RegisterMFAAlgorithm >
{
  
  

//////////////////////////////////////////////////////////////////////////////////////////
  ccipdConstructorDeclarationMacro( RegisterMFAAlgorithm )
  ccipdFactorySuperclassTypedefsMacro

public: // typedefs
  typedef RegisterMFAImages                    SuperclassMetric;
  typedef RegisterResolutionSingle             SuperclassResolution;
  typedef SuperclassMetric                     Superclass;
  typedef Superclass::ParametersType           ParametersType;

  // object types
  typedef RegisterOptimizerParticleSwarm       OptimizerType;
  typedef OptimizerType::Pointer               OptimizerPointer;
  typedef RegisterInterpolatorCombo            InterpolatorType;
  typedef InterpolatorType::Pointer            InterpolatorPointer;
  typedef RegisterTransformLinearCombo         TransformType;
  typedef TransformType::Pointer               TransformPointer;
  typedef SuperclassMetric::MetricType         MetricType;
  typedef SuperclassMetric::MetricPointer      MetricPointer;
  typedef RegisterMFAStudyOutput               OutputType;
  typedef OutputType::Pointer                  OutputPointer;
//////////////////////////////////////////////////////////////////////////////////////////



public:
//////////////////////////////////////////////////////////////////////////////////////////
  virtual void InitializeRegistration();

  virtual void CopyParameters( const CopyableBase & other );
  
  /// We provide the trained MFA
  ccipdGetConstDeclareMacro( TrainedMFA,   MFAVolumeBaseConstPointer )

  /// We provide the moving study-- this will past the study to the output and metric.  
  ccipdSetByrefDeclareMacro( MovingStudy, StudyConstPointer )
  ccipdGetConstDeclareMacro(  StudyOutput, OutputPointer    )
//////////////////////////////////////////////////////////////////////////////////////////



private:
//////////////////////////////////////////////////////////////////////////////////////////
  /** Serialization */
#ifdef Boost_SERIALIZATION_FOUND
  /** We are serializable */
  ccipdSerializationDeclarationMacro
#endif // Boost_SERIALIZATION_FOUND
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
  ccipdPIMPLMacro
//////////////////////////////////////////////////////////////////////////////////////////



}; // class RegisterMFAAlgorithm
} // namespace ccipd



//////////////////////////////////////////////////////////////////////////////////////////
#ifdef Boost_SERIALIZATION_FOUND

BOOST_CLASS_VERSION( ccipd::RegisterMFAAlgorithm, 1 )
BOOST_CLASS_TRACKING( ccipd::RegisterMFAAlgorithm, 
  boost::serialization::track_always )

ccipdBoostVirtualBaseTrue(
  ccipd::RegisterMFAAlgorithm::SuperclassMetric,
  ccipd::RegisterMFAAlgorithm )

ccipdBoostVirtualBaseFalse(
  ccipd::RegisterMFAAlgorithm::SuperclassResolution,
  ccipd::RegisterMFAAlgorithm )

#endif // Boost_SERIALIZATION_FOUND
//////////////////////////////////////////////////////////////////////////////////////////



#endif // __ccipdRegisterMFAAlgorithm_h
