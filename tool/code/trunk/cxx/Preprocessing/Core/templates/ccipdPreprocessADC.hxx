#ifndef __ccipdPreprocessADC_hxx
#define __ccipdPreprocessADC_hxx

#include "ccipdBoundingBox.h"
#include "ccipdIntensityStandardization.h"

namespace ccipd
{

	template <class TImageType>
	typename TImageType::Pointer PreprocessADC(  typename TImageType::Pointer pInputImage
                                               , typename TImageType::Pointer pT2Image
                                               , typename TImageType::Pointer pMaskImage          
                                               , typename TImageType::Pointer pRefImage            
                                               , const    unsigned int        paddingPixelsNum 
                                               , const    unsigned int        histogramBinsNum 
                                               , const    unsigned int        histogramMatchingPointsNum 
                                              )
	{
		// 1. Register to T2 (OR it may come already registerd

		// 2. Crop

		// 3. Standardize


	}


}


#endif