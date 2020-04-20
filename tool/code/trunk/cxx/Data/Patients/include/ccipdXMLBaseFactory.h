#ifndef __ccipdXMLBaseFactory_h
#define __ccipdXMLBaseFactory_h



//////////////////////////////////////////////////////////////////////////////////////////
// ccipd includes
#include "ccipdXMLBase.h"
#include "ccipdLoadableFactory.h"
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



/**
 * If you are XMLBase
 * like so:
 * 
 * class Foo : public XMLBase { };
 * 
 * Then you can also do:
 * class Foo : public XMLBase, public XMLBaseFactory< Foo > { };
 * 
 * This will then let you do:
 *  const std::shared_ptr< Foo > pointer = Foo::Create( someXMLNode );
 * 
 * \author  Rob Toth
 * \ingroup Data
 * \date    2013-01-22
 */
template< class TSelf >
class XMLBaseFactory :
  public LoadableFactory< TSelf >,
  virtual public boost::noncopyable
{



//////////////////////////////////////////////////////////////////////////////////////////
  ccipdConstructorDeclarationMacro( XMLBaseFactory )

public:
  typedef LoadableFactory< TSelf > Superclass;

  /// Define a smart pointer for ourself
  typedef typename Superclass::Pointer      Pointer;
  typedef typename Superclass::ConstPointer ConstPointer;
  
  /**
   * \brief We provide the ability to create ourself by loading a node.
   * 
   * If unable to load the node, a
   * default pointer is returned.
   * 
   * \author Rob Toth
   */
  static Pointer      New     ( const XMLNode & rootNode, const bool returnNULLOnError );
  static ConstPointer ConstNew( const XMLNode & rootNode, const bool returnNULLOnError );

  using Superclass::New;
  using Superclass::ConstNew;
  
//////////////////////////////////////////////////////////////////////////////////////////



}; // class XMLBaseFactory
} // namespace ccipd
#endif // __ccipdXMLBaseFactory_h
