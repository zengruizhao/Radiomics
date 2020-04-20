#ifndef __ccipdRegisterMFAImages_h
#define __ccipdRegisterMFAImages_h



//////////////////////////////////////////////////////////////////////////////////////////

// ccipd includes
//
#include "ccipdRegisterBase.h"
#include "ccipdRegisterMFARegularizedMetric.h" // for getting the metric
#include "ccipdRegisterStudyImage.h"
#include "ccipdRegisterImageFeature.h"
#include "ccipdMFATypes.h"

//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



/**
 * \brief Registration with the MFA segmentation images.
 * 
 * You must set the moving study, which will represent
 * the study to be segmented.
 * 
 * \author  Rob Toth
 * \date    2013-01-18
 * \ingroup MFA
 */
class RegisterMFAImages :
  virtual public CopyableDerived<
    RegisterMFAImages,
    RegisterBase >
{



//////////////////////////////////////////////////////////////////////////////////////////
  ccipdConstructorDeclarationMacro( RegisterMFAImages )
//////////////////////////////////////////////////////////////////////////////////////////




public: // typedefs
//////////////////////////////////////////////////////////////////////////////////////////
  /// Our base class
  typedef RegisterBase                     Superclass;
  typedef RegisterStudyImage               MovingImageType; 
  typedef RegisterImageFeature             FixedImageType;
  typedef FixedImageType::FeatureImageConstPointer 
    FeatureImageConstPointer;

  typedef RegisterMFARegularizedMetric        MetricType;
  typedef RegisterMFARegularizedMetricPointer MetricPointer;
//////////////////////////////////////////////////////////////////////////////////////////



public: // set/get functions
//////////////////////////////////////////////////////////////////////////////////////////
  /// We supply the fixed image from the model. You must set the trained model.
  ccipdGetConstDeclareMacro( FixedFeatureImage, FeatureImageConstPointer )

  /// We provide the trained MFA
  ccipdGetConstDeclareMacro( TrainedMFA, MFAVolumeBaseConstPointer )
    
  /// We provide the features
  ccipdGetConstDeclareMacro( FeaturePreparer, MFAFeaturePreparerConstPointer )
  
  /**
  * You must give the trained MFA model to use in registration.
  * 
  * This should also contain the trained MFA.
  */
  ccipdSetByrefDeclareMacro( DrivingFeatures,
    MFADrivingFeaturePreparerConstPointer )

  /**
  * We must explicitly instantiate this function
  * because RegisterMFAMetric requires it (pure virtual)
  * yet can't see RegisterMovingStudy's version
  * of it. We will simply pass on control to the
  * underlying GetMovingStudy function.
  */
  ccipdSetByrefDeclareMacro( MovingStudy, StudyConstPointer );
  ccipdGetConstDeclareMacro( MovingStudy, StudyConstPointer );

  virtual void InitializeRegistration(); 
//////////////////////////////////////////////////////////////////////////////////////////


  
protected:
//////////////////////////////////////////////////////////////////////////////////////////'
  virtual void CopyParametersInternal( const Self & other );

  
  /// Rachel's Note: Okay I think Rob uses m_DrivingFeatures as his FixedFeatureImage...
  /// I am making this protected so if this is not the case it can be easy to fix
  ccipdSetByrefDeclareMacro( FixedFeatureImage, FeatureImageConstPointer )
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



}; // class RegisterMFAImages 
} // namespace ccipd

  

//////////////////////////////////////////////////////////////////////////////////////////
#ifdef Boost_SERIALIZATION_FOUND

BOOST_CLASS_VERSION( ccipd::RegisterMFAImages, 1 )

ccipdBoostVirtualBaseTrue(
  ccipd::RegisterBase,
  ccipd::RegisterMFAImages )
  
#endif // Boost_SERIALIZATION_FOUND
//////////////////////////////////////////////////////////////////////////////////////////



#endif // __ccipdRegisterMFAImages_h
