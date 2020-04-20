#ifndef __ccipdFactory_h
#define __ccipdFactory_h



//////////////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdEnableSharedFromThis.h"
#include "ccipdDisableWarningsMacro.h"
  #include <boost/noncopyable.hpp>
#include "ccipdEnableWarningsMacro.h"
//////////////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



/**
 * Extend from this class
 * to be allowed to call
 * the "New()" function
 * or "ConstNew()" function
 * for default object creation.
 * 
 * Define your own "New( const int parameter )" function
 * if you want to have parameters. 
 *
 * See Fatory.hxx for details on implementing serialization.
 * 
 * \author  Rob Toth
 * \date    2013-02-19
 * \ingroup Common
 */
template< class TSelf >
class Factory :
  virtual public EnableSharedFromThis< TSelf >,
  virtual public boost::noncopyable
{



public:
//////////////////////////////////////////////////////////////////////////////////////////////////

  typedef TSelf Self;
  typedef EnableSharedFromThis< Self >  SuperclassShared;
  typedef std::shared_ptr<       Self > Pointer;
  typedef std::shared_ptr< const Self > ConstPointer;

  static Pointer      New();
  static ConstPointer ConstNew();

//////////////////////////////////////////////////////////////////////////////////////////////////



protected:
//////////////////////////////////////////////////////////////////////////////////////////////////
           Factory(); ///< No public default constructor
  virtual ~Factory(); ///< Non-public destructor
//////////////////////////////////////////////////////////////////////////////////////////////////



}; // class Factory
} // namespace ccipd
#endif // __ccipdFactory_h
