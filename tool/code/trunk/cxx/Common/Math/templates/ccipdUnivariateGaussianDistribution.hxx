#ifndef __ccipdUniformDistribution_hxx
#define __ccipdUniformDistribution_hxx



//////////////////////////////////////////////////////////////////////////////////////////
// ccipd includes
#include "ccipdUnivariateGaussianDistribution.h"

// std includes
#include <cmath>

// boost includes
#include <boost/math/distributions/normal.hpp>

#include "ccipdDisableWarningsMacro.h"
  // eigen includes
  #include <Eigen/Core>
#include "ccipdEnableWarningsMacro.h"
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{


//////////////////////////////////////////////////////////////////////////////////////////
template< typename TDataType >
class UnivariateGaussianDistribution< TDataType >::SecretData
{
  
public:

  //boost::math::
  typedef boost::math::normal_distribution< TDataType > NormalDistributionType;

  NormalDistributionType
    m_Normal;

}; // class SecretData
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
template< typename TDataType >
UnivariateGaussianDistribution< TDataType >::UnivariateGaussianDistribution() :
ccipdNewPIMPLMacro
{

  this->Regenerate();

  // give it the distribution with the desired min and max
  this->Superclass1::SetDistribution( DistributionType(
    this->GetMeanPrimitive    (),
    this->GetVariancePrimitive() ) );

} // constructor
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
template< typename TDataType >
UnivariateGaussianDistribution< TDataType >::~UnivariateGaussianDistribution()
{
} // destructor
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
template< typename TDataType >
TDataType
UnivariateGaussianDistribution< TDataType >
::GetMeanPrimitive() const
{

  // Return the first (only) element
  return (*this->GetMean())[ 0 ];

} // GetMeanPrimitive
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
template< typename TDataType >
typename
UnivariateGaussianDistribution< TDataType >
::DistributionVectorPointer
UnivariateGaussianDistribution< TDataType >
::GetMode() const
{
  return this->Superclass2::GetMode();
}
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
template< typename TDataType >
typename
UnivariateGaussianDistribution< TDataType >
::DataType
UnivariateGaussianDistribution< TDataType >
::GetModeValue() const
{
  return ( *this->GetMode() )[ 0 ];
}
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
template< typename TDataType >
TDataType
UnivariateGaussianDistribution< TDataType >
::GetVariancePrimitive() const
{

  // Return the first (only) element
  return (*this->GetVariance())[ 0 ];

} // GetVariancePrimitive
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
template< typename TDataType >
void
UnivariateGaussianDistribution< TDataType >
::SetMean( const DataType mean )
{

  // Set the first (only) element
  MeanVectorPointer meanWrapped ( new MeanVectorType );
  (*meanWrapped)[ 0 ] = mean;
  
  this->SetMean( meanWrapped );

} // SetMean
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
template< typename TDataType >
void
UnivariateGaussianDistribution< TDataType >
::SetVariance( const DataType variance )
{

  // Set the first (only) element
  CovarianceMatrixPointer varianceWrapped ( new CovarianceMatrixType );
  (*varianceWrapped)[ 0 ] = variance;
  
  this->SetVariance( varianceWrapped );

} // GetVariancePrimitive
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
template< typename TDataType >
typename
UnivariateGaussianDistribution< TDataType >
::ProbabilityType
UnivariateGaussianDistribution< TDataType >
::GetPDF( const DataType value ) const
{

  return boost::math::pdf( this->m_SecretData->m_Normal, value );

} // GetProbability
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
template< typename TDataType >
typename
UnivariateGaussianDistribution< TDataType >
::ProbabilityType
UnivariateGaussianDistribution< TDataType >
::GetCDF( const DataType value ) const
{

  return boost::math::cdf( this->m_SecretData->m_Normal, value );

} // GetCDF
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
template< typename TDataType >
typename
UnivariateGaussianDistribution< TDataType >
::ProbabilityType
UnivariateGaussianDistribution< TDataType >
::GetCDFComplement( const DataType value ) const
{

  return boost::math::cdf(
    boost::math::complement( this->m_SecretData->m_Normal, value ) );

} // GetCDFComplement
//////////////////////////////////////////////////////////////////////////////////////////


  
//////////////////////////////////////////////////////////////////////////////////////////
template< typename TDataType >
void
UnivariateGaussianDistribution< TDataType >
::SetMean( const MeanVectorPointer & mean )
{

  this->Superclass2::SetMean( mean );
  this->Regenerate();

} // SetMean
//////////////////////////////////////////////////////////////////////////////////////////


  
//////////////////////////////////////////////////////////////////////////////////////////
template< typename TDataType >
void
UnivariateGaussianDistribution< TDataType >
::SetVariance( const CovarianceMatrixPointer & variance )
{

  this->Superclass2::SetVariance( variance );
  this->Regenerate();

} // SetMean
//////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////
template< typename TDataType >
void
UnivariateGaussianDistribution< TDataType >
::Regenerate()
{

  // recreate our distributions
  const TDataType
    meanPrimitive     = this->GetMeanPrimitive(),
    variancePrimitive = this->GetVariancePrimitive(),
    sigmaPrimitive    = std::sqrt( variancePrimitive );

  this->m_SecretData->m_Normal =
    typename SecretData::NormalDistributionType( 
      meanPrimitive,
      sigmaPrimitive );

  this->Superclass1::SetDistribution( DistributionType(
    meanPrimitive,
    sigmaPrimitive ) );

} // Regenerate
//////////////////////////////////////////////////////////////////////////////////////////


  
} // namespace ccipd
#endif // __ccipdUnivariateDistribution_hxx
