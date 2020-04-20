

//////////////////////////////////////////////////////////////////////////////////////////

#include "ccipdSaveLinearTransformGUI.h"
#include "ccipdSaveLinearTransform.h" // for actually saving

#ifdef _MSC_VER
#pragma warning( push, 0 )
#endif // _MSC_VER

// itk includes

// std includes
#include <iostream>

// qt includes
#include <QFileDialog> // for opening files
#include <QMessageBox> // for displaying messages

#ifdef _MSC_VER
#pragma warning( pop )
#endif // _MSC_VER

//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////
bool SaveLinearTransform(
  const VolumeLinearTransformType &       transform,
        QWidget                   * const parent,
  const bool                              verbose )
{

  // select a file to load
  const std::string fileName = QFileDialog::getSaveFileName(
    parent, "Save Linear Transform",
    QString(),
    "Text Files (*.txt );;All Files (*)" ).toStdString();

  // do nothing
  if ( fileName.empty() )
    return false;

  // try to save it
  
  const bool saved = // try to save
    SaveLinearTransform( transform, fileName );

  if ( !saved && verbose )
    {
    QMessageBox::warning(
      parent,
      "Save Failed",
      std::string( "Unable to save " + fileName ).c_str(),
      QMessageBox::Ok );
    }

  return saved;

} // SaveRGBSlice
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace ccipd
