#ifndef __ccipdSIMFeaturePreparerUsingExtractors_h
#define __ccipdSIMFeaturePreparerUsingExtractors_h



//////////////////////////////////////////////////////////////////////////////////////////

// ccipd includes
#include "ccipdDimensionalityReductionOptions.h"
#include "ccipdSIMTypes.h"
#include "ccipdAlignerTypes.h"
#include "ccipdSIMFeaturePreparerInterface.h"
#include "ccipdMacros.h"
#include "ccipdFactory.h"

// itk includes
#include <itkMacro.h>

// serialization includes
#ifdef Boost_SERIALIZATION_FOUND
  ccipdSerializationForwardDeclarationMacro
  #include <boost/serialization/version.hpp>
#endif // Boost_SERIALIZATION_FOUND

//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



class SIMFeaturePreparerUsingExtractors :
  public SIMFeaturePreparerInterface,
  public Factory< SIMFeaturePreparerUsingExtractors >
{



//////////////////////////////////////////////////////////////////////////////////////////
  ccipdConstructorDeclarationMacro( SIMFeaturePreparerUsingExtractors )
  ccipdFactorySuperclassTypedefsMacro
//////////////////////////////////////////////////////////////////////////////////////////



public: // set/get
//////////////////////////////////////////////////////////////////////////////////////////

  ccipdSetByvalDeclareMacro( Verbose, bool )
  itkBooleanMacro          ( Verbose )

  /// Remove any saved extractors
  virtual void ClearExtractors();

  /// Add a new extractor to use
  virtual void AddExtractor( const SIMFeatureExtractorPointer & extractor );

  /// Add a new collection of extractors to use
  virtual void AddExtractors( const SIMFeatureExtractorCollection & extractors );

  /// To give to the feature extractors.
  ccipdSetByrefDeclareMacro( StudyAligner, StudyAlignerPointer ) 
  ccipdSetByrefDeclareMacro( BoundingBoxExtractor, SIMBoundingBoxExtractorPointer )

  /// Return all the feature ID's from all extractors
  virtual FeatureIDConstCollection GetWhichFeatures() const;
  
  /// Extract all features for a given study
  virtual FeatureCollectionPointer GetFeatures(
    const StudyConstPointer & study ) const;

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



}; // class SIMFeaturePreparerUsingExtractors
} // namespace ccipd

  
#ifdef Boost_SERIALIZATION_FOUND
   BOOST_CLASS_VERSION( ccipd::SIMFeaturePreparerUsingExtractors, 1 )
#endif // Boost_SERIALIZATION_FOUND


#endif // __ccipdSIMFeaturePreparerUsingExtractors_h
