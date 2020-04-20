#ifndef __ccipdRegisterMFAMetric_h
#define __ccipdRegisterMFAMetric_h



//////////////////////////////////////////////////////////////////////////////////////////

// ccipd includes
#include "ccipdRegisterMetricBase.h"
#include "ccipdRegisterMFABase.h"
#include "ccipdPatientTypes.h"
#include "ccipdVolumeTypes.h"

// serialization includes
#ifdef Boost_SERIALIZATION_FOUND
  ccipdSerializationForwardDeclarationMacro
  #include <boost/type_traits/is_virtual_base_of.hpp>
#endif // Boost_SERIALIZATION_FOUND

// itk includes
#include <itkMacro.h>

//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



/**
 * \brief Registration with the MFA segmentation metric.
 * 
 * \author  Rob Toth
 * \date    2013-01-15
 * \ingroup MFA
 */
class RegisterMFAMetric :
  virtual public CopyableDerived<
    RegisterMFAMetric,
    RegisterMetricBase >,
  virtual public RegisterMFABase
{



//////////////////////////////////////////////////////////////////////////////////////////
  ccipdConstructorDeclarationMacro( RegisterMFAMetric )
//////////////////////////////////////////////////////////////////////////////////////////



public: // set/get functions
//////////////////////////////////////////////////////////////////////////////////////////

  /// Use NCC as the metric? (alternatively, SSD). Default = true
  ccipdSetByvalDeclareMacro( UseNCC, bool )
  ccipdGetConstDeclareMacro( UseNCC, bool )
  itkBooleanMacro          ( UseNCC )
  
  /// Use the inverse pixel variance as a weight for the reconstruction? (default = false)
  ccipdSetByvalDeclareMacro( UseVarianceWeights, bool )
  ccipdGetConstDeclareMacro( UseVarianceWeights, bool )
  itkBooleanMacro          ( UseVarianceWeights )
  
  /// How many eigenvectors can possibly be used? (default = 999)
  ccipdSetByvalDeclareMacro( MaxEigenvectors, unsigned int )
  ccipdGetConstDeclareMacro( MaxEigenvectors, unsigned int )
  
  typedef float PercentType;

  /**
   * \brief How many pixels to sample?
   * 
   * The value must be between 0.0 and 1.0 inclusive.
   * Values outside this range will
   * just use the default value.
   * 
   * The default is 0.50.
   */
  ccipdSetByvalDeclareMacro( PercentPixels, PercentType )
  ccipdGetConstDeclareMacro( PercentPixels, PercentType )

  typedef RegisterMetricBase             Superclass;
  typedef Superclass::MetricType         MetricType;
  typedef Superclass::MetricPointer      MetricPointer;
  
  typedef MFAVolumeReconstructionMetricPointer MFAMetricPointer;

  /// Get an MFA-specific metric to use
  ccipdGetConstDeclareMacro( MFAMetric, MFAMetricPointer )

  /// Get a metric to use
  ccipdGetConstDeclareMacro( Metric, MetricPointer )

  /// Maximizing the metric?
  ccipdGetConstDeclareMacro( MaximizeMetric, bool )

  /// Display outputs
  ccipdGetConstDeclareMacro( Verbose, bool )
  ccipdSetByvalDeclareMacro( Verbose, bool )

  /// We need to have a FeaturePreparer
  ccipdSetByrefDeclareMacro( FeaturePreparer, MFAFeaturePreparerConstPointer )
  ccipdGetConstDeclareMacro( FeaturePreparer, MFAFeaturePreparerConstPointer )

  /// We need to have a MovingStudy  
  ccipdSetByrefDeclareMacro( MovingStudy, StudyConstPointer )
  ccipdGetConstDeclareMacro( MovingStudy, StudyConstPointer )

  
  /// We need to have a FixedImageRegion
  ccipdSetByrefDeclareMacro( FixedImageRegion, VolumeRegionType )
  ccipdGetConstDeclareMacro( FixedImageRegion, VolumeRegionType )

  /// We need to have a TrainedMFA
  ccipdSetByrefDeclareMacro( TrainedMFA, MFAVolumeBaseConstPointer )
  ccipdGetConstDeclareMacro( TrainedMFA, MFAVolumeBaseConstPointer )

  virtual void InitializeRegistration();
//////////////////////////////////////////////////////////////////////////////////////////



protected: // functions
//////////////////////////////////////////////////////////////////////////////////////////
  virtual void CopyParametersInternal( const Self & other );
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



}; // class RegisterMFAMetric 
} // namespace ccipd

  

//////////////////////////////////////////////////////////////////////////////////////////
#ifdef Boost_SERIALIZATION_FOUND

/**
 * 2013-03-13
 *   Version 2 added support for the maximum eigenvectors
 */
BOOST_CLASS_VERSION( ccipd::RegisterMFAMetric, 3 )
BOOST_CLASS_TRACKING(ccipd::RegisterMFAMetric,
  boost::serialization::track_always )

ccipdBoostVirtualBaseTrue(
  ccipd::RegisterMetricBase,
  ccipd::RegisterMFAMetric )
  
ccipdBoostVirtualBaseTrue(
  ccipd::RegisterMFABase,
  ccipd::RegisterMFAMetric )
  
#endif // Boost_SERIALIZATION_FOUND
//////////////////////////////////////////////////////////////////////////////////////////



#endif // __ccipdRegisterMFAMetric_h
