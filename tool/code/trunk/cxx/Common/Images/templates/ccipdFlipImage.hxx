#ifndef __ccipdFlipImage_hxx
#define __ccipdFlipImage_hxx



//////////////////////////////////////////////////////////////////////////////////////////
// ccipd includes
//#include "ccipdFlipImage.h"

#include "ccipdDisableWarningsMacro.h"
  // itk includes
  #include <itkFlipImageFilter.h> // we are simply wrapping this
//  #include <itkFixedArray.h>
#include "ccipdEnableWarningsMacro.h"
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{

/*
   This function flips a given image over an axis
   axisIndex is zero for x axis, 1 for y axis, 2 for z axis
   Ahmad
*/
  template<class TImage>
  typename TImage::Pointer FlipImage(typename TImage::Pointer inImage, unsigned int axisIndex /*= 1*/)
  {
    if(!inImage)
    {
      std::cout << "\nError(34-750): Invalid Image pointer ...\n"; 
      return typename TImage::Pointer();
    }

    if(axisIndex >= (TImage::ImageDimension))
    {
      std::cout << "\nError(365-719): Invalid axis index, image is not flipped ...\n"; 
      return inImage;
    }
    
    typedef itk::FlipImageFilter<TImage> FlipImageFilterType;
    typename FlipImageFilterType::Pointer flipFilter = FlipImageFilterType::New();
    
    flipFilter->SetInput(inImage);

    typename FlipImageFilterType::FlipAxesArrayType flipAxes;
    flipAxes.Fill(false);
    flipAxes[axisIndex] = true;
    
    
    flipFilter->SetFlipAxes(flipAxes);
    flipFilter->Update();
    return flipFilter->GetOutput();
	
  }
}




#endif