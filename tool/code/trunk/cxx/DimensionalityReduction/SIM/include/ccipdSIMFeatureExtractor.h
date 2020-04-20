#ifndef __ccipdSIMFeatureExtractor_h
#define __ccipdSIMFeatureExtractor_h



//////////////////////////////////////////////////////////////////////////////////////////

// ccipd includes
#include "ccipdSIMTypes.h"
#include "ccipdFeatureCollectionTypes.h"
#include "ccipdVolumeTypes.h"
#include "ccipdPatientTypes.h"
#include "ccipdAlignerTypes.h"
#include "ccipdMacros.h"
#include "ccipdEnableSharedFromThis.hxx"

// itk includes
#include <itkMacro.h>

// serialization includes
#include "ccipdDimensionalityReductionOptions.h"
#ifdef Boost_SERIALIZATION_FOUND
  ccipdSerializationForwardDeclarationMacro
  #include <boost/serialization/version.hpp>
  #include <boost/serialization/tracking.hpp>
#endif // Boost_SERIALIZATION_FOUND

//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{

class SIMFeatureExtractor :
  public EnableSharedFromThis< SIMFeatureExtractor >
{



//////////////////////////////////////////////////////////////////////////////////////////
  ccipdConstructorDeclarationMacro( SIMFeatureExtractor )
//////////////////////////////////////////////////////////////////////////////////////////


  
public: // set/get functions
//////////////////////////////////////////////////////////////////////////////////////////
  typedef StudyAlignerPointer            AlignerPointer;
  typedef SIMBoundingBoxExtractorPointer BoundingBoxPointer;

  /// Set the alignment of the training studies
  ccipdSetByrefDeclareMacro( Aligner, AlignerPointer )
  ccipdGetConstDeclareMacro( Aligner, AlignerPointer )

  ccipdSetByrefDeclareMacro( BoundingBoxExtractor, BoundingBoxPointer )
  ccipdGetConstDeclareMacro( BoundingBoxExtractor, BoundingBoxPointer )

  /**
   * If you set this to non-zero (default is 0),
   * then after optionally transforming
   * and cropping the feature, it will be
   * extrapolated by this many mm.
   */
  ccipdSetByvalDeclareMacro( ExtrapolateInMM, float )
  ccipdGetConstDeclareMacro( ExtrapolateInMM, float )
  
  /// Set whether to show progress
  ccipdSetByvalDeclareMacro( Verbose, bool )
  ccipdGetConstDeclareMacro( Verbose, bool )
  itkBooleanMacro          ( Verbose )

  /// Set the study for which we will be extracting features
  ccipdSetByrefDeclareMacro( Study, StudyConstPointer )

  /// Go through the entire process of pre, post-processing and extracting.
  virtual VolumeImageConstPointer GetFeature(
    const FeatureID & whichFeature );

  /// Child classes must say which features they are providing.
  virtual FeatureIDConstCollection GetWhichFeatures() const = 0;

//////////////////////////////////////////////////////////////////////////////////////////



protected:
//////////////////////////////////////////////////////////////////////////////////////////

  /// Which study are we extracting features from?
  ccipdGetConstDeclareMacro( Study, StudyConstPointer )

  /// Child classes can optionally prepare a feature.
  virtual void PreprocessFeature(
    const FeatureID & whichFeature ) const;

  /// Child classes must be able to extract the features they claimed to provide.
  virtual VolumeImageConstPointer ExtractFeature(
    const FeatureID & whichFeature ) const = 0;

  /// Child classes can optionally post-process the final feature
  virtual VolumeImageConstPointer PostprocessFeature(
    const VolumeImageConstPointer & feature ) const;

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



}; // class SIMFeatureExtractor
} // namespace ccipd



//////////////////////////////////////////////////////////////////////////////////////////
#ifdef Boost_SERIALIZATION_FOUND
/**
 * 2013-03-14
 *   Version 2 added support for extrapolating the feature.
 */
BOOST_CLASS_VERSION( ccipd::SIMFeatureExtractor, 2 )
BOOST_CLASS_TRACKING(ccipd::SIMFeatureExtractor,
  boost::serialization::track_always )
#endif // Boost_SERIALIZATION_FOUND
//////////////////////////////////////////////////////////////////////////////////////////



#endif // __ccipdSIMFeatureExtractor_h
