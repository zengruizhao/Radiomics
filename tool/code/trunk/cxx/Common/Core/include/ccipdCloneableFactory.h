#ifndef __ccipdCloneableFactory_h
#define __ccipdCloneableFactory_h



//////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdCloneable.h"
#include "ccipdFactory.h"
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////
/**
 * This class provides both
 * clone and factor methods.
 * 
 * The only thing
 * your class "TDerived"
 * needs to do is implement
 * CopyParameters if desired.
 * 
 * \author  Rob Toth
 * \date    2013-02-23
 * \ingroup Common
 */
template< class TDerived >
class CloneableFactory :
  public Cloneable< TDerived >,
  public Factory  < TDerived >
{

public:

  typedef TDerived                    Derived;
  typedef CloneableFactory< Derived > Self;
  typedef Cloneable       < Derived > SuperclassCloneable;
  typedef Factory         < Derived > SuperclassFactory;

  typedef typename SuperclassCloneable::Pointer      Pointer;
  typedef typename SuperclassCloneable::ConstPointer ConstPointer;

  using SuperclassCloneable::NonStaticNew;
  using SuperclassCloneable::NonStaticConstNew;
  using SuperclassCloneable::New;
  using SuperclassCloneable::ConstNew;
  using SuperclassFactory  ::New;
  using SuperclassFactory  ::ConstNew;

}; // class CloneableFactory
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace ccipd
#endif // __ccipdCloneableFactory_h
