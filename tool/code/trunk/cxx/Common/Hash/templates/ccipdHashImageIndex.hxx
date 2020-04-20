#ifndef __ccipdHashImageIndex_hxx
#define __ccipdHashImageIndex_hxx



//////////////////////////////////////////////////////////////////////////////////////////
// ccipd includes
#include "ccipdHashImageIndex.h"

// itk includes
#include "ccipdDisableWarningsMacro.h"
  #include <itkIndex.h>
#include "ccipdEnableWarningsMacro.h"

// boost includes
#include <boost/functional/hash.hpp>
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////
template< unsigned int IndexDimension >
HashValueType
ImageIndexHashFunctor< IndexDimension >
::operator()( const IndexType & index ) const
{

  // just initialize a seed
  const HashValueType seed = 0;

  HashValueType hashResult = seed;

  // loop through each dimension and hash the current index value
  for ( unsigned int dimension = 0; dimension < IndexDimension; ++dimension )
    boost::hash_combine( hashResult, index[ dimension ] );

  return hashResult;

} // operator()
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace ccipd
#endif // __ccipdHashImageIndex_hxx
