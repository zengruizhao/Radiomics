#ifndef __ccipdClosestPoint_hxx
#define __ccipdClosestPoint_hxx



//////////////////////////////////////////////////////////////////////////////////////////

// ccipd includes
#include "ccipdClosestPoint.h"
#include "ccipdNormalDirectionCalculator.h"

// itk includes
#include "ccipdDisableWarningsMacro.h"
#include <itkPoint.h>
#include <itkCovariantVector.h>
#include "ccipdEnableWarningsMacro.h"

//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////
template< typename TPrecision, unsigned int TDimension >
itk::Point< TPrecision, TDimension >
GetClosestSurfacePoint(
  const itk::Point< TPrecision, TDimension >    & point,
  const NormalDirectionCalculator< TDimension > & calculator )
{

  typedef itk::Point< TPrecision, TDimension > OutputType;
  const TPrecision nan = std::numeric_limits< TPrecision >::quiet_NaN();
  const OutputType badOutput( nan );

  // Calculate the signed distance.
  // If we are inside (d < 0), then we want to keep
  // the to be positive (towards the surface).
  // However, if we are outside (d > 0)
  // we want to reverse the direction so
  // it goes towards the surface.
  // As such, we want the negative value.
  const TPrecision distance =
    -( calculator.GetSignedDistanceToSurface( point ) );
  if ( distance != distance ) return badOutput;

  // calculate the direction
  const auto direction = calculator.GetNormalDirection( point );
  if ( direction.GetSquaredNorm() == 0 ) return badOutput;

  // now get the surface point
  OutputType closestSurfacePoint = point;
  for ( unsigned int dimension = 0; dimension < TDimension; ++dimension )
    {
    closestSurfacePoint[ dimension ] += 
      direction[ dimension ] * distance;
    } // dimension

  // output
  return closestSurfacePoint;

} // GetClosestSurfacePoint
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace ccipd
#endif // __ccipdClosestPoint_hxx
