#ifndef __ccipdUniqueValuesTypes_h
#define __ccipdUniqueValuesTypes_h


//////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdCore.h" // out pixel types

// std includes
#include <unordered_set> // for the output
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////
/**
 * \addtogroup Common
 * @{
 */



/// A collection of unique internal pixel values.
typedef std::unordered_set< InternalPixelType >
  UniqueInternalValuesCollection;

/// A collection of unique mask values.
typedef std::unordered_set< MaskPixelType >
  UniqueMaskValuesCollection;



/** @} */
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace ccipd
#endif // __ccipdUniqueValuesTypes_h
