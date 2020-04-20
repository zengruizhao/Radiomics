#ifndef __ccipdUnorderedMapSerialization_h
#define __ccipdUnorderedMapSerialization_h



//////////////////////////////////////////////////////////////////////////////////////////
// Base on:
// http://bit.ly/ZcyOdp
// http://bit.ly/10jpoR3
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdDisableWarningsMacro.h"

  // std unordered map
  #include <unordered_map>

  // boost config
  #include <boost/config.hpp>

  // boost serialization
  #include <boost/serialization/utility.hpp>
  #include <boost/serialization/collections_save_imp.hpp>
  #include <boost/serialization/collections_load_imp.hpp>
  #include <boost/serialization/split_free.hpp>

#include "ccipdEnableWarningsMacro.h"
//////////////////////////////////////////////////////////////////////////////////////////



namespace boost {
namespace serialization {



////////////////////////////////////////////////////////////////////////////////////////////////////
template<class Archive, class Type, class Key, class Compare, class Allocator >
inline void save(
  Archive & archive,
  const std::unordered_map< Key, Type, Compare, Allocator > & myMap,
  const unsigned int /* version */ )
{

  boost::serialization::stl::save_collection<
    Archive,
    std::unordered_map< Key, Type, Compare, Allocator >
  >( archive, myMap );

} // save
////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////
template<class Archive, class Type, class Key, class Compare, class Allocator >
inline void load(
  Archive & archive,
  std::unordered_map< Key, Type, Compare, Allocator > & myMap,
  const unsigned int /* version */ )
{

  boost::serialization::stl::load_collection
    <
    Archive,
    std::unordered_map< Key, Type, Compare, Allocator >,
    boost::serialization::stl::archive_input_map
      <
      Archive,
      std::unordered_map< Key, Type, Compare, Allocator >
      >,
    boost::serialization::stl::no_reserve_imp
      <
      std::unordered_map< Key, Type, Compare, Allocator >
      >
    >( archive, myMap );

} // load
////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////
// split non-intrusive serialization function member into separate
// non intrusive save/load member functions
template<class Archive, class Type, class Key, class Compare, class Allocator >
inline void serialize(
  Archive & archive,
  std::unordered_map< Key, Type, Compare, Allocator > & myMap,
  const unsigned int version )
{

  boost::serialization::split_free( archive, myMap, version );

} // serialize
////////////////////////////////////////////////////////////////////////////////////////////////////



} // serialization
} // namespace boost
#endif // __ccipdUnorderedMapSerialization_h
