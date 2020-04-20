#ifndef __ccipdConcatenateParameters_h
#define __ccipdConcatenateParameters_h



//////////////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdITKForwardDeclarations.h"
//////////////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * Return a concatenation
 * of two parameters.
 * 
 * First will appear
 * as the first segment
 * of the output, followed
 * by second.
 * 
 * \author  Rob Toth
 * \date    2013-02-21
 * \ingroup Common
 */
template< typename TValueType >
itk::OptimizerParameters< TValueType >
ConcatenateParameters(
  const itk::OptimizerParameters< TValueType > & first,
  const itk::OptimizerParameters< TValueType > & second );
//////////////////////////////////////////////////////////////////////////////////////////////////



} // namespace ccipd
#endif // __ccipdConcatenateParameters_h
