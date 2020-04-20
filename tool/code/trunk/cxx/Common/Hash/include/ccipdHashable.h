#ifndef __ccipdHashable_h
#define __ccipdHashable_h



//////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdCore.h"
#include "ccipdMacros.h"

#include <memory>
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{

  
  
/**
 * 
 * This is just an abstract base class
 * for objects which are hashable.
 * 
 * Hashable objects must be able to return
 * a size_t hash value, defining its
 * uniqueness.
 * 
 * It's pretty easy to use Hashable
 * objects in an unordered_set.
 * For example, if HashableChild extends
 * the Hashable class, then you can define:
 * 
 * \code
 * typedef std::unordered_set<
 *   HashableChild, HashableFunctor<> >
 *     MySetType1;
 * 
 * typedef std::unordered_set<
 *   HashableChild *,
 *     HashableFunctor<>, HashableFunctor<> >
 *       MySetType2;
 * 
 * typedef std::shared_ptr< HashableChild > ChildPtr;
 * typedef std::unordered_set<
 *   ChildPtr, HashableFunctor< ChildPtr >,
 *     HashableFunctor< ChildPtr >>
 *       MySetType3;
 * \endcode
 * 
 */
class Hashable
{



//////////////////////////////////////////////////////////////////////////////////////////
  ccipdConstructorDeclarationMacro( Hashable )
//////////////////////////////////////////////////////////////////////////////////////////



public:
//////////////////////////////////////////////////////////////////////////////////////////

  /**
   * This will return either a cached
   * value or recalculate the hash.
   * 
   * Override this to never cache.
   */
  virtual HashValueType GetHash() const;

  /**
   * This will make sure we recalculate 
   * the hash next time we call GetHash.
   */
  virtual void ClearHash() const;

  /**
   * The child class must override the 
   * equality operator so that unordered containers
   * will work properly.
   * 
   * It may be a good idea to dynamically cast the
   * other Hashable object to the actual child class's
   * type for the comparison as follows:
   * 
   * virtual bool HashableChild::operator==(
   *   const Hashable & other ) const
   * {
   *   const HashableChild & otherChild =
   *     dynamic_cast< const HashableChild & >( other );
   *   
   *   if ( &otherChild == this )
   *     return true;
   *   
   *   ... ( Actually perform comparison ) ...
   * 
   * } // operator==
   * 
   */
  virtual bool operator==( const Hashable & other ) const = 0;
//////////////////////////////////////////////////////////////////////////////////////////



protected:
//////////////////////////////////////////////////////////////////////////////////////////
  /**
   * Class should override this function
   * to hash themselves
   */
  virtual HashValueType ReHash() const = 0;
//////////////////////////////////////////////////////////////////////////////////////////



private:
//////////////////////////////////////////////////////////////////////////////////////////
  ccipdPIMPLMacro
//////////////////////////////////////////////////////////////////////////////////////////



}; // class



//////////////////////////////////////////////////////////////////////////////////////////
/**
 * \brief A functor to use hashable objects in std::unordered_map.
 * 
 * \author  Rob Toth
 * \ingroup Common
 */
template< typename HashablePointer = Hashable * >
struct HashableFunctor
{
  HashValueType operator()( const Hashable & hashableObject ) const
  {
    return hashableObject.GetHash();
  }
  HashValueType operator()( const HashablePointer hashableObject ) const
  {
    if ( hashableObject )
      return hashableObject->GetHash();
    else
      return 34432255; // null - random value
  }
  bool operator()(
    const Hashable & hashableObject1,
    const Hashable & hashableObject2 ) const
  {
    return hashableObject1 == hashableObject2;
  }
  bool operator()(
    const HashablePointer hashableObject1,
    const HashablePointer hashableObject2 ) const
  {
    if ( !hashableObject1 && !hashableObject2 )
      return true;
    else if ( !hashableObject1 || !hashableObject2 )
      return false;
    else
      return (*hashableObject1) == (*hashableObject2);
  } 
};
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace
#endif // __ccipdHashable_h
