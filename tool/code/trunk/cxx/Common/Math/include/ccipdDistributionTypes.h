#ifndef __ccipdDistributionTypes_h
#define __ccipdDistributionTypes_h



////////////////////////////////////////////////////////////////////////////////////////////////////
// ccipd includes
#include "ccipdCore.h"
////////////////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



////////////////////////////////////////////////////////////////////////////////////////////////////

// forward declarations
template< typename TDataType, int TDimensionality >
class Distribution;

template< typename TDataType >
class UnivariateDistribution;

template< typename TDataType >
class UniformDistribution;

template< typename TDataType, typename TDistributionType >
class StandardDistribution;

template< typename TDataType >
class UnivariateGaussianDistribution;

template< typename TDataType, int TDimensionality >
class MultivariateGaussianDistribution;

typedef double DistributionProbabilityType;

////////////////////////////////////////////////////////////////////////////////////////////////////




} // namespace ccipd
#endif // __ccipdDistributionTypes_h
