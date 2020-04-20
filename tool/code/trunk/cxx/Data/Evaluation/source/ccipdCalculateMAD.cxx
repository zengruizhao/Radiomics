


//////////////////////////////////////////////////////////////////////////////////////////
// ccipd includes
#include "ccipdCalculateMAD.h"
#include "ccipdPrepareMaskForEvaluation.h"

// our own itk includes
#include "itkMeanAbsoluteDistanceImageFilter.h" // MAD filter
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
using std::cout;
using std::endl;
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////
EvaluationValueType CalculateMAD(
  const VolumeMaskImageType * segmentation,
  const VolumeMaskImageType * groundTruth,
  const float              segmentationBorderThickness,
  const float              smoothVariance,
  const float              increaseSizeInMM )
{

  if ( !segmentation || !groundTruth )
    return -1;

  // initialize
  const VolumeMaskImageConstPointer
    finalSegmentationMask = PrepareMaskForEvaluation(
      segmentation, groundTruth, smoothVariance, increaseSizeInMM ),
    finalGroundTruthMask = PrepareMaskForEvaluation(
        groundTruth, groundTruth, smoothVariance, 0 );

  if ( !finalSegmentationMask || !finalGroundTruthMask )
    return -2;

  // prepare our calculator
  typedef itk::MeanAbsoluteDistanceImageFilter< VolumeMaskImageType > MADFilterType;
  const MADFilterType::Pointer MADFilter = MADFilterType::New();

  MADFilter->SetSegmentation( finalSegmentationMask );
  MADFilter->SetGroundTruth ( finalGroundTruthMask  );
  MADFilter->SetSegmentationBorderThickness( segmentationBorderThickness );

  // calculate
  MADFilter->Update();
  return MADFilter->GetMeanAbsoluteDistance();

}; // CalculateMAD
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace
