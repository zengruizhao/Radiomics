


//////////////////////////////////////////////////////////////////////////////////////////
// ccipd includes
#include "ccipdAnimateWarpField.h"
#include "ccipdSaveImage.h" // for saving the output
#include "ccipdApplyWarpField.hxx"
#include "ccipdResizeImage.hxx"
#include "ccipdMaskPixelValue.h" // to create masks
#include "ccipdBlendImages.h" // for blend fixed and moved images

#include "ccipdDisableWarningsMacro.h"
  // itk includes
  #include <itkRGBPixel.h>
  #include <itkImageAdaptor.h> // for scaling the warp field
  #include <itkWarpVectorImageFilter.h>
  #include <itkWarpImageFilter.h>
  #include <itkVectorLinearInterpolateImageFunction.h>
  #include <itkNearestNeighborInterpolateImageFunction.h>
#include "ccipdEnableWarningsMacro.h"
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
using std::string;
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////
void GenerateWarpFieldAnimationFrames(
  const SliceWarpFieldType * const warpField,
  const RGBSliceType       * const fixedImage,
  const RGBSliceType       * const movingImage,
  const unsigned int               numberOfFrames,
  const MaskPixelType              movingImageOpacity,
  const bool                       makeBlackTransparent,
  const std::string              & outputDirectory,
  const std::string              & fileNameFormat )
{

  // we need our moving image to be like our fixed image
  const RGBSliceConstPointer movingImageResampledLikeFixedImage(
    ResizeImage< RGBPixelType, SliceDimension >(
      movingImage, fixedImage, false,
      static_cast< unsigned char/*InternalPixelType*/ >( 0 ) ) );

  // we need to create a mask of our fixed image
  const MaskPixelType fixedMaskValue = 255;
  SliceMaskImagePointer fixedMask;
  if ( makeBlackTransparent )
    {
    fixedMask = MaskPixelValue(
      fixedImage, RGBPixelType( (unsigned char)0.0f ), 0, fixedMaskValue );
    }
  else // just make it all opaque
    {
    fixedMask->SetOrigin   ( fixedImage->GetOrigin        () );
    fixedMask->SetSpacing  ( fixedImage->GetSpacing       () );
    fixedMask->SetDirection( fixedImage->GetDirection     () );
    fixedMask->SetRegions  ( fixedImage->GetBufferedRegion() );
    fixedMask->Allocate();
    fixedMask->FillBuffer( fixedMaskValue );
    } // makeBlackTransparent
  
  // we need to create a mask of our moving image
  const MaskPixelType movingMaskValue = movingImageOpacity;
  SliceMaskImagePointer movingMask;
  if ( makeBlackTransparent )
    {
    movingMask = MaskPixelValue(
      movingImageResampledLikeFixedImage,
      RGBPixelType( (unsigned char)0.0f ), 0, movingMaskValue );
    }
  else // just make it all opaque
    {
    movingMask->SetOrigin   ( fixedImage->GetOrigin        () );
    movingMask->SetSpacing  ( fixedImage->GetSpacing       () );
    movingMask->SetDirection( fixedImage->GetDirection     () );
    movingMask->SetRegions  ( fixedImage->GetBufferedRegion() );
    movingMask->Allocate();
    movingMask->FillBuffer( movingMaskValue );
    } // makeBlackTransparent

  // loop through each frame
  #pragma omp parallel for
  for ( int frameIndex = 0;
    frameIndex < static_cast< int >( numberOfFrames ); ++frameIndex )
    {

    // scale the vector field by a factor (between 0 and 1 inclusive)
    const WarpValueType percentage =
      static_cast< WarpValueType >( frameIndex ) /
      static_cast< WarpValueType >( numberOfFrames - 1 );

    // typedef's
    typedef VectorScalePixelAccessor<
      WarpValueType,
      SliceDimension
    > AccessorType;

    typedef itk::ImageAdaptor<
      SliceWarpFieldType,
      AccessorType
    > AdaptorType;

    // scale the warp field by the percentage
    const AccessorType accessor( percentage );

    // create an image adaptor
    const AdaptorType::Pointer adaptor = AdaptorType::New();
    adaptor->SetPixelAccessor( accessor );

    // I promise I won't modify the image!
    adaptor->SetImage( const_cast< SliceWarpFieldType * >( warpField ) );

    // typedefs
    typedef itk::VectorLinearInterpolateImageFunction<
      RGBSliceType
    > ImageInterpolatorType;

    typedef itk::NearestNeighborInterpolateImageFunction<
      SliceMaskImageType
    > MaskInterpolatorType;

    typedef itk::WarpVectorImageFilter<
      RGBSliceType,
      RGBSliceType,
      AdaptorType
    > ImageWarpFilterType;

    typedef itk::WarpImageFilter<
      SliceMaskImageType,
      SliceMaskImageType,
      AdaptorType
    > MaskWarpFilterType;

    // apply the warp field to the moving image
    const RGBSliceConstPointer movedImage(
      ApplyWarpField<
        AdaptorType,
        SliceDimension,
        RGBSliceType,
        ImageInterpolatorType,
        ImageWarpFilterType
        >(
        adaptor,
        movingImageResampledLikeFixedImage,
        fixedImage,
        ImageInterpolatorType::New() ) );
    
    // apply the warp field to the moving mask
    const SliceMaskImageConstPointer movedMask(
      ApplyWarpField<
        AdaptorType,
        SliceDimension,
        SliceMaskImageType,
        MaskInterpolatorType,
        MaskWarpFilterType
        >(
        adaptor,
        movingMask,
        movingMask,
        MaskInterpolatorType::New() ) );
    
    // blend the result
    const RGBSliceConstPointer blendedImage(
      BlendImages< RGBPixelType, SliceDimension >( 
        fixedImage, fixedMask, movedImage, movedMask ) );

    // save the frame
    char fileName[1024];
    sprintf( fileName, fileNameFormat.c_str(), frameIndex );
    const string fullFileName = outputDirectory + "/" + fileName;
    SaveRGBSliceAfterCasting< unsigned char >(
      blendedImage, fullFileName, false );

    } // frameIndex

} // GenerateWarpFieldAnimationFrames
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
template< class TPixelType, unsigned int TDimension >
VectorScalePixelAccessor< TPixelType, TDimension >
::VectorScalePixelAccessor( const TPixelType scale ) :
m_Scale( scale )
{
}
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
template< class TPixelType, unsigned int TDimension >
VectorScalePixelAccessor< TPixelType, TDimension >
::VectorScalePixelAccessor() :
m_Scale( 1 )
{
}
//////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////
template< class TPixelType, unsigned int TDimension >
void
VectorScalePixelAccessor< TPixelType, TDimension >
::SetScale( const TPixelType scale )
{
  this->m_Scale = scale;
}
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
template< class TPixelType, unsigned int TDimension >
void
VectorScalePixelAccessor< TPixelType, TDimension >
::operator=( const VectorScalePixelAccessor & other )
{
  this->SetScale( other.m_Scale );
} // operator=
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
template< class TPixelType, unsigned int TDimension >
typename
VectorScalePixelAccessor< TPixelType, TDimension >
::VectorPixelType
VectorScalePixelAccessor< TPixelType, TDimension >
::Get( const VectorPixelType & input ) const
{
  return ( input * this->m_Scale );
} // Get
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace ccipd
