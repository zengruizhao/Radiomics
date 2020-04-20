


//////////////////////////////////////////////////////////////////////////////////////////

// ccipd includes
#include "ccipdMFAAlgorithmTableWidget.h"
#include "ccipdMFATransformDelegate.h"
#include "ccipdMFAInterpolatorDelegate.h"
#include "ccipdMFARegularizerTypeDelegate.h"
#include "ccipdPercentSpinBoxDelegate.h"
#include "ccipdSetDoubleSpinBoxDelegate.h"
#include "ccipdSetIntSpinBoxDelegate.h"
#include "ccipdCheckBoxDelegate.h"

//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////
MFAAlgorithmTableWidget::MFAAlgorithmTableWidget(
  QWidget * const parent ) :
Superclass( parent )
{

  // make certain columns have combo boxes
  ColumnType colIndex = Self::GetTransformColumn();
  this->setItemDelegateForColumn(
    colIndex, new MFATransformDelegate( parent ) );

  colIndex = Self::GetInterpolatorColumn();
  this->setItemDelegateForColumn(
    colIndex, new MFAInterpolatorDelegate( parent ) );
  
  colIndex = Self::GetLikelihoodColumn();
  this->setItemDelegateForColumn(
    colIndex, new MFARegularizerTypeDelegate( parent ) );

  // make certain columns have spin boxes
  colIndex = Self::GetPercentPixelsColumn();
  this->setItemDelegateForColumn(
    colIndex, new PercentSpinBoxDelegate( parent ) );

  SetDoubleSpinBoxDelegate
    * const boundingBoxMantissaDelegate = new SetDoubleSpinBoxDelegate( parent ),
    * const knownPointsMantissaDelegate = new SetDoubleSpinBoxDelegate( parent );

  boundingBoxMantissaDelegate->SetMinimumValue( -9.99 );
  boundingBoxMantissaDelegate->SetMaximumValue( +9.99 );
  knownPointsMantissaDelegate->SetMinimumValue( -9.99 );
  knownPointsMantissaDelegate->SetMaximumValue( +9.99 );
  
  colIndex = Self::GetBoundingBoxMantissaColumn();
  this->setItemDelegateForColumn(
    colIndex, boundingBoxMantissaDelegate );

  colIndex = Self::GetKnownPointsMantissaColumn();
  this->setItemDelegateForColumn(
    colIndex, knownPointsMantissaDelegate );

  SetIntSpinBoxDelegate
    * const boundingBoxExponentDelegate = new SetIntSpinBoxDelegate( parent ),
    * const knownPointsExponentDelegate = new SetIntSpinBoxDelegate( parent );

  boundingBoxExponentDelegate->SetMinimumValue( -99 );
  boundingBoxExponentDelegate->SetMaximumValue( +99 );
  boundingBoxExponentDelegate->SetStep( 1 );

  knownPointsExponentDelegate->SetMinimumValue( -99 );
  knownPointsExponentDelegate->SetMaximumValue( +99 );
  knownPointsExponentDelegate->SetStep( 1 );

  colIndex = Self::GetBoundingBoxExponentColumn();
  this->setItemDelegateForColumn(
    colIndex, boundingBoxExponentDelegate );
  
  colIndex = Self::GetKnownPointsExponentColumn();
  this->setItemDelegateForColumn(
    colIndex, knownPointsExponentDelegate );

  colIndex = Self::GetDisplayColumn();
  this->setItemDelegateForColumn(
    colIndex, new PercentSpinBoxDelegate( parent ) );

  colIndex = Self::GetParticlesColumn();
  const auto particlesSpin = new SetIntSpinBoxDelegate( parent );
  particlesSpin->SetMinimumValue( 1 );
  particlesSpin->SetMaximumValue( 9999 );
  this->setItemDelegateForColumn( colIndex, particlesSpin );
  
  colIndex = Self::GetIterationsColumn();
  const auto iterationsSpin = new SetIntSpinBoxDelegate( parent );
  iterationsSpin->SetMinimumValue( 1 );
  iterationsSpin->SetMaximumValue( 999 );
  this->setItemDelegateForColumn( colIndex, iterationsSpin );

  colIndex = Self::GetMaxEigenvectorsColumn();
  const auto maxEigenvectorsSpin = new SetIntSpinBoxDelegate( parent );
  maxEigenvectorsSpin->SetMinimumValue( 0 );
  maxEigenvectorsSpin->SetMaximumValue( 999 );
  this->setItemDelegateForColumn( colIndex, maxEigenvectorsSpin );

  // make certain columns have check boxes
  colIndex = Self::GetNCCColumn();
  this->setItemDelegateForColumn(
    colIndex, new CheckBoxDelegate( parent ) );
  
  colIndex = Self::GetVarianceWeightsColumn();
  this->setItemDelegateForColumn(
    colIndex, new CheckBoxDelegate( parent ) );

  // set up the column headers
  this->UpdateColumnLabels();

} // constructor
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void MFAAlgorithmTableWidget::UpdateColumnLabels()
{

  // make certain columns have combo boxes
  ColumnType colIndex = Self::GetTransformColumn();
  this->setHorizontalHeaderItem(
    colIndex, new QTableWidgetItem( tr( "Transform" ) ) );

  colIndex = Self::GetInterpolatorColumn();
  this->setHorizontalHeaderItem(
    colIndex, new QTableWidgetItem( tr( "Interpolator" ) ) );

  colIndex = Self::GetPercentPixelsColumn();
  this->setHorizontalHeaderItem(
    colIndex, new QTableWidgetItem( tr( "% Pixels" ) ) );
  
  colIndex = Self::GetNCCColumn();
  this->setHorizontalHeaderItem(
    colIndex, new QTableWidgetItem( tr( "Use NCC" ) ) );
  
  colIndex = Self::GetParticlesColumn();
  this->setHorizontalHeaderItem(
    colIndex, new QTableWidgetItem( tr( "Particles" ) ) );

  colIndex = Self::GetIterationsColumn();
  this->setHorizontalHeaderItem(
    colIndex, new QTableWidgetItem( tr( "Iterations" ) ) );

  colIndex = Self::GetDisplayColumn();
  this->setHorizontalHeaderItem(
    colIndex, new QTableWidgetItem( tr( "Display %" ) ) );
  
  colIndex = Self::GetVarianceWeightsColumn();
  this->setHorizontalHeaderItem(
    colIndex, new QTableWidgetItem( tr( "Use Variance Weights" ) ) );
  
  colIndex = Self::GetLikelihoodColumn();
  this->setHorizontalHeaderItem(
    colIndex, new QTableWidgetItem( tr( "Regularization Type" ) ) );
  
  colIndex = Self::GetBoundingBoxMantissaColumn();
  this->setHorizontalHeaderItem(
    colIndex, new QTableWidgetItem( tr( "BB Mantissa" ) ) );
  
  colIndex = Self::GetBoundingBoxExponentColumn();
  this->setHorizontalHeaderItem(
    colIndex, new QTableWidgetItem( tr( "BB Exponent (10^)" ) ) );

  colIndex = Self::GetKnownPointsMantissaColumn();
  this->setHorizontalHeaderItem(
    colIndex, new QTableWidgetItem( tr( "Correction Mantissa" ) ) );
  
  colIndex = Self::GetKnownPointsExponentColumn();
  this->setHorizontalHeaderItem(
    colIndex, new QTableWidgetItem( tr( "Correction Exponent (10^)" ) ) );

  colIndex = Self::GetMaxEigenvectorsColumn();
  this->setHorizontalHeaderItem(
    colIndex, new QTableWidgetItem( tr( "Max Eigenvectors" ) ) );
    
} // UpdateColumnLabels
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
MFAAlgorithmTableWidget::~MFAAlgorithmTableWidget()
{
} // destructor
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
MFAAlgorithmTableWidget::ColumnType
MFAAlgorithmTableWidget::GetTransformColumn()
{
  return 0;
} // GetTransformColumn
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
MFAAlgorithmTableWidget::ColumnType
MFAAlgorithmTableWidget::GetInterpolatorColumn()
{
  return 1;
} // GetInterpolatorColumn
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
MFAAlgorithmTableWidget::ColumnType
MFAAlgorithmTableWidget::GetPercentPixelsColumn()
{
  return 2;
} // GetPercentPixelsColumn
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
MFAAlgorithmTableWidget::ColumnType
MFAAlgorithmTableWidget::GetNCCColumn()
{
  return 3;
} // GetNCCColumn
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
MFAAlgorithmTableWidget::ColumnType
MFAAlgorithmTableWidget::GetParticlesColumn()
{
  return 4;
} // GetParticlesColumn
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
MFAAlgorithmTableWidget::ColumnType
MFAAlgorithmTableWidget::GetIterationsColumn()
{
  return 5;
} // GetIterationsColumn
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
MFAAlgorithmTableWidget::ColumnType
MFAAlgorithmTableWidget::GetDisplayColumn()
{
  return 6;
} // GetDisplayColumn
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
MFAAlgorithmTableWidget::ColumnType
MFAAlgorithmTableWidget::GetVarianceWeightsColumn()
{
  return 7;
} // GetVarianceWeightsColumn
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
MFAAlgorithmTableWidget::ColumnType
MFAAlgorithmTableWidget::GetLikelihoodColumn()
{
  return 8;
} // GetLikelihoodColumn
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
MFAAlgorithmTableWidget::ColumnType
MFAAlgorithmTableWidget::GetBoundingBoxMantissaColumn()
{
  return 9;
} // GetBoundingBoxMantissaColumn
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
MFAAlgorithmTableWidget::ColumnType
MFAAlgorithmTableWidget::GetBoundingBoxExponentColumn()
{
  return 10;
} // GetBoundingBoxExponentColumn
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
MFAAlgorithmTableWidget::ColumnType
MFAAlgorithmTableWidget::GetKnownPointsMantissaColumn()
{
  return 11;
} // GetKnownPointsMantissaColumn
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
MFAAlgorithmTableWidget::ColumnType
MFAAlgorithmTableWidget::GetKnownPointsExponentColumn()
{
  return 12;
} // GetKnownPointsExponentColumn
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
MFAAlgorithmTableWidget::ColumnType
MFAAlgorithmTableWidget::GetMaxEigenvectorsColumn()
{
  return 13;
} // GetMaxEigenvectorsColumn
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace ccipd
