#ifndef __itKPCAShapeModelEstimatorEigen_hxx
#define __itKPCAShapeModelEstimatorEigen_hxx



//////////////////////////////////////////////////////////////////////////////////////////

// ccipd includes
#include "ccipdPCATraining.h"

// itk includes
#include "itkPCAShapeModelEstimatorEigen.h"

// eigen includes
#include <Eigen/Core>

// std includes
#include <algorithm>

//////////////////////////////////////////////////////////////////////////////////////////



namespace itk
{



//////////////////////////////////////////////////////////////////////////////////////////
/**
 * \brief PIMPL interface to PCAShapeModelEstimatorEigen class.
 * 
 * \author  Rob Toth
 * \ingroup MFA
 * \date    2012-09-16
 */
template < class TInputImage, class TOutputImage >
class PCAShapeModelEstimatorEigen< TInputImage, TOutputImage >
::SecretData
{
public: // public to the PCAShapeModelEstimatorEigen class

  typedef PCAShapeModelEstimatorEigen<
    TInputImage, TOutputImage
  > EncapsulatingClass;
  
  /// Constructor
  SecretData();

  typedef typename
    EncapsulatingClass::ProjectionPointer ProjectionPointer;
  typedef typename
    EncapsulatingClass::Superclass::OutputImagePointer OutputImagePointer;
  typedef typename
    EncapsulatingClass::OutputEigenVectorType OutputEigenVectorType;
  
  typedef std::vector<
    ProjectionPointer
  > ProjectionCollection;
  
  /// Store the training images' projections.
  ProjectionCollection m_Projections;
  
  bool m_Verbose;
  
  /// Assign a vector to an image
  static void SetOutputImage(
    const OutputImagePointer    & output,
    const OutputEigenVectorType & vector );
    
}; // class SecretData
////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////
template < class TInputImage, class TOutputImage >
PCAShapeModelEstimatorEigen< TInputImage, TOutputImage >
::SecretData::SecretData() :
m_Verbose( false )
{
} // constructor
////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////
template < class TInputImage, class TOutputImage >
PCAShapeModelEstimatorEigen< TInputImage, TOutputImage >
::PCAShapeModelEstimatorEigen() :
ccipdNewPIMPLMacro
{
} // constructor
////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////
template < class TInputImage, class TOutputImage >
PCAShapeModelEstimatorEigen< TInputImage, TOutputImage >
::~PCAShapeModelEstimatorEigen()
{
} // destructor
////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////////
template < class TInputImage, class TOutputImage >
void
PCAShapeModelEstimatorEigen< TInputImage, TOutputImage >
::SetVerbose( const bool verbose )
{
  this->m_SecretData->m_Verbose = verbose;
}
////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////////
template < class TInputImage, class TOutputImage >
void
PCAShapeModelEstimatorEigen< TInputImage, TOutputImage >
::SecretData::SetOutputImage(
  const OutputImagePointer    & output,
  const OutputEigenVectorType & vector )
{

  // allocate the image
  output->SetBufferedRegion(
    output->GetRequestedRegion() );
  output->Allocate();
  
  // now we must copy over the data from
  // the eigenvector to the output image
  std::copy(
    vector.data(),
    vector.data() + vector.size(),
    output->GetBufferPointer() );
    
} // SetOutputImage
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
template< class TInputImage, class TOutputImage >
void
PCAShapeModelEstimatorEigen< TInputImage, TOutputImage >
::GenerateData()
{
  this->EstimateShapeModels();
}
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
template< class TInputImage, class TOutputImage >
void
PCAShapeModelEstimatorEigen< TInputImage, TOutputImage >
::EstimateShapeModels()
{

  const bool verbose = this->m_SecretData->m_Verbose;
  using std::cout;
  using std::endl;
  
  // we will use the eigen library
  typedef typename Superclass::InputImagePixelType InputImagePixelType;
  
  typedef Eigen::Matrix<
    InputImagePixelType,
    Eigen::Dynamic, 1
  > InputEigenVectorType;

  // for mapping the data into the eigen library
  // without copying it
  typedef Eigen::Map<
    const InputEigenVectorType
  > InputEigenVectorMap;
  
  typedef std::shared_ptr<
    const InputEigenVectorMap
  > InputEigenVectorMapPointer;

  typedef std::vector<
    InputEigenVectorMapPointer
  > InputEigenVectorCollection;
  
  typedef Eigen::Matrix<
    OutputImagePixelType,
    Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor
  > OutputEigenMatrixType;
  
  typedef std::shared_ptr<
    OutputEigenMatrixType
  > OutputEigenMatrixPointer;
  
  typedef OutputEigenMatrixType CovarianceMatrixType;
  
  // prepare the collection
  InputEigenVectorCollection inputVectorCollection;
  
  // prepare the mean
  OutputEigenVectorType meanVector;
  
  const unsigned int numberOfTrainingImages = this->GetNumberOfTrainingImages();
  for ( unsigned int trainingImageIndex = 0;
    trainingImageIndex < numberOfTrainingImages;
    ++trainingImageIndex )
    {
    
    if ( verbose ) cout << "Training image = " << trainingImageIndex << endl;
    
    // first, get the input image
    typedef typename
      Superclass::InputImageConstPointer
    InputImageConstPointer;
    
    const InputImageConstPointer inputImage =
      dynamic_cast< const TInputImage * >(
        this->ProcessObject::GetInput( trainingImageIndex ) );
        
    if ( !inputImage ) continue;
    
    const itk::SizeValueType numberOfPixels =
      inputImage->GetBufferedRegion().GetNumberOfPixels();
    
    // make sure our mean is initialized
    if ( meanVector.size() == 0 )
      {
      if ( verbose )
        cout << "Resizing mean vector to have " <<
          numberOfPixels << " elements." << endl;
      meanVector = InputEigenVectorType::Zero(
        numberOfPixels );
        
      } // mean vector zero sized
    else if ( static_cast< itk::SizeValueType >(
      meanVector.size() ) != numberOfPixels )
      {
      
      itkExceptionMacro( <<
        "Error: Image should have " <<
        meanVector.size() << " pixels, but has " <<
        numberOfPixels << " pixels. Skipping." );
        
      } // mean vector size
      
    // add it into our collection
    if ( verbose ) cout << "Adding to our collection:" << endl;

    InputEigenVectorMapPointer mappedImage(
      new InputEigenVectorMap(
      inputImage->GetBufferPointer(), numberOfPixels, 1 ) );

    // this should be light weight enough
    // to avoid using smart pointers
    inputVectorCollection.push_back( mappedImage );
    
    if ( verbose ) cout << "Adding to our collection done." << endl;
        
    // add it into our mean
    if ( verbose ) cout << "Adding to our mean:" << endl;
    
    meanVector += mappedImage->template cast< OutputImagePixelType >();
    
    if ( verbose ) cout << "Adding to our mean done." << endl;
    
    } // for trainingImageIndex
    
  // calculate the mean
  if ( verbose ) cout << "Computing mean:" << endl;
  meanVector /= static_cast< OutputImagePixelType >(
    numberOfTrainingImages );
  if ( verbose ) cout << "Computing mean done." << endl;
  
  // now, we need to compute the covariance
  if ( verbose ) cout << "Computing covariance:" << endl;
  
  // create a square covariance matrix
  CovarianceMatrixType covarianceMatrix(
    numberOfTrainingImages, numberOfTrainingImages );
  
  // we only compute the lower part of the triangular matrix
  #pragma omp parallel for
  for ( int col = 0; col < static_cast< int >( numberOfTrainingImages ); ++col )
    {
    for ( unsigned int row = col; row < numberOfTrainingImages; ++row )
      {
      
      // take the dot product of the
      // images cast as the correct type
      const OutputImagePixelType
        dotProduct =
        ( inputVectorCollection[ row ]->template cast<
          OutputImagePixelType >() - meanVector ).dot(
        ( inputVectorCollection[ col ]->template cast<
          OutputImagePixelType >() - meanVector ) );

      // only one thread should write at a time
      #pragma omp critical
        {
        covarianceMatrix.coeffRef( row, col ) = dotProduct;
        }
      
      } // for row
    } // for col
  covarianceMatrix /= static_cast< float >( numberOfTrainingImages - 1 );
  
  if ( verbose )
    {
    cout << "Computing covariance done." << endl <<
      "Covariance matrix = " << endl <<
        CovarianceMatrixType( covarianceMatrix.
          template selfadjointView< Eigen::Lower >() ) << endl;
    } // verbose
  
  // calculate PCA
  
  if ( verbose ) cout << "Computing PCA:" << endl;
  
  OutputEigenVectorType eigenvalues;
  
  const OutputEigenMatrixPointer eigenvectors =
    ccipd::CalculateEigenvectorsOfCovariance< OutputImagePixelType >(
      covarianceMatrix.template selfadjointView< Eigen::Lower >(),
      eigenvalues, 0.98f, verbose );
  
  if ( verbose )
    {
    cout << "Computing PCA done." << endl <<
      "Eigenvalues = " << endl << eigenvalues << endl <<
      "Eigenvectors = " << endl << *eigenvectors << endl;
    } // verbose

  // prepare the outputs
  const unsigned int numberOfEigenvalues =
    static_cast< unsigned int >( eigenvalues.size() );
  this->SetNumberOfPrincipalComponentsRequired( 
    numberOfEigenvalues );
  
  // the mean is the first output
  Self::SecretData::SetOutputImage(
    this->GetOutput( 0 ), meanVector );
  
  typedef typename SecretData::ProjectionCollection ProjectionCollection;
  ProjectionCollection & projections = this->m_SecretData->m_Projections;
  
  // initialize the projections to null pointers
  projections.clear();
  projections.resize( numberOfTrainingImages );
  
  // We must multiply back.
  // If we have input images A as 10^6 x 100,
  // we calculate the eigenvectors of 100x100 cov as u.
  // Then, we need v_i = A * u_i
  if ( verbose )
    {
    cout << "Computing large eigenvectors:" << endl;
    }

  #pragma omp parallel for
  for ( int eigenvalueIndex = 0;
    eigenvalueIndex < static_cast< int >( numberOfEigenvalues );
    ++eigenvalueIndex )
    {
    
    #pragma omp critical
      {
      if ( verbose )
        cout << "Eigenvalue index = " <<
          eigenvalueIndex << endl <<
          "Calculating large eigenvector #" << 
          eigenvalueIndex << ":" << endl;
      } // critical

    // start at 0
    const itk::SizeValueType numberOfPixels =
      static_cast< itk::SizeValueType >( meanVector.size() );
    
    OutputEigenVectorType currentLargeEigenVector =
      OutputEigenVectorType::Zero( numberOfPixels );
    
    // we must sum in each image
    for ( unsigned int trainingImageIndex = 0;
      trainingImageIndex < numberOfTrainingImages;
      ++trainingImageIndex )
      {

      currentLargeEigenVector +=
        ( inputVectorCollection[ trainingImageIndex ]->template
          cast< OutputImagePixelType >() - meanVector ) *
          eigenvectors->coeff( trainingImageIndex, eigenvalueIndex );

      } // trainingImageIndex
    
    currentLargeEigenVector.normalize();
    
    #pragma omp critical
      {
      if ( verbose )
        cout << "Calculating large eigenvector #" << 
          eigenvalueIndex << " done." << endl <<
          "Calculating projection values for eigenvector " << 
          eigenvalueIndex << ":" << endl;
      } // critical

    const OutputImagePixelType standardDeviation =
      std::sqrt(  eigenvalues.coeff( eigenvalueIndex ) );
      
    // go through each training image
    for ( unsigned int trainingImageIndex = 0;
      trainingImageIndex < numberOfTrainingImages;
      ++trainingImageIndex )
      {
      
      // make sure we are initialized
      #pragma omp critical
        {
        if ( verbose )
          cout << "Calculating projection values for training image " << 
            trainingImageIndex << ":" << endl;

        if ( !projections[ trainingImageIndex ] )
          projections[ trainingImageIndex ] = 
            ProjectionPointer( new ProjectionType(
              ProjectionType::Zero( numberOfEigenvalues ) ) );
        } // critical
      
      // calculate the dot product
      const OutputImagePixelType dotProduct =
        currentLargeEigenVector.dot(
        ( inputVectorCollection[ trainingImageIndex ]->template
          cast< OutputImagePixelType >() - meanVector ) )
          / standardDeviation; // put it in terms of standard deviations

      #pragma omp critical
        {
        // fill in the value
        projections[ trainingImageIndex ]->
          coeffRef( eigenvalueIndex ) = dotProduct;
          
        if ( verbose )
          cout << "Calculating projection values for training image " << 
            trainingImageIndex << " done." << endl;

        } // critical
      
      } // trainingImageIndex

    #pragma omp critical
      {
      if ( verbose )
        cout << "Calculating projection values for eigenvector " << 
          eigenvalueIndex << " done." << endl <<
          "Copying eigenvector to output image #" <<
          eigenvalueIndex << ":" << endl;
      } // critical

    // we will also take this time
    // to include the square root of
    // the eigenvalue so that
    // all projections are in terms of
    // standard deviations in the future
    currentLargeEigenVector *= standardDeviation;
    
    #pragma omp critical
      {
      Self::SecretData::SetOutputImage(
        this->GetOutput( eigenvalueIndex + 1 ), // 0 is for the mean
        currentLargeEigenVector );
      
      if ( verbose )
        cout << "Copying eigenvector to output image #" <<
          eigenvalueIndex << " done." << endl;

      } // critical
        
    } // for eigenvalue index
    
  if ( verbose )
    {
    cout << "Computing large eigenvectors done." << endl;
    }

} // EstimateShapeModels
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
template< class TInputImage, class TOutputImage >
typename
PCAShapeModelEstimatorEigen< TInputImage, TOutputImage >
::ProjectionPointer
PCAShapeModelEstimatorEigen< TInputImage, TOutputImage >
::GetProjection( const unsigned int trainingImageIndex ) const
{

  typename SecretData::ProjectionCollection
    & projections = this->m_SecretData->m_Projections;
    
  if ( trainingImageIndex < projections.size() )
    return projections[ trainingImageIndex ];
  else
    return ProjectionPointer();
    
} // GetProjection
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace itk
#endif // __itKPCAShapeModelEstimatorEigen_hxx
