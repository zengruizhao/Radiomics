#ifndef __ccipdReconstructionBase_h
#define __ccipdReconstructionBase_h



////////////////////////////////////////////////////////////////////////////////////////////////////

// ccipd includes
#include "ccipdMacros.h"                 // for easy coding
#include "ccipdFeatureCollectionTypes.h" // for feature id types
#include "ccipdMFATypes.h"               // for MFA typedefs
#include "ccipdSIMTypes.h"               // for MFA typedefs
#include "ccipdMatrixTypes.h"            // for our projection types

// std includes
#include <memory>                       // for smart pointers
#include <vector>                       // for containers

#include "ccipdDisableWarningsMacro.h"
   // itk includes
  #include <itkMacro.h>
#include "ccipdEnableWarningsMacro.h"

////////////////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * This is the (abstract) base class for reconstructing images using the MFA.
 *
 * It will compute the pseudo-inverse of the basis images, and it will
 * contain the mean samples.
 *
 * You must extend the CalculateSampleLocations function to store a list
 * of physical locations to be sampled.
 *
 * The basis and mean images need to be set.
 *
 * \author  Rob Toth.
 * \ingroup MFA
 */
template< class TPixelType, unsigned int Dimension > // template types
class MFAReconstructionBase
////////////////////////////////////////////////////////////////////////////////////////////////////
{



////////////////////////////////////////////////////////////////////////////////////////////////////
  ccipdConstructorDeclarationMacro( MFAReconstructionBase )
////////////////////////////////////////////////////////////////////////////////////////////////////



public:
////////////////////////////////////////////////////////////////////////////////////////////////////
  // Typedefs

  /// The dimensionality of the images.
  static const unsigned int ImageDimension = Dimension;

  /** Some image types */
  typedef TPixelType                                 PixelType;
  typedef itk::VectorImage< PixelType, Dimension >   VectorImageType;
  typedef itk::SmartPointer< const VectorImageType > VectorImageConstPointer;
  typedef itk::ImageBase< Dimension >                ImageBaseType;
  typedef typename ImageBaseType::PointType          PointType;
  typedef float                                      RealType;

  /// Types for sample locations.
  typedef std::vector< PointType > SampleLocationsContainer;

  /// Types for the trained MFA.
  typedef SIMStorage< PixelType, ImageDimension > MFABaseType;
  typedef std::shared_ptr< const MFABaseType >    MFABaseConstPointer;
////////////////////////////////////////////////////////////////////////////////////////////////////


  
public:
////////////////////////////////////////////////////////////////////////////////////////////////////
  /** Our protected types. */

  /// Easily define a dynamic eigen vector
  typedef Eigen::Matrix<
    PixelType,
    -1, +1, 0, -1, +1
  > VectorType; 

  //typedef VectorType ProjectionType;
  typedef VectorType ReconstructionType;
  typedef VectorType SampledValuesType;

  typedef std::shared_ptr<
    const VectorType
  > VectorConstPointer;

  typedef VectorConstPointer ReconstructionConstPointer;
  typedef VectorConstPointer SampledValuesConstPointer;

////////////////////////////////////////////////////////////////////////////////////////////////////



public:
////////////////////////////////////////////////////////////////////////////////////////////////////
  /**
   * When non-zero, this will smooth the image with
   * a Gaussian prior to sampling.
   * The same variance is applied to all dimensions.
   * 
   * \note This is not currently implemented!
   */
  ccipdSetByvalDeclareMacro( SmoothingVariance, RealType )
  ccipdGetConstDeclareMacro( SmoothingVariance, RealType )

  /**
   * If you don't want to use all
   * the eigenvectors for reconstruction,
   * set this variable.
   * By default, it is the maximum
   * value of unsigned int,
   * signifying that all the eigenvectors
   * should be used for reconstruction.
   * If 0, then only the mean is used.
   */
  ccipdSetByvalDeclareMacro( MaxEigenvectors, unsigned int )
  ccipdGetConstDeclareMacro( MaxEigenvectors, unsigned int )
  
  /** Set/Get the option to display verbose output */
  ccipdSetByvalDeclareMacro( Verbose, bool )
  ccipdGetConstDeclareMacro( Verbose, bool )
  itkBooleanMacro( Verbose )

  /**
   * If true, estimate the variance
   * of each pixel, and use 1/variance
   * as the weights for the pseudo
   * inverse.
   * 
   * default = true
   * 
   * You must set this variable prior to initializing.
   */
  ccipdSetByvalDeclareMacro( UseVarianceWeights, bool )
  ccipdGetConstDeclareMacro( UseVarianceWeights, bool )
  itkBooleanMacro          ( UseVarianceWeights )

  /**
   * Input a collection of features
   * to use for estimation of the linked matrix,
   * which is used to estimate the reconstructions.
   * 
   * You specify a vector image, where each component
   * is a feature image, and each component
   * corresponds to a specific feature ID.
   */
  virtual void SetInputFeatures(
    const VectorImageType          * inputFeatures,
    const FeatureIDConstCollection & featureIDs );

  ccipdGetConstDeclareMacro( InputFeatures, VectorImageConstPointer )
  
  /**
   * \brief Which features did you input?
   *
   * This function will return the
   * feature ID's you have previously
   * had input.
   *
   * \note Was protected, but is now public.
   * 
   * \date 2012-05-01
   */
  ccipdGetConstDeclareMacro( InputFeatureIDs, FeatureIDConstCollection )
 
  /**
   * Input a trained MFA to use for the reconstruction.
   * 
   * This must be set before you reconstruct!
   */
  virtual void SetTrainedMFA( const MFABaseType & trainedMFA );
  
  /// Extract the trained MFA (or NULL)
  ccipdGetConstDeclareMacro( TrainedMFA, MFABaseConstPointer )

  /// Output which locations are being sampled.
  ccipdGetConstDeclareMacro( SampleLocations, SampleLocationsContainer )
    
  /// Output the last-calculated linked projection
  ccipdGetConstDeclareMacro( LinkedProjection, VectorConstPointer )
  
////////////////////////////////////////////////////////////////////////////////////////////////////





protected:
////////////////////////////////////////////////////////////////////////////////////////////////////
  /** Protected functions */

  /**
   * \brief Initialize the ability to reconstruct images.
   * 
   * This will calculate the locations to sample,
   * will resample all the mean and eigenvector images,
   * and calculate the pseudo inverse of each feature.
   */
  virtual void Initialize();

  /**
   * \brief Calculate physical points to sample.
   * 
   * This one must be overridden.
   * The overriding function will get a list of physical
   * locations which will be sampled.
   * After this has been called once, you can
   * call GetSampleLocations() to look at the locations.
   */
  virtual SampleLocationsContainer CalculateSampleLocations() = 0;

  /**
   * \brief Sample the mean and eigenvectors at each sample location.
   * 
   * This will first recalculate all the sample locations
   * and then sample the mean and eigenvectors
   * of all driving features at those locations.
   */
  virtual void SampleMeanAndEigenvectors();

  /**
   * \brief Recompute the projection of the linked matrix.
   * 
   * Given a collection of physical points
   * in space, it will sample those
   * points on all the input images.
   * 
   * It will then calculate the projection
   * for each feature, and concatenate them.
   * 
   * Finally, it will use the concatenated
   * feature projections to compute the
   * linked projection.
   *
   * After it's done, you can call
   * GetReconstruction().
   *
   * \param[in] onlySample
   *   If true, this will not actually
   *   use the sampled values to
   *   calculate the linked projection, but
   *   rather only sample the features
   *   and leave the linked projections
   *   untouched.
   * 
   * \returns
   *   Nothing. The linked projection
   *   is stored internally until you call
   *   GetReconstruction().
   *
   */
  virtual void CalculateLinkedProjection(
    const SampleLocationsContainer & sampleLocations,
    const bool                       onlySample = false );

  /**
   * Overloaded version of CalculateLinkedProjection()
   * which will use the fixed sample locations
   * to calculate the linked projection.
   * 
   * \date 2012-05-14
   */
  virtual void CalculateLinkedProjection();

  /**
   * \brief Calculate a reconstruction from input locations.
   * 
   * This function assumes that CalculateLinkedProjection()
   * has already been called, and will use the linked
   * projection from the last call.
   * 
   * This linked projections will then be used
   * to calculate the projection for a given feature,
   * which will in turn be used to calculate a
   * reconstruction of that feature.
   * 
   * Then, it will extract the reconstruction
   * of the feature determined from 'whichFeature'.
   *
   */
  virtual ReconstructionConstPointer GetReconstruction(
    const FeatureIDConstPointer & whichFeature ) const;

  /**
   * \brief Get the last set of sampled values.
   *
   * This function will get the
   * sampled values for a given feature
   * from the last time CalculateLinkedProjection
   * was called.
   *
   */
  SampledValuesConstPointer GetSampledValues(
    const FeatureIDConstPointer & whichFeature ) const;

  /**
   * \brief Extract the weights as 1/var which are already calculated.
   * 
   * \returns NULL if no weights are used.
   */
  virtual VectorConstPointer GetPseudoInverseWeights( 
    const FeatureIDConstPointer & whichFeature ) const;

////////////////////////////////////////////////////////////////////////////////////////////////////



private:
////////////////////////////////////////////////////////////////////////////////////////////////////
  ccipdPIMPLMacro
  friend class Self::SecretData;
////////////////////////////////////////////////////////////////////////////////////////////////////



}; // class MFAReconstructionBase
} // end namespace ccipd
#endif // __ccipdReconstructionBase_h
