#ifndef __ccipdCreateBinaryMask_hxx
#define __ccipdCreateBinaryMask_hxx



//////////////////////////////////////////////////////////////////////////////////////////
// ccipd includes
#include "ccipdCore.h"
#include "ccipdCreateBinaryMask.h"
#include "ccipdMaskImage.h"

#include "ccipdDisableWarningsMacro.h"
  // itk includes
  #include <itkImage.h>
#include "ccipdEnableWarningsMacro.h"
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{


  
//////////////////////////////////////////////////////////////////////////////////////////
template< typename TPixelType, unsigned int TDimension >
itk::SmartPointer< itk::Image< TPixelType, TDimension > >
CreateBinaryMask( const itk::Image< TPixelType, TDimension > * const mask )
{

  // define the output pixel values
  const TPixelType
    background = 0,
    foreground = 1;

  // create a new mask filled with ones
  typedef itk::Image< TPixelType, TDimension > ImageType;
  typedef itk::SmartPointer< ImageType > ImagePointer;

  const ImagePointer binaryMask = ImageType::New();
  binaryMask->SetOrigin   ( mask->GetOrigin        () );
  binaryMask->SetDirection( mask->GetDirection     () );
  binaryMask->SetSpacing  ( mask->GetSpacing       () );
  binaryMask->SetRegions  ( mask->GetBufferedRegion() );

  // fill it with ones
  binaryMask->Allocate();
  binaryMask->FillBuffer( foreground );

  // now mask out any "0"s in our original mask
  return MaskImage( binaryMask.GetPointer(), mask, background );

} // CreateBinaryMask
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace ccipd
#endif // __ccipdCreateBinaryMask_hxx
