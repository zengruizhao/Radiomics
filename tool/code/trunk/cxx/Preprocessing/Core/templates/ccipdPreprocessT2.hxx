#ifndef __ccipdPreprocessT2_hxx
#define __ccipdPreprocessT2_hxx

#include "ccipdPreprocessT2.h"
#include "ccipdBoundingBox.h"
#include "ccipdIntensityStandardization.h"
#include "ccipdCropImage.h"
#include "ccipdPreprocessVolume.h"
#include "ccipdDuplicateImage.h"
#include "ccipdResizeImage.h"

namespace ccipd
{
	template <class TImageType, class TMaskType>
	typename TImageType::RegionType
    PreprocessT2(  typename TImageType::Pointer & pInputImage 
                 , typename TImageType::Pointer & pOutputImage  
                 , typename TImageType::Pointer & pRefImage
                 , typename TMaskType::Pointer  & pMaskImage
                 , const    bool                  bStandardize                          
                 , const    unsigned int          histogramBinsNum         
                 , const    unsigned int          histogramMatchingPointsNum 
                 , const    bool                  bCrop                                
                 , const    float                 paddingINmm                
                 , const    bool                  bNormalize                                   
                 , const    bool                  bApplyBFC                  
                 , const    float                 biasSigma                  
                )
	{

    typename TImageType::RegionType boundingBox = pMaskImage->GetLargestPossibleRegion();


		// Simple Check
		if (!pInputImage)
		{
			cout << "\nError(28-33): No valid input image ....";
			return boundingBox;
		}

    if(pOutputImage != pInputImage)
		{
            pOutputImage = DuplicateImage<TImageType>(pInputImage);
        }

		// AA TODO: All used functions Should be templated
		// 1. Correct for bias field
		if (bApplyBFC && biasSigma > 0.00f)
		{
			pOutputImage = CohenCorrectBiasField(pOutputImage, biasSigma);
		}

    // 2. Additional Check
    pOutputImage = ResizeImage/*<typename TImageType::PixelType, typename TImageType::ImageDimension>*/(pOutputImage, pMaskImage);

		// 3. Crop Image
		if (bCrop && pMaskImage)
		{
            boundingBox = GetBoundingBox(pMaskImage, (const float)paddingINmm);
		    pOutputImage = CropVolume(pOutputImage, boundingBox);
			  pMaskImage  = CropMask(pMaskImage, boundingBox);
		}
		
		// 4. Standardize
		if (bStandardize && pRefImage && histogramBinsNum > 3 && histogramMatchingPointsNum > 3)
		{
            pOutputImage = StandardizeImage<TImageType>(pOutputImage, pRefImage, histogramBinsNum, histogramMatchingPointsNum);
		}
		
		// 5. Normalize
		if (bNormalize)
		{
			pOutputImage = NormalizeVolume(pOutputImage);
		}
		
		return boundingBox;
	}

}




#endif
