#ifndef __ccipdHashOptimizerParameters_hxx
#define __ccipdHashOptimizerParameters_hxx




//////////////////////////////////////////////////////////////////////////////////////////
// ccipd includes
#include "ccipdHashOptimizerParameters.h"

#include "ccipdDisableWarningsMacro.h"
  // itk includes
  #include <itkOptimizerParameters.h>

  // boost includes
  #include <boost/functional/hash.hpp>

  // std includes
  #include <algorithm> // for std::equal
#include "ccipdEnableWarningsMacro.h"
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{


  
//////////////////////////////////////////////////////////////////////////////////////////
  template< typename TValueType >
HashValueType
OptimizerParametersFunctor< TValueType >
::operator()( const ParametersType & parameters ) const
{

  HashValueType hashValue = 0;
  
  // hash the size
  boost::hash_combine( hashValue, parameters.size() );

  // hash the elements
  boost::hash_range( hashValue, parameters.begin(), parameters.end() );

  // output
  return hashValue;

} // hash
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
template< typename TValueType >
bool
OptimizerParametersFunctor< TValueType >
::operator()(
  const ParametersType & parameters1,
  const ParametersType & parameters2 ) const
{
  
  // make sure the # of parameters are equal
  // and the values are equal
  return
    ( parameters1.size() == parameters2.size() ) &&
    ( parameters1.size() == 0 ||
      std::equal(
        parameters1.begin(),
        parameters1.end(),
        parameters2.begin() ) );

} // hash
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace ccipd
#endif // __ccipdHashOptimizerParameters_hxx
