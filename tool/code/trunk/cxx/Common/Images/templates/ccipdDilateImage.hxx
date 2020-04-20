#ifndef __ccipdDilateImage_hxx
#define __ccipdDilateImage_hxx



//////////////////////////////////////////////////////////////////////////////////////////


#include "ccipdDilateImage.h"

#include "ccipdDisableWarningsMacro.h"
  #include <itkBasicDilateImageFilter.h>
  #include <itkFlatStructuringElement.h> 
#include "ccipdEnableWarningsMacro.h"
//////////////////////////////////////////////////////////////////////////////////////////

namespace ccipd
{


  
//////////////////////////////////////////////////////////////////////////////////////////
template< class TPixelType, unsigned int TDimension >
itk::SmartPointer< itk::Image< TPixelType, TDimension > >
DilateImage( 
 const itk::Image< TPixelType, TDimension >  * const image )
{
  
  // define how we are going to dilate our image
  
  // typedefs 
  typedef itk::Image< TPixelType, TDimension >     ImageType;
  typedef itk::FlatStructuringElement< 
    TDimension > StructuringElementType;
  
  typename StructuringElementType::RadiusType elementRadius;
  elementRadius.Fill( 2 );

  StructuringElementType element = StructuringElementType::Box( elementRadius );


  typedef itk::BasicDilateImageFilter< 
    ImageType,
    ImageType,
    StructuringElementType > DilateFilterType;

  typename DilateFilterType::Pointer dilator = DilateFilterType::New();

  dilator->SetInput( image );
  dilator->SetKernel ( element );

  dilator->Update();
  return dilator->GetOutput();
} // DilateImage

} // namespace ccipd

#endif // __ccipdDilateImage_hxx