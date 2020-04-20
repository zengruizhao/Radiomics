


//////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdXMLBase.h"
#include "ccipdXMLFile.h"                 // for xml functions

#include <fstream>                       // for saving files
#include <boost/property_tree/ptree.hpp> // for xml trees
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
using std::string;
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////
XMLBase::XMLBase()
{
}
XMLBase::~XMLBase()
{
}
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
std::ostream & operator<<( std::ostream & stream, const XMLBase & base )
{

  base.OutputToStream( stream );
  return stream;

} // operator<<
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
// every child still must override this
bool XMLBase::LoadFromXML( const XMLNode & rootNode )
{
  // don't warn about unused rootNode
  (void)rootNode;
  
  return false;
}
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
bool XMLBase::Load( const string & xmlFile )
{

  // try to load the root node
  const ccipd::XMLPointer rootNode = GetXMLRootNode( xmlFile );

  // if successful, use it to load the XML
  if ( rootNode )
    {
    return this->LoadFromXML( *rootNode );
    }
  else
    {
    return false;
    }

} // LoadFromXML
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
bool XMLBase::Save( const string & xmlFile ) const
{

  // open the file
  std::ofstream file;
  file.open( xmlFile );

  const bool opened = file.is_open();

  // write the xml data
  if ( opened )
    file << *this;

  // was writing successful?
  const bool successful = opened && !file.fail();

  // close the file
  file.close();

  return successful;

} // SaveToXML
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace