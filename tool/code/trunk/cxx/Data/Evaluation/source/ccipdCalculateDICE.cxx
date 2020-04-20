


//////////////////////////////////////////////////////////////////////////////////////////
// ccipd includes
#include "ccipdCalculateDICE.h"
#include "ccipdPrepareMaskForEvaluation.h"

#include "ccipdDisableWarningsMacro.h"
  // itk includes
  // see http://www.itk.org/pipermail/insight-users/2008-May/026060.html
  #include <itkSimilarityIndexImageFilter.h>      // Dice filter
  #include <itkAndImageFilter.h>                  // Dice filter for N images
  #include <itkStatisticsImageFilter.h>         // count the number of pixels
#include "ccipdEnableWarningsMacro.h"
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
using std::cout;
using std::endl;
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{

  
  
//////////////////////////////////////////////////////////////////////////////////////////
EvaluationValueType CalculateDice(
  const VolumeMaskImageType * segmentation,
  const VolumeMaskImageType * groundTruth,
  const float                 smoothVariance,
  const float                 increaseSizeInMM )
{

  // initialize
  const VolumeMaskImageConstPointer
    finalSegmentationMask = PrepareMaskForEvaluation(
      segmentation, groundTruth, smoothVariance, increaseSizeInMM ),
    finalGroundTruthMask = PrepareMaskForEvaluation(
        groundTruth, groundTruth, smoothVariance, 0 );
  
  // define itk's built in dice type
  typedef itk::SimilarityIndexImageFilter<
    VolumeMaskImageType, VolumeMaskImageType > DiceType;
  DiceType::Pointer diceCalculator = DiceType::New();
  
  // give it some inputs
  diceCalculator->SetInput1( finalSegmentationMask );
  diceCalculator->SetInput2( finalGroundTruthMask  );

  /*
  SaveImage( finalSegmentationMask, "finalSegmentationMask.mhd", true );
  SaveImage( finalGroundTruthMask , "finalGroundTruthMask.mhd" , true );
  */
  
  // calculate and return
  diceCalculator->Update();
  return diceCalculator->GetSimilarityIndex();

}
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
EvaluationValueType CalculateDice(
 VolumeMaskImagesContainer masks)
{

 typedef itk::AndImageFilter <VolumeMaskImageType> AndImageFilterType;
  AndImageFilterType::Pointer andFilter
          = AndImageFilterType::New();

  typedef itk::StatisticsImageFilter<VolumeMaskImageType> StatisticsImageFilterType;
  StatisticsImageFilterType::Pointer statisticsImageFilter
          = StatisticsImageFilterType::New ();
    
  VolumeMaskImageType::Pointer inter = masks[0];
  statisticsImageFilter->SetInput(masks[0]);
  statisticsImageFilter->Update();
  auto countSum =statisticsImageFilter->GetSum();

  for (unsigned int i=1; i<masks.size(); i++)
  {
    andFilter = AndImageFilterType::New();
    andFilter->SetInput(0, inter);
    andFilter->SetInput(1, masks[i]);
    andFilter->Update();
    inter = andFilter->GetOutput();

    //compute the size of the sum 
    statisticsImageFilter = StatisticsImageFilterType::New ();
    statisticsImageFilter->SetInput(masks[i]);
    statisticsImageFilter->Update();
    countSum += statisticsImageFilter->GetSum();
  }

  //compute the size of the intersection 
  statisticsImageFilter = StatisticsImageFilterType::New ();
  statisticsImageFilter->SetInput(inter);
  statisticsImageFilter->Update();
  auto countInter = statisticsImageFilter->GetSum();

  return masks.size()*countInter/(double)countSum; 
}
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace
