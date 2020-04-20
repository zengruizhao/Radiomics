


////////////////////////////////////////////////////////////////////////////////////////////////////
// ccipd includes
#include "ccipdImageToProp.hxx"

#include "ccipdDisableWarningsMacro.h"
  // itk includes
  #include <itkImage.h>
  #include <itkRGBPixel.h>
  #include <itkRGBAPixel.h>
#include "ccipdEnableWarningsMacro.h"
////////////////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{


//////////////////////////////////////////////////////////////////////////////////////////
template
vtkPropPointer CreatePropFromImage< RGBSliceType >(
  const RGBSliceType * const image,
  const unsigned int         slice,
  const bool                 makeZeroTransparent );
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
vtkPropPointer CreatePropFromImage(
  const RGBSliceType * const image,
  const bool                 makeZeroTransparent )
{

  const unsigned int slice = 0; // 2D image; doesn't matter
  return CreatePropFromImage< RGBSliceType >( image, slice, makeZeroTransparent );

} // CreatePropFromImage (rgb slice)
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
vtkPropPointer CreatePropFromImage(
  const VolumeImageType * const image,
  const unsigned int            slice,
  const bool                    makeZeroTransparent )
{

  return CreatePropFromImage< VolumeImageType >( image, slice, makeZeroTransparent );

} // CreatePropFromImage (volume)
//////////////////////////////////////////////////////////////////////////////////////////

 vtkPropPointer CreatePropFromImage(
   const RGBVolumeType * const image,
   const unsigned int          slice,
   const bool                  makeZeroTransparent )
 {
 
   return CreatePropFromImage< RGBVolumeType >( image, slice, makeZeroTransparent );
 
 } // CreatePropFromImage (RGB Volume)

  vtkPropPointer CreatePropFromImage(
   const RGBAVolumeType * const image,
   const unsigned int          slice,
   const bool                  makeZeroTransparent )
 {
 
   return CreatePropFromImage< RGBAVolumeType >( image, slice, makeZeroTransparent );
 
 }

} // namespace ccipd
