#ifndef __ccipdHashCollection_hxx
#define __ccipdHashCollection_hxx



//////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdHashCollection.h"
#include <boost/foreach.hpp>
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{


  
//////////////////////////////////////////////////////////////////////////////////////////
template< typename ContainerOfHashablePointers >
SortedHashCollectionType GetSortedHashValues(
  const ContainerOfHashablePointers & hashableObjects )
{

  // prepare our output
  SortedHashCollectionType sortedHashes;

  // hash each object and add it to our collection
  BOOST_FOREACH( const auto hashableObject, hashableObjects )
    sortedHashes.insert( hashableObject->GetHash() );

  return sortedHashes;

} // GetSortedHashValues
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace



#endif // __ccipdHashCollection_hxx
