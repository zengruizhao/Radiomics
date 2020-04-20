#ifndef __ccipdMaskPixelValue_h
#define __ccipdMaskPixelValue_h


#include "ccipdMaskPixelValue.hxx"
//////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdRGBSliceTypes.h"
#include "ccipdVolumeTypes.h"
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Create a mask of certain pixel values.
 * 
 * This will check if each pixel is a certain value,
 * and if so give the mask a value of
 * "outputValueIfInputPixelValue".
 * Otherwise, give the mask a value you specify in
 * "outputValueIfNotInputPIxelValue".
 * 
 * \date    2012-05-09
 * \author  Rob Toth
 * \ingroup Common
 */
template< class TPixelType, unsigned int Dimension >
itk::SmartPointer< itk::Image< MaskPixelType, Dimension > >
MaskPixelValue(
  const itk::Image< TPixelType, Dimension > * const           image,
  const TPixelType    inputPixelValue,
  const MaskPixelType outputValueIfInputPixelValue    = 0u,
  const MaskPixelType outputValueIfNotInputPIxelValue = 255u );
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
/// \see MaskPixelValue()
SliceMaskImagePointer MaskPixelValue(
  const RGBSliceType * const image,
  const RGBPixelType         inputPixelValue,
  const MaskPixelType        outputValueIfInputPixelValue    = 0u,
  const MaskPixelType        outputValueIfNotInputPIxelValue = 255u );
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
/// \see MaskPixelValue()
VolumeMaskImagePointer MaskPixelValue(
  const VolumeImageType * const image,
  const InternalPixelType       inputPixelValue                 = 0.0f,
  const MaskPixelType           outputValueIfInputPixelValue    = 0u,
  const MaskPixelType           outputValueIfNotInputPIxelValue = 255u );
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace ccipd
#endif //__ccipdMaskPixelValue_h
