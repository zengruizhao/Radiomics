#ifndef __ccipdSharedPointerSerialization_hxx
#define __ccipdSharedPointerSerialization_hxx



//////////////////////////////////////////////////////////////////////////////////////////
// Based on http://bit.ly/10zumsX
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////

// ccipd includes
#include "ccipdEnableSharedFromThis.h"

// boost includes
#include <boost/serialization/split_free.hpp>
#include <boost/unordered_map.hpp>
#include <boost/serialization/nvp.hpp>

#ifdef _MSC_VER
#pragma warning( disable: 4127 )
#endif

//////////////////////////////////////////////////////////////////////////////////////////



namespace boost { namespace serialization
{



////////////////////////////////////////////////////////////////////////////////////////////////////
template<class Archive, class Type>
void save(Archive & archive, const std::shared_ptr<Type> & value, const unsigned int /*version*/)
{
  Type * data = value.get();
  archive << make_nvp( "shared_ptr", data );
}
////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * Polymorphic load so we can try
 * to see if we are "EnableSharedFromThis"
 * which will screw up weak pointers if we
 * are not careful.
 */
template< class Archive, class Type >
typename std::enable_if< std::is_polymorphic< Type >::value >::type
load( Archive & archive, std::shared_ptr< Type > & value, const unsigned int /*version*/ )
{

  // if we are trying to load a const type,
  // we will essentially perform a "const_cast"
  // type of conversion
  typedef typename std::remove_const< Type >::type NonConstType;
  NonConstType * data;
  //Type * data;

  archive >> make_nvp( "shared_ptr", data );

  typedef std::weak_ptr< Type > WeakPtr;
  static boost::unordered_map< void *, WeakPtr > hash;

  if ( hash[ data ].expired() )
    {

    // start at null
    value.reset();

    typedef ccipd::EnableSharedFromThis< NonConstType > SharedFromThis;

    BOOST_CONSTEXPR_OR_CONST bool
      extendsSharedBase = 
        std::is_base_of< ccipd::EnableSharedFromThisBase, NonConstType >::value,
      extendsSharedSelf = 
        std::is_base_of< SharedFromThis, NonConstType >::value;

    static_assert(
      !extendsSharedBase || ( extendsSharedBase && extendsSharedSelf ),
      "If you extend the EnableSharedFromThisBase, then you need to extend EnableSharedFromThis< Self >" );

    if ( extendsSharedSelf )
      {
      if ( SharedFromThis * sharedBase = dynamic_cast< SharedFromThis * >( data ) )
        {
        // shared_from_this will work
        value = sharedBase->shared_from_this();
        }
      } // extendsSharedSelf

    if ( !value )
      {
      // we haven't been able to use shared_from_this
      value = std::shared_ptr< Type >( data );
      }

    hash[ data ] = value;

    }
  else
    {
    value = hash[ data ].lock();
    }

} // load (polymorphic)
////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////
template< class Archive, class Type >
typename std::enable_if< !std::is_polymorphic< Type >::value >::type
load( Archive & archive, std::shared_ptr< Type > & value, const unsigned int /*version*/ )
{

  // if we are trying to load a const type,
  // we will essentially perform a "const_cast"
  // type of conversion
  typedef typename std::remove_const< Type >::type NonConstType;
  NonConstType * data;
  //Type * data;

  archive >> make_nvp( "shared_ptr", data );

  typedef std::weak_ptr< Type > WeakPtr;
  static boost::unordered_map< void *, WeakPtr > hash;

  if ( hash[ data ].expired() )
    {

    value = std::shared_ptr< Type >( data );

    hash[ data ] = value;

    }
  else
    {
    value = hash[ data ].lock();
    }

} // load (non-polymorphic)
////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////
template< class Archive, class Type >
inline void serialize(
  Archive & archive, std::shared_ptr< Type > & value,
  const unsigned int version )
{
  split_free( archive, value, version );
} // serialize
////////////////////////////////////////////////////////////////////////////////////////////////////



}} // namespace boost serialization
#endif // __ccipdSharedPointerSerialization_hxx
