#ifndef __ccipdCopyable_h
#define __ccipdCopyable_h



//////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdMacros.h"
#include <boost/mpl/accumulate.hpp>
#include <boost/mpl/empty_base.hpp>
#include <boost/noncopyable.hpp>
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////
/*
 * \brief Base class for any class which can copy its parameters.
 * 
 * \author Rob Toth
 */
class CopyableBase : virtual public boost::noncopyable
{

  ccipdConstructorEmptyDefinitionsMacro( CopyableBase )
public:
  virtual void CopyParameters( const Self & other ) = 0;
}; // class CopyableBase
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
/*
 * \brief Helper class which requires only an internal copy function to work.
 * 
 * \author Rob Toth
 */
template< class TDerived >
class Copyable : virtual public CopyableBase
{

  ccipdConstructorEmptyDefinitionNoSelfMacro( Copyable )

public:
  typedef TDerived            Derived;
  typedef Copyable< Derived > Self;
  typedef CopyableBase        Superclass;

  /// We provide the parameter copying.
  virtual void CopyParameters( const Superclass & other )
  {
    // try to cast it
    try
      {
      const Derived & castedOther = dynamic_cast< const Derived & >( other );
      this->CopyParametersInternal( castedOther );
      }
    catch( ... ) { }
  } // CopyParameters
  
protected:
  /// You must provide an internal function to copy your own parameters.
  virtual void CopyParametersInternal( const Derived & other ) = 0;

}; // class Copyable
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
/**
 * If you wish to extend another "Copyable" class,
 * and also be Copyable yourself,
 * instead of just extending it directly,
 * extend this class.
 * This will call the correct "copy"
 * functions, etc.
 * 
 * For example:
 * 
 * class Derived1 : public Copyable< Derived1 >
 * ...
 * class Derived2 : public Derived1, public Copyable< Derived2 >
 * 
 * The above is no good. Rather,
 * use the following:
 * 
 * class Derived2 : public CopyableDerived< Derived2, Derived1 >
 * 
 * \ingroup Common
 * \author  Rob Toth
 * \date    2013-02-23
 */
template<
  class TDerived,
  class TBaseOfDerived
  >
class CopyableDerived :
  virtual public Copyable< TDerived >,
  virtual public TBaseOfDerived
{
public:

  // If your code compiles here, make sure
  // that TBaseOfDerived also extends the Copyable
  // class.
  
  static_assert( std::is_base_of<
    CopyableBase, TBaseOfDerived >::value,
    "This class is only for deriving other copyable classes.\
TBaseOfDerived needs to also be copyable." );

  typedef CopyableDerived< TDerived, TBaseOfDerived > CopyableDerivedSelf;

  virtual ~CopyableDerived() { }

  virtual void CopyParameters( const CopyableBase & other )
  {
    this->Copyable< TDerived >::CopyParameters( other );
    this->TBaseOfDerived      ::CopyParameters( other );
  } // CopyParameters  

}; // class Copyable
//////////////////////////////////////////////////////////////////////////////////////////




//////////////////////////////////////////////////////////////////////////////////////////
/**
 * This class can be used
 * to chain multiple derived
 * copyable classes as follows:
 
struct TestSingle1 : Copyable< TestSingle1 > { };
struct TestSingle2 : Copyable< TestSingle2 > { };

struct TestMultiple1 :
  CopyableDerived<
    TestMultiple1,
    CopyableDerivedChainable<
      TestSingle1,
      CopyableDerivedChainable<
        TestSingle2 > > >
{
  void TestFunction()
  {
    this->TestMultiple1::CopyParameters( *this );
    this->TestSingle1  ::CopyParameters( *this );
    this->TestSingle2  ::CopyParameters( *this );
  }
};

 *
 * If this class complains about "ambiguous" inheritance
 * of some functions, that's because it cannot handle
 * the same function name in several of its child classes.
 * 
 * If that occurs, you can either (1) rename your functions,
 * or (2) not use the class. Instead, do this:
 * 

 struct TestMultiple1 :
   virtual public TestSingle1,
   virtual public TestSingle2
 {
   virtual void CopyParameters( const CopyableBase & other )
   {

     // only if we are also copyable:
     this->Copyable< Self >::CopyParameters( other );

     // call our child classe's copy parameters
     this->TestSingle1::CopyParameters( other );
     this->TestSingle2::CopyParameters( other );

   } // CopyParameters

 }; // struct


 * 
 * \author Rob Toth
 */
template<
  class TBaseOfDerived1,
  class TBaseOfDerived2 = boost::mpl::empty_base >
class CopyableDerivedChainable :
  virtual public TBaseOfDerived1,
  virtual public TBaseOfDerived2
{
public:

  virtual ~CopyableDerivedChainable() { }
  
private:

  /**
   * If we are an empty base,
   * then don't do anything.
   */
  template< typename TBaseOfDerived >
  typename std::enable_if<
    std::is_same< TBaseOfDerived, boost::mpl::empty_base >::value >::type
  CopyParametersEnabled( const CopyableBase & )
  {
  }
  
  template< typename TBaseOfDerived >
  typename std::enable_if<
    !std::is_same< TBaseOfDerived, boost::mpl::empty_base >::value >::type
  CopyParametersEnabled( const CopyableBase & other )
  {

    static_assert(
      std::is_same< TBaseOfDerived, TBaseOfDerived1 >::value ||
      std::is_same< TBaseOfDerived, TBaseOfDerived2 >::value,
      "TBaseOfDerived must be one of your input template arguments!" );

    static_assert(
      std::is_base_of< CopyableBase, TBaseOfDerived >::value,
      "TBaseOfDerived must be Copyable!" );

    this->TBaseOfDerived::CopyParameters( other );

  } // CopyParametersEnabled
  
public:
  virtual void CopyParameters( const CopyableBase & other )
  {
    // call "CopyParameters" from each derived
    this->CopyParametersEnabled< TBaseOfDerived1 >( other );
    this->CopyParametersEnabled< TBaseOfDerived2 >( other );
  }  

}; // class Copyable
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
/**
 * Boost mpl helper struct.
 * \see http://bit.ly/15dp4VQ
 * 
 * \author Rob Toth
 */
struct CopyableChainMetafunctionClass
{
  template< class TNewClass, class TExistingChain >
  struct apply
  {
    typedef CopyableDerivedChainable< TNewClass, TExistingChain > type;
  };
};
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
/**
 * You are recommended
 * to do the following if you
 * are deriving from multiple
 * other copyable
 * classes.
 
struct TestMultiple2 :
    CopyableDerivedChain< boost::mpl::set< TestSingle1, TestSingle2, TestSingle3 > >
{
  void TestFunction()
  {
    this->TestSingle1  ::CopyParameters( *this );
    this->TestSingle2  ::CopyParameters( *this );
    this->TestSingle3  ::CopyParameters( *this );
  }
};

 * 
 * \author Rob Toth
 */
template< class TBasesOfDerived >
struct CopyableDerivedChain :
  boost::mpl::accumulate<
    TBasesOfDerived,
    boost::mpl::empty_base,
    CopyableChainMetafunctionClass >::type
{
  virtual ~CopyableDerivedChain() { }
};
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
/**
 * You are recommended
 * to do the following
 * if you are copyable
 * and you want to
 * derived from multiple other
 * copyable classes.
 * 

struct TestMultiple3 :
  CopyableMultipleDerived<
    TestMultiple3,
    boost::mpl::set< TestSingle1, TestSingle2, TestSingle3 > >
{
  void TestFunction()
  {
    this->TestMultiple3::CopyParameters( *this );
    this->TestSingle1  ::CopyParameters( *this );
    this->TestSingle2  ::CopyParameters( *this );
    this->TestSingle3  ::CopyParameters( *this );
  }

};

 * 
 * See the above comments in CopyableDerivedChainable
 * for what to do when you have ambiguous
 * inheritance problems
 * 
 * \author Rob Toth
 */
template< class TDerived, class TBasesOfDerived >
struct CopyableMultipleDerived :
  CopyableDerived< TDerived, CopyableDerivedChain< TBasesOfDerived > >
{
  virtual ~CopyableMultipleDerived() { }
};
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace ccipd
#endif // __ccipdCopyable_h
