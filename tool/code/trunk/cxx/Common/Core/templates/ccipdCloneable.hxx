#ifndef __ccipdCloneable_hxx
#define __ccipdCloneable_hxx



//////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdCloneable.h"

#ifdef _MSC_VER
#pragma warning( disable: 4127 )
#endif

//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////
template< class TDerived >
typename
Cloneable< TDerived >::Pointer
Cloneable< TDerived >::Clone() const
{
  // copy the parameters of ourself to a new instance
  return this->NonStaticNew( *this );
  //return Self::New( *this );
}
//////////////////////////////////////////////////////////////////////////////////////////

  
  
//////////////////////////////////////////////////////////////////////////////////////////
template< class TDerived >
typename
Cloneable< TDerived >::ConstPointer
Cloneable< TDerived >::ConstNew( const CopyableBase & other )
{
  return ConstPointer( Self::New( other ) );
}
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
template< class TDerived >
typename
Cloneable< TDerived >::ConstPointer
Cloneable< TDerived >::NonStaticConstNew( const CopyableBase & other ) const
{
  return ConstPointer( this->NonStaticNew( other ) );
}
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
/**
 * If T has a static function New, this will say so.
 * If T has a const member function New, this will say so.
 */
template< typename T1 >
struct HasNew
{

private:
  typedef char ( &HasNewFalse )[ 1 ];
  typedef char ( &HasNewTrue  )[ 2 ];

  template < typename T2, typename T2::Pointer(*)() >
  struct HasStaticNewHelper {};
  
  //template < typename T2, typename T2::Pointer( T2::* )() >
  template < typename T2, typename T2::Pointer ( T2::* )() const >
  struct HasNonStaticNewHelper {};

  template< typename T2 >
  static HasNewFalse HasStaticNewTest(...);

  template< typename T2 >
  static HasNewFalse HasNonStaticNewTest(...);

  template< typename T2 >
  static HasNewTrue HasStaticNewTest( HasStaticNewHelper< T2, &T2::New > * );

  template< typename T2 >
  static HasNewTrue HasNonStaticNewTest( HasNonStaticNewHelper< T2, &T2::New > * );

public:
  BOOST_STATIC_CONSTANT( bool, hasStaticNew =
    sizeof( HasStaticNewTest< T1 >( 0 ) ) == sizeof( HasNewTrue ) );
  BOOST_STATIC_CONSTANT( bool, hasNonStaticNew =
    sizeof( HasNonStaticNewTest< T1 >( 0 ) ) == sizeof( HasNewTrue ) );

}; // struct HasNew
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
/**
 * This is a helper class which
 * will return NULL pointers
 * if the given class T1
 * does not have the appropriate
 * "New" functions.
 */
template< typename T1 >
struct CloneableNewHelper
{

private:

  typedef typename T1::Pointer Pointer;

  template< typename T2, typename TEnabled = void >
  struct CloneableNewHelperStatic
  {
    static_assert( std::is_same< T1, T2 >::value,
      "T1 and T2 must be the same!" );
    static Pointer New() { return Pointer(); }
  };

  template< typename T2 >
  struct CloneableNewHelperStatic< T2,
    typename std::enable_if< HasNew< T2 >::hasStaticNew >::type >
  {
    static_assert( std::is_same< T1, T2 >::value,
      "T1 and T2 must be the same!" );
    static Pointer New() { return T2::New(); }
  };
  
  template< typename T2, typename TEnabled = void >
  struct CloneableNewHelperNonStatic
  {
    static_assert( std::is_same< T1, T2 >::value,
      "T1 and T2 must be the same!" );
    static Pointer New( const T2 & )
    {
      return Pointer();
    }
  };

  template< typename T2 >
  struct CloneableNewHelperNonStatic< T2,
    typename std::enable_if< HasNew< T2 >::hasNonStaticNew >::type >
  {
    static_assert( std::is_same< T1, T2 >::value,
      "T1 and T2 must be the same!" );
    static Pointer New( const T2 & existingObject)
    {
      return existingObject.New();
    }
  };

public:
  static Pointer StaticNew()
  {
    return CloneableNewHelperStatic< T1 >::New();
  }

  static Pointer NonStaticNew( const T1 & existingObject )
  {
    return CloneableNewHelperNonStatic< T1 >::New( existingObject );
  }

}; // CloneableNewHelper
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
template< class TDerived >
typename
Cloneable< TDerived >::Pointer
Cloneable< TDerived >
::NonStaticNew( const CopyableBase & other ) const
{
  
  BOOST_CONSTEXPR_OR_CONST bool
    hasStaticNew    = HasNew< TDerived >::hasStaticNew,
    hasNonStaticNew = HasNew< TDerived >::hasNonStaticNew;

  // we need one or the other
  static_assert( hasStaticNew || hasNonStaticNew,
    "You need either a static 'New()' or \
  non-static 'New() const' function to be Cloneable!" );

  // prefer the static version
  if ( hasStaticNew )
    {
    return Self::New( other );
    }
  else if ( hasNonStaticNew )
    {

    // we need to make sure we derive from this class
    static_assert( std::is_base_of< Self, TDerived >::value,
      "You must derived yourself from the Cloneable class!" );

    // try the non-static version
  
    // first create a new instance
    const Pointer newInstance =
      CloneableNewHelper< TDerived >::NonStaticNew(
        static_cast< const TDerived & >( *this ) );

    // if it exists, copy the parameters
    if ( newInstance )
      {
      newInstance->CopyParameters( other );
      }

    // output
    return newInstance;

    } // non static new

} // NonStaticNew
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
template< class TDerived >
typename
Cloneable< TDerived >::Pointer
Cloneable< TDerived >
::New( const CopyableBase & other )
{

  /*
  static_assert( HasStaticNew< TFactory >::value,
    "TFactory must have a static New function returning a TDerived::Pointer!" );
  */

  // first create a new instance
  const Pointer newInstance =
    CloneableNewHelper< TDerived >::StaticNew();

  //const Pointer newInstance = TFactory::New();

  // if it exists, copy the parameters
  if ( newInstance )
    {
    newInstance->CopyParameters( other );
    }

  // output
  return newInstance;

} // Clone
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace ccipd
#endif // __ccipdCloneable_hxx
