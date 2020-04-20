

//////////////////////////////////////////////////////////////////////////////////////////

#include "ccipdLoadRGBSliceGUI.h"
#include "ccipdLoadRGBSlice.h" // for actually loading

#ifdef _MSC_VER
#pragma warning( push, 0 )
#endif // _MSC_VER

// itk includes
#include <itkImage.h>
#include <itkRGBPixel.h>

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
RGBSlicePointer LoadRGBSlice(
  QWidget * const parent,
  const bool      verbose )
{

  // select a file to load
  const std::string fileName = QFileDialog::getOpenFileName(
    parent, "Open 2D RGB Image Slice",
    QString(),
    "Image Files (*.png *.jpg *.jpeg *.tif *.tiff *.bmp);;\
All Files (*)" ).toStdString();

  // do nothing
  if ( fileName.empty() )
    return RGBSlicePointer();

  // try to load it
  const RGBSlicePointer image( LoadRGBSlice( fileName ) );

  if ( !image && verbose )
    {
    QMessageBox::warning(
      parent,
      "Invalid File",
      std::string( "Unable to load " + fileName ).c_str(),
      QMessageBox::Ok );
    }

  return image;

} // LoadRGBSlice
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace ccipd
