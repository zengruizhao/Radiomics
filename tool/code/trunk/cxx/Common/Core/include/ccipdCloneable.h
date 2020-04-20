#ifndef __ccipdCloneable_h
#define __ccipdCloneable_h



//////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdCopyable.hxx"
#include <memory>
#include <functional>
#include <type_traits>
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////
template< class TDerived >
class Cloneable : virtual public CopyableBase
{

public:

  typedef TDerived             Derived;
  typedef Cloneable< Derived > Self;

  typedef std::shared_ptr<       Derived > Pointer;
  typedef std::shared_ptr< const Derived > ConstPointer;

  /**
   * We can create a new instance of
   * ourself and copy the parameters
   * of another copyable object
   * 
   * The factory class must provide a static "New"
   * function or you will get an compilation error.
   */
  static Pointer      New     ( const CopyableBase & other );
  static ConstPointer ConstNew( const CopyableBase & other );
  
  Pointer      NonStaticNew     ( const CopyableBase & other ) const;
  ConstPointer NonStaticConstNew( const CopyableBase & other ) const;

  /**
   * We provide the ability
   * to clone the derived class.
   */
  Pointer Clone() const;
  
}; // class Cloneable
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace ccipd
#endif // __ccipdCloneable_h
