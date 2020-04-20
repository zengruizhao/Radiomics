


//////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdXMLFile.h"
#include "ccipdDisableWarningsMacro.h"

#include <boost/property_tree/ptree.hpp>      // for our xml tree
#include <boost/property_tree/xml_parser.hpp> // for our xml parser
#include <boost/foreach.hpp>                  // for looping over child nodes

//std includes
#include <iostream>
#include <string>
//////////////////////////////////////////////////////////////////////////////////////////

#include "ccipdEnableWarningsMacro.h"

//////////////////////////////////////////////////////////////////////////////////////////
using std::cerr;
using std::endl;
using std::string;
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////
XMLPointer GetXMLRootNode( const std::string & xmlFile, const bool silent )
{

  // if there's an error, what should we return?
  XMLPointer badOutput;

  // try to read the file
  XMLNode rootNode;
  try
    {

    // here we actually try to read
    boost::property_tree::read_xml( xmlFile, rootNode );

    // if it's empty, there's nothing to load
    if ( rootNode.empty() )
      return badOutput;

    // output the read node
    return XMLPointer( new XMLNode( rootNode ) );

    } // try

  catch( std::exception & e )
    {

    if ( !silent )
      {
      cerr << "Unable to read " << xmlFile << endl;
      cerr << e.what() << endl;
      }

    return badOutput;

    } // catch

  catch( ... )
    {

    if ( !silent )
      cerr << "Unknown error." << endl;

    return badOutput;

    } // catch 

} // GetXMLRootNode
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
/**
 * This will extract all child nodes with a given <tagName></tagName>
 */
XMLPointerCollectionType GetXMLChildNodes(
  const XMLNode & rootNode,
  const std::string tagName )
{

  // prepare the output
  XMLPointerCollectionType childNodes;

  // go through each node with the given tag
  BOOST_FOREACH( const auto & childIter, rootNode.equal_range( tagName ) )
    {

    // insert the given node
    childNodes.insert( XMLPointer( new XMLNode( childIter.second ) ) );

    } // for each mask

  return childNodes;

} // GetChildNodes
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
bool GetXMLString(
  const XMLNode & node,
  const string  & tagName,
        string  & output )
{

  try
    {

    // try to load the tag
    const string stringValue = node.get< string >( tagName );

    // set the output
    output = stringValue;

    // success
    return true;

    } // try
  catch( ... )
    {

    // failure
    return false;

    } // catch

} // GetXMLString
//////////////////////////////////////////////////////////////////////////////////////////


} // namespace
