#ifndef __ccipdMFADrivingFeaturePreparer_h
#define __ccipdMFADrivingFeaturePreparer_h



//////////////////////////////////////////////////////////////////////////////////////////

// ccipd includes
#include "ccipdSIMFeaturePreparerInterface.h"
#include "ccipdMFATypes.h"
#include "ccipdMacros.h"
#include "ccipdFactory.h"

// serialization includes
#include "ccipdSegmentationOptions.h"
#ifdef Boost_SERIALIZATION_FOUND
  ccipdSerializationForwardDeclarationMacro
  #include <boost/serialization/version.hpp>
  #include <boost/serialization/tracking.hpp>
#endif // Boost_SERIALIZATION_FOUND

//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



/**
 * This class provides
 * the features to drive
 * the MFA segmentation.
 * 
 * \ingroup MFA
 * \author  Rob Toth
 * \date    2013-01-22
 */
class MFADrivingFeaturePreparer :
  public MFAFeaturePreparerInterface,
  public Factory< MFADrivingFeaturePreparer >
{



//////////////////////////////////////////////////////////////////////////////////////////
  ccipdConstructorDeclarationMacro( MFADrivingFeaturePreparer )
  ccipdFactorySuperclassTypedefsMacro
//////////////////////////////////////////////////////////////////////////////////////////


  
public:
//////////////////////////////////////////////////////////////////////////////////////////

  typedef MFAFeaturePreparerInterface Superclass;

  /// Return all the feature ID's
  virtual FeatureIDConstCollection GetWhichFeatures() const;
  
  /// Extract all features for a given study
  virtual FeatureCollectionPointer GetFeatures(
    const StudyConstPointer & study ) const;

//////////////////////////////////////////////////////////////////////////////////////////



public: // set/get
//////////////////////////////////////////////////////////////////////////////////////////

  /**
   * \brief Set the trained MFA model.
   * 
   * The feature extractors from the
   * trained model will be used to prune
   * the features.
   */
  ccipdSetByrefDeclareMacro( MFAModel, MFAModelConstPointer )

  /// Get the trained MFA model you have previously input.
  ccipdGetConstDeclareMacro( MFAModel, MFAModelConstPointer )

  /**
   * \brief Set the levelset features.
   * 
   * This will be parsed through the levelset
   * extractor from the trained model, and only
   * levelset objects in the trained levelset extractor
   * will be used as a driving feature.
   * 
   * This prevents you from trying to set a levelset
   * which wasn't trained.
   */
  ccipdSetByrefDeclareMacro( InputLevelsetFeatures, FeatureIDConstCollection )
  
  /// Get the pruned, valid levelset features which will drive the segmentation.
  ccipdGetConstDeclareMacro( InputLevelsetFeatures, FeatureIDConstCollection )

  /**
   * \brief Set the texture features.
   * 
   * This will be parsed through the texture
   * extractor from the trained model, and only
   * levelset objects in the trained texture extractor
   * will be used as a driving feature.
   * 
   * This prevents you from trying to set a texture
   * which wasn't trained.
   */
  ccipdSetByrefDeclareMacro( InputTextureFeatures, FeatureIDConstCollection )

  /// Get the pruned, valid texture features which will drive the segmentation.
  ccipdGetConstDeclareMacro( InputTextureFeatures, FeatureIDConstCollection )
  
  /**
   * If you set this to non-zero (default is 0),
   * then after optionally transforming
   * and cropping the feature, it will be
   * extrapolated by this many mm.
   */
  ccipdSetByvalDeclareMacro( ExtrapolateInMM, float )
  ccipdGetConstDeclareMacro( ExtrapolateInMM, float )
  
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



}; // class MFADrivingFeaturePreparer
} // namespace ccipd



#ifdef Boost_SERIALIZATION_FOUND
/**
 * 2013-03-14
 *   Version 2: Add in extrapolation value.
 */
BOOST_CLASS_VERSION( ccipd::MFADrivingFeaturePreparer, 2 )

BOOST_CLASS_TRACKING(ccipd::MFADrivingFeaturePreparer,
  boost::serialization::track_always )
#endif



#endif // __ccipdMFADrivingFeaturePreparer_h
