


//////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdNormalDirectionCalculator.hxx"
#include "ccipdCloneable.hxx"
#include "ccipdFactory.hxx"
#include "ccipdVolumeTypes.h"
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////
template
class Cloneable< NormalDirectionCalculator< VolumeDimension > >;

template
class Factory< NormalDirectionCalculator< VolumeDimension > >;

template
class Copyable< NormalDirectionCalculator< VolumeDimension > >;

template
class NormalDirectionCalculator< VolumeDimension >;

template
double
NormalDirectionCalculator< VolumeDimension >
::GetSignedDistanceToSurface< double >(
  const itk::Point< double, VolumeDimension > & );

template
double
NormalDirectionCalculator< VolumeDimension >
::GetSignedDistanceToSurface< double >(
  const itk::Point< double, VolumeDimension > & ) const;

template
float
NormalDirectionCalculator< VolumeDimension >
::GetSignedDistanceToSurface< float >(
  const itk::Point< float, VolumeDimension > & );

template
float
NormalDirectionCalculator< VolumeDimension >
::GetSignedDistanceToSurface< float >(
  const itk::Point< float, VolumeDimension > & ) const;

template
itk::CovariantVector< double, VolumeDimension >
NormalDirectionCalculator< VolumeDimension >
::GetNormalDirection< double >( const itk::Point< double, VolumeDimension > & );

template
itk::CovariantVector< double, VolumeDimension >
NormalDirectionCalculator< VolumeDimension >
::GetNormalDirection< double >( const itk::Point< double, VolumeDimension > & ) const;

template
itk::CovariantVector< float, VolumeDimension >
NormalDirectionCalculator< VolumeDimension >
::GetNormalDirection< float >( const itk::Point< float, VolumeDimension > & );

template
itk::CovariantVector< float, VolumeDimension >
NormalDirectionCalculator< VolumeDimension >
::GetNormalDirection< float >( const itk::Point< float, VolumeDimension > & ) const;

//////////////////////////////////////////////////////////////////////////////////////////



} // namespace ccipd
