#ifndef __ccipdGetLargestContiguousRegion_hxx
#define __ccipdGetLargestContiguousRegion_hxx



//////////////////////////////////////////////////////////////////////////////////////////

// ccipd includes
#include "ccipdGetLargestContiguousRegion.h"
#include "ccipdMaskImage.h"
#include "ccipdExtractUniqueValues.h"

// itk includes
#include "ccipdDisableWarningsMacro.h"
#include <itkConnectedComponentImageFilter.h>
#include <itkRelabelComponentImageFilter.h>
#include <itkBinaryThresholdImageFilter.h>
#include <itkOrImageFilter.h>
#include "ccipdEnableWarningsMacro.h"

//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////
template< typename TPixelType, unsigned int TDimension >
itk::SmartPointer< itk::Image< TPixelType, TDimension > >
GetLargestContiguousRegions(
  const itk::Image< TPixelType, TDimension > * const image )
{

  // get the mask
  typedef itk::Image< TPixelType, TDimension > InputImageType;
  typedef itk::SmartPointer< InputImageType > InputImagePointer;

  if ( !image ) return InputImagePointer();

  // get the unique labels in the mask
  const auto uniqueValues = ExtractUniqueValues( image );

  // binary image type
  typedef bool BinaryPixelType;
  typedef itk::Image< BinaryPixelType, TDimension > BinaryImageType;
  typedef typename BinaryImageType::ConstPointer BinaryImageConstPointer;

  BinaryImageConstPointer unionBinaryImage;

  // loop through the unique values
  const auto uniqueValuesEnd = uniqueValues.cend();
  for ( auto uniqueValueIter = uniqueValues.cbegin();
    uniqueValueIter != uniqueValuesEnd; ++uniqueValueIter )
    {

    // get the current unique value
    const TPixelType uniqueValue = *uniqueValueIter;

    // ignore background
    if ( uniqueValue == 0 ) continue;

    // create an image just of that value
    typedef itk::Image< BinaryPixelType, TDimension > BinaryImageType;
    typedef typename BinaryImageType::ConstPointer BinaryImageConstPointer;

    // the output will be a binary image

    typedef itk::BinaryThresholdImageFilter<
      InputImageType, BinaryImageType
    > BinaryFilterType;

    const typename BinaryFilterType::Pointer
      binaryFilter = BinaryFilterType::New();

    binaryFilter->SetInput( image );

    // extract only the current value
    binaryFilter->SetLowerThreshold( uniqueValue );
    binaryFilter->SetUpperThreshold( uniqueValue );

    // the output will have values of true or false
    binaryFilter->SetInsideValue ( true  );
    binaryFilter->SetOutsideValue( false );

    // calculate
    binaryFilter->Update();

    // get the largest region
    const BinaryImageConstPointer
      currentUniqueBinaryImage( binaryFilter->GetOutput() ),
      largestRegionBinaryImage( GetLargestContiguousRegion(
          currentUniqueBinaryImage.GetPointer() ) );

    // include that into our current
    // binary image
    if ( !unionBinaryImage )
      {
      unionBinaryImage = largestRegionBinaryImage;
      }
    else
      {
      // we must "or" it into our current union
      typedef itk::OrImageFilter< BinaryImageType > OrFilterType;
      const typename OrFilterType::Pointer
        orFilter = OrFilterType::New();
      orFilter->SetInput( 0, unionBinaryImage );
      orFilter->SetInput( 1, largestRegionBinaryImage );

      orFilter->Update();
      const BinaryImageConstPointer
        newUnionImage = orFilter->GetOutput();

      unionBinaryImage = newUnionImage;

      } // unionBinaryImage

    } // for each unique value

  // output
  const InputImagePointer outputImage =
    MaskImage( image, unionBinaryImage.GetPointer() );
  
  return outputImage;

} // GetLargestContiguousRegion
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
template< typename TPixelType, unsigned int TDimension >
itk::SmartPointer< itk::Image< TPixelType, TDimension > >
GetLargestContiguousRegion(
  const itk::Image< TPixelType, TDimension > * const mask )
{
  
  // get the mask
  typedef itk::Image< TPixelType, TDimension > MaskImageType;
  typedef itk::SmartPointer< MaskImageType > MaskImagePointer;

  if ( !mask ) return MaskImagePointer();

  // label each contiguous region

  // we may have a lot of "speckle" each of which will
  // produce its own label, and hence we will use
  // an unsigned int instead of a mask image

  typedef unsigned int ConnectedLabelValue;
  typedef itk::Image< ConnectedLabelValue, TDimension > LabelImageType;

  typedef itk::ConnectedComponentImageFilter<
    MaskImageType, LabelImageType
  > ConnectedFilterType;

  const typename ConnectedFilterType::Pointer
    connectedFilter = ConnectedFilterType::New();

  connectedFilter->SetInput( mask );

  // we now want to relabel the spots in the image,
  // sorted by size, which is what the
  // relabel filter does

  typedef itk::RelabelComponentImageFilter<
    LabelImageType, LabelImageType
  > RelabelFilterType;

  const typename RelabelFilterType::Pointer
    relabelFilter = RelabelFilterType::New();

  relabelFilter->SetInput( connectedFilter->GetOutput() );

  // we now want to extract only the largest region(s)

  typedef bool BinaryPixelType;
  typedef itk::Image< BinaryPixelType, TDimension > BinaryImageType;

  // the output will be a binary image

  typedef itk::BinaryThresholdImageFilter<
    LabelImageType, BinaryImageType
  > BinaryFilterType;

  const typename BinaryFilterType::Pointer
    binaryFilter = BinaryFilterType::New();

  binaryFilter->SetInput( relabelFilter->GetOutput() );

  // extract only the largest label (which has a value of 1)
  binaryFilter->SetLowerThreshold( 1 );
  binaryFilter->SetUpperThreshold( 1 );

  // the output will have values of true or false
  binaryFilter->SetInsideValue ( true  );
  binaryFilter->SetOutsideValue( false );

  // calculate
  binaryFilter->Update();

  // get the binary mask
  const typename BinaryImageType::Pointer
    binaryMaskImage = binaryFilter->GetOutput();

  // now we have all those pixels in the image
  // which are within the desired largest
  // regions as true, and false outside

  // we can then mask out the false pixels
  // in our mask image to get our result,
  // thus retaining the original labels

  // output
  const MaskImagePointer outputImage =
    MaskImage( mask, binaryMaskImage.GetPointer() );
  
  return outputImage;

} // GetLargestContiguousRegion
//////////////////////////////////////////////////////////////////////////////////////////


} // namespace ccipd
#endif // __ccipdGetLargestContiguousRegion_hxx
