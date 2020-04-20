#ifndef __ccipdGammaDistribution_hxx
#define __ccipdGammaDistribution_hxx



//////////////////////////////////////////////////////////////////////////////////////////
// ccipd includes
#include "ccipdGammaDistribution.h"

#include "ccipdDisableWarningsMacro.h"
  // eigen includes
  #include <Eigen/Dense>


// boost includes
#include <boost/math/distributions/gamma.hpp>
//////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdEnableWarningsMacro.h"


namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////
template< typename TDataType >
class GammaDistribution< TDataType >::SecretData
{
  
public:
  SecretData();

  typedef boost::math::gamma_distribution< TDataType > GammaDistributionType;

  TDataType
    m_Shape,
    m_Scale;

  /// for pdf and cdf
  GammaDistributionType
    m_Gamma;

  typedef long double PreciseFloatingType;

}; // class SecretData
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
template< typename TDataType >
GammaDistribution< TDataType >::SecretData::SecretData() :
m_Shape( 1 ), // start out randomly (will set in the other constructor)
m_Scale( 1 ),
m_Gamma( m_Shape, m_Scale )
{
}
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
template< typename TDataType >
GammaDistribution< TDataType >::GammaDistribution() :
ccipdNewPIMPLMacro
{

  // recreate our distributions to make sure we set it correctly
  const TDataType
    shape = this->m_SecretData->m_Shape,
    scale = this->m_SecretData->m_Scale;

  // make sure we recreate them!
  this->SetShapeAndScale( shape + 1, scale + 1 );
  this->SetShapeAndScale( shape    , scale     );

} // constructor
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
template< typename TDataType >
GammaDistribution< TDataType >::~GammaDistribution()
{
} // destructor
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
template< typename TDataType >
typename
GammaDistribution< TDataType >
::DataType
GammaDistribution< TDataType >
::GetModeValue() const
{

  typedef typename SecretData::PreciseFloatingType PreciseFloatingType;

  BOOST_CONSTEXPR_OR_CONST PreciseFloatingType
    one = 1;
  const PreciseFloatingType
    shape = static_cast< PreciseFloatingType >( this->m_SecretData->m_Shape ),
    scale = static_cast< PreciseFloatingType >( this->m_SecretData->m_Scale ),
    mode  = ( shape - one ) * scale;

  return static_cast< DataType >( mode );

} // GetMode
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
template< typename TDataType >
typename
GammaDistribution< TDataType >
::DataType
GammaDistribution< TDataType >
::GetVariance() const
{
  
  typedef typename SecretData::PreciseFloatingType PreciseFloatingType;

  const PreciseFloatingType
    shape = static_cast< PreciseFloatingType >( this->m_SecretData->m_Shape ),
    scale = static_cast< PreciseFloatingType >( this->m_SecretData->m_Scale ),
    variance = shape * scale * scale;

  return static_cast< DataType >( variance );

} // GetVariance
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
template< typename TDataType >
typename
GammaDistribution< TDataType >
::DataType
GammaDistribution< TDataType >
::GetMean() const
{
  
  typedef typename SecretData::PreciseFloatingType PreciseFloatingType;

  const PreciseFloatingType
    shape = static_cast< PreciseFloatingType >( this->m_SecretData->m_Shape ),
    scale = static_cast< PreciseFloatingType >( this->m_SecretData->m_Scale ),
    mean  = shape * scale;

  return static_cast< DataType >( mean );

} // GetMean
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
template< typename TDataType >
typename
GammaDistribution< TDataType >
::DataType
GammaDistribution< TDataType >
::GetSkewness() const
{
    
  typedef typename SecretData::PreciseFloatingType PreciseFloatingType;

  BOOST_CONSTEXPR_OR_CONST PreciseFloatingType
    two = 2;
  const PreciseFloatingType
    shape     = static_cast< PreciseFloatingType >( this->m_SecretData->m_Shape ),
    sqrtShape = std::sqrt( shape ),
    skewness  = two / sqrtShape;

  return static_cast< DataType >( skewness );

} // GetSkewness
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
template< typename TDataType >
typename
GammaDistribution< TDataType >
::DataType
GammaDistribution< TDataType >
::GetExcessKurtosis() const
{
  
  typedef typename SecretData::PreciseFloatingType PreciseFloatingType;

  BOOST_CONSTEXPR_OR_CONST PreciseFloatingType
    six = 6;
  const PreciseFloatingType
    shape          = static_cast< PreciseFloatingType >( this->m_SecretData->m_Shape ),
    excessKurtosis = six / shape;

  return static_cast< DataType >( excessKurtosis );

} // GetExcessKurtosis
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
template< typename TDataType >
typename
GammaDistribution< TDataType >
::ProbabilityType
GammaDistribution< TDataType >
::GetPDF( const TDataType value ) const
{
  
  return boost::math::pdf( this->m_SecretData->m_Gamma, value );

} // GetPDF
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
template< typename TDataType >
typename
GammaDistribution< TDataType >
::ProbabilityType
GammaDistribution< TDataType >
::GetCDF( const TDataType value ) const
{

  return boost::math::cdf( this->m_SecretData->m_Gamma, value );

} // GetCDF
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
template< typename TDataType >
typename
GammaDistribution< TDataType >
::ProbabilityType
GammaDistribution< TDataType >
::GetCDFComplement( const TDataType value ) const
{

  return boost::math::cdf(
    boost::math::complement( this->m_SecretData->m_Gamma, value ) );

} // GetCDFComplement
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
template< typename TDataType >
void
GammaDistribution< TDataType >
::SetShapeAndScale( const TDataType shape, const TDataType scale )
{

  if ( shape <= 0 || scale <= 0 )
    throw std::out_of_range( "Shape and scale must be positive!" );

  if ( this->m_SecretData->m_Shape != shape || 
       this->m_SecretData->m_Scale != scale ) 
    {

    // update the values
    this->m_SecretData->m_Shape = shape;
    this->m_SecretData->m_Scale = scale;

    // recreate our distribution

    // for the superclass (randomness)
    this->Superclass::SetDistribution(
      DistributionType( shape, scale ) );

    // for boost (pdf and cdf)
    this->m_SecretData->m_Gamma =
      typename SecretData::GammaDistributionType( shape, scale );

    } // !=

} // SetMinimumValue
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
template< typename TDataType >
void
GammaDistribution< TDataType >
::SetModeAndVariance( const TDataType inputMode, const TDataType inputVariance )
{

  // we need to compute alpha and beta from the mode
  // and variance

  // mode = (shape - 1) * scale
  // variance = ( shape * scale ^2 )

  // Solution = http://bit.ly/XH4J6Y

  // calculate using intermediate calculations for speed
  typedef typename SecretData::PreciseFloatingType PreciseFloatingType;

  BOOST_CONSTEXPR_OR_CONST PreciseFloatingType
    one  = 1,
    two  = 2,
    half = one / two;
  const PreciseFloatingType
    mode                = static_cast< PreciseFloatingType >( inputMode     ),
    variance            = static_cast< PreciseFloatingType >( inputVariance ),
    mSquared            = mode * mode,
    mFourth             = mSquared * mSquared,
    vSquared            = variance * variance,
    twoVariance         = two * variance,
    insideSquareRoot    = ( mFourth + two * mSquared * twoVariance ) / vSquared,
    squareRoot          = std::sqrt( insideSquareRoot ),
    shape               = mSquared / twoVariance + one + half * squareRoot,
    scale               = -mode / ( one - shape );

  // set
  this->SetShapeAndScale(
    static_cast< DataType >( shape ),
    static_cast< DataType >( scale ) );

  // make sure we've calculated correction
  assert(
    std::abs( ( mode - this->GetModeValue() ) / mode ) < 1e-4 &&
    "Incorrectly calculated mode!" );
  assert(
    std::abs( ( variance - this->GetVariance() ) / variance ) < 1e-4 &&
    "Incorrectly calculated variance!" );

} // SetMaximumValue
//////////////////////////////////////////////////////////////////////////////////////////


  
} // namespace ccipd
#endif // __ccipdGammaDistribution_hxx
