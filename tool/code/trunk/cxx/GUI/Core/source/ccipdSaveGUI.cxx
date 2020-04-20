


//////////////////////////////////////////////////////////////////////////////////////////

// ccipd includes
#include "ccipdSaveGUI.h"
#include "ccipdSavable.h"
#include "ccipdDisableWarningsMacro.h"


// std includes
#include <iostream>

// qt includes

#include <QFileDialog> // for opening files
#include <QMessageBox> // for displaying messages
#include "ccipdEnableWarningsMacro.h"

//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
using std::cout;
using std::endl;
using std::string;
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////
string SaveObjectGUI(
  const Savable &       objectToSave,
        QWidget * const parent,
  const bool            verbose )
{
  
  // select a file to save
  const string fileName = QFileDialog::getSaveFileName(
    parent, "Save object file",
    QString(),
    "XML File (*.xml);;All Files (*)" ).toStdString();

  if ( verbose ) cout << "Saving " << fileName << ":" << endl;
  const bool saved = SaveObjectGUI(
    objectToSave, fileName, parent, verbose );

  if ( saved )
    {
    if ( verbose ) cout << "Saving " << fileName << " done." << endl;
    return fileName;
    }
  else
    {
    if ( verbose ) cout << "Saving " << fileName << " failed." << endl;
    return string();
    }

} // SaveMFASegmenter
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
bool SaveObjectGUI(
  const Savable &       objectToSave,
  const string  &       fileName,
        QWidget * const parent,
  const bool            verbose )
{

  // do nothing if there is no filename
  if ( fileName.empty() )
    return false;

  // try to save it
  if ( verbose ) cout << "Saving " << fileName << ":" << endl;
  const bool saved = objectToSave.Save( fileName );

  if ( !saved && verbose )
    {
    if ( verbose ) cout << "Saving " << fileName << " failed." << endl;

    QMessageBox::warning(
      parent,
      "Not Saved",
      std::string( "Unable to save " + fileName ).c_str(),
      QMessageBox::Cancel );
    } // saved
  else if ( verbose )
    {
    cout << "Saving " << fileName << " done." << endl;
    }

  return saved;

} // SaveMFASegmenter
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace ccipd
