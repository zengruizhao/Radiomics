#ifndef __ccipdSetChannelByIndex_hxx__
#define __ccipdSetChannelByIndex_hxx__

#include "ccipdSetChannelByIndex.h"
#include "ccipdGetChannelByIndex.h"
#include "ccipdDisableWarningsMacro.h"
  
		// itk includes
		#include <itkImage.h>
		#include <itkVectorImage.h>
		#include <itkComposeImageFilter.h>

		// std includes
		#include <iostream>

#include "ccipdEnableWarningsMacro.h"



namespace ccipd
{


template< class TVectorImage, class TScalarImage >
void
SetChannelByIndex(  typename TVectorImage::Pointer & vectorImage
                  , typename TScalarImage::Pointer & scalarImage
                  , const    unsigned int            index 
			     )
{
	// safety checks
	if(!vectorImage || !scalarImage)
	{
		std::cerr << "\nError(399-96-37): Invalid input ...";
		return;
	}

	if ( index >= vectorImage->GetNumberOfComponentsPerPixel() )
	{
		std::cerr << "\nError(109-46-36): Invalid channel index ...";
		return;
	}


	// objects
    typedef itk::ComposeImageFilter<TScalarImage>  ImageToVectorImageFilterType;
    typename ImageToVectorImageFilterType::Pointer imageToVectorImageFilter = ImageToVectorImageFilterType::New();
    
	// Set inputs
    for(unsigned int i = 0; i < vectorImage->GetNumberOfComponentsPerPixel(); i++)
    {
      if(i != index) 
      {
          imageToVectorImageFilter->SetInput(i, GetChannelByIndex<TVectorImage, TScalarImage>(vectorImage, i));
      }
	  else
	  {
		  imageToVectorImageFilter->SetInput(i, scalarImage);
	  }
    }

	// update and output
    imageToVectorImageFilter->Update();
    vectorImage = imageToVectorImageFilter->GetOutput();

} // SetChannelByIndex





}



#endif

