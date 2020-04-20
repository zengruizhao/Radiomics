#ifndef __ccipdReconstructionBase_hxx
#define __ccipdReconstructionBase_hxx



////////////////////////////////////////////////////////////////////////////////////////////////////

// ccipd includes
#include "ccipdMFAReconstructionBase.h"
#include "ccipdFeatureID.h"
#include "ccipdSIMStorage.h"                     // for storing the trained MFA
#include "ccipdSIMSampleImage.h"                 // for sampling the images
#include "ccipdPseudoInverse.h"                  // for matrix inversion
#include "ccipdMatrixConcatenation.h"            // for concatenating projections
#include "ccipdUnivariateGaussianDistribution.h" // for random number generation


// boost includes
#include <boost/foreach.hpp> // for loops

// std includes
#include <unordered_map> // for storing the matrices for each given feature


#include "ccipdDisableWarningsMacro.h"
  // itk includes
  #include <itkVectorImage.h>     // for our input features
  #include <itkExceptionObject.h> // for errors

  // eigen includes
  #include <Eigen/Core> // for matrices
#include "ccipdEnableWarningsMacro.h"


#ifdef _MSC_VER
#pragma warning( disable: 4127 )
#pragma warning( disable: 4714 )
#endif // _MSC_VER
////////////////////////////////////////////////////////////////////////////////////////////////////




namespace ccipd
{


  
//////////////////////////////////////////////////////////////////////////////////////////
/**
 * \brief PIMPL interface to MFAReconstructionBase class.
 * 
 * \author  Rob Toth
 * \ingroup MFA
 */
template< class TPixelType, unsigned int Dimension >
class MFAReconstructionBase< TPixelType, Dimension >::SecretData
{
public: // public to the MFAReconstructionBase class

  /// Constructor
  SecretData();

  /// Typedef of our container class
  typedef TPixelType PixelType;
  typedef MFAReconstructionBase<
    PixelType,
    Dimension
  > MFAReconstructionBaseType;

  typedef typename MFAReconstructionBaseType::RealType RealType;

  typedef typename MFAReconstructionBaseType::SampleLocationsContainer
    SampleLocationsContainer;

  typedef typename MFAReconstructionBaseType::VectorImageType
    VectorImageType;
  typedef typename MFAReconstructionBaseType::VectorImageConstPointer
    VectorImageConstPointer;

  /// Our projections are Eigen vectors.
  typedef typename MFAReconstructionBaseType::VectorConstPointer ProjectionConstPointer;

  /// Our class storing the features.
  typedef typename MFAReconstructionBaseType::MFABaseType         MFABaseType;
  typedef typename MFAReconstructionBaseType::MFABaseConstPointer MFABaseConstPointer;

  // matrix types
  typedef typename MFABaseType::LinkedMatrixType          EigenvectorsMatrixType;
  typedef std::shared_ptr< const EigenvectorsMatrixType > EigenvectorsMatrixConstPointer;

  /// A vector of mean values
  typedef typename MFAReconstructionBaseType::VectorType MeanVectorType;

  /// A pointer to a vector of the mean values
  typedef typename MFAReconstructionBaseType::VectorConstPointer MeanVectorConstPointer;

  /// The pseudo-inverse is actually a matrix.
  typedef EigenvectorsMatrixType PseudoInverseType;

  /// A smart pointer to a pseudo inverse matrix
  typedef EigenvectorsMatrixConstPointer PseudoInverseConstPointer;

  /// Smart pointer to sampled values
  typedef typename MFAReconstructionBaseType::SampledValuesConstPointer SampledValuesConstPointer;

  /**
   * The key is which feature it is.
   * The value is the sampled mean vector
   * for that feature.
   */
  typedef std::unordered_map<
    FeatureIDConstPointer,
    MeanVectorConstPointer,
    HashableFunctor< FeatureIDConstPointer >,
    HashableFunctor< FeatureIDConstPointer >
  > MeanVectorMap;

  /// This is the same type as the means
  typedef MeanVectorMap WeightsMap;

  /**
   * The key is which feature it is.
   * The value is a matrix of eigenvectors
   * for that feature, where each column
   * is an eigenvector.
   */
  typedef std::unordered_map<
    FeatureIDConstPointer,
    EigenvectorsMatrixConstPointer,
    HashableFunctor< FeatureIDConstPointer >,
    HashableFunctor< FeatureIDConstPointer >
  > EigenvectorsMatrixMap;

  /**
   * The key is which feature it is.
   * The value is the pseudoinverse
   * for that feature.
   */
  typedef std::unordered_map<
    FeatureIDConstPointer,
    PseudoInverseConstPointer,
    HashableFunctor< FeatureIDConstPointer >,
    HashableFunctor< FeatureIDConstPointer >
  > PseudoInverseMap;

  /**
   * The key is which feature it is.
   * The value is the current set of
   * sampled input values for a given
   * feature, which were used to calcuate
   * the last projection.
   */
  typedef std::unordered_map<
    FeatureIDConstPointer,
    SampledValuesConstPointer,
    HashableFunctor< FeatureIDConstPointer >,
    HashableFunctor< FeatureIDConstPointer >
  > SampledValuesMap;

  /* Local storage variables */
  RealType
    m_SmoothingVariance;
  VectorImageConstPointer
    m_InputFeatures;
  FeatureIDConstCollection
    m_InputFeatureIDs;
  MFABaseConstPointer
    m_TrainedMFA;
  MeanVectorMap
    m_MeanVectors; ///< sampled mean vectors for each feature
  MeanVectorConstPointer
    m_ConcatenatedMeanVectors; ///< Concatenate for all driving features (5N x 1)
  EigenvectorsMatrixMap
    m_EigenvectorMatrices; ///< N x d (d from linked matrix) each column of each value is an eigenvector
  SampleLocationsContainer
    m_FixedSampleLocations;
  SampledValuesMap
    m_SampledValues; ///< Remember the feature values we sampled
  WeightsMap
    m_PseudoInverseWeights; ///< Weights for each pixel in a given feature.
  EigenvectorsMatrixConstPointer
    m_ConcatenatedDrivingEigenvectors, ///< concatenate all eigenvector matrices
    m_DrivingEigenvectorsPseudoInverse; ///< p-inv of the contatenations
  ProjectionConstPointer
    m_LinkedProjection; ///< This stores the linked projection of the current iteration.
  
  bool
    m_Verbose,     ///< Whether to show progress while generating (default = false )
    m_Initialized, ///< Remember if we've initialized.
    m_UseVarianceWeights,
    m_NormalizeMeanAndSamples;

  unsigned int
    m_MaxEigenvectors;
    
  /// Sample for a specific feature.
  void SampleMeanAndEigenvectors( const FeatureIDConstPointer & whichFeature );

  /**
   * \brief Take each eigenvector matrix and compute the inverse.
   * 
   * \todo Make it multi-threaded.
   */
  void CalculatePseudoInverses();

  /// Calculate pseudo inverse of all input features.
  /*
  void CalculateFeaturesPseudoInverses();

  /// Calculate pseudo inverse of a single feature.
  PseudoInverseConstPointer CalculateFeaturePseudoInverse(
    const FeatureIDConstPointer & whichFeature ) const;
  */

  /// Calculate pseudo inverse of the linked matrix.
  //void CalculateLinkedPseudoInverse();

}; // MFAReconstructionBase::SecretData
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
// Constructor
template<        class TPixelType, unsigned int Dimension >
MFAReconstructionBase< TPixelType,              Dimension >
::SecretData::SecretData() :
m_SmoothingVariance ( 0     ),
m_Verbose           ( false ),
m_Initialized       ( false ),
m_UseVarianceWeights( true  ),
m_NormalizeMeanAndSamples( false ),
m_MaxEigenvectors( std::numeric_limits< unsigned int >::max() )
{
} // secret constructor

template<        class TPixelType, unsigned int Dimension >
MFAReconstructionBase< TPixelType,              Dimension >
::MFAReconstructionBase() :
ccipdNewPIMPLMacro
{
} // constructor

template<        class TPixelType, unsigned int Dimension >
MFAReconstructionBase< TPixelType,              Dimension >
::~MFAReconstructionBase()
{
} // destructor
//////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////
// Set/Get smoothing variance
template<             class TPixelType, unsigned int Dimension >
void MFAReconstructionBase< TPixelType,              Dimension >
::SetSmoothingVariance( const RealType smoothingVariance )
{
  this->m_SecretData->m_SmoothingVariance = smoothingVariance;
} // SetSmoothingVariance
template<        class TPixelType, unsigned int Dimension > typename
MFAReconstructionBase< TPixelType,              Dimension >::RealType
MFAReconstructionBase< TPixelType,              Dimension >
::GetSmoothingVariance() const
{
  return this->m_SecretData->m_SmoothingVariance;
} // GetSmoothingVariance
////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////
template<        class TPixelType, unsigned int Dimension > typename
MFAReconstructionBase< TPixelType,              Dimension >::VectorConstPointer
MFAReconstructionBase< TPixelType,              Dimension >
::GetLinkedProjection() const
{
  ccipdGetBodyMacro( LinkedProjection )
} // GetLinkedProjection
////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////
// Set/Get verbose
template<             class TPixelType, unsigned int Dimension >
void MFAReconstructionBase< TPixelType,              Dimension >
::SetVerbose( const bool variable )
{
  ccipdSetBodyMacro( Verbose )
} // SetVerbose
template<             class TPixelType, unsigned int Dimension >
bool MFAReconstructionBase< TPixelType,              Dimension >
::GetVerbose() const
{
  ccipdGetBodyMacro( Verbose )
} // GetVerbose
////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////
// Set/Get maximum eigenvectors
template< class TPixelType, unsigned int Dimension >
void MFAReconstructionBase< TPixelType,              Dimension >
::SetMaxEigenvectors( const unsigned int variable )
{
  ccipdSetBodyMacro( MaxEigenvectors )
} // SetMaxEigenvectors
template< class TPixelType, unsigned int Dimension >
unsigned int
MFAReconstructionBase< TPixelType,              Dimension >
::GetMaxEigenvectors() const
{
  ccipdGetBodyMacro( MaxEigenvectors )
} // GetMaxEigenvectors
////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////
template<             class TPixelType, unsigned int Dimension >
void MFAReconstructionBase< TPixelType,              Dimension >
::SetInputFeatures(
    const VectorImageType          * inputFeatures,
    const FeatureIDConstCollection & featureIDs )
{
  
  if ( !inputFeatures )
    {
    throw itk::ExceptionObject( __FILE__, __LINE__,
      "inputFeatures is NULL!" );
    }

  // first check if the sizes are correct
  const auto numberOfFeatures = inputFeatures->GetNumberOfComponentsPerPixel();
  if ( numberOfFeatures != featureIDs.size() )
    {
    throw itk::ExceptionObject( __FILE__, __LINE__,
      "inputFeatures has a different number of features than featureIDs!" );
    }

  // Okay, assign it.
  // We will check if the featureIDs exist
  // later, when we actually run, because
  // the user might want to call this
  // function before setting the trained mfa,
  // or the trained MFA might change later.
  this->m_SecretData->m_InputFeatures   = inputFeatures;
  this->m_SecretData->m_InputFeatureIDs = featureIDs;

} // SetInputFeatures
////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////
// set the trained mfa
template<             class TPixelType, unsigned int Dimension >
void MFAReconstructionBase< TPixelType,              Dimension >
//::SetTrainedMFA( const MFABaseConstPointer trainedMFA )
::SetTrainedMFA( const MFABaseType & trainedMFA )
{

  // extract a shared pointer and store it
  this->m_SecretData->m_TrainedMFA = trainedMFA.shared_from_this();

} // SetTrainedMFA
////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////
template<             class TPixelType, unsigned int Dimension >
void MFAReconstructionBase< TPixelType,              Dimension >
::Initialize()
{

  // show progress?
  const bool verbose = this->GetVerbose();

  // check to make sure we have a trained MFA
  const typename SecretData::MFABaseConstPointer & mfa = this->m_SecretData->m_TrainedMFA;
  if ( !mfa )
    {
    throw itk::ExceptionObject( __FILE__, __LINE__,
      "No trained MFA exists!" );
    return;
    }

  // check that each feature exists
  const FeatureIDConstCollection & whichFeatures = this->m_SecretData->m_InputFeatureIDs;
  BOOST_FOREACH( const FeatureIDConstPointer & whichFeature, whichFeatures )
    {
    if ( ! mfa->HasFeature( whichFeature ) )
      {
      throw itk::ExceptionObject( __FILE__, __LINE__,
        "An input feature to use for the reconstruction was not trained!" );
      return;
      } // if not has feature
    } // for each feature

  // sample the means and eigenvectors
  if ( verbose ) std::cout << "Sampling mean and eigenvector images of the MFA:" << std::endl;
  this->SampleMeanAndEigenvectors();
  if ( verbose ) std::cout << "Sampling mean and eigenvector images of the MFA done." << std::endl;

  // calculate the pseudo inverses
  if ( verbose ) std::cout << "Calculating pseudoinverses:" << std::endl;
  this->m_SecretData->CalculatePseudoInverses();
  if ( verbose ) std::cout << "Calculating pseudoinverses done." << std::endl;

  this->m_SecretData->m_Initialized = true;

} // Initialize
////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////
template<             class TPixelType, unsigned int Dimension >
void MFAReconstructionBase< TPixelType,              Dimension >::SecretData
::SampleMeanAndEigenvectors( const FeatureIDConstPointer & whichFeature )
{

  // extract the trained mfa
  const MFABaseConstPointer & mfa = this->m_TrainedMFA;

  // extract the sample locations
  const SampleLocationsContainer & sampleLocations = this->m_FixedSampleLocations;

  // extract the feature
  const VectorImageConstPointer meanAndEigenvectors =
    mfa->GetMeanAndEigenvectors( whichFeature );
  
  // where will we store the output?
  MeanVectorMap         & meanVectors         = this->m_MeanVectors;
  EigenvectorsMatrixMap & eigenvectorMatrices = this->m_EigenvectorMatrices;

  // make sure it exists
  if ( !meanAndEigenvectors )
    {
    std::cerr << "Warning: One of the features does not exist. Skipping." << std::endl;
    return;
    }

  // prepare the locations to store the output
  typedef std::shared_ptr< MeanVectorType         > MeanVectorPointer;
  typedef std::shared_ptr< EigenvectorsMatrixType > EigenvectorsMatrixPointer;
  MeanVectorPointer         meanVector        ( new MeanVectorType         );
  EigenvectorsMatrixPointer eigenvectorsMatrix( new EigenvectorsMatrixType );

  // sample it
  ccipd::SampleMeanAndEigenvectors< PixelType, Dimension >(
    meanAndEigenvectors,
    sampleLocations,
    *meanVector,
    *eigenvectorsMatrix );

  // normalize
  if ( this->m_NormalizeMeanAndSamples )
    {
    std::cout << "Normalizing mean." << std::endl;
    meanVector->normalize();
    }

  // now iteigenvectorsMatrix is simply N x d1
  // (where d1 is the number of eigenvectors for that
  // feature). However, we want each feature to
  // be N x d (where d is constant), which is what
  // we use the linked matrix for

  // extract the subsection of the linked matrix
  // for this feature
  const EigenvectorsMatrixPointer
    linkedSubMatrix = this->m_TrainedMFA->GetLinkedMatrix( whichFeature );
  
  // now we have N x d1 which we multiply by d1 x d (the linked matrix)
  // to yield N x d

  const signed int currentNumberOfEigenvectors =
    static_cast< signed int >( eigenvectorsMatrix->cols() );
  
  // make sure it's the correct size
  if ( linkedSubMatrix->rows() != currentNumberOfEigenvectors )
    {
    throw itk::ExceptionObject( __FILE__, __LINE__,
      "The linked matrix has the wrong number of rows!" );
    }

  // multiply to yield N x d
  *eigenvectorsMatrix *= *linkedSubMatrix;

  // we might need to "crop" it by eliminating some columns
  const signed int maxEigenvectors = this->m_MaxEigenvectors;
  if ( currentNumberOfEigenvectors > maxEigenvectors )
    {
    eigenvectorsMatrix->conservativeResize(
      eigenvectorsMatrix->rows(),
      maxEigenvectors );
    }
  
  // now that we've 
  // store the sampled values
  meanVectors        [ whichFeature ] = meanVector;
  eigenvectorMatrices[ whichFeature ] = eigenvectorsMatrix;

} // SampleMeanAndEigenvectors 
////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////
template<             class TPixelType, unsigned int Dimension >
void MFAReconstructionBase< TPixelType,              Dimension >
::SampleMeanAndEigenvectors()
{

  // show progress?
  const bool verbose = this->GetVerbose();

  // get the data
  SampleLocationsContainer & sampleLocations = this->m_SecretData->m_FixedSampleLocations;
   
  // extract our locations to sample
  // by calling the child function
  // which each child class has to override
  if ( verbose ) std::cout << "Calculating locations to sample:" << std::endl;
  sampleLocations = this->CalculateSampleLocations();
  if ( verbose )
    std::cout << "Calculating locations to sample done." << std::endl <<
      "Number of locations = " << sampleLocations.size() << std::endl;

  // check to make sure some are specified
  if ( sampleLocations.empty() )
    {
    throw itk::ExceptionObject( __FILE__, __LINE__,
      "No locations to sample!" );
    return;
    }

  // extract which features to use
  const FeatureIDConstCollection & whichFeatures = this->m_SecretData->m_InputFeatureIDs;

  // we will store the eigenvectors to later concatenate
  typedef typename SecretData::EigenvectorsMatrixConstPointer EigenvectorsMatrixConstPointer;
  typedef typename SecretData::MeanVectorConstPointer MeanVectorConstPointer;
  typedef typename SecretData::MeanVectorType         MeanVectorType;
  std::vector< EigenvectorsMatrixConstPointer > sampledUnconcatenatedEigenvectors;
  std::vector< MeanVectorConstPointer         > sampledUnconcatenatedMeans;

  // go through each feature
  BOOST_FOREACH( const FeatureIDConstPointer & whichFeature, whichFeatures )
    {

    if ( verbose ) std::cout << "Sample mean and eigenvectors for feature " <<
      whichFeature->GetHash() << ":" << std::endl;

    // sample
    this->m_SecretData->SampleMeanAndEigenvectors( whichFeature );

    // add it to our collection of unconcatenated matrices
    sampledUnconcatenatedEigenvectors.push_back(
      this->m_SecretData->m_EigenvectorMatrices[ whichFeature ] );
    sampledUnconcatenatedMeans.push_back(
      this->m_SecretData->m_MeanVectors[ whichFeature ] );

    if ( verbose ) std::cout << "Sample mean and eigenvectors for feature " <<
      whichFeature->GetHash() << " done." << std::endl;

    } // for each feature

  // now concatenate all of them since they are all N x d (so it's 5N x d, for example)
  if ( verbose ) std::cout << "Concatenating sampled eigenvectors:" << std::endl;
  this->m_SecretData->m_ConcatenatedDrivingEigenvectors =
    ConcatenateMatrixRows( sampledUnconcatenatedEigenvectors );
  if ( verbose ) std::cout << "Concatenating sampled eigenvectors done." << std::endl;

  // concatenate the means
  if ( verbose ) std::cout << "Concatenating sampled means:" << std::endl;
  this->m_SecretData->m_ConcatenatedMeanVectors =
    ConcatenateVectors( sampledUnconcatenatedMeans );
  if ( verbose ) std::cout << "Concatenating sampled means done." << std::endl;

} // SampleMeanAndEigenvectors
////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////
template<             class TPixelType, unsigned int Dimension >
void MFAReconstructionBase< TPixelType,              Dimension >::SecretData
::CalculatePseudoInverses()
{

  const bool verbose = this->m_Verbose;

  const EigenMatrixTypeF & concatenatedEigenvectors =
    *this->m_ConcatenatedDrivingEigenvectors;

  if ( this->m_UseVarianceWeights )
    {

    // see the comment above
    const unsigned int
      numberOfRandomReconstructions = 100;
    
    std::vector< VectorConstPointer > unconcatenatedWeights;

    // go through each feature
    const FeatureIDConstCollection & whichFeatures = this->m_InputFeatureIDs;
    BOOST_FOREACH( const FeatureIDConstPointer & whichFeature, whichFeatures )
      {

      if ( verbose ) std::cout << "Estimating variance for feature " <<
        whichFeature->GetHash() << ":" << std::endl;

      const EigenMatrixTypeF & currentFeatureEigenvectors =
        *this->m_EigenvectorMatrices[ whichFeature ];

      const unsigned int
        randomRows = static_cast< unsigned int >( currentFeatureEigenvectors.cols() ),
        randomCols = numberOfRandomReconstructions;
      
      // Psi is eigenvectors 5N x d
      // (where 5 is the # of driving features for example)
      // RandomNormal is a random set of values
      // from a normal distrubtion, d x ?
      // ( Psi.^2 )( RandomNormal.^2 )
      // yields N x ?
      // where the mean of each row is the variance
      // of ? random reconstructions
      typedef ccipd::UnivariateGaussianDistribution<
        EigenMatrixTypeF::Scalar > GaussianDistributionType;
      const GaussianDistributionType::Pointer
        gaussianPointer = GaussianDistributionType::New();
      GaussianDistributionType & gaussian = *gaussianPointer;
      gaussian.SetMean    ( 0 );
      gaussian.SetVariance( 1 );
      const EigenMatrixConstPointerF
        randomNormal = gaussian.GetRandomMatrix( randomRows, randomCols );
      const EigenMatrixTypeF
        eigenvectorsSquared = currentFeatureEigenvectors.array().abs2(),
        randomNormalSquared = randomNormal->array().abs2(),
        variancesUnsummed   = eigenvectorsSquared * randomNormalSquared;

      // then calculate the mean of each row.
      // the sum is slightly faster but less intuitive
      const EigenVectorTypeF variances = variancesUnsummed.rowwise().mean();

      if ( verbose )
        std::cout << "Estimating variance for feature " <<
          whichFeature->GetHash() << " done." << std::endl <<
          "First 10 values = " << variances.head( 10 ) << std::endl;

      // the weights are 1 / variances
      const std::shared_ptr< VectorType > weights( new VectorType );
      *weights = EigenVectorTypeF::Constant(
        currentFeatureEigenvectors.rows(), 1.0 ).array() / variances.array();

      // save it
      this->m_PseudoInverseWeights[ whichFeature ] = weights;
      unconcatenatedWeights.push_back( weights );

      } // whichFeature

    if ( verbose )
      std::cout << "Calculating weighted pseudo-inverse of a " <<
        concatenatedEigenvectors.rows() << " x " << concatenatedEigenvectors.cols() <<
        " matrix:" << std::endl;

    // concatenate the weights for the pseudo inverse
    const VectorConstPointer concatenatedWeights =
      ccipd::ConcatenateVectors( unconcatenatedWeights );

    // calculate the entire weighted pseudo-inverse
    this->m_DrivingEigenvectorsPseudoInverse =
      ccipd::CalculatePseudoInverseUsingQR(
        concatenatedEigenvectors, *concatenatedWeights );

    if ( verbose )
      std::cout << "Calculating weighted pseudo-inverse of a " <<
        concatenatedEigenvectors.rows() << " x " << concatenatedEigenvectors.cols() <<
        " matrix done." << std::endl;

    } // use variance weights
  else // no weights
    {
      
    if ( verbose )
      std::cout << "Calculating weighted pseudo-inverse of a " <<
        concatenatedEigenvectors.rows() << " x " << concatenatedEigenvectors.cols() <<
        " matrix:" << std::endl;

    this->m_DrivingEigenvectorsPseudoInverse =
      ccipd::CalculatePseudoInverseUsingQR( concatenatedEigenvectors );
    
    if ( verbose )
      std::cout << "Calculating weighted pseudo-inverse of a " <<
        concatenatedEigenvectors.rows() << " x " << concatenatedEigenvectors.cols() <<
        " matrix done." << std::endl;

    } // weights?

} // CalculatePseudoInverses
////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////
template< class TPixelType, unsigned int Dimension >
void
MFAReconstructionBase< TPixelType, Dimension >
::CalculateLinkedProjection()
{
  
  // this version of the function will simply use
  // the fixed sample locations for the linked projection
  this->CalculateLinkedProjection( this->m_SecretData->m_FixedSampleLocations );

} // CalculateLinkedProjection
////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////
template< class TPixelType, unsigned int Dimension >
void
MFAReconstructionBase< TPixelType, Dimension >
::CalculateLinkedProjection(
  const SampleLocationsContainer & sampleLocations,
  const bool                       onlySample )
{

  // extract the data
  const bool
    debug   = false,
    verbose = debug ||
      ( this->GetVerbose() && rand() < ( RAND_MAX * 1e-3 ) );

  if ( verbose ) std::cout << "Sampling input features:" << std::endl;

  const typename SecretData::VectorImageConstPointer & inputFeatures =
    this->m_SecretData->m_InputFeatures;
  const FeatureIDConstCollection & whichFeatures =
    this->m_SecretData->m_InputFeatureIDs;

  // first, sample our input features
  const auto currentSampledValues =
    ccipd::SampleImageToEigenVectors< PixelType, ImageDimension >(
      inputFeatures, sampleLocations );
  if ( verbose ) std::cout << "Sampling input features done." << std::endl;

  // will we normalize our samples?
  const bool normalizeSampleValues = this->m_SecretData->m_NormalizeMeanAndSamples;

  // remember them for later
  typename SecretData::SampledValuesMap & allSampledValues =
    this->m_SecretData->m_SampledValues;
  std::size_t sampledValuesIndex = 0;
  std::vector< SampledValuesConstPointer > currentSampledValuesConst;
  BOOST_FOREACH( const FeatureIDConstPointer & whichFeature, whichFeatures )
    {

    // get the values for this feature
    const auto currentSampledValuesForThisFeature =
      currentSampledValues[ sampledValuesIndex++ ];

    // normalize them
    if ( normalizeSampleValues )
      {
      std::cout << "Normalizing." << std::endl;
      currentSampledValuesForThisFeature->normalize();
      }

    if ( debug && sampleLocations.size() >= 20 )
      {
      
      std::cout << "Feature = " << whichFeature->GetHash() << std::endl;
      for ( unsigned int pixelIndex = 0; pixelIndex < 20; ++pixelIndex )
        {
        std::cout << "Point = " << sampleLocations[ pixelIndex ] << std::endl;
        std::cout << "Value = " <<
          currentSampledValuesForThisFeature->coeff( pixelIndex ) << std::endl;
        VolumeImageBase::IndexType index;
        inputFeatures->TransformPhysicalPointToIndex( sampleLocations[ pixelIndex ], index );
        std::cout << "Index = " << index << std::endl;
        std::cout << "Image value = " << inputFeatures->GetPixel( index ) << std::endl;
        }
        
      } // debug
      
    // remember the sampled values
    currentSampledValuesConst.push_back( currentSampledValuesForThisFeature );
    allSampledValues[ whichFeature ] = currentSampledValuesForThisFeature;

    } // for each feature

  // contatenate them
  const SampledValuesConstPointer concatenatedSampledValues =
    ConcatenateVectors( currentSampledValuesConst );

  if ( !onlySample )
    {

    // use the concatenated p-inverse to calculate the linked projection

    // calculate the linked projection
    typedef VectorType ProjectionType;
    typedef std::shared_ptr< ProjectionType > ProjectionPointer;
    const ProjectionPointer linkedProjection( new ProjectionType );
    *linkedProjection =
      *this->m_SecretData->m_DrivingEigenvectorsPseudoInverse *
      ( *concatenatedSampledValues - *this->m_SecretData->m_ConcatenatedMeanVectors );

    if ( verbose )
      std::cout << "Linked projection = " <<
        linkedProjection->transpose() << std::endl;

    // output
    // return linkedProjection;
    this->m_SecretData->m_LinkedProjection = linkedProjection;

    } // onlySample

} // CalculateLinkedProjection
////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////
template< class TPixelType, unsigned int Dimension >
typename
MFAReconstructionBase< TPixelType, Dimension >
::ReconstructionConstPointer
MFAReconstructionBase< TPixelType, Dimension >
::GetReconstruction( const FeatureIDConstPointer & whichFeature ) const
{

  typedef typename SecretData::ProjectionConstPointer   ProjectionConstPointer;
  typedef typename ProjectionConstPointer::element_type ProjectionType;

  // get the data
  const MFABaseType
    & trainedMFA = *this->m_SecretData->m_TrainedMFA; 
  const ProjectionConstPointer
    & linkedProjection = this->m_SecretData->m_LinkedProjection;
  const typename SecretData::MeanVectorMap
    & meanVectors = this->m_SecretData->m_MeanVectors;
  const typename SecretData::EigenvectorsMatrixMap
    & eigenvectorMatrices = this->m_SecretData->m_EigenvectorMatrices;

  // make sure we have that feature
  if ( !trainedMFA.HasFeature( whichFeature ) )
    {
    std::cerr << "Error: MFA doesn't have the feature!" << std::endl;
    return ReconstructionConstPointer();
    }

  /*
  // make sure there's a linked projection calculated
  if ( !linkedProjection )
    {
    std::cerr << "Error: no linked projection!" << std::endl;
    return ReconstructionConstPointer();
    }
  */

  // calculate the reconstruction

  // first, make sure we've sampled
  typename SecretData::MeanVectorMap::const_iterator
    meanVectorIter = meanVectors.find( whichFeature );
  if ( meanVectorIter == meanVectors.cend() )
    {
    // nope, we must sample it
    this->m_SecretData->SampleMeanAndEigenvectors( whichFeature );
    
    // now, point the iterator to the correct spot
    meanVectorIter = meanVectors.find( whichFeature );

    } // not sampled

  const volatile bool debug = 
    //false;
    rand() < ( RAND_MAX * .001 );
    //true;

  if ( debug )
    {
    if ( linkedProjection )
      std::cout << "Linked projections = " << linkedProjection->transpose() << std::endl;
    else
      std::cout << "No linked projections." << std::endl;
    }

  // extract the mean and eigenvectors
  const typename SecretData::MeanVectorType
    & meanVector= *meanVectorIter->second; // no reason to discount the existing iterator 
  const typename SecretData::EigenvectorsMatrixType
    & eigenvectorsMatrix = *eigenvectorMatrices.at( whichFeature );

  // use the feature projection to calculate the reconstruction
  typedef std::shared_ptr< ReconstructionType > ReconstructionPointer;
  const ReconstructionPointer reconstruction( new ReconstructionType ); 
  *reconstruction = linkedProjection ?
    ( meanVector + eigenvectorsMatrix * ( *linkedProjection ) ) :
    meanVector;

  // output
  return reconstruction;
  
} // GetReconstruction
////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////
template< class TPixelType, unsigned int Dimension > typename
MFAReconstructionBase< TPixelType, Dimension >::MFABaseConstPointer
MFAReconstructionBase< TPixelType, Dimension >
::GetTrainedMFA() const
{
  return this->m_SecretData->m_TrainedMFA;
} // GetTrainedMFA
////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////
template< class TPixelType, unsigned int Dimension > typename
MFAReconstructionBase< TPixelType, Dimension >::SampleLocationsContainer
MFAReconstructionBase< TPixelType, Dimension >
::GetSampleLocations() const
{
  return this->m_SecretData->m_FixedSampleLocations;
} // GetSampleLocations
////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////
template< class TPixelType, unsigned int Dimension >
FeatureIDConstCollection
MFAReconstructionBase< TPixelType, Dimension >
::GetInputFeatureIDs() const
{
  return this->m_SecretData->m_InputFeatureIDs;
} // GetInputFeatureIDs
////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////
template< class TPixelType, unsigned int Dimension >
bool
MFAReconstructionBase< TPixelType, Dimension >
::GetUseVarianceWeights() const
{
  return this->m_SecretData->m_UseVarianceWeights;
} // GetUseVarianceWeights
////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////
template< class TPixelType, unsigned int Dimension >
void
MFAReconstructionBase< TPixelType, Dimension >
::SetUseVarianceWeights( const bool useVarianceWeights )
{
  this->m_SecretData->m_UseVarianceWeights = useVarianceWeights;
} // SetUseVarianceWeights
////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////
template< class TPixelType, unsigned int Dimension >
typename 
MFAReconstructionBase< TPixelType, Dimension >
::VectorImageConstPointer
MFAReconstructionBase< TPixelType, Dimension >
::GetInputFeatures() const
{
  return this->m_SecretData->m_InputFeatures;
}
////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////
template< class TPixelType, unsigned int Dimension > typename
MFAReconstructionBase< TPixelType, Dimension >::VectorConstPointer
MFAReconstructionBase< TPixelType, Dimension >
::GetPseudoInverseWeights( const FeatureIDConstPointer & whichFeature ) const
{

  if ( this->GetUseVarianceWeights() )
    return this->m_SecretData->m_PseudoInverseWeights.at( whichFeature );
  else
    return VectorConstPointer();

} // GetPseudoInverseWeights
////////////////////////////////////////////////////////////////////////////////////////////////////

    

////////////////////////////////////////////////////////////////////////////////////////////////////
template< class TPixelType, unsigned int Dimension >
typename 
MFAReconstructionBase< TPixelType, Dimension >
::SampledValuesConstPointer
MFAReconstructionBase< TPixelType, Dimension >
::GetSampledValues( const FeatureIDConstPointer & whichFeature ) const
{

  // get the data
  const typename SecretData::SampledValuesMap
    & allSampledValues = this->m_SecretData->m_SampledValues;

  // try to get the values (will throw an error if not sampled)
  try
    {
    // extract the sampled values
    return allSampledValues.at( whichFeature );
    }
  catch( ... )
    {
    std::cerr << "Error: No sampled values to return." << std::endl;
    return SampledValuesConstPointer();
    }

} // GetSampledValues
////////////////////////////////////////////////////////////////////////////////////////////////////



} // namespace itk
#endif // __ccipdReconstructionBase_hxx
