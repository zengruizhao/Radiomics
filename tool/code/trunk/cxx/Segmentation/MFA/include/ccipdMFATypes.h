#ifndef __ccipdMFATypes_h
#define __ccipdMFATypes_h




//////////////////////////////////////////////////////////////////////////////////////////

// ccipd includes
#include "ccipdMatrixTypes.h"
#include "ccipdSIMTypes.h"
#include "ccipdMFAForwardDeclarations.h"
#include "ccipdITKForwardDeclarations.h"

// std includes
#include <memory>

//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////
/**
 * \addtogroup MFA
 * @{
 */


// SIM declarations. 
typedef SIMEstimator                      MFAEstimator;
typedef SIMVolumeStorage                  MFAVolumeBase;
typedef SIMBoundingBoxExtractor           MFABoundingBoxExtractor;
typedef SIMBoundingBoxType                MFABoundingBoxType;
typedef SIMFeatureExtractor               MFAFeatureExtractor;
typedef SIMFeaturePreparerInterface       MFAFeaturePreparerInterface;
typedef SIMFeaturePreparerUsingExtractors MFAFeaturePreparerUsingExtractors;

// MFA declarations.
typedef itk::MFAReconstructionFilter<
  InternalPixelType,
  VolumeDimension
> MFAVolumeReconstructionFilter;

typedef itk::MFAReconstructionMetric<
  InternalPixelType,
  VolumeDimension
> MFAVolumeReconstructionMetric;

typedef VolumeRegionType MFABoundingBoxType;

typedef itk::SmartPointer< MFAVolumeReconstructionFilter > MFAVolumeReconstructionFilterPointer;
typedef itk::SmartPointer< MFAVolumeReconstructionMetric > MFAVolumeReconstructionMetricPointer;

typedef std::shared_ptr<       RegisterMFAMetric       > RegisterMFAMetricPointer;
typedef std::shared_ptr<       RegisterMFARegularizedMetric > RegisterMFARegularizedMetricPointer;
typedef std::shared_ptr<       MFASegmenter            > MFASegmenterPointer;
typedef std::shared_ptr< const MFASegmenter            > MFASegmenterConstPointer;
typedef std::shared_ptr<       MFAModel                > MFAModelPointer;
typedef std::shared_ptr< const MFAModel                > MFAModelConstPointer;
typedef std::shared_ptr<       MFAVolumeBase           > MFAVolumeBasePointer;
typedef std::shared_ptr< const MFAVolumeBase           > MFAVolumeBaseConstPointer;
typedef std::shared_ptr<       MFAEstimator            > MFAEstimatorPointer;
typedef std::shared_ptr<       MFAMaskAligner          > MFAMaskAlignerPointer;
typedef std::shared_ptr< const MFAMaskAligner          > MFAMaskAlignerConstPointer;
typedef std::shared_ptr<       MFAFeatureExtractor     > MFABoundingBoxExtractorPointer;
typedef std::shared_ptr< const SIMBoundingBoxExtractor > MFABoundingBoxExtractorConstPointer;
typedef std::shared_ptr<       MFAFeatureExtractor     > MFAFeatureExtractorPointer;
typedef std::shared_ptr<       MFALevelsetExtractor    > MFALevelsetExtractorPointer;
typedef std::shared_ptr<       MFATextureExtractor     > MFATextureExtractorPointer;
typedef std::shared_ptr< const MFALevelsetExtractor    > MFALevelsetExtractorConstPointer;
typedef std::shared_ptr< const MFATextureExtractor     > MFATextureExtractorConstPointer;
typedef std::shared_ptr<       RegisterMFAAlgorithm    > RegisterMFAAlgorithmPointer;
typedef std::shared_ptr< const RegisterMFAAlgorithm    > RegisterMFAAlgorithmConstPointer;

typedef std::vector< RegisterMFAAlgorithmPointer > RegisterMFAAlgorithmCollection;

typedef std::shared_ptr< MFAFeaturePreparerInterface >
  MFAFeaturePreparerPointer;
typedef std::shared_ptr< const MFAFeaturePreparerInterface >
  MFAFeaturePreparerConstPointer;

typedef std::shared_ptr< MFADrivingFeaturePreparer >
  MFADrivingFeaturePreparerPointer;
typedef std::shared_ptr< const MFADrivingFeaturePreparer >
  MFADrivingFeaturePreparerConstPointer;

//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
// For the affine parameters

// How many transform parameters are there?
const int MFANumberOfAffineParameters = 12;

typedef double MFAAffineParametersValueType;

/// Define a dynamic vector to store the mean parameters
typedef Eigen::Matrix<
  MFAAffineParametersValueType,
  MFANumberOfAffineParameters, +1, 0,
  MFANumberOfAffineParameters, +1
> MFAMeanAffineParametersType;
         
typedef std::shared_ptr<
  MFAMeanAffineParametersType
> MFAMeanAffineParametersPointer;

// Define a matrix for the covariance matrix.
typedef Eigen::Matrix<
  MFAAffineParametersValueType,
  MFANumberOfAffineParameters,
  MFANumberOfAffineParameters, 0,
  MFANumberOfAffineParameters,
  MFANumberOfAffineParameters 
> MFACovarianceAffineParametersType;
 
typedef std::shared_ptr<
  MFACovarianceAffineParametersType
> MFACovarianceAffineParametersPointer;
                                                    
//////////////////////////////////////////////////////////////////////////////////////////



/** @} */

} // namespace
#endif //__ccipdMFATypes_h
