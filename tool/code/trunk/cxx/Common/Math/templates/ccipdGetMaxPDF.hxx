#ifndef __ccipdGetMaxPDF_hxx
#define __ccipdGetMaxPDF_hxx



////////////////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdGetMaxPDF.h"
#include "ccipdDistribution.h"
////////////////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



////////////////////////////////////////////////////////////////////////////////////////////////////
template< typename TDataType, int TDimension >
DistributionProbabilityType GetMaxPDF(
  const Distribution< TDataType, TDimension > & distribution )
{

  // the maximum PDF occurs at the mode

  const auto mode = distribution.GetMode();

  if ( !mode ) return 0;

  return distribution.GetPDF( *mode );

} // GetMaxPDF
////////////////////////////////////////////////////////////////////////////////////////////////////



} // namespace ccipd
#endif // __ccipdGetMaxPDF_hxx
