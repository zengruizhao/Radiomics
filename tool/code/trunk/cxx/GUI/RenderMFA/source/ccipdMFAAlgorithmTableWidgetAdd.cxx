


//////////////////////////////////////////////////////////////////////////////////////////

// ccipd includes
#include "ccipdMFAAlgorithmTableWidgetAdd.h"
#include "ccipdMFAAlgorithmTableWidget.h"
#include "ccipdRegisterMFAAlgorithm.h"
#include "ccipdMFATransformDelegate.h"
#include "ccipdMFAInterpolatorDelegate.h"
#include "ccipdMFARegularizerTypeDelegate.h"

// boost includes
#include <boost/foreach.hpp>
#include <memory>
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{


  
//////////////////////////////////////////////////////////////////////////////////////////
void AddAlgorithm(
        MFAAlgorithmTableWidget & table,
  const RegisterMFAAlgorithm    & algorithm )
{

  // how many?
  const unsigned int
    rowIndex = table.rowCount();
    
  table.insertRow( rowIndex /* +1 ? */ );
    
  // get the properties of that algorithm
  ccipd::RegisterMFAAlgorithm::OptimizerPointer optimizer(
    std::dynamic_pointer_cast< RegisterMFAAlgorithm::OptimizerType > ( 
    algorithm.GetOptimizer() ) );
  const auto particles          = optimizer->GetNumberOfParticles   ();

  // get the properties of the transform
  const RegisterMFAAlgorithm::TransformPointer transform (
    std::dynamic_pointer_cast< RegisterMFAAlgorithm::TransformType > ( 
      algorithm.GetTransform() ) );
  const auto whichTransform     = transform->GetWhichLinearTransform();

  // get the properties of the interpolator
  const  RegisterMFAAlgorithm::InterpolatorPointer interpolator (
    std::dynamic_pointer_cast< RegisterMFAAlgorithm::InterpolatorType >( 
      algorithm.GetInterpolator() ) ); 
  const auto whichInterpolator = interpolator->GetWhichInterpolator();

  // get the properties of the metric
  const RegisterMFAAlgorithm::MetricPointer metric (
    std::dynamic_pointer_cast< RegisterMFAAlgorithm::MetricType >( 
      algorithm.GetMetric() ) );

  const auto percentPixels      = metric->GetPercentPixels       ();
  const auto useNCC             = metric->GetUseNCC              ();
  const auto useVarianceWeights = metric->GetUseVarianceWeights  ();
  const auto likelihoodType     = metric->GetLikelihoodType      ();
  const auto boundingBoxWeight  = metric->GetBoundingBoxWeight   ();
  const auto knownPointsWeight  = metric->GetKnownPointsWeight   ();
  const auto maxEigenvectors    = metric->GetMaxEigenvectors     ();

 
//  const auto iterations         = algorithm.GetMaxIterations       ();
  const auto displayPercent     = algorithm.GetDisplayPercent      (); 

  // update the row entries

  MFAAlgorithmTableWidget::ColumnType
    colIndex = MFAAlgorithmTableWidget::GetTransformColumn();
  table.setItem( rowIndex, colIndex, new QTableWidgetItem(
    MFATransformDelegate::GetComboItem( whichTransform ).c_str() ) );

  colIndex = MFAAlgorithmTableWidget::GetInterpolatorColumn();
  table.setItem( rowIndex, colIndex, new QTableWidgetItem(
    MFAInterpolatorDelegate::GetComboItem( whichInterpolator ).c_str() ) );
  
  colIndex = MFAAlgorithmTableWidget::GetLikelihoodColumn();
  table.setItem( rowIndex, colIndex, new QTableWidgetItem(
    MFARegularizerTypeDelegate::GetComboItem( likelihoodType ).c_str() ) );

  colIndex = MFAAlgorithmTableWidget::GetPercentPixelsColumn();
  table.setItem( rowIndex, colIndex, new QTableWidgetItem(
    QString::number( percentPixels ) ) );
  
  colIndex = MFAAlgorithmTableWidget::GetNCCColumn();
  table.setItem( rowIndex, colIndex, new QTableWidgetItem(
    QString::number( useNCC ) ) );
  
  colIndex = MFAAlgorithmTableWidget::GetParticlesColumn();
  table.setItem( rowIndex, colIndex, new QTableWidgetItem(
    QString::number( particles ) ) );

//  colIndex = MFAAlgorithmTableWidget::GetIterationsColumn();
//  table.setItem( rowIndex, colIndex, new QTableWidgetItem(
//    QString::number( iterations ) ) );

  colIndex = MFAAlgorithmTableWidget::GetDisplayColumn();
  table.setItem( rowIndex, colIndex, new QTableWidgetItem(
    QString::number( displayPercent ) ) );
  
  colIndex = MFAAlgorithmTableWidget::GetVarianceWeightsColumn();
  table.setItem( rowIndex, colIndex, new QTableWidgetItem(
    QString::number( useVarianceWeights ) ) );
  
  colIndex = MFAAlgorithmTableWidget::GetMaxEigenvectorsColumn();
  table.setItem( rowIndex, colIndex, new QTableWidgetItem(
    QString::number( maxEigenvectors ) ) );
  
  const bool
    boundingBoxIsZero     = boundingBoxWeight == 0,
    knownPointsIsZero     = knownPointsWeight == 0,
    boundingBoxIsNegative = ( boundingBoxWeight < 0 ),
    knownPointsIsNegative = ( knownPointsWeight < 0 );

  const auto
    boundingBoxPositiveAlpha = std::abs( boundingBoxWeight ),
    knownPointsPositiveAlpha = std::abs( knownPointsWeight );

  const int
    boundingBoxExponent = boundingBoxIsZero ? 0 : static_cast< int >(
      std::floor( std::log10( boundingBoxPositiveAlpha ) ) ),
    knownPointsExponent = knownPointsIsZero ? 0 : static_cast< int >(
      std::floor( std::log10( knownPointsPositiveAlpha ) ) );
  
  colIndex = MFAAlgorithmTableWidget::GetBoundingBoxExponentColumn();
  table.setItem( rowIndex, colIndex, new QTableWidgetItem(
    QString::number( boundingBoxExponent ) ) );
  
  colIndex = MFAAlgorithmTableWidget::GetKnownPointsExponentColumn();
  table.setItem( rowIndex, colIndex, new QTableWidgetItem(
    QString::number( knownPointsExponent ) ) );

  const double
    boundingBoxMantissa = ( boundingBoxIsNegative ? -1 : +1 ) *
      boundingBoxPositiveAlpha / std::pow( 10.0, boundingBoxExponent ),
    knownPointsMantissa = ( knownPointsIsNegative ? -1 : +1 ) *
      knownPointsPositiveAlpha / std::pow( 10.0, knownPointsExponent );

  colIndex = MFAAlgorithmTableWidget::GetBoundingBoxMantissaColumn();
  table.setItem( rowIndex, colIndex, new QTableWidgetItem(
    QString::number( boundingBoxMantissa ) ) );
  
  colIndex = MFAAlgorithmTableWidget::GetKnownPointsMantissaColumn();
  table.setItem( rowIndex, colIndex, new QTableWidgetItem(
    QString::number( knownPointsMantissa ) ) );

  table.UpdateColumnLabels();

} // AddAlgorithm
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void AddDefaultAlgorithm( MFAAlgorithmTableWidget & table )
{

  const RegisterMFAAlgorithm::ConstPointer
    defaultAlgorithm = RegisterMFAAlgorithm::ConstNew();
  AddAlgorithm( table, *defaultAlgorithm );
} // AddDefaultAlgorithm
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void AddAlgorithms(
        MFAAlgorithmTableWidget        & table,
  const RegisterMFAAlgorithmCollection & algorithms )
{

  BOOST_FOREACH( const auto & algorithm, algorithms )
    {
    if ( algorithm )
      AddAlgorithm( table, *algorithm );
    } // for each algorithm

} // AddAlgorithms
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace ccipd
