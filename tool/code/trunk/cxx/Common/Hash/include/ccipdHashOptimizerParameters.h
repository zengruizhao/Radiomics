#ifndef __ccipdHashOptimizerParameters_h
#define __ccipdHashOptimizerParameters_h


//////////////////////////////////////////////////////////////////////////////////////////
// ccipd includes
#include "ccipdCore.h"
#include "ccipdITKForwardDeclarations.h"
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



/**
 * \brief A functor to use optimizer parameters as a key in std::unordered_map.
 * 
 * \author  Rob Toth
 * \ingroup Common
 */
template< typename TValueType >
struct OptimizerParametersFunctor
{

  typedef itk::OptimizerParameters< TValueType > ParametersType;

  /// Hash the parameters
  HashValueType operator()( const ParametersType & parameters ) const;

  /// Check if the parameters are exactly equal.
  bool operator()(
    const ParametersType & parameters1,
    const ParametersType & parameters2 ) const;

}; // struct OptimizerParametersFunctor



} // namespace ccipd
#endif // __ccipdHashOptimizerParameters_h
