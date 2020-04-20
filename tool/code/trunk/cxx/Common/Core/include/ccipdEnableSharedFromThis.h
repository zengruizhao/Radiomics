#ifndef __ccipdEnableSharedFromThis_h
#define __ccipdEnableSharedFromThis_h



//////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdMacros.h"
#include <memory>
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{


/**
 * Somewhat based on http://bit.ly/XJWZS5
 * and http://bit.ly/VSJb7D
 * 
 * If others will be extending you
 * as a virtual class, such as:
 * 
 * class Other : virtual public You { };
 * 
 * and you want the enable_shared_from_this
 * functionality, you should use this class instead
 * as follows:
 * 
 * class You : public EnableSharedFromThis< You >
 * class Other : virtual public You, public EnableSharedFromThis< Other >
 * 
 * Or, you can not make 'You' virtual and it will still work.
 * 
 */
class EnableSharedFromThisBase :
  public std::enable_shared_from_this< EnableSharedFromThisBase >
{
  ccipdConstructorEmptyDefinitionNoSelfMacro( EnableSharedFromThisBase )
}; // class EnableSharedFromThisBase
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
template< class T >
class EnableSharedFromThis : virtual public EnableSharedFromThisBase
{

  ccipdConstructorEmptyDefinitionNoSelfMacro( EnableSharedFromThis )

public:
  typedef T                             Self;
  typedef std::shared_ptr<       Self > Pointer;
  typedef std::shared_ptr< const Self > ConstPointer;

  Pointer      shared_from_this();
  ConstPointer shared_from_this() const;

}; // class EnableSharedFromThis
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace ccipd
#endif // __ccipdEnableSharedFromThis_h
