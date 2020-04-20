


//////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdMaskPixelValue.hxx"

#include "ccipdDisableWarningsMacro.h"
  // itk includes
  #include <itkImage.h>
  #include <itkRGBPixel.h>
  #include <itkRGBAPixel.h>
#include "ccipdEnableWarningsMacro.h"
//////////////////////////////////////////////////////////////////////////////////////////


namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////
SliceMaskImagePointer MaskPixelValue(
  const RGBSliceType * const image,
  const RGBPixelType         inputPixelValue,
  const MaskPixelType        outputValueIfInputPixelValue,
  const MaskPixelType        outputValueIfNotInputPIxelValue )
{

  return MaskPixelValue< RGBPixelType, SliceDimension >(
    image, inputPixelValue,
    outputValueIfInputPixelValue, outputValueIfNotInputPIxelValue );

} // RGBSliceType
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
VolumeMaskImagePointer MaskPixelValue(
  const VolumeImageType * const image,
  const InternalPixelType       inputPixelValue,
  const MaskPixelType           outputValueIfInputPixelValue,
  const MaskPixelType           outputValueIfNotInputPIxelValue )
{

  return MaskPixelValue< InternalPixelType, VolumeDimension >(
    image, inputPixelValue,
    outputValueIfInputPixelValue, outputValueIfNotInputPIxelValue );

} // VolumeImageType
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace ccipd
