#ifndef __ccipdMFASegmenter_h
#define __ccipdMFASegmenter_h



//////////////////////////////////////////////////////////////////////////////////////////

// ccipd includes
#include "ccipdSegmentationOptions.h"
#include "ccipdMFADLL.h"             // for creating dll's
#include "ccipdSegmenter.h"          // our base class
#include "ccipdMacros.h"             // for some commonly typed code
#include "ccipdPatientTypes.h"       // for some typedef's
#include "ccipdFeatureCollectionTypes.h" // for some typedef's
#include "ccipdMFATypes.h"
#include "ccipdSerializable.h"
#include "ccipdFactory.h"

// std includes
#include <memory>                   // for smart pointers
#include <string>
#include <vector>

// itk includes
#include <itkMacro.h>

// serialization includes
#ifdef Boost_SERIALIZATION_FOUND
  ccipdSerializationForwardDeclarationMacro
#endif // Boost_SERIALIZATION_FOUND

//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



/**
 * \brief
 *   This class will perform an image
 *   segmentation using a trained MFA.
 * 
 * \author  Rob Toth
 * \ingroup MFA
 */
class MFA_EXPORT MFASegmenter : // class declaration
  public Segmenter,             // our base class
  public Serializable< MFASegmenter >, // we are serializable
  public Factory     < MFASegmenter >
{



public: // tyepdefs
//////////////////////////////////////////////////////////////////////////////////////////
  ccipdConstructorDeclarationMacro( MFASegmenter )
  ccipdFactorySuperclassTypedefsMacro

  typedef Segmenter Superclass;

//////////////////////////////////////////////////////////////////////////////////////////



public:
//////////////////////////////////////////////////////////////////////////////////////////
  /**
   * This version will use its internal trained MFA file.
   * It won't cache the trained MFA, and will always reload.
   */
  virtual bool Segment( const StudyConstPointer & inputStudy );

  virtual bool Segment(
    const StudyConstPointer             & inputStudy,
    const SegmentationModelConstPointer & trainedModel );

  /**
   * Always re-run the segmentation,
   * even if we've already segmented.
   */
  virtual bool Resegment( const StudyConstPointer & inputStudy );
  
  virtual bool Resegment(
    const StudyConstPointer             & inputStudy,
    const SegmentationModelConstPointer & trainedModel );

  /**
   * Load in the model, and
   * prepare the driving features.
   * 
   * After this, you can get
   * the trained MFA model.
   */
  virtual void Initialize();

//////////////////////////////////////////////////////////////////////////////////////////



public:
//////////////////////////////////////////////////////////////////////////////////////////

  /** Set variables */
  ccipdSetByrefDeclareMacro( TrainedMFAFile        , std::string                     )
  ccipdSetByrefDeclareMacro( SegmentationObject    , MaskObjectType                  )
  ccipdSetByrefDeclareMacro( RegistrationAlgorithms, RegisterMFAAlgorithmCollection  )
  ccipdSetByvalDeclareMacro( Verbose               , bool                            )
 
  /**
   * Set the bounding box for a given study
   * to be used to both initialize the
   * transformation, and to regularize
   * the transformation.
   */
  ccipdSetByrefDeclareMacro( BoundingBox, MFABoundingBoxType )

  /**
   * Remove any "set" bounding boxe
   * for this study. This will force
   * the segmenter to try using
   * the trained bounding box extractor.
   * 
   * However, if that fails, then no
   * bounding box will be used to either
   * initial or regularize.
   */
  virtual void ClearBoundingBox();

  /// Output the trained MFA model
  ccipdGetConstDeclareMacro( TrainedModel, MFAModelConstPointer )

  /**
   * Set to true if you want the moved study to
   * contain all the reconstructed features
   * in its feature collection.
   * 
   * Alternatively, you can set this
   * to false and call the individual Get* functions
   * to get the features.
   * 
   * Default is true.
   */
  ccipdSetByvalDeclareMacro( ReconstructFeaturesInMoved, bool )
  ccipdGetConstDeclareMacro( ReconstructFeaturesInMoved, bool )
  itkBooleanMacro          ( ReconstructFeaturesInMoved )

  /**
   * Output the moved study after
   * segmentation. The volume will
   * contain the moved volume,
   * and the feature collection
   * will contain the mean training
   * features, and optionally
   * the reconstructed features.
   */
  ccipdGetConstDeclareMacro( MovedStudy, StudyPointer )

  ccipdGetConstDeclareMacro( RegistrationAlgorithms, RegisterMFAAlgorithmCollection   )
  ccipdGetConstDeclareMacro( DrivingFeaturePreparer, MFADrivingFeaturePreparerPointer )

  /// Get all possible features you can reconstruct.
  ccipdGetConstDeclareMacro( AllFeatures, FeatureIDConstCollection )

  /// Get all possible objects you can return a segmentation for
  ccipdGetConstDeclareMacro( AllSegmentationObjects, MaskObjectCollectionType )

  /// Where will you load a trained MFA from?
  ccipdGetConstDeclareMacro( TrainedMFAFile, std::string )

  /// If we only segment a specific object, which object is it?
  ccipdGetConstDeclareMacro( SegmentationObject, MaskObjectType )

  ccipdGetConstDeclareMacro( Verbose, bool )

  /**
   * Calculate and output the reconstruction for a feature
   * in the coordinate frame of the feature.
   */
  virtual VolumeImagePointer GetReconstruction(
    const FeatureIDConstPointer & whichFeature ) const;

  /**
   * Calculate and output the reconstruction for a feature
   * in the coordinate frame of the MFA.
   */
  virtual VolumeImagePointer GetTransformedReconstruction(
    const FeatureIDConstPointer & whichFeature ) const;

  /// In the original image's coordinate frame.
  virtual VolumeImagePointer GetReconstructedLevelset(
    const MaskObjectType    & whichObject ) const;

  /// In the MFA's coordinate frame.
  virtual VolumeImagePointer GetReconstructedTransformedLevelset(
    const MaskObjectType    & whichObject ) const;

  /// In the original image's coordinate frame.
  virtual VolumeImagePointer GetReconstructedTexture(
    const TextureFeatureEnum  whichTexture ) const;

  /// In the MFA's coordinate frame.
  virtual VolumeImagePointer GetReconstructedTransformedTexture(
    const TextureFeatureEnum  whichTexture ) const;

  /// In the original image's coordinate frame.
  virtual VolumeMaskImagePointer GetSegmentation(
    const MaskObjectType & whichObject ) const;

  /// In the MFA's coordinate frame.
  virtual VolumeMaskImagePointer GetTransformedSegmentation(
    const MaskObjectType & whichObject ) const;
    
  /// In the original image's coordinate frame.
  virtual VolumeMaskImagePointer GetSegmentation(
    const MaskObjectType    & whichObject,
    const StudyConstPointer & whichStudy ) const;

  /// In the MFA's coordinate frame.
  virtual VolumeMaskImagePointer GetTransformedSegmentation(
    const MaskObjectType    & whichObject,
    const StudyConstPointer & whichStudy ) const;

//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
  ccipdPIMPLMacro
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
#ifdef Boost_SERIALIZATION_FOUND
  /** We are serializable */
  ccipdSerializationDeclarationMacro
#endif // Boost_SERIALIZATION_FOUND
//////////////////////////////////////////////////////////////////////////////////////////



}; // class MFASegmenter



} // namespace
#endif // __ccipdMFASegmenter_h
