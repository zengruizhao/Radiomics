

//////////////////////////////////////////////////////////////////////////////////////////

#include "ccipdLoadVolumeGUI.h"
#include "ccipdLoadVolume.h" // for actually loading

#ifdef _MSC_VER
#pragma warning( push, 0 )
#endif // _MSC_VER

// itk includes
#include <itkImage.h>

// qt includes
#include <QFileDialog> // for opening files
#include <QMessageBox> // for displaying messages
#include <qfilesystemmodel.h>
#include <qlistview.h>

#ifdef _MSC_VER
#pragma warning( pop )
#endif // _MSC_VER

//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////
VolumeImagePointer LoadVolume(
  QWidget * const parent,
  const bool      verbose )
{

  // select a file to load
  const std::string fileName = QFileDialog::getOpenFileName(
    parent, "Open Volume Image",
    QString(),
    "Image Files (*.mha *.mhd *.nrrd);;\
All Files (*)" ).toStdString();

  // do nothing
  if ( fileName.empty() )
    return VolumeImagePointer();

  // try to load it
  const VolumeImagePointer image( LoadVolume( fileName, true ) );

  if ( !image && verbose )
    {
    QMessageBox::warning(
      parent,
      "Invalid File",
      std::string( "Unable to load " + fileName ).c_str(),
      QMessageBox::Ok );
    }

  return image;

} // VolumeImagePointer
//////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////
VolumeImagePointer LoadVolumeFromDirectory(
  QWidget * const parent,
  const bool      verbose )
{

  // select a directory to load
  const std::string directoryName = QFileDialog::getExistingDirectory(
    parent, "Open Volume Image From Directory",
    QString(), 
    0 ).toStdString();
  
  // do nothing
  if ( directoryName.empty() )
    return VolumeImagePointer();

  // try to load it
  const VolumeImagePointer image( LoadVolume( directoryName, true ) );

  if ( !image && verbose )
    {
    QMessageBox::warning(
      parent,
      "Invalid Directory",
      std::string( "Unable to load " + directoryName ).c_str(),
      QMessageBox::Ok );
    }

  return image;

} // VolumeImagePointer
//////////////////////////////////////////////////////////////////////////////////////////
} // namespace ccipd
