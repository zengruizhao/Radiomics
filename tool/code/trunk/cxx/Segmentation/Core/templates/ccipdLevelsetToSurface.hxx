#ifndef __ccipdLevelsetToSurface_hxx
#define __ccipdLevelsetToSurface_hxx



//////////////////////////////////////////////////////////////////////////////////////////
// ccipd includes
#include "ccipdCore.h"
#include "ccipdLevelsetToSurface.h"
#include "ccipdDuplicateImage.h"
#include "ccipdStatistics.h"

#include "ccipdDisableWarningsMacro.h"
  // itk includes
  #include <itkSubtractImageFilter.h> // for subtracting from the levelset
  #include <itkZeroCrossingImageFilter.h> // for getting the 0 levelset
  #include <itkImageRegionIterator.h>
#include "ccipdEnableWarningsMacro.h"
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{


//////////////////////////////////////////////////////////////////////////////////////////
template< typename TLevelsetPixelType, typename TMaskPixelType, unsigned int TDimension >
itk::SmartPointer< itk::Image< TMaskPixelType, TDimension > >
GetSurfaceImage(
  const itk::Image< TLevelsetPixelType, TDimension > * const levelset,
  const TLevelsetPixelType                                   levelsetValue,
  const TMaskPixelType                                       backgroundValue,
  const TMaskPixelType                                       foregroundValue )
{

  // we subtract the value from the levelset
  // so that what used to be the value is
  // now 0 and there is a sign change
  // at that location

  typedef itk::Image< TLevelsetPixelType, TDimension > LevelsetImageType;

  typedef itk::SubtractImageFilter<
    LevelsetImageType
  > SubtractFilterType;

  const typename SubtractFilterType::Pointer
    subtractFilter = SubtractFilterType::New();

  // give it the output levelset
  subtractFilter->SetInput1   ( levelset      );
  subtractFilter->SetConstant2( levelsetValue );

  // now, the desired levelset is actually shifted to be 0

  // we need to extract the "0" levelset
  typedef itk::Image<
    TMaskPixelType, TDimension 
  > MaskImageType;

  typedef itk::ZeroCrossingImageFilter<
    LevelsetImageType, MaskImageType
  > ZeroCrossingFilterType;

  const typename ZeroCrossingFilterType::Pointer
    zeroCrossingFilter = ZeroCrossingFilterType::New();

  // give it the subtracted levelset
  zeroCrossingFilter->SetInput( subtractFilter->GetOutput() );

  // give it the foreground and background values
  zeroCrossingFilter->SetForegroundValue( foregroundValue );
  zeroCrossingFilter->SetBackgroundValue( backgroundValue );

  // now actually calculate the zero cross filter
  zeroCrossingFilter->Update();

  // output
  return zeroCrossingFilter->GetOutput();

} // GetSurfaceImage
//////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Merge a image with the surface image.
 * 
 * \date    2013-03-26
 * \ingroup Segmentation
 */
template< typename TPixelType, typename TLevelsetPixelType, unsigned int TDimension >
itk::SmartPointer< itk::Image< TPixelType, TDimension > >
MergeImageWithSurfaceImage(
  const itk::Image< TPixelType, TDimension > * const          image,
  const itk::Image< TLevelsetPixelType, TDimension > * const  levelset,
  const TLevelsetPixelType                                    levelsetValue )
{

  typedef itk::Image< TPixelType, TDimension >          ImageType;
  typedef typename ImageType::Pointer                   ImageTypePointer;
  
  if (!image)
    return ImageTypePointer();

  ImageTypePointer newImage = DuplicateImage ( image ) ;

 if (!levelset)
    return newImage;
 
  if (image->GetLargestPossibleRegion().GetSize(0) !=
      levelset->GetLargestPossibleRegion().GetSize(0) ||
      image->GetLargestPossibleRegion().GetSize(1) !=
      levelset->GetLargestPossibleRegion().GetSize(1) ||
      image->GetLargestPossibleRegion().GetSize(2) !=
      levelset->GetLargestPossibleRegion().GetSize(2) )
  {
    std::cout << "The image and the levelsets are not the same size !!! "
              << "Exiting now !" << std::endl;
    return ImageTypePointer();
  }

  typedef itk::Image< TLevelsetPixelType, TDimension >  LevelsetImageType;
  typedef typename itk::ImageRegionIterator<ImageType>  ImageIterator;
   
  const ImageTypePointer surfaceImage = 
    GetSurfaceImage(levelset, TLevelsetPixelType(levelsetValue), TPixelType(0.0),
    TPixelType(1.0) );

  const TPixelType
    maxValue = GetMax( newImage ),
    minValue = GetMin( newImage );
 
  ImageIterator  
    itIm    ( newImage,     newImage->GetLargestPossibleRegion() ),
    itEdges ( surfaceImage, surfaceImage->GetLargestPossibleRegion() );

  itIm.GoToBegin();
  itEdges.GoToBegin();

  while( !itIm.IsAtEnd() )
    {
    if (itEdges.Get()>0.0) // on the edge
      {
        const int randomNumber = (rand() % 100); // only modify half points on 
        if ( randomNumber < 25 )
          itIm.Set( minValue );
        if ( randomNumber >=25 && randomNumber < 50 )
          itIm.Set( maxValue );
      }
    ++itIm;
    ++itEdges;
    } //while itIm

  return newImage;
};
//////////////////////////////////////////////////////////////////////////////////////////




} // namespace ccipd
#endif // __ccipdLevelsetToSurface_hxx
