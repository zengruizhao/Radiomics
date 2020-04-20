#ifndef __ccipdDuplicateImage_hxx
#define __ccipdDuplicateImage_hxx


/**
 * Actually call the itk functions.
 */



//////////////////////////////////////////////////////////////////////////////////////////
// ccipd includes
#include "ccipdDuplicateImage.h"

#include "ccipdDisableWarningsMacro.h"
  // itk includes
  #include <itkImageDuplicator.h> // we are simply wrapping this
#include "ccipdEnableWarningsMacro.h"
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////
template< class ImageType >
typename ImageType::Pointer DuplicateImage( const ImageType * const image )
{

  // create a duplicator
  typedef itk::ImageDuplicator<
    ImageType
  > DuplicatorType;
  typename DuplicatorType::Pointer duplicator = DuplicatorType::New();

  // give it the inputs
  duplicator->SetInputImage( image );

  // duplicate and return
  duplicator->Update();
  return duplicator->GetOutput();

} // DuplicateImage
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace
#endif // __ccipdDuplicateImage_hxx
