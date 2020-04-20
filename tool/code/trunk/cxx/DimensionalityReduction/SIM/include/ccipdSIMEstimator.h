#ifndef __itkSIMEstimator_h
#define __itkSIMEstimator_h



//////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdCore.h"
#include "ccipdFeatureCollectionTypes.h"   // for FeatureID, etc.
#include "ccipdSIMTypes.h"       // for our SIM output type
#include "ccipdVolumeTypes.h"    // we are estimating an SIM for volumes
#include "ccipdMacros.h"         // for set/get functions
#include "ccipdFactory.h"
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



/**
 * \brief SIM training class.
 * 
 * This class is used to input a bunch
 * of feature images, and train
 * an ccipd::SIMBase class.
 * 
 * PCA will be performed
 * on the input images to train
 * the resulting SIM.
 * 
 * \note
 *   Right now, 3D images are used, and
 *   the resulting SIMBase is 3D.
 * 
 * \author  Rob Toth
 * \ingroup SIM
 */
class SIMEstimator : public Factory< SIMEstimator >
{



//////////////////////////////////////////////////////////////////////////////////////////
  ccipdConstructorDeclarationMacro( SIMEstimator )
  ccipdFactorySuperclassTypedefsMacro
//////////////////////////////////////////////////////////////////////////////////////////



public:
//////////////////////////////////////////////////////////////////////////////////////////
  /** Set/Get whether or not to show progress */
  ccipdSetByvalDeclareMacro( Verbose, bool )
  ccipdGetConstDeclareMacro( Verbose, bool )

  /** Set/Get the features to use. */
  ccipdSetByrefDeclareMacro( WhichFeatures, FeatureIDConstCollection )
  ccipdGetConstDeclareMacro( WhichFeatures, FeatureIDConstCollection )

  /** This will return the number of features in WhichFeatures. */
  ccipdGetConstDeclareMacro( NumberOfFeatures, unsigned int )

  /**
   * \brief Set/Get the amount of variance to retain for all eigenvectors.
   *
   * Eigenvectors which don't contribute at least
   * this much variance are dropped from the model.
   *  
   * The filter produces the required number of principal components plus
   * one outputs. Output index 0 represents the mean image and the
   * remaining outputs the requested principal components. */
   ccipdSetByvalDeclareMacro( VarianceToRetain, double )
   ccipdGetConstDeclareMacro( VarianceToRetain, double )

  /** Get the number of training images in the input. */
  //ccipdGetConstDeclareMacro( NumberOfTrainingImages, unsigned int );
//////////////////////////////////////////////////////////////////////////////////////////



public:
//////////////////////////////////////////////////////////////////////////////////////////
  /** Input a set of training features for a single study/patient/whatever. */
  virtual void AddTrainingFeatures( const FeatureCollectionPointer trainingFeatures );

  /** Remove a collection of training features for training */
  virtual void RemoveTrainingFeatures( const FeatureCollectionPointer trainingFeatures );
  
  /// How many training features exist?
  ccipdGetConstDeclareMacro( NumberOfTrainingFeatures, std::size_t );
  
//////////////////////////////////////////////////////////////////////////////////////////



public:
//////////////////////////////////////////////////////////////////////////////////////////
  /**
   * This function actually computes an SIMBase.
   *
   * Call this after you've input all your training features.
   */
   virtual SIMVolumeStoragePointer CalculateSIM();
//////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////
  ccipdPIMPLMacro
////////////////////////////////////////////////////////////////////////////////////////////////////



}; // class SIMEstimator
} // namespace ccipd




#endif // __ccipdSIMEstimator_h
