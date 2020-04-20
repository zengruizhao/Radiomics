


//////////////////////////////////////////////////////////////////////////////////////////
// ccipd includes
#include "ccipdPatientTypes.h" // for typedef's
#include "ccipdStudy.h"        // test study loading
#include "ccipdPatient.h"      // test patient loading
#include "ccipdPatients.h"     // test patients loading

// boost includes
#include <boost/type_traits/is_base_of.hpp>
#include <boost/static_assert.hpp>

// std includes
#include <cstdlib>            // for EXIT_SUCCESS
#include <string>             // for file names
//////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////
using std::cout;
using std::endl;
using std::string;
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void LoadXMLTest( ccipd::XMLBase & xmlClass, const string & xmlFile )
{

  cout << "Loading xml file " << xmlFile << ":" << endl;
    
  xmlClass.Load( xmlFile );

  cout << "Loading xml file " << xmlFile << " done." << endl;
  cout << xmlClass << endl;

} // LoadXMLTest
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
template< class TXMLClass >
bool SaveXMLTest( const TXMLClass & xmlClass, const string & xmlFile )
{

  BOOST_STATIC_ASSERT_MSG(
    ( boost::is_base_of< ccipd::LoadableFactory< TXMLClass >, TXMLClass >::value ),
    "You must be derived from LoadableFactory." );

  cout << "Saving xml file " << xmlFile << ":" << endl;

  const bool saved = xmlClass.Save( xmlFile );

  cout << "Saving xml file " << xmlFile;
  if ( saved )
    cout << " done." << endl;
  else
    {
    cout << " failed." << endl;
    return false;
    }
  
  // try reloading the saved class
  cout << "Reloading " << xmlFile << ":" << endl;
    
  const typename TXMLClass::Pointer reloadedClassPointer =
    TXMLClass::New( xmlFile, true );

  const TXMLClass & reloadedClass = *reloadedClassPointer;

  cout << "Reloading " << xmlFile << " done." << endl;

  // check the hashes
  const auto
    classHash    = xmlClass     .GetHash(),
    reloadedHash = reloadedClass.GetHash();
  const bool
    hashesEqual = ( classHash == reloadedHash );

  cout << "Class    hash = " << classHash    << endl;
  cout << "Reloaded hash = " << reloadedHash << endl;

  // check equality
  const bool classesEqual = ( xmlClass == reloadedClass );
  cout << "Hashes  equal = " << hashesEqual  << endl;
  cout << "Classes equal = " << classesEqual << endl;

  return hashesEqual && classesEqual;

} // SaveXMLTest
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
int main( int argc, char * argv[] )
{

  if ( argc == 1 )
    {

    cout << endl << "Usage: " << endl;
    cout <<
      argv[ 0 ] <<
      " [study input xml file]"     <<
      " [patient input xml file]"   <<
      " [patients input xml file]"  <<
      " [study output xml file]"    << 
      " [patient output xml file]"  << 
      " [patients output xml file]" << endl;

    return EXIT_SUCCESS;

    } // argc == 1

  // initialize
  using ccipd::Study;
  using ccipd::Patient;
  using ccipd::Patients;
  
  Study   ::Pointer study    = Study   ::New();
  Patient ::Pointer patient  = Patient ::New();
  Patients::Pointer patients = Patients::New();
  
  if ( argc > 1 )
    LoadXMLTest( *study, argv[ 1 ] );

  if ( argc > 2 )
    LoadXMLTest( *patient, argv[ 2 ] );
  
  if ( argc > 3 )
    LoadXMLTest( *patients, argv[ 3 ] );
  
  bool successful = true;

  if ( argc > 4 )
    successful = successful && SaveXMLTest< Study >( *study, argv[ 4 ] );

  if ( argc > 5 )
    successful = successful && SaveXMLTest< Patient >( *patient, argv[ 5 ] );

  if ( argc > 6 )
    successful = successful && SaveXMLTest< Patients >( *patients, argv[ 6 ] );

  if ( successful )
    {
    cout << "All XML tests successful." << endl;
    return EXIT_SUCCESS;
    }
  else
    {
    cout << "One or more XML tests failed." << endl;
    return EXIT_FAILURE;
    }

} // main
//////////////////////////////////////////////////////////////////////////////////////////
