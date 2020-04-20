


//////////////////////////////////////////////////////////////////////////////////////////

// ccipd includes
#include "ccipdMFAAlgorithmTableWidgetGet.h"
#include "ccipdMFAAlgorithmTableWidget.h"
#include "ccipdRegisterMFAAlgorithm.h"
#include "ccipdMFATransformDelegate.h"
#include "ccipdMFAInterpolatorDelegate.h"
#include "ccipdMFARegularizerTypeDelegate.h"

#include <cmath>
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{


  
//////////////////////////////////////////////////////////////////////////////////////////
RegisterMFAAlgorithmCollection GetAlgorithms(
  const MFAAlgorithmTableWidget & table )
{

  RegisterMFAAlgorithmCollection algorithms;

  // how many?
  const unsigned int numberOfAlgorithms = table.rowCount();

  // loop through each algorithm
  for ( unsigned int algorithmIndex = 0;
    algorithmIndex < numberOfAlgorithms; ++algorithmIndex )
    {

    // get the properties of that algorithm
    const unsigned int rowIndex = algorithmIndex;

    // get the text of each item in the table

    // transform
    MFAAlgorithmTableWidget::ColumnType
      colIndex = MFAAlgorithmTableWidget::GetTransformColumn();
    const MFATransformDelegate::ComboItemType whichTransformItem =
      table.item( rowIndex, colIndex )->text().toStdString();

    // interpolator
    colIndex = MFAAlgorithmTableWidget::GetInterpolatorColumn();
    const MFATransformDelegate::ComboItemType whichInterpolatorItem =
      table.item( rowIndex, colIndex )->text().toStdString();

    // % pixels
    colIndex = MFAAlgorithmTableWidget::GetPercentPixelsColumn();
    typedef RegisterMFAAlgorithm::MetricType::PercentType PercentType;
    const PercentType percentPixels = static_cast< PercentType >(
      table.item( rowIndex, colIndex )->text().toDouble() );

    // weights
    colIndex = MFAAlgorithmTableWidget::GetBoundingBoxMantissaColumn();
    const double boundingBoxMantissa =
      table.item( rowIndex, colIndex )->text().toDouble();

    colIndex = MFAAlgorithmTableWidget::GetKnownPointsMantissaColumn();
    const double knownPointsMantissa =
      table.item( rowIndex, colIndex )->text().toDouble();

    colIndex = MFAAlgorithmTableWidget::GetBoundingBoxExponentColumn();
    const int boundingBoxExponent =
      table.item( rowIndex, colIndex )->text().toInt();
    
    colIndex = MFAAlgorithmTableWidget::GetKnownPointsExponentColumn();
    const int knownPointsExponent =
      table.item( rowIndex, colIndex )->text().toInt();

    typedef RegisterMFAAlgorithm::MetricType::RegularizationWeightType RegularizationWeightType;
    const RegularizationWeightType
      boundingBoxWeight = static_cast< RegularizationWeightType >(
        boundingBoxMantissa * std::pow( 10.0, boundingBoxExponent ) ),
      knownPointsWeight = static_cast< RegularizationWeightType >(
        knownPointsMantissa * std::pow( 10.0, knownPointsExponent ) );
    
    // use NCC
    colIndex = MFAAlgorithmTableWidget::GetNCCColumn();
    const bool useNCC = 
      table.item( rowIndex, colIndex )->text().toInt() != 0;
    
    // particles
    colIndex = MFAAlgorithmTableWidget::GetParticlesColumn();
    const unsigned int particles =
      table.item( rowIndex, colIndex )->text().toUInt();

    // iterations
    colIndex = MFAAlgorithmTableWidget::GetIterationsColumn();
//    const unsigned int iterations =
//      table.item( rowIndex, colIndex )->text().toUInt();

    // iterations
    colIndex = MFAAlgorithmTableWidget::GetMaxEigenvectorsColumn();
    const unsigned int maxEigenvectors =
      table.item( rowIndex, colIndex )->text().toUInt();

    // display %
    colIndex = MFAAlgorithmTableWidget::GetDisplayColumn();
    const float displayPercent =
      table.item( rowIndex, colIndex )->text().toFloat();
    
    // use variance weights
    colIndex = MFAAlgorithmTableWidget::GetVarianceWeightsColumn();
    const bool useVarianceWeights = 
      table.item( rowIndex, colIndex )->text().toInt() != 0;
    
    // likelihood type
    colIndex = MFAAlgorithmTableWidget::GetLikelihoodColumn();
    const MFARegularizerTypeDelegate::ComboItemType whichLikelihoodItem =
      table.item( rowIndex, colIndex )->text().toStdString();

    // convert to MFA algorithm types
    const auto whichTransform =
      MFATransformDelegate::GetWhichTransform( whichTransformItem );
    const auto whichInterpolator =
      MFAInterpolatorDelegate::GetWhichInterpolator( whichInterpolatorItem );
    const auto likelihoodType =
      MFARegularizerTypeDelegate::GetLikelihoodType( whichLikelihoodItem );

    // create a new algorithm with those properties
    const RegisterMFAAlgorithmPointer algorithm =
      RegisterMFAAlgorithmPointer( RegisterMFAAlgorithm::New() );

    const RegisterMFAAlgorithm::OptimizerPointer optimizer =
      std::dynamic_pointer_cast< RegisterMFAAlgorithm::OptimizerType >( algorithm->GetOptimizer() );

    optimizer->SetNumberOfParticles   ( particles          );
    algorithm->SetDisplayPercent      ( displayPercent     );
    
    // set our transform types
    const RegisterMFAAlgorithm::TransformPointer transform (
      std::dynamic_pointer_cast< RegisterMFAAlgorithm::TransformType >(
      algorithm->GetTransform() ) ); 
    transform->SetWhichTransform( whichTransform );

    // set interpolator types
    const  RegisterMFAAlgorithm::InterpolatorPointer interpolator (
      std::dynamic_pointer_cast< RegisterMFAAlgorithm::InterpolatorType >( 
        algorithm->GetInterpolator() ) ); 
    interpolator->SetWhichInterpolator ( whichInterpolator  );

    // set metric types
    const RegisterMFAAlgorithm::MetricPointer metric (
      std::dynamic_pointer_cast< RegisterMFAAlgorithm::MetricType >( 
        algorithm->GetMetric() ) );
    metric->SetPercentPixels       ( percentPixels      );
    metric->SetUseNCC              ( useNCC             );
    metric->SetUseVarianceWeights  ( useVarianceWeights );
    metric->SetLikelihoodType      ( likelihoodType     );
    metric->SetBoundingBoxWeight   ( boundingBoxWeight  );
    metric->SetKnownPointsWeight   ( knownPointsWeight  );
    metric->SetMaxEigenvectors     ( maxEigenvectors    );
//    algorithm->SetMaxIterations       ( iterations         );

    algorithms.push_back( algorithm );

    } // for each algorithm

  // output
  return algorithms;

} // GetAlgorithms
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace ccipd
