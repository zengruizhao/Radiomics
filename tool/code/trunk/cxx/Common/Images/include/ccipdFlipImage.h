#ifndef __ccipdFlipImage_h
#define __ccipdFlipImage_h

// ccipd includes
#include "ccipdFlipImage.hxx"


namespace ccipd
{
/*
   This function flips a given image over an axis
   axisIndex is zero for x axis, 1 for y axis, 2 for z axis
   Ahmad
*/
  template<class TImage>
  typename TImage::Pointer FlipImage(typename TImage::Pointer inImage, unsigned int axisIndex = 1);

}



#endif