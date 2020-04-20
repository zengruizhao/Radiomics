#ifndef __ccipdRegisterMFARegularizedMetric_h
#define __ccipdRegisterMFARegularizedMetric_h



//////////////////////////////////////////////////////////////////////////////////////////

// ccipd includes
#include "ccipdRegisterMFAMetric.h"
#include "ccipdAffineTypes.h"
#include "ccipdLandmarkTypes.h"
#include "ccipdCloneableFactory.h"
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
class RegisterMFARegularizedMetric :
  virtual public CopyableDerived<
    RegisterMFARegularizedMetric,
    RegisterMFAMetric >,
  public CloneableFactory< RegisterMFARegularizedMetric >
{



//////////////////////////////////////////////////////////////////////////////////////////
  ccipdConstructorDeclarationMacro( RegisterMFARegularizedMetric )
    
public:
  typedef CloneableFactory< Self > SuperclassFactory;
  ccipdFactoryTypedefsMacro
//////////////////////////////////////////////////////////////////////////////////////////



public:
//////////////////////////////////////////////////////////////////////////////////////////
  typedef RegisterMFAMetric              Superclass;
  typedef Superclass::MetricType         MetricType;
  typedef Superclass::MetricPointer      MetricPointer;
//////////////////////////////////////////////////////////////////////////////////////////



public:
//////////////////////////////////////////////////////////////////////////////////////////
  /// Define how the likelihood is calculated
  enum LikelihoodType
  {
    LikelihoodPDF,
    LikelihoodLogPDF,
    LikelihoodRelativePDF
  };

  /// Set how the likelihood is calculated (default is log pdf)
  ccipdSetByvalDeclareMacro( LikelihoodType, LikelihoodType )
  ccipdGetConstDeclareMacro( LikelihoodType, LikelihoodType )

  typedef double RegularizationWeightType;

  /// Set how much to regularize (default = 0)
  ccipdSetByvalDeclareMacro( BoundingBoxWeight, RegularizationWeightType )
  ccipdGetConstDeclareMacro( BoundingBoxWeight, RegularizationWeightType )
  ccipdSetByvalDeclareMacro( KnownPointsWeight, RegularizationWeightType )
  ccipdGetConstDeclareMacro( KnownPointsWeight, RegularizationWeightType )

  /// Clear any points set with SetKnownPoints
  virtual void ClearKnownPoints();

  /**
   * Set points known
   * to be on the surface
   * of some objects
   */
  virtual void SetKnownPoints(
    const LandmarkSetConstPointer & points,
    const MaskObjectType          & whichLevelsetObject );

  /// Set the regularization distribution
  ccipdSetByrefDeclareMacro( Distribution, AffineDistributionConstPointer )

  /// Set the intermediate transform
  ccipdSetByrefDeclareMacro( IntermediateTransform, VolumeAffineTransformConstPointer )

  /// Get a regularized metric to use
  ccipdGetConstDeclareMacro( Metric, MetricPointer )

//////////////////////////////////////////////////////////////////////////////////////////

  

//////////////////////////////////////////////////////////////////////////////////////////'
protected:
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



}; // class RegisterMFARegularizedMetric 
} // namespace ccipd

  

//////////////////////////////////////////////////////////////////////////////////////////
#ifdef Boost_SERIALIZATION_FOUND

/**
 * History:
 * 
 * 2013-03-31
 *   Version 2 added support for known points weights.
 */
BOOST_CLASS_VERSION( ccipd::RegisterMFARegularizedMetric, 2 )
BOOST_CLASS_TRACKING(ccipd::RegisterMFARegularizedMetric,
  boost::serialization::track_always )

ccipdBoostVirtualBaseTrue(
  ccipd::RegisterMFAMetric,
  ccipd::RegisterMFARegularizedMetric )
  
#endif // Boost_SERIALIZATION_FOUND
//////////////////////////////////////////////////////////////////////////////////////////



#endif // __ccipdRegisterMFARegularizedMetric_h
