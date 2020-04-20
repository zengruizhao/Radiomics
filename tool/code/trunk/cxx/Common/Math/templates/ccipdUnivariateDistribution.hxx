#ifndef __ccipdUnivariateDistribution_hxx
#define __ccipdUnivariateDistribution_hxx



//////////////////////////////////////////////////////////////////////////////////////////

// ccipd includes
#include "ccipdUnivariateDistribution.h"

#include "ccipdDisableWarningsMacro.h"
  // eigen includes
  #include <Eigen/Dense>
#include "ccipdEnableWarningsMacro.h"
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{

  
//////////////////////////////////////////////////////////////////////////////////////////
template< typename TDataType >
UnivariateDistribution< TDataType >::UnivariateDistribution()
{
}
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
template< typename TDataType >
UnivariateDistribution< TDataType >::~UnivariateDistribution()
{
}
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
template< typename TDataType >
typename
UnivariateDistribution< TDataType >::DistributionVectorPointer
UnivariateDistribution< TDataType >::GetRandomVector()
{

  // calculate the value
  const TDataType randomValue = this->GetRandomValue();

  // return
  const DistributionVectorPointer randomVector(
    new DistributionVectorType( 1 ) );
  randomVector->coeffRef( 0 ) = randomValue;

  return randomVector;

} // GetRandomVector
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
template< typename TDataType >
typename
UnivariateDistribution< TDataType >::DistributionVectorPointer
UnivariateDistribution< TDataType >::GetMode() const
{

  // calculate the value
  const TDataType mode = this->GetModeValue();

  // return
  const DistributionVectorPointer modeVector(
    new DistributionVectorType( 1 ) );
  modeVector->coeffRef( 0 ) = mode;

  return modeVector;

} // GetMode
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
template< typename TDataType >
typename
UnivariateDistribution< TDataType >::DistributionMatrixPointer
UnivariateDistribution< TDataType >::GetRandomMatrix(
  const unsigned int rows, const unsigned int cols )
{

  // create a new matrix
  const DistributionMatrixPointer
    randomMatrix( new DistributionMatrixType( rows, cols ) );

  typedef typename DistributionMatrixType::Index IndexType;
  const IndexType totalElements =
    static_cast< IndexType >( rows ) *
    static_cast< IndexType >( cols );

  TDataType * matrixData = randomMatrix->data();
  const TDataType * const matrixDataEnd = matrixData + totalElements;

  for ( ; matrixData != matrixDataEnd; ++matrixData )
    {

    // calculate the value
    const TDataType randomValue = this->GetRandomValue();
      
    // fill it in
    *matrixData = randomValue;

    } // for data

  // return
  return randomMatrix;

} // GetRandomVector
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
template< typename TDataType >
typename
UnivariateDistribution< TDataType >::DistributionVectorsPointer
UnivariateDistribution< TDataType >::GetRandomVectors(
  const unsigned int numberOfVectors )
{

  // we are required to overload this function

  // so we will use our existing matrix function
  const DistributionMatrixPointer
    randomMatrix = this->GetRandomMatrix( 1, numberOfVectors );

  // now we must copy it to the correct type
  const DistributionVectorsPointer
    randomVectors( new DistributionVectorsType( *randomMatrix ) );

  // output
  return randomVectors;

} // GetRandomVectors
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
template< typename TDataType >
typename
UnivariateDistribution< TDataType >::ProbabilityType 
UnivariateDistribution< TDataType >::GetPDF(
  const DistributionVectorType & value ) const
{

  // univariate means one value
  return this->GetPDF( value.coeff( 1, 1 ) );

} // GetPDF
//////////////////////////////////////////////////////////////////////////////////////////

  

//////////////////////////////////////////////////////////////////////////////////////////
template< typename TDataType >
typename
UnivariateDistribution< TDataType >::ProbabilityType 
UnivariateDistribution< TDataType >::GetCDF(
  const DistributionVectorType & value ) const
{

  // univariate means one value
  return this->GetCDF( value.coeff( 1, 1 ) );

} // GetCDF
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
template< typename TDataType >
typename
UnivariateDistribution< TDataType >::ProbabilityType 
UnivariateDistribution< TDataType >::GetCDFComplement(
  const DistributionVectorType & value ) const
{

  // univariate means one value
  return this->GetCDFComplement( value.coeff( 1, 1 ) );

} // GetCDF
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
template< typename TDataType >
typename
UnivariateDistribution< TDataType >::ProbabilityType 
UnivariateDistribution< TDataType >::GetCDFComplement(
  const TDataType value ) const
{

  // univariate means one value
  // we are simply using the simplistic approach
  return 1.0 - this->GetCDF( value );

} // GetCDF
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
template< typename TDataType >
typename
UnivariateDistribution< TDataType >::ProbabilityType 
UnivariateDistribution< TDataType >::GetProbability(
  const TDataType value1,
  const TDataType value2 ) const
{

  return std::abs( this->GetCDF( value1 ) - this->GetCDF( value2 ) );

} // GetCDF
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace ccipd
#endif // __ccipdUnivariateDistribution_hxx
