#ifndef __ccipdPreprocessADC_h
#define __ccipdPreprocessADC_h

#include "ccipdPreprocessADC.hxx"

/**
* This file contains the preprocessing code for
* Diffusion ADC images. Note these are parameter specifics
* calls to functions in ccipdPreprocessVolume.
*/

namespace ccipd
{
	/**
	* This function will input a ADC image
	* and correct the bias field, crop image with 20 (default) pixels
	around a given mask volume and then standardize its histogram
	to a given volume image or histogram .
	* \param inputImage
	* \param input mask (usually prostate mask)
	* \param number of pixels to leave around the mask when cropping
	* \param reference image for standardization
	* \param histogram bins
	* \param Number of matching points
	* \return preprocessed image
	*/

	template <class TImageType>
	typename TImageType::Pointer PreprocessADC(  typename TImageType::Pointer pInputImage          // input image
		                                       , typename TImageType::Pointer pT2Image
		                                       , typename TImageType::Pointer pMaskImage           // mask image (prostate)
                                               , typename TImageType::Pointer pRefImage            // reference image for histogram standardization
                                               , const    unsigned int        paddingPixelsNum = 20 // this is in mm temporarily and will be changed AA TODO
                                               , const    unsigned int        histogramBinsNum = 100
                                               , const    unsigned int        histogramMatchingPointsNum = 25
                                               );



}


#endif