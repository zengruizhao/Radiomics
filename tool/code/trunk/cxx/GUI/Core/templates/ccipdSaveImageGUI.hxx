

//////////////////////////////////////////////////////////////////////////////////////////

#include "ccipdSaveImageGUI.h"
#include "ccipdSaveImage.h" // for actually saving

#ifdef _MSC_VER
#pragma warning( push, 0 )
#endif // _MSC_VER

// std includes
#include <iostream>
#include <string>

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
template< class TImageType >
bool SaveImage(
  const TImageType * const image,
        QWidget    * const parent,
  const bool               verbose )
{

  if ( !image ) return false;
  
  using std::string;

  // select a file to save
  const string fileName = QFileDialog::getSaveFileName(
    parent, "Save Image",
    QString(),
    (string( "Meta-Image Files (*.mha *.mhd);;" ) +
    string( "VTK Files (*.vtk);;" ) +
    string( "NRRD Files (*.nrrd);;" ) +
    string( "All Files (*)" )).c_str() ).toStdString();

  // do nothing
  if ( fileName.empty() )
    return false;

  // try to save it
  const bool saved = SaveImage< TImageType >( image, fileName, verbose );

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
