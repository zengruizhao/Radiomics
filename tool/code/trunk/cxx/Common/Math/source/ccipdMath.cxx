


//////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdMath.h"

// std includes
#include <utility>   // for pairs
#include <algorithm> // for sorting
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



std::vector< unsigned int > randperm( const unsigned int numberOfElements )
{

  // we need to create a random ordering
  // the second value will contain which index 
  typedef std::pair< int, unsigned int > SortOrderType;
  typedef std::vector< SortOrderType > PairsType;
  PairsType randomPairs;
  for ( unsigned int index = 0; index != numberOfElements; ++index )
    randomPairs.push_back( SortOrderType( rand(), index ) );
  std::sort( randomPairs.begin(), randomPairs.end() );

  // now just extract the index values
  std::vector< unsigned int > output;
  for ( PairsType::const_iterator iter = randomPairs.begin();
    iter != randomPairs.end(); ++iter)
    {
    output.push_back( iter->second );
    }

  return output;

}



} // namespace
