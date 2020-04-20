#ifndef __ccipdVectorIndexTypes_h
#define __ccipdVectorIndexTypes_h



//////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdDisableWarningsMacro.h"
  #include <vector>        // for the index type
  #include <unordered_set> // our collection type
#include "ccipdEnableWarningsMacro.h"
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



/**
 * \addtogroup Common
 * @{
 */



//////////////////////////////////////////////////////////////////////////////////////////

/**
 * \note
 *   I'm pretty sure "size_type" doesn't change based
 *   on the specific template parameter, so we'll
 *   just pick a primitive type.
 */
typedef std::vector< int >::size_type VectorIndexType;

/// A collection of indices for a std::vector
typedef std::unordered_set< VectorIndexType > VectorIndexContainer;

//////////////////////////////////////////////////////////////////////////////////////////



/** @} */
} // namespace ccipd
#endif // __ccipdVectorIndexTypes_h
