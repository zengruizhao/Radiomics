#ifndef __ccipdSIMSampleImage_h
#define __ccipdSIMSampleImage_h



////////////////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdMatrixTypes.h"
#include "ccipdITKForwardDeclarations.h"

#include <vector>
#include <limits>

////////////////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Sample a trained SIM image.
 * 
 * This function is used to take an
 * existing, trained itk::VectorImage
 * and sample it at specific locations.
 * 
 * The output will be stored in an
 * Eigen vector containing the sampled
 * mean values, and an Eigen matrix
 * containing the sampled
 * eigenvector values.
 * 
 * \param[in] meanAndEigenvectors
 *   The input image to sample.
 *   This assumes that the first component
 *   is the mean image and the subsequent components
 *   are the eigenvectors.
 * 
 * \param[in] sampleLocations
 *   A std::vector of physical
 *   locations to sample
 *   the input image.
 * 
 * \param[out] meanValues
 *   An Eigen vector containing
 *   the values from the mean
 *   image at the specified
 *   sample locations.
 * 
 * \param[in] eigenvectorValues
 *   An Eigen matrix containing
 *   the values from each of the
 *   eigenvector images. Each
 *   column corresponds to
 *   an eigenvector (in the
 *   same order as in the
 *   input image), and each row
 *   corresponds to one of the sample
 *   locations.
 * 
 * \param[in] maximumEigenvectors
 *   If you set this variable, then
 *   only this many eigenvectors
 *   can possibly be sampled. This
 *   is useful if you only want to
 *   sample the first few eigenvectors
 *   of a PCA output (assuming they
 *   are sorted in order of decreasing
 *   variance).
 * 
 * \returns
 *   All returns are given by non-const references.
 *   
 * \author  Rob Toth
 * \ingroup MFA
 */
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
  
  const unsigned int maximumEigenvectors =
    std::numeric_limits< unsigned int >::max()
    
);
////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Sample an itk::VectorImage and output as a std::vector of Eigen vectors.
 * 
 * This function will sample a feature image and
 * output the result as a std::vector of Eigen::Vector's.
 * 
 * \param[in] inputImage
 *   The image to sample.
 * 
 * \param[in] sampleLocations
 *   The physical locations
 *   to sample in each of the
 *   input image's components
 * 
 * \param[out] individualVectors
 *   Each element contains an
 *   Eigen vector with the sampled
 *   values, where each row
 *   contains the value of
 *   a physical location.
 * 
 * \returns
 *   All returns are given by non-const references.
 * 
 * \author  Rob Toth
 * \ingroup MFA
 * 
 */
// output values

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
  > & sampleLocations );
////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Overloaded to output const pointers
 */
template< class TPixelType, unsigned int Dimension >

// output values
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
  > & sampleLocations );
////////////////////////////////////////////////////////////////////////////////////////////////////



} // namespace ccipd
#endif // __ccipdSIMSampleImage_h
