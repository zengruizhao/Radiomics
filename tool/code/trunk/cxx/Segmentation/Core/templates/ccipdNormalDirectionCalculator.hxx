#ifndef __ccipdNormalDirectionCalculator_hxx
#define __ccipdNormalDirectionCalculator_hxx



//////////////////////////////////////////////////////////////////////////////////////////

// ccipd includes
#include "ccipdNormalDirectionCalculator.h"
#include "ccipdMaskToLevelset.h"
#include "ccipdResizeImage.h"

// itk includes
#include "ccipdDisableWarningsMacro.h"
#include <itkGradientImageFilter.h>
#include <itkLinearInterpolateImageFunction.h>
#include "ccipdEnableWarningsMacro.h"

// boost includes
#include <boost/noncopyable.hpp>

//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
using std::cout;
using std::endl;
using std::cerr;
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{
  


//////////////////////////////////////////////////////////////////////////////////////////
template< unsigned int TDimension >
class NormalDirectionCalculator< TDimension >::SecretData : boost::noncopyable
{

public:

  SecretData();

  typedef NormalDirectionCalculator< TDimension > EncapsulatingClass;

  typename EncapsulatingClass::MaskConstPointer
    m_Mask;

  typedef double HighPrecision; // There is no itkDefaultPixelTraits for long double

  // interpolate the levelset
  typedef itk::Image<
    HighPrecision, TDimension
  > LevelsetImageType;
  typedef itk::LinearInterpolateImageFunction<
    LevelsetImageType
  > LevelsetInterpolatorType;
  typedef typename LevelsetInterpolatorType::Pointer
    LevelsetInterpolatorPointer;

  const LevelsetInterpolatorPointer
    m_LevelsetInterpolator;
  
  /// for the normal direction
  typedef itk::Image<
    itk::CovariantVector< HighPrecision, TDimension >,
    TDimension
  > DirectionImageType;
  typedef itk::LinearInterpolateImageFunction<
    DirectionImageType
  > DirectionInterpolatorType;
  typedef typename DirectionInterpolatorType::Pointer
    DirectionInterpolatorPointer;

  DirectionInterpolatorPointer
    m_DirectionInterpolator;

  /// Recalculate things from the mask
  void CalculateMask();

  /// whether or not we have calculated information from the mask
  bool
    m_Verbose,
    m_CalculatedMask;

}; // class SecretData
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
template< unsigned int TDimension >
NormalDirectionCalculator< TDimension >::NormalDirectionCalculator() :
ccipdNewPIMPLMacro
{
}
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
template< unsigned int TDimension >
NormalDirectionCalculator< TDimension >::SecretData::SecretData() :
m_LevelsetInterpolator ( LevelsetInterpolatorType ::New() ),
m_DirectionInterpolator( DirectionInterpolatorType::New() ),
m_Verbose       ( false ),
m_CalculatedMask( false )
{
}
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
template< unsigned int TDimension >
NormalDirectionCalculator< TDimension >::~NormalDirectionCalculator()
{
}
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
template< unsigned int TDimension >
typename
NormalDirectionCalculator< TDimension >
::Pointer
NormalDirectionCalculator< TDimension >::New(
  const MaskConstPointer & mask, const bool verbose )
{
  const Pointer newCalculator = Self::New();
  newCalculator->SetVerbose( verbose );
  newCalculator->SetMask( mask );
  return newCalculator;
}
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
template< unsigned int TDimension >
typename
NormalDirectionCalculator< TDimension >
::ConstPointer
NormalDirectionCalculator< TDimension >::ConstNew(
  const MaskConstPointer & mask, const bool verbose )
{
  return ConstPointer( Self::New( mask, verbose ) );
}
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
template< unsigned int TDimension >
void
NormalDirectionCalculator< TDimension >
::CopyParametersInternal( const Self & other )
{
  this->SetMask( other.GetMask() );
}
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
template< unsigned int TDimension >
void
NormalDirectionCalculator< TDimension >
::SecretData
::CalculateMask()
{

  const bool verbose = this->m_Verbose;

  // extract the mask
  MaskConstPointer mask = this->m_Mask;
  if ( !mask ) return;

#ifdef NDEBUG
  // resize the mask isotropically for higher precision
  mask = ResizeMaskIsotropically( mask.GetPointer() );
#endif

  // we compute a levelset from the mask
   if ( verbose ) cout << "Calculating levelset:" << endl;
  const typename LevelsetImageType::ConstPointer levelset(
	  MaskToLevelset< HighPrecision >( mask.GetPointer(), false, false, 4.0, 0.0, false ) );
  if ( !levelset )
    {
    if ( verbose ) cerr << "Calculating levelset failed." << endl;
    return;
    }
  if ( verbose ) cout << "Calculating levelset done." << endl;

  // we compute a gradient direction
  typedef itk::GradientImageFilter<
    LevelsetImageType,
    HighPrecision,
    HighPrecision,
    DirectionImageType
  > GradientFilterType;

  if ( verbose ) cout << "Calculating levelset gradient:" << endl;
  const typename GradientFilterType::Pointer
    gradientFilter = GradientFilterType::New();
  gradientFilter->SetInput( levelset );
  gradientFilter->Update();
  if ( verbose ) cout << "Calculating levelset gradient done." << endl;

  // get the direction image
  const typename DirectionImageType::ConstPointer
    direction = gradientFilter->GetOutput();
  if ( !direction ) return;

  // we have successfully finished the calculation
  this->m_LevelsetInterpolator ->SetInputImage( levelset  );
  this->m_DirectionInterpolator->SetInputImage( direction );
  this->m_CalculatedMask = true;

} // CalculateMask
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
template< unsigned int TDimension >
template< typename TPrecision >
TPrecision
NormalDirectionCalculator< TDimension >
::GetSignedDistanceToSurface( const itk::Point< TPrecision, TDimension > & point )
{
  
  // make sure we are calculated
  if ( !this->m_SecretData->m_CalculatedMask )
    this->m_SecretData->CalculateMask();

  // get the const version's output
  const Self * const constThis = this;
  return constThis->GetSignedDistanceToSurface( point );

} // GetSignedDistanceToSurface
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
template< unsigned int TDimension >
template< typename TPrecision >
TPrecision
NormalDirectionCalculator< TDimension >
::GetSignedDistanceToSurface( const itk::Point< TPrecision, TDimension > & point ) const
{
  
  const bool verbose = this->GetVerbose();

  const TPrecision badOutput = std::numeric_limits< TPrecision >::quiet_NaN();

  // if we are STILL not calculated we have a problem
  if ( !this->m_SecretData->m_CalculatedMask )
    {
    if ( verbose )
      cerr << "Error: Unable to calculate the distance! Returning NaN." << endl;
    return badOutput;
    }

  // get the interpolator
  const typename SecretData::LevelsetInterpolatorType::ConstPointer
    levelsetInterpolator( this->m_SecretData->m_LevelsetInterpolator );

  if ( !levelsetInterpolator )
    {
    if ( verbose )
      cerr << "Error: No interpolator! Returning NaN." << endl;
    return badOutput;
    }

  const bool isInside = levelsetInterpolator->IsInsideBuffer( point );
  if ( !isInside )
    {
    if ( verbose )
      cerr << "Error: Point is outside image! Returning NaN." << endl;
    return badOutput;
    }

  // output the signed distance to the surface
  const TPrecision signedDistance =
    static_cast< TPrecision >(
      levelsetInterpolator->Evaluate( point ) );

  return signedDistance;

} // GetSignedDistanceToSurface
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
template< unsigned int TDimension >
template< typename TPrecision >
itk::CovariantVector< TPrecision, TDimension >
NormalDirectionCalculator< TDimension >
::GetNormalDirection( const itk::Point< TPrecision, TDimension > & point )
{
  
  // make sure we are calculated
  if ( !this->m_SecretData->m_CalculatedMask )
    this->m_SecretData->CalculateMask();

  // get the const version's output
  const Self * const constThis = this;
  return constThis->GetNormalDirection( point );

} // GetNormalDirection
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
template< unsigned int TDimension >
template< typename TPrecision >
itk::CovariantVector< TPrecision, TDimension >
NormalDirectionCalculator< TDimension >
::GetNormalDirection( const itk::Point< TPrecision, TDimension > & point ) const
{
  
  const bool verbose = this->GetVerbose();

  typedef itk::CovariantVector< TPrecision, TDimension > OutputType;
  const OutputType badOutput( TPrecision( 0 ) );

  // if we are STILL not calculated we have a problem
  if ( !this->m_SecretData->m_CalculatedMask )
    {
    if ( verbose )
      cerr << "Error: Unable to calculate the distance! Returning 0." << endl;
    return badOutput;
    }

  // get the interpolator
  const typename SecretData::DirectionInterpolatorType::ConstPointer
    directionInterpolator( this->m_SecretData->m_DirectionInterpolator );

  if ( !directionInterpolator )
    {
    if ( verbose )
      cerr << "Error: No interpolator! Returning 0." << endl;
    return badOutput;
    }

  const bool isInside = directionInterpolator->IsInsideBuffer( point );
  if ( !isInside )
    {
    if ( verbose )
      cerr << "Error: Point is outside image! Returning 0." << endl;
    return badOutput;
    }

  // get the direction to the surface
  const OutputType
    evaluatedDirection = directionInterpolator->Evaluate( point );

  // get the normal and make sure it's not 0
  const TPrecision
    norm        = static_cast< TPrecision >( evaluatedDirection.GetNorm() ),
    epsilon     = std::numeric_limits< TPrecision >::epsilon(),
    nonZeroNorm = norm + epsilon;

  const OutputType normalizedDirection = evaluatedDirection / nonZeroNorm;

  assert( std::abs( normalizedDirection.GetNorm() - 1 ) < 1e-4 && "Invalid norm!" );

  // output
  return normalizedDirection;

} // GetNormalDirection
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
template< unsigned int TDimension >
void
NormalDirectionCalculator< TDimension >
::SetMask( const MaskConstPointer & mask )
{

  this->m_SecretData->m_Mask = mask;

  this->m_SecretData->m_CalculatedMask = false; // we will have to recalculate

  // try to recalculate
  this->m_SecretData->CalculateMask();

} // SetMask
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
template< unsigned int TDimension >
void
NormalDirectionCalculator< TDimension >
::ClearMask()
{
  this->SetMask( MaskConstPointer() );
}
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
template< unsigned int TDimension >
typename
NormalDirectionCalculator< TDimension >
::MaskConstPointer
NormalDirectionCalculator< TDimension >
::GetMask() const
{
  ccipdGetBodyMacro( Mask )
}
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
template< unsigned int TDimension >
void
NormalDirectionCalculator< TDimension >
::SetVerbose( const bool variable )
{
  ccipdSetBodyMacro( Verbose )
}

template< unsigned int TDimension >
bool
NormalDirectionCalculator< TDimension >
::GetVerbose() const
{
  ccipdGetBodyMacro( Verbose )
}
//////////////////////////////////////////////////////////////////////////////////////////


} // namespace ccipd
#endif // __ccipdNormalDirectionCalculator_hxx
