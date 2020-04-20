


//////////////////////////////////////////////////////////////////////////////////////////
// ccipd includes
#include "ccipdBlendImages.hxx"
#include "ccipdRGBSliceTypes.h"

#include "ccipdDisableWarningsMacro.h"
  // itk includes 
  #include <itkImage.h>
  #include <itkRGBPixel.h>
#include "ccipdEnableWarningsMacro.h"
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////
template
RGBSlicePointer BlendImages< RGBPixelType, SliceDimension >(
  const RGBSliceType       * const image1,
  const SliceMaskImageType * const mask1,
  const RGBSliceType       * const image2,
  const SliceMaskImageType * const mask2 );
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace ccipd
