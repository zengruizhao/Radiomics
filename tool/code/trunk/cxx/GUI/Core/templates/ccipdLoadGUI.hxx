


//////////////////////////////////////////////////////////////////////////////////////////

// ccipd includes
#include "ccipdLoadGUI.h"
#include "ccipdLoadableFactory.h"

#ifdef _MSC_VER
#pragma warning( push, 0 )
#endif // _MSC_VER

// std includes
#include <string>

// qt includes
#include <QFileDialog> // for opening files
#include <QMessageBox> // for displaying messages

// boost includes
#include <boost/static_assert.hpp>
#include <boost/type_traits/is_base_of.hpp>

#ifdef _MSC_VER
#pragma warning( pop )
#endif // _MSC_VER

//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
using std::string;
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{




//////////////////////////////////////////////////////////////////////////////////////////
template< class TLoadable >
std::shared_ptr< TLoadable > LoadObjectGUI(
        QWidget * const parent,
  const bool            verbose )
{
  return LoadObjectAndFileNameGUI< TLoadable >( parent, verbose ).first;
} // LoadObjectGUI
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
template< class TLoadable >
std::string LoadFileNameGUI(
        QWidget * const parent,
  const bool            verbose )
{
  return LoadObjectAndFileNameGUI< TLoadable >( parent, verbose ).second;
} // LoadFileNameGUI
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
template< class TLoadable >
std::pair< std::shared_ptr< TLoadable >, std::string >
LoadObjectAndFileNameGUI(
        QWidget * const parent,
  const bool            verbose )
{

  BOOST_STATIC_ASSERT_MSG(
    ( boost::is_base_of< Loadable, TLoadable >::value ),
    "You can only use the GUI to load 'Loadable'-derived classes!" );

  // select a file to load
  const std::string fileName = QFileDialog::getOpenFileName(
    parent, "Open file",
    QString(),
    "XML File (*.xml);;All Files (*)" ).toStdString();

  // typedefs
  typedef LoadableFactory< TLoadable > FactoryType;
  typedef typename FactoryType::Pointer Pointer;
  typedef std::pair< Pointer, std::string > OutputType;

  // do nothing if the user pressed cancel
  if ( fileName.empty() )
    return OutputType();

  // try to load it
  const bool returnNULLOnError = true;
  const Pointer loadedObject =
    FactoryType::New( fileName, returnNULLOnError );

  if ( loadedObject )
    {

    // okay return the loaded object
    return OutputType( loadedObject, fileName );

    }
  else // there was a problem loading
    {

    // display a message sometimes
    if ( verbose )
      {
      QMessageBox::warning(
        parent,
        "Invalid File",
        std::string( "Unable to load " + fileName ).c_str(),
        QMessageBox::Cancel );
      }

    // return an empty pointer on a failure
    return OutputType();

    } // loaded

} // LoadGUI
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace ccipd
