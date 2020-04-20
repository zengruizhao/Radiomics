#ifndef __ccipdUniformDistribution_hxx
#define __ccipdUniformDistribution_hxx



//////////////////////////////////////////////////////////////////////////////////////////
// ccipd includes
#include "ccipdUniformDistribution.h"

#include "ccipdDisableWarningsMacro.h"
  // eigen includes
  #include <Eigen/Dense>
#include "ccipdEnableWarningsMacro.h"

// std includes
#include <type_traits>
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{


//////////////////////////////////////////////////////////////////////////////////////////
template< typename TDataType >
class UniformDistribution< TDataType >::SecretData
{
  
public:
  SecretData();

  TDataType
    m_MinimumValue,
    m_MaximumValue;

}; // class SecretData
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
template< typename TDataType >
UniformDistribution< TDataType >::SecretData::SecretData() :
m_MinimumValue( 0 ),
m_MaximumValue( 1 )
{
}
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
template< typename TDataType >
UniformDistribution< TDataType >::UniformDistribution() :
ccipdNewPIMPLMacro
{

  // give it the distribution with the desired min and max
  this->Superclass::SetDistribution( DistributionType(
    this->m_SecretData->m_MinimumValue,
    this->m_SecretData->m_MaximumValue ) );

} // constructor
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
template< typename TDataType >
UniformDistribution< TDataType >::~UniformDistribution()
{
} // destructor
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
template< typename TDataType >
typename
UniformDistribution< TDataType >
::ProbabilityType
UniformDistribution< TDataType >
::GetPDF( const TDataType value ) const
{

  const TDataType
    minRaw  = this->m_SecretData->m_MinimumValue,
    maxRaw  = this->m_SecretData->m_MaximumValue;

  const ProbabilityType
    minCast = static_cast< ProbabilityType >( minRaw ),
    maxCast = static_cast< ProbabilityType >( maxRaw );
  BOOST_CONSTEXPR_OR_CONST ProbabilityType    
    zero   = 0.0,
    one    = 1.0;

  // no chance we are outside the bounds
  if ( value < minRaw || value >= maxRaw )
    return zero;
  else if ( volatile bool isIntegral = std::is_integral< TDataType >::value )
    {
    // for integral types
    // http://bit.ly/WU30Oy
    return one / ( maxCast - minCast + one );
    }
  else if ( volatile bool isFloat = std::is_floating_point< TDataType >::value )
    {
    // for floating types
    // http://bit.ly/Rr6WkS
    return one / ( maxCast - minCast );
    }
  else
    {
    // should never get here
    return 0;
    }

} // GetPDF
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
template< typename TDataType >
typename
UniformDistribution< TDataType >
::ProbabilityType
UniformDistribution< TDataType >
::GetCDF( const TDataType value ) const
{

  const TDataType
    minRaw  = this->m_SecretData->m_MinimumValue,
    maxRaw  = this->m_SecretData->m_MaximumValue;

  const ProbabilityType
    minCast   = static_cast< ProbabilityType >( minRaw ),
    maxCast   = static_cast< ProbabilityType >( maxRaw ),
    valueCast = static_cast< ProbabilityType > ( value );
  BOOST_CONSTEXPR_OR_CONST ProbabilityType    
    zero   = 0.0;

  if ( value < minRaw || value >= maxRaw )
    return zero;
  else
    return ( valueCast - minCast ) / ( maxCast - minCast );

} // GetCDF
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
template< typename TDataType >
typename
UniformDistribution< TDataType >
::DataType
UniformDistribution< TDataType >
::GetModeValue() const
{

  // just return the mean of min and max
  return static_cast< DataType >(
    ( this->m_SecretData->m_MaximumValue +
      this->m_SecretData->m_MinimumValue ) / 2.0 );

} // GetModeValue
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
template< typename TDataType >
void
UniformDistribution< TDataType >
::SetMinimumValue( const TDataType minimumValue )
{
  if ( this->m_SecretData->m_MinimumValue != minimumValue )
    {

    // update the min value
    this->m_SecretData->m_MinimumValue = minimumValue;

    // recreate our distribution

    // note that min must be less than max all times!

    const TDataType maximumValue = this->m_SecretData->m_MaximumValue;

    if ( minimumValue <= maximumValue )
      {
      this->Superclass::SetDistribution( DistributionType(
        minimumValue, maximumValue ) );
      }
    else
      {
      //std:cerr << "Warning: min > max!" << std::endl;
      }

    } // !=

} // SetMinimumValue
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
template< typename TDataType >
void
UniformDistribution< TDataType >
::SetMaximumValue( const TDataType maximumValue )
{
  if ( this->m_SecretData->m_MaximumValue != maximumValue )
    {

    // update the max value
    this->m_SecretData->m_MaximumValue = maximumValue;

    const TDataType minimumValue = this->m_SecretData->m_MinimumValue;

    if ( minimumValue <= maximumValue )
      {
      // recreate our distribution
      this->Superclass::SetDistribution( DistributionType(
        minimumValue, maximumValue ) );
      }
    else
      {
      // error!
      }

    } // !=

} // SetMaximumValue
//////////////////////////////////////////////////////////////////////////////////////////


  
} // namespace ccipd
#endif // __ccipdUnivariateDistribution_hxx
