#ifndef __ccipdMFAModel_h
#define __ccipdMFAModel_h



//////////////////////////////////////////////////////////////////////////////////////////

// ccipd includes
#include "ccipdSIMModel.h"            // base class
#include "ccipdPatientTypes.h"
#include "ccipdSegmentationOptions.h" // do we have some boost libraries?
#include "ccipdMacros.h"              // for commonly typed code
#include "ccipdMFATypes.h"            // for typedefs and MFA pointers
#include "ccipdAffineTypes.h"         // for the bounding box distribution
#include "ccipdSerializable.h"        // to add Load/Save functions
#include "ccipdLoadableFactory.h"     // to add the "Create" function

// std includes
#include <memory> // for smart pointers

// serialization includes
#ifdef Boost_SERIALIZATION_FOUND
  ccipdSerializationForwardDeclarationMacro
  #include <boost/serialization/version.hpp>
#endif // Boost_SERIALIZATION_FOUND
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



/**
 * \brief A trained MFA model.
 * 
 * This class is an extension of the
 * ccipd::SIMStorage class and allows
 * you to input a collection
 * of patients, extract texture features
 * and levelsets, and then use the
 * ccipd::MFAEstimator to train a MFA.
 * 
 * This class has some additional parameters
 * over just the trained class, such as
 * which levelsets were used, and the levelset
 * parameters.
 * 
 * It also extends the ccipd::SIMModel
 * class so it can be used in the cross validation
 * code and any code that needs a ccipd::SIMModel
 * as input.
 * 
 * \author  Rob Toth
 * \ingroup MFA
 */
class MFAModel :
  public SIMModel,
  public Serializable   < MFAModel >,
  public LoadableFactory< MFAModel >
{



//////////////////////////////////////////////////////////////////////////////////////////
  ccipdConstructorDeclarationMacro( MFAModel )

public:
  typedef SIMModel Superclass;

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
  virtual StudyCollectionType GetTrainingStudies( const Patients & trainingData );
//////////////////////////////////////////////////////////////////////////////////////////



public:
//////////////////////////////////////////////////////////////////////////////////////////
  /** Get variables */
  ccipdGetConstDeclareMacro( BoundingBoxExtractor   , MFABoundingBoxExtractorConstPointer )
  ccipdGetConstDeclareMacro( LevelsetExtractor      , MFALevelsetExtractorConstPointer    )
  ccipdGetConstDeclareMacro( TextureExtractor       , MFATextureExtractorConstPointer     )
  ccipdGetConstDeclareMacro( BoundingBoxDistribution, AffineDistributionConstPointer      )
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
  BOOST_CLASS_VERSION( ccipd::MFAModel, 1 )
//  BOOST_CLASS_TRACKING(ccipd::MFAModel, boost::serialization::track_always )
#endif
//////////////////////////////////////////////////////////////////////////////////////////



#endif // __ccipdMFAModel_h
