#ifndef __ccipdPreprocessT2_h
#define __ccipdPreprocessT2_h

#include "ccipdPreprocessT2.hxx"

/**
* This file contains the preprocessing code for
* T2 images. Note these are parameter specifics
* calls to functions in ccipdPreprocessVolume.
*/

namespace ccipd
{
	/**
	* This function will input a T2 image
	* and correct the bias field, crop image with 20 (default) pixels 
	  around a given mask volume and then standardize its histogram 
	  to a given volume image or histogram .
	* \param inputImage
	* \param input mask (usually prostate mask) 
	* \param number of pixels to leave around the mask when cropping
	* \param biasSigma sigma used for bias field correction
	* \param reference image for standardization 
	* \param histogram bins
	* \param Number of matching points
	* \return preprocessed image

	This function will modify (crop) the input image, and the input mask
  This function will return the bounding box used in cropping so that 
  it may be used later because the mask will be already cropped

	*/

	template <class TImageType, class TMaskType>
	typename TImageType::RegionType
    PreprocessT2(  typename TImageType::Pointer & pInputImage              // input image
                 , typename TImageType::Pointer & pOutputImage             // output image
                 , typename TImageType::Pointer & pRefImage                //= NULL// reference image for histogram standardization
                 , typename TMaskType::Pointer  & pMaskImage               //= NULL// mask image (prostate)
                 , const    bool                  bStandardize               = false
                 , const    unsigned int          histogramBinsNum           = 100
                 , const    unsigned int          histogramMatchingPointsNum = 25
                 , const    bool                  bCrop                      = false
                 , const    float                 paddingINmm                = 5.0f // this is in mm 
                 , const    bool                  bNormalize                 = false
                 , const    bool                  bApplyBFC                  = true                              
                 , const    float                 biasSigma                  = 6.0f
                );

	bool PreprocessT2(  const std::string  & inputImageFilePath
                      , const std::string  & maskImage
                      , const std::string  & refImage
                      , const unsigned int   paddingPixelsNum
                      , const unsigned int   histogramBinsNum
                      , const unsigned int   histogramMatchingPointsNum
                      , const float          biasSigma
                     );

}


#endif