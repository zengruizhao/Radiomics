


//////////////////////////////////////////////////////////////////////////////////////////

// ccipd includes
#include "ccipdSIMEstimator.h"
#include "ccipdSIMStorage.h" // the SIM Storage Base class
#include "ccipdFeatureCollection.h" // all the possible features to train with
#include "ccipdFeatureID.h" // for which features we should use to train
#include "ccipdVolumeContainerToVolumeFeatures.h"
#include "ccipdPCATraining.h" // for eigen analysis of the linked matrix
#include "ccipdMatrixConcatenation.h" // for combining vectors
#include "ccipdFactory.hxx"

// our own itk includes
#include "itkPCAShapeModelEstimatorEigen.h" // for creating the PCA models

// std includes
#include <vector>
#include <unordered_set> // for storing feature collections
#include <unordered_map> // for storing projections

// boost includes
#include <boost/foreach.hpp>

// eigen includes
#include <Eigen/Core> // for matrices
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
using std::cout;
using std::cerr;
using std::endl;
using std::vector;
using std::unordered_map;
using std::unordered_set;
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{


//////////////////////////////////////////////////////////////////////////////////////////
template
class Factory< SIMEstimator >;
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
class SIMEstimator::SecretData
{
public: // public to the SIMEstimator class

  /* Constructor */
  SecretData();

  /** Typedefs */
  typedef EigenVectorTypeF            ProjectionType;
  typedef EigenVectorPointerF         ProjectionPointer;
  typedef EigenVectorConstPointerF    ProjectionConstPointer;
  typedef EigenVectorConstCollectionF ProjectionConstCollection;

  typedef unsigned int TrainingIndexType;

  typedef unordered_set<
    FeatureCollectionPointer,
    HashableFunctor< FeatureCollectionPointer >,
    HashableFunctor< FeatureCollectionPointer >
  > TrainingFeaturesCollection;

  /** Called by [FeatureID][TrainingIndexType] */
  typedef unordered_map<
    FeatureIDConstPointer,        // Key #1 is which feature
    unordered_map<
      TrainingIndexType,     // Key #2 is which training index
      ProjectionConstPointer // Value is a pointer to the projection
    >,
    HashableFunctor< FeatureIDConstPointer >, // FeatureID is ccipd::Hashable
    HashableFunctor< FeatureIDConstPointer >
  > ProjectionMapType;

  /** PCA Types */
  typedef VolumeImageType         PCAInputType;
  typedef VolumeImageConstPointer PCAInputConstPointer;
  typedef VolumeImageType         PCAOutputType;
  typedef VolumeImageConstPointer PCAOutputConstPointer;

  // define a PCA estimator type
  typedef itk::PCAShapeModelEstimatorEigen<
    PCAInputType,
    PCAOutputType
  > PCAEstimatorType;

  typedef PCAEstimatorType::Pointer      PCAEstimatorPointer;
  typedef PCAEstimatorType::ConstPointer PCAEstimatorConstPointer;

  /** Local storage variables */
  // the projection for each PC
  ProjectionMapType m_Projections;

  /// The amount of variance to retain in all eigenvectors
  double m_VarianceToRetain;

  // which features to input?
  // this must be set prior to actually inputting anything
  FeatureIDConstCollection m_WhichFeatures;

  // each training image will have an associated set of training features
  TrainingFeaturesCollection m_TrainingFeatures;

  // whether to show progress while generating (default = false )
  bool m_Verbose;

  /**
   * This will estimate the PCA result for a given feature.
   */
  virtual PCAEstimatorPointer CalculatePCA(
    const FeatureIDConstPointer & whichFeature );

  /**
   * This will calculate all the projections
   * for a given feature and update m_Projections.
   */
  void CalculateProjections(
    const FeatureIDConstPointer & whichFeature,
    const PCAEstimatorPointer     pcaResult );

  /** The function for linking all the projections and giving it to the SIM */
  virtual void CalculatePCALinkedModel( SIMVolumeStorage & SIM );

  /** Once you've performed PCA, you must give the result to an SIM */
  void GivePCAResultToSIM(
    const FeatureIDConstPointer & whichFeature,
          PCAEstimatorType      & pcaResult,
          SIMVolumeStorage      & SIM ) const;

  /**
   * Given the amount of variance to retain,
   * how many eigenvectors does that correspond to?
   */
  virtual unsigned int GetNumberOfEigenvectorsToRetain(
      const PCAEstimatorType & pcaResult ) const;

  ccipdGetConstDeclareMacro( NumberOfTrainingImages, TrainingIndexType )

}; // SIMEstimator::SecretData
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
// Constructor
SIMEstimator::SecretData::SecretData() :
m_VarianceToRetain( 0.98  ),
m_Verbose         ( false )
{
}

SIMEstimator::SIMEstimator() :
ccipdNewPIMPLMacro
{
} // constructor
SIMEstimator::~SIMEstimator()
{
}
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void SIMEstimator::AddTrainingFeatures(
  const FeatureCollectionPointer trainingFeatures )
{

  // give us the input
  this->m_SecretData->m_TrainingFeatures.insert( trainingFeatures );

} // AddTrainingFeatures
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void SIMEstimator::RemoveTrainingFeatures(
  const FeatureCollectionPointer trainingFeatures )
{

  this->m_SecretData->m_TrainingFeatures.erase( trainingFeatures );

} // RemoveTrainingFeatures
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
SIMVolumeStoragePointer SIMEstimator::CalculateSIM()
{

  const bool verbose = this->GetVerbose();

  // prepare the output
  const SIMVolumeStoragePointer trainedSIM = SIMVolumeStorage::New();

  // go through each feature in parallel
  // visual studio requires them to be signed ints
  const int
    numberOfFeatures = static_cast< int >(
      this->GetNumberOfFeatures() ),
    numberOfTrainingImages = static_cast< int >(
      this->m_SecretData->GetNumberOfTrainingImages() );

  if ( numberOfTrainingImages < 1 )
    {
    cerr << "You need to input at least 1 training image!" << endl;
    return SIMVolumeStoragePointer();
    }

  const FeatureIDConstCollection & whichFeatures =
    this->m_SecretData->m_WhichFeatures;

  //# pragma omp parallel for
  for ( int featureIndex = 0;
    featureIndex < numberOfFeatures;
    ++featureIndex )
    {

    if ( verbose )
      {
      # pragma omp critical
        {
        cout << "PCA for feature " << featureIndex << " begun." << endl;
        }
      } // verbose

    const FeatureIDConstPointer whichFeature = whichFeatures[ featureIndex ];

    // perform PCA estimation for the given feature
    typedef SecretData::PCAEstimatorPointer PCAEstimatorPointer;

    const PCAEstimatorPointer pcaResult =
      this->m_SecretData->CalculatePCA( whichFeature );

    // give the output to the SIM
    this->m_SecretData->GivePCAResultToSIM( whichFeature, *pcaResult, *trainedSIM );

    if ( verbose )
      {
      # pragma omp critical
        {
        cout << "PCA for feature " << featureIndex << " done." << endl;
        cout << "Eigenvalues = " << endl << pcaResult->GetEigenValues() << endl;
        cout << "Calculating projections for feature " <<
          featureIndex << " begun." << endl;
        }
      } // verbose

    this->m_SecretData->CalculateProjections( whichFeature, pcaResult );

    if ( verbose )
      {
      # pragma omp critical
        {
        cout << "Calculating projections for feature " <<
          featureIndex << " done." << endl;
        }
      } // verbose

    } // featureIndex

  // after everything has been estimated, link the models
  // and give the linked matrix to the SIM
  this->m_SecretData->CalculatePCALinkedModel( *trainedSIM );
  
  return trainedSIM;

} // CalculateSIM
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
unsigned int SIMEstimator::SecretData::GetNumberOfEigenvectorsToRetain(
    const PCAEstimatorType & pcaResult ) const
{
  return pcaResult.GetNumberOfPrincipalComponentsRequired();
} // GetNumberOfEigenvectorsToRetain
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void SIMEstimator::SecretData::GivePCAResultToSIM(
  const FeatureIDConstPointer & whichFeature,
        PCAEstimatorType      & pcaResult,
        SIMVolumeStorage      & SIM ) const
{

  // first, we extract all the mean and eigenvector images into a collection
  VolumeConstImagesContainer meanAndEigenvectors;

  // the mean
  const PCAOutputConstPointer meanImage = pcaResult.GetOutput( 0 );
  meanAndEigenvectors.push_back( meanImage );

  // the eigenvectors
  const unsigned int numberOfEigenvectors =
    pcaResult.GetNumberOfPrincipalComponentsRequired();   

  // the eigenvalues
  const auto eigenvalues = pcaResult.GetEigenValues();

  for ( unsigned int eigenvectorIndex = 0;
    eigenvectorIndex < numberOfEigenvectors; ++eigenvectorIndex )
    {

    const unsigned int outputIndex = eigenvectorIndex + 1; // 1st is mean

    // extract the eigenvector image
    // which is already multiplied by the square
    // root of the eigenvalue
    const PCAOutputConstPointer eigenvectorImage =
      pcaResult.GetOutput( outputIndex );

    // give the multiplied eigenvector image to the collection
    meanAndEigenvectors.push_back( eigenvectorImage );

    } // for each eigenvector

  // turn it into a vector image to give to the SIM
  const VolumeFeaturesConstPointer meanAndEigenvectorsAsVectorImage =
    VolumeContainerToVolumeFeaturesConst( meanAndEigenvectors );
  
  // give it to the SIM
  SIM.AddFeature( whichFeature, meanAndEigenvectorsAsVectorImage );

} // GivePCAResultToSIM
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
SIMEstimator::SecretData::PCAEstimatorPointer
SIMEstimator::SecretData::CalculatePCA(
  const FeatureIDConstPointer & whichFeature )
{

  // create the PCA object
  PCAEstimatorPointer estimator = PCAEstimatorType::New();
  const PCAEstimatorPointer badOutput;

  // how many training images do we have?
  //const TrainingIndexType numberOfTrainingImages =
  //  this->GetNumberOfTrainingImages();

  // define the estimator's properties
  // retain all eigenvectors for now (will prune later)
  const unsigned int numberOfTrainingImages =
    this->GetNumberOfTrainingImages();
  estimator->SetNumberOfTrainingImages(
    numberOfTrainingImages );
  estimator->SetNumberOfPrincipalComponentsRequired(
    numberOfTrainingImages );

  // go through each training image
  TrainingIndexType trainingIndex = 0;
  BOOST_FOREACH( FeatureCollectionPointer currentTrainingFeatures, m_TrainingFeatures )
    {

    // extract the given feature
    PCAInputConstPointer currentFeature;
    #pragma omp critical
      {
      currentFeature = currentTrainingFeatures->GetFeature( whichFeature ).at(0);
      }

    if ( !currentFeature )
      {
      cerr << endl << "Error: no feature " <<
        whichFeature->GetDescription() << endl;
      return badOutput;
      }
    // give it to our estimator
    estimator->SetInput( trainingIndex++, currentFeature );

    } // for training feature

  // calculate PCA
  try
    {
    estimator->Update();
    }
  catch( itk::ExceptionObject & exception )
    {
    #pragma omp critical
      {
      cerr << endl << "Error in file " << __FILE__ << 
        " at line " << __LINE__ << ":" << endl << exception << endl;
      } // critical
    return badOutput;
    } // catch
  catch ( ... )
    {
    #pragma omp critical
      {
      cerr << endl << "Unknown error in file " << __FILE__ << 
        " at line " << __LINE__ << "." << endl;
      } // critical
    return badOutput;
    } // catch

  // output
  return estimator;

} // EstimateShapeModel
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void SIMEstimator::SecretData::CalculateProjections(
    const FeatureIDConstPointer & whichFeature,
    const PCAEstimatorPointer     pcaResult )
{

  // go through each training image
  for ( TrainingIndexType trainingIndex = 0;
    trainingIndex < m_TrainingFeatures.size();
    ++trainingIndex )
    {
    this->m_Projections[ whichFeature ][ trainingIndex ] =
      pcaResult->GetProjection( trainingIndex );
    } // for trainingIndex
  
} // CalculateProjections
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void SIMEstimator::SecretData::CalculatePCALinkedModel( SIMVolumeStorage & SIM )
{

  // get variables
  const bool verbose = this->m_Verbose;

  // get our features
  const FeatureIDConstCollection & whichFeatures = this->m_WhichFeatures;

  // get our # of training images
  const TrainingIndexType numberOfTrainingImages =
    this->GetNumberOfTrainingImages();

  // what are our projections?
  const ProjectionMapType & projections = this->m_Projections;

  if ( verbose ) cout << "Concatenating projections:" << endl;

  // prepare a matrix to store all our projections
  EigenMatrixTypeF concatenatedProjections;

  // how do we store the row #'s?
  typedef SIMVolumeStorage::WhichRowsType WhichRowsType;
  WhichRowsType
    startingRows,
    numberOfRows;
  typedef SIMVolumeStorage::LinkedMatrixIndexType LinkedMatrixIndexType;

  // go through each training image
  for ( TrainingIndexType trainingIndex = 0;
    trainingIndex < numberOfTrainingImages; ++trainingIndex )
    {

    // we will store a collection of all the projections
    // for a given training image
    ProjectionConstCollection projectionsForCurrentTrainingImage;

    LinkedMatrixIndexType cumulativeNumberOfRows = 0;

    // go through each feature
    BOOST_FOREACH(
      const FeatureIDConstPointer & whichFeature, whichFeatures )
      {

      // extract the current projection
      // we have to use at since it's constant
      const ProjectionConstPointer currentProjection =
        projections.at( whichFeature ).at( trainingIndex );

      if ( verbose )
        cout << "Projection for training " << trainingIndex <<
          " = " << endl << *currentProjection << endl;

      // add it to our list
      projectionsForCurrentTrainingImage.push_back( currentProjection );

      if ( trainingIndex == 0 ) // only need it for one training image
        {

        // how big is the current projection?
        const LinkedMatrixIndexType
          currentNumberOfRows = currentProjection->size(),
          startingRow         = cumulativeNumberOfRows; // start at the end of the last one

        // update the number of rows
        cumulativeNumberOfRows += currentNumberOfRows;

        // save it
        startingRows[ whichFeature ] = startingRow;
        numberOfRows[ whichFeature ] = currentNumberOfRows;

        } // first training image

      } // for each feature

    // now turn it into an Eigenvector by concatenating
    const EigenVectorPointerF concatenatedProjectionsForCurrentTrainingImage =
      ConcatenateVectors( projectionsForCurrentTrainingImage );

    if ( verbose )
      cout << "Concatenated projections = " << endl <<
        *concatenatedProjectionsForCurrentTrainingImage << endl;

    // we might have to resize
    if ( concatenatedProjections.rows() == 0 )
      concatenatedProjections.resize(
        concatenatedProjectionsForCurrentTrainingImage->size(),
        numberOfTrainingImages );

    // add it to our large matrix
    concatenatedProjections.col( trainingIndex) =
      *concatenatedProjectionsForCurrentTrainingImage;

    } // for each training image

  if ( verbose )
    {
    cout << "Concatenating projections done." << endl;
    cout << endl << "Projection matrix = " << endl <<
      concatenatedProjections << endl << endl;
    cout << "Calculating covariance matrix:" << endl;
    } // verbose

  // calculate covariance matrix
  const EigenMatrixTypeF covarianceMatrix =
    ( concatenatedProjections * concatenatedProjections.transpose() ).array() /
    static_cast< float >( numberOfTrainingImages - 1 );

  if ( verbose )
    {
    cout << "Calculating covariance matrix done." << endl;
    cout << endl << "Covariance matrix = " << endl << covarianceMatrix << endl << endl;
    }

  // calculate eigen vectors
  if ( verbose ) cout << "Calculating eigenvectors for the linked matrix:" << endl;

  const double varianceToRetain = this->m_VarianceToRetain;

  // and eigenvalues
  EigenVectorTypeF eigenvalues;

  const EigenMatrixPointerF eigenvectors =
    CalculateEigenvectorsOfCovariance(
      covarianceMatrix,
      eigenvalues,
      static_cast< float >( varianceToRetain ),
      verbose );

  // multiply each eigenvector by its standard deviation
  const EigenVectorTypeF::Index numberOfEigenvectors = eigenvalues.size();
  for ( EigenVectorTypeF::Index eigenvectorIndex = 0;
    eigenvectorIndex < numberOfEigenvectors; ++eigenvectorIndex )
    {
    eigenvectors->col( eigenvectorIndex ).array() *=
      std::sqrt( eigenvalues[ eigenvectorIndex ] );
    } // for eigenvectorIndex

  if ( verbose ) cout << "Calculating eigenvectors for the linked matrix done." << endl;

  // our linked matrix is simply the eigenvectors
  SIM.SetLinkedMatrix( eigenvectors, startingRows, numberOfRows );

} // EstimateLinkedModel
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
unsigned int SIMEstimator::GetNumberOfFeatures() const
{
  return static_cast< unsigned int >( this->GetWhichFeatures().size() );
}
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
std::size_t SIMEstimator::GetNumberOfTrainingFeatures() const
{
  return this->m_SecretData->GetNumberOfTrainingImages();
}
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
SIMEstimator::SecretData::TrainingIndexType
SIMEstimator::SecretData::GetNumberOfTrainingImages() const
{
  return static_cast< TrainingIndexType >( this->m_TrainingFeatures.size() );
}
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
ccipdSetByvalImplementMacro( SIMEstimator, VarianceToRetain, double )
ccipdGetConstImplementMacro( SIMEstimator, VarianceToRetain, double )  

ccipdSetByrefImplementMacro( SIMEstimator, WhichFeatures, FeatureIDConstCollection )
ccipdGetConstImplementMacro( SIMEstimator, WhichFeatures, FeatureIDConstCollection )

ccipdSetByvalImplementMacro( SIMEstimator, Verbose, bool )
ccipdGetConstImplementMacro( SIMEstimator, Verbose, bool )
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace ccipd
