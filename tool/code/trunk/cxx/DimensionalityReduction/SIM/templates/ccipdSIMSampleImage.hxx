#ifndef __ccipdSIMSampleImage_hxx
#define __ccipdSIMSampleImage_hxx



////////////////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdSIMSampleImage.h"
#include "ccipdSampleImage.hxx"

// boost includes
#include <boost/foreach.hpp> // for loops

// eigen includes
#include "Eigen/Core"

// std includes
#include <cstddef> // for size_t
////////////////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



////////////////////////////////////////////////////////////////////////////////////////////////////
template< class TPixelType, unsigned int Dimension >
void SampleMeanAndEigenvectors(

  // input image
  const itk::VectorImage<
    TPixelType,
    Dimension
  > * meanAndEigenvectors,

  // input locations
  const std::vector<
    typename itk::VectorImage<
      TPixelType,
      Dimension
    >::PointType
  > & sampleLocations,
  
  // output mean values
  Eigen::Matrix<
    TPixelType,
    -1,  1, Eigen::ColMajor, -1,  1
  > & meanValues,

  // output eigenvector values
  Eigen::Matrix<
    TPixelType,
    -1, -1, Eigen::RowMajor, -1, -1
  > & eigenvectorValues,
  
  const unsigned int maximumEigenvectors )
{

  typedef Eigen::Matrix<
    TPixelType,
    -1,  1, Eigen::ColMajor, -1,  1
  > TemplatedVectorType;

  typedef Eigen::Matrix<
    TPixelType,
    -1, -1, Eigen::RowMajor, -1, -1
  > TemplatedMatrixType;

  // check input
  if ( !meanAndEigenvectors )
    {
    std::cerr << "Error in " << __FILE__ <<
      ": no meanAndEigenvectors image!" << std::endl;
    return;
    }

  // count
  const std::size_t
    numberOfSamples      = sampleLocations.size(),
    numberOfComponents   = meanAndEigenvectors->GetNumberOfComponentsPerPixel(),
    totalEigenvectors    = numberOfComponents - 1, // first is the mean
    numberOfEigenvectors = ( totalEigenvectors > maximumEigenvectors ) ?
      maximumEigenvectors : totalEigenvectors;

  // check input
  if ( numberOfComponents == 0 )
    {
    std::cerr << "Error in " << __FILE__ <<
      ": 0 components!" << std::endl;
    return;
    }

  // check input
  if ( numberOfSamples == 0 )
    {
    std::cerr << "Error in " << __FILE__ <<
      ": 0 pixels to sample!" << std::endl;
    return;
    }
  
  // prepare temporary vectors and sample
  const std::vector< std::shared_ptr< const TemplatedVectorType > > outputVectors =
    SampleImageToConstEigenVectors( meanAndEigenvectors, sampleLocations );

  // extract the mean
  meanValues = *outputVectors[ 0 ];

  // prepare the eigenvectors
  eigenvectorValues.resize( numberOfSamples, numberOfEigenvectors );

  // extract the eigenvectors and assign them to our matrix
  for ( std::size_t eigenvectorIndex = 0;
    eigenvectorIndex < numberOfEigenvectors; 
    ++eigenvectorIndex )
    {

    eigenvectorValues.col( eigenvectorIndex ) =
      *outputVectors[ eigenvectorIndex + 1 ]; // remember, #1 was the mean

    } // eigenvectorIndex

  return;

} // SampleMeanAndEigenvectors
////////////////////////////////////////////////////////////////////////////////////////////////////




////////////////////////////////////////////////////////////////////////////////////////////////////
template< class TPixelType, unsigned int Dimension >
std::vector<
  std::shared_ptr<
    Eigen::Matrix<
      TPixelType,
      -1,  1, Eigen::ColMajor, -1,  1
    >
  >
>
SampleImageToEigenVectors(

  // input image
  const itk::VectorImage<
    TPixelType,
    Dimension
  > * inputImage,

  // input locations
  const std::vector<
    typename itk::VectorImage<
      TPixelType,
      Dimension
    >::PointType
  > & sampleLocations )
{

  typedef Eigen::Matrix<
    TPixelType,
    -1,  1, Eigen::ColMajor, -1,  1
  > TemplatedVectorType;
  typedef std::shared_ptr< TemplatedVectorType > TemplatedVectorPointer;

  // how many pixels will we be sampling?
  const std::size_t
    numberOfSamples    = sampleLocations.size(),
    numberOfComponents = inputImage->GetNumberOfComponentsPerPixel();

  // prepare the output
  // where we have one output per component
  std::vector< TemplatedVectorPointer > individualVectors;

  // where will we store the sampled values?
  typedef std::vector< TPixelType * > OutputDataCollection;
  OutputDataCollection outputDataPointers;

  // we first prepare all out outputs
  for ( unsigned int vectorIndex = 0; vectorIndex < numberOfComponents; ++vectorIndex )
    {

    // resize to the correct # of samples
    const TemplatedVectorPointer individualVector(
      new TemplatedVectorType( numberOfSamples ) );

    // add it to our output collection
    individualVectors.push_back( individualVector );

    // save a pointer to store the output data
    outputDataPointers.push_back( individualVector->data() );
    
    } // for vectorIndex

  // now, actually sample our vector image
  const bool extrapolate = false;
  const TPixelType
    nan = std::numeric_limits< TPixelType >::quiet_NaN(),
    defaultPixelValue = nan;
    //defaultPixelValue = 20; // just pick a random value
  SampleVectorImageColWise(
    sampleLocations,
    inputImage,
    outputDataPointers.begin(),
    extrapolate,
    defaultPixelValue );

  // output
  return individualVectors;

} // SampleImageToEigenVectors
////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////
template< class TPixelType, unsigned int Dimension >
std::vector<
  std::shared_ptr<
    const Eigen::Matrix<
      TPixelType,
      -1,  1, Eigen::ColMajor, -1,  1
    >
  >
>
SampleImageToConstEigenVectors(

  // input image
  const itk::VectorImage<
    TPixelType,
    Dimension
  > * inputImage,

  // input locations
  const std::vector<
    typename itk::VectorImage<
      TPixelType,
      Dimension
    >::PointType
  > & sampleLocations )
{

  // sample
  const auto sampledVectors = SampleImageToEigenVectors( inputImage, sampleLocations );
  
  // prepare the output
  std::vector<
    std::shared_ptr<
      const Eigen::Matrix<
        TPixelType,
        -1,  1, Eigen::ColMajor, -1,  1
      >
    >
  > outputVectors;

  // add it to our const output vector
  BOOST_FOREACH( const auto & sampledVector, sampledVectors )
    outputVectors.push_back( sampledVector );

  // output
  return outputVectors;

} // SampleImageToConstEigenVectors
////////////////////////////////////////////////////////////////////////////////////////////////////



} // namespace ccipd
#endif // __ccipdSIMSampleImage_hxx
