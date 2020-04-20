

//////////////////////////////////////////////////////////////////////////////////////////

#include "ccipdSaveImageGUI.h"
#include "ccipdSaveImage.h" // for actually saving

#ifdef _MSC_VER
#pragma warning( push, 0 )
#endif // _MSC_VER

// itk includes
#include <itkImage.h>
#include <itkRGBPixel.h>

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
/**
 * Special for RGB slice types.
 */
template <>
bool SaveImage< RGBSliceType >(
  const RGBSliceType * const image,
        QWidget      * const parent,
  const bool                 verbose )
{

  // select a file to load
  const std::string fileName = QFileDialog::getSaveFileName(
    parent, "Save 2D RGB Image Slice",
    QString(),
    "Image Files (*.png *.jpg *.jpeg *.tif *.tiff *.bmp);;\
All Files (*)" ).toStdString();

  // do nothing
  if ( fileName.empty() )
    return false;

  // try to save it
  
  const bool saved = // try different types of saving
    SaveImage   ( image, fileName, verbose ) ||
    SaveRGBSlice( image, fileName, verbose ) ||
    SaveRGBSliceAfterCasting< unsigned char >(
      image, fileName, verbose ) ||
    SaveRGBSliceAfterCasting< unsigned short >(
      image, fileName, verbose );

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
