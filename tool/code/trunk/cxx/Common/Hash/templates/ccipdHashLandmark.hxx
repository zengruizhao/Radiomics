#ifndef __ccipdHashLandmarks_hxx
#define __ccipdHashLandmarks_hxx



//////////////////////////////////////////////////////////////////////////////////////////
// ccipd includes
#include "ccipdHashLandmark.h"

// boost includes
#include <boost/functional/hash.hpp>
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////
template< class TPointType >
HashValueType
HashLandmark( const TPointType & landmark )
{

  // just initialize a seed
  const HashValueType seed = 0;

  HashValueType hashResult = seed;

  const unsigned int size = landmark.Size();

  boost::hash_combine( hashResult, size );

  for(unsigned int i=0; i < size; ++i )
    {
    boost::hash_combine( hashResult, landmark[i] );
    } // for unsigned int i

  return hashResult;

} // HashLandmark
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace ccipd
#endif // __ccipdHashLandmarks_hxx