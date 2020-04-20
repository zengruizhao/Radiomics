#ifndef __ccipdXMLBase_h
#define __ccipdXMLBase_h



//////////////////////////////////////////////////////////////////////////////////////////
/**
 * Any class that wishes to save/load itself to/from
 * xml files must implement this interface.
 */
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
// ccipd includes
#include "ccipdXMLFile.h"
#include "ccipdLoadable.h"
#include "ccipdSavable.h"
#include "ccipdMacros.h"

#include <string>
#include <ostream>
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



/**
 * \brief Base class for XML reading/writing.
 * 
 * \date    2012-02-18
 * \author  Rob Toth
 * \ingroup PatientData
 */
class XMLBase :
  virtual public Savable,
  virtual public Loadable
{



//////////////////////////////////////////////////////////////////////////////////////////
  // Constructors.
  ccipdConstructorDeclarationMacro( XMLBase )
//////////////////////////////////////////////////////////////////////////////////////////



public:
//////////////////////////////////////////////////////////////////////////////////////////
  /**
   * You must provide a way to load your variables from an xml node.
   * 
   * Do as follows for loading <variable_name>variable_value</variable_name>
   * 
   * std::string variable;
   * const bool loadedVariable = GetXMLString( rootNode, "variable_name", variable );
   * if ( loadedVariable )
   *   this->SetVariable( variable );
   * 
   * \returns true if any data was loaded
   */
  virtual bool LoadFromXML( const XMLNode & rootNode ) = 0;

  /**
   * \brief Load this class from an XML file.
   * \returns true if successful.
   */
  virtual bool Load( const std::string & xmlFile  );

  /**
   * \brief Save this class to an XML file.
   * \returns true if successful.
   */
  virtual bool Save( const std::string & xmlFile ) const;

  /**
   * You must override this.
   * 
   * You should do this:
   * 
   *   stream << "<variable_name>" << variable_value << "</variable_name>" << endl;
   *   ...
   * 
   */
  virtual void OutputToStream( std::ostream & stream ) const = 0; // must override

  /**
   * So you can cout << study
   * http://ccipd.rutgers.edu/short/o
   */
  friend std::ostream & operator<<( std::ostream & stream, const XMLBase & base );

//////////////////////////////////////////////////////////////////////////////////////////



}; // class
} // namespace
#endif // __ccipdXMLBase_h
