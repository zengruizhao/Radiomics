#ifndef __ccipdHashLandmarks_hxx
#define __ccipdHashLandmarks_hxx



//////////////////////////////////////////////////////////////////////////////////////////
// ccipd includes
#include "ccipdHashLandmarks.h"
#include "ccipdHashLandmark.h"

// boost includes
#include <boost/functional/hash.hpp>
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////
template< class TPointSet >
HashValueType
HashLandmarks( const TPointSet * const landmarks )
{

  // first make sure we have landmarks
  if ( !landmarks ) return 0;

  // just initialize a seed
  const HashValueType seed = 0;

  HashValueType hashResult = seed;

  // loop through each point and hash the current value
  //set up point set iterators
  typedef  typename TPointSet::PointsContainerConstIterator   IteratorType;
  typedef  typename TPointSet::PointType                      PointType;

  IteratorType landmarkIter = landmarks->GetPoints()->Begin();
  const IteratorType landmarkEnd = landmarks->GetPoints()->End();

  for(;landmarkIter!=landmarkEnd; landmarkIter++)
    {
    PointType point = landmarkIter.Value(); 
    boost::hash_combine( hashResult, HashLandmark( point ) );
    
    } //for landmarkIter

  return hashResult;

}
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace ccipd
#endif // __ccipdHashLandmarks_hxx