#ifndef __ccipdMaskPixelValue_hxx
#define __ccipdMaskPixelValue_hxx



//////////////////////////////////////////////////////////////////////////////////////////
// ccipd includes
#include "ccipdMaskPixelValue.h"

#include "ccipdDisableWarningsMacro.h"
  // itk includes
  #include <itkImageRegionIterator.h>
  #include <itkImageRegionConstIterator.h>
#include "ccipdEnableWarningsMacro.h"
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////
template< class TPixelType, unsigned int Dimension >
itk::SmartPointer< itk::Image< MaskPixelType, Dimension > >
MaskPixelValue(
  const itk::Image<
    TPixelType,
    Dimension
  > * const           image,
  const TPixelType    inputPixelValue,
  const MaskPixelType outputValueIfInputPixelValue,
  const MaskPixelType outputValueIfNotInputPIxelValue )
{

  // typedefs
  typedef itk::Image< MaskPixelType, Dimension > MaskType;
  typedef itk::Image< TPixelType   , Dimension > ImageType;

  typedef itk::SmartPointer< MaskType > MaskPointer;

  typedef itk::ImageRegionConstIterator< ImageType > InputIteratorType;
  typedef itk::ImageRegionIterator     < MaskType  > OutputIteratorType;

  // what is the image region?
  const typename ImageType::RegionType region = image->GetBufferedRegion();

  // prepare the output
  const MaskPointer outputMask = MaskType::New();
  outputMask->SetOrigin   ( image->GetOrigin   () );
  outputMask->SetSpacing  ( image->GetSpacing  () );
  outputMask->SetDirection( image->GetDirection() );
  outputMask->SetRegions  ( region );
  outputMask->Allocate();

  // prepare the iterators
  InputIteratorType  inputIterator ( image     , region );
  OutputIteratorType outputIterator( outputMask, region );

  // iterate through each pixel
  for ( inputIterator.GoToBegin(), outputIterator.GoToBegin();
    !inputIterator.IsAtEnd(); ++inputIterator, ++outputIterator )
    {

    const MaskPixelType maskValue = 
      inputIterator.Get() == inputPixelValue ? 
        outputValueIfInputPixelValue :
        outputValueIfNotInputPIxelValue;

    outputIterator.Set( maskValue );

    } // for iterators

  return outputMask;

} // MaskPixelValue
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace ccipd
#endif //__ccipdMaskPixelValue_h
