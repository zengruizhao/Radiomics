#ifndef __ccipdExtractUniqueValues_hxx
#define __ccipdExtractUniqueValues_hxx



//////////////////////////////////////////////////////////////////////////////////////////

// ccipd includes
#include "ccipdCore.h"
#include "ccipdExtractUniqueValues.h"

#include "ccipdDisableWarningsMacro.h"
  // itk includes
  #include <itkImage.h>
  #include <itkImageRegionConstIterator.h> // for iterating
#include "ccipdEnableWarningsMacro.h"
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////
template< class TPixelType, unsigned int Dimension >
std::unordered_set< TPixelType > // output
ExtractUniqueValues( const itk::Image< TPixelType, Dimension > * const image )
{

  // initialize an empty set
  std::unordered_set< TPixelType > uniqueValues;

  // check if there's no image
  if ( !image ) return uniqueValues;

  // what type of image do we have?
  typedef itk::Image< TPixelType, Dimension > ImageType;

  // we will iterate through the image
  typedef itk::ImageRegionConstIterator< ImageType > IteratorType;
  IteratorType iterator( image, image->GetBufferedRegion() );
  iterator.GoToBegin();

  // for some slight efficiency gains start at a different value
  TPixelType previousPixelValue = iterator.Get() + 1;

  // iterator
  for ( iterator.GoToBegin(); !iterator.IsAtEnd(); ++iterator )
    {

    // see if it's different from the previous pixel
    const TPixelType pixelValue = iterator.Get();

    // this was already considered at least once
    if ( pixelValue != previousPixelValue )
      {

      // insert it into our set
      // which will ignore if the
      // value already exists
      uniqueValues.insert( pixelValue );

      // update the previous pixel value
      previousPixelValue = pixelValue;

      } // previous pixel different

    } // for iterator

  // output
  return uniqueValues;

} // ExtractUniqueValues
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace ccipd
#endif // __ccipdExtractUniqueValues_hxx
