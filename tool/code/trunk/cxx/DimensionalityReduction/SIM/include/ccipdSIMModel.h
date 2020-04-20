#ifndef __ccipdSIMModel_h
#define __ccipdSIMModel_h



//////////////////////////////////////////////////////////////////////////////////////////

// ccipd includes
#include "ccipdCrossValidationModel.h"   // base class
#include "ccipdDimensionalityReductionOptions.h" // do we have some boost libraries?
#include "ccipdMacros.h"              // for commonly typed code
#include "ccipdPatientTypes.h"        // for our typedef's
#include "ccipdFeatureTypes.h"        // for our typedef's
#include "ccipdHashable.h"            // saying we are hashable
#include "ccipdVolumeTypes.h"         // for our Volume image type
#include "ccipdSIMTypes.h"            // for typedefs and MFA pointers
#include "ccipdEnableSharedFromThis.h"

// std includes
#include <memory> // for smart pointers

// serialization includes
#ifdef Boost_SERIALIZATION_FOUND
  ccipdSerializationForwardDeclarationMacro
  #include <boost/serialization/version.hpp>
  #include <boost/serialization/tracking.hpp>
#endif // Boost_SERIALIZATION_FOUND
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



/**
 * \brief A SIM model with training capabilities.
 * 
 * This class is an extension of the
 * ccipd::SIMStorage class and allows
 * you to input a collection of patients,
 * extract features and then use the
 * ccipd::SIMEstimator to train a SIM.
 * 
 * It also extends the ccipd::CrossValidationModel
 * class so it can be used in the cross validation
 * code and any code that needs a ccipd::CrossValidationModel
 * as input.
 * 
 * Note that this is an abstract class, children must perform their 
 * own checks to ensure that the training studies have the appropriate
 * data associated with them. 
 *
 * \author  Rachel Sparks
 * \ingroup SIM
 */
class SIMModel :
  public CrossValidationModel,
  public EnableSharedFromThis< SIMModel >
{



//////////////////////////////////////////////////////////////////////////////////////////
  ccipdConstructorDeclarationMacro( SIMModel )
public:
  typedef EnableSharedFromThis< Self > SuperclassShared;
  
  typedef SuperclassShared::Pointer      Pointer;
  typedef SuperclassShared::ConstPointer ConstPointer;
  
  /** The main function to extract a set of studies and train. */
  virtual bool Train( const Patients & trainingData );

  virtual bool operator==( const Hashable & other ) const;

//////////////////////////////////////////////////////////////////////////////////////////



protected:
//////////////////////////////////////////////////////////////////////////////////////////
  virtual HashValueType ReHash() const;
  
  /** The function to train for a given set of studies */
  virtual bool TrainStudies( const StudyCollectionType & studies );
  
  /** Function to set the alignment patient for the studies aligner*/
  virtual bool UpdateStudiesAligner( const Patients & trainingData );

  /** The function to extract a set of studies with the necessary training data */
  virtual StudyCollectionType GetTrainingStudies( const Patients & trainingData ) = 0;
//////////////////////////////////////////////////////////////////////////////////////////



public:
//////////////////////////////////////////////////////////////////////////////////////////
  /** Set variables */
  ccipdSetByvalDeclareMacro( Verbose             , bool          )
  ccipdSetByvalDeclareMacro( ExcludeTemplateStudy, bool          )
  ccipdSetByrefDeclareMacro( AlignmentPatient    , PatientIDType )
  ccipdSetByrefDeclareMacro( BoundingBoxExtractor, SIMBoundingBoxExtractorPointer )
  ccipdSetByrefDeclareMacro( StudiesAligner      , SIMStudiesAlignerPointer       )
  ccipdSetByrefDeclareMacro( Extractors          , SIMFeatureExtractorCollection  )

  /** Get variables */
  ccipdGetConstDeclareMacro( TrainedSIM          , SIMVolumeStoragePointer    )
  ccipdGetConstDeclareMacro( IsTrained           , bool                       )
  ccipdGetConstDeclareMacro( ExcludeTemplateStudy, bool                       )
  ccipdGetConstDeclareMacro( AlignmentPatient    , PatientIDType )
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
#ifdef Boost_SERIALIZATION_FOUND
  /** We are serializable */
  ccipdSerializationDeclarationMacro
#endif // Boost_SERIALIZATION_FOUND
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
  ccipdPIMPLMacro
//////////////////////////////////////////////////////////////////////////////////////////



}; // class
} // namespace



//////////////////////////////////////////////////////////////////////////////////////////
#ifdef Boost_SERIALIZATION_FOUND

/**
 * 2013-03-08:
 *   Version 2 added support to exclude a template study.
 *   Version 2 added support for feature extractors/.
 */
BOOST_CLASS_VERSION( ccipd::SIMModel, 2 )
BOOST_CLASS_TRACKING(ccipd::SIMModel,
  boost::serialization::track_always )
#endif
//////////////////////////////////////////////////////////////////////////////////////////



#endif // __ccipdSIMModel_h
