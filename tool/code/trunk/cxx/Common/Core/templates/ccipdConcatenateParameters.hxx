#ifndef __ccipdConcatenateParameters_hxx
#define __ccipdConcatenateParameters_hxx



//////////////////////////////////////////////////////////////////////////////////////////////////

// ccipd includes
#include "ccipdCore.h"
#include "ccipdConcatenateParameters.h"

// itk includes
#include "ccipdDisableWarningsMacro.h"
#include <itkOptimizerParameters.h>
#include "ccipdEnableWarningsMacro.h"

//////////////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////////////
template< typename TValueType >
itk::OptimizerParameters< TValueType >
ConcatenateParameters(
  const itk::OptimizerParameters< TValueType > & first,
  const itk::OptimizerParameters< TValueType > & second )
{

  typedef itk::OptimizerParameters< TValueType > ParametersType;

  typedef typename ParametersType::SizeValueType NumberOfParametersType;

  const NumberOfParametersType
    numberOfParametersFirst  = first .size(),
    numberOfParametersSecond = second.size(),
    numberOfParametersTotal  = numberOfParametersFirst + numberOfParametersSecond;

  ParametersType concatenated( numberOfParametersTotal );

  // loop through
  for ( NumberOfParametersType firstParameterIndex = 0;
    firstParameterIndex < numberOfParametersFirst; ++firstParameterIndex )
    {

    const TValueType parameterValue = first.GetElement( firstParameterIndex );
    const NumberOfParametersType concatenatedIndex = firstParameterIndex;
    concatenated.SetElement( concatenatedIndex, parameterValue );

    } // for firstParameterIndex

  // loop through
  for ( NumberOfParametersType secondParameterIndex = 0;
    secondParameterIndex < numberOfParametersSecond; ++secondParameterIndex )
    {

    const TValueType parameterValue = second.GetElement( secondParameterIndex );
    const NumberOfParametersType concatenatedIndex =
      secondParameterIndex + numberOfParametersFirst;
    concatenated.SetElement( concatenatedIndex, parameterValue );

    } // for firstParameterIndex

  return concatenated;

} // ConcatenateParameters
//////////////////////////////////////////////////////////////////////////////////////////////////



} // namespace ccipd
#endif // __ccipdConcatenateParameters_h
