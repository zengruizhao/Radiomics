#ifndef __ccipdXMLFile_h
#define __ccipdXMLFile_h


#include "ccipdDisableWarningsMacro.h"
//////////////////////////////////////////////////////////////////////////////////////////
#include <string>
#include <memory>     // for pointers
#include <functional> // for std::less
#include <set>        // for xml pointer collections
#include <boost/property_tree/ptree_fwd.hpp> // forward declaration
//////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdEnableWarningsMacro.h"


namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////
/** \ingroup PatientData */
typedef boost::property_tree::basic_ptree<
  std::string,
  std::string,
  boost::property_tree::detail::less_nocase< std::string >
> XMLNode;

/**
 * This pointer isn't meant to be passed around.
 * Just use it after you get your root node, and then
 * let it get deleted, preferably just passing the XMLNode
 * itself by reference to child functions.
 * 
 * The reason it's a pointer is so we can forward declare
 * the boost class without including any boost headers here.
 * 
 * \ingroup PatientData
 */
typedef std::shared_ptr<
  const XMLNode
> XMLPointer;

/** \ingroup PatientData */
typedef std::set<
  XMLPointer
> XMLPointerCollectionType;
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
/**
 * This will extract a "root" node from a given XML file.
 * 
 * The root node doesn't correspond with any <> tags.
 * 
 * You can then use the equal_range function to get all
 * the children with a given tag.
 * 
 * For example:
 * 
 *   // go through each node in the file with the tag <node1></node1>
 *   BOOST_FOREACH(
 *     const auto & myNodeIter,
 *     GetXMLRootNode( xmlFile )->equal_range( "node1" ) )
 *     {
 * 
 *     const XMLNode currentNode = myNodeIter.second;
 *     // do something with the XML node, possibly recursively
 * 
 *     }
 * 
 * \ingroup PatientData
 */
XMLPointer GetXMLRootNode(
  const std::string & xmlFile,
  const bool          silent = false );
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
/**
 * This will extract all child nodes with a given <tagName></tagName>
 * 
 * \see GetXMLRootNode()
 * \ingroup PatientData
 */
XMLPointerCollectionType GetXMLChildNodes(
  const XMLNode & rootNode,
  const std::string tagName );
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
/**
 * 
 * Given a root node, it will search the immediate
 * children for a tag <tagName>MyString<tagName>
 * 
 * and set output to be "MyString".
 * 
 * If successful, it will return true.
 * 
 * \ingroup PatientData
 */
bool GetXMLString(
  const XMLNode     & node,
  const std::string & tagName,
        std::string & output );
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace
#endif // __ccipdXMLFile_h
