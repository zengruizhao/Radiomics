#ifndef __ccipdIntensityStandardization_hxx
#define __ccipdIntensityStandardization_hxx



//////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdIntensityStandardization.h"
#include "ccipdVolumeTypes.h"

#include "ccipdDisableWarningsMacro.h"
    #include <itkImage.h>
    #include <itkHistogramMatchingImageFilter.h>
#include "ccipdEnableWarningsMacro.h"
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{
	////////////////////////////////////////////////////////////////////////////////////////////////////
	/**
	* This function will standardize the histogram of
	* an image to a template image.
	*
	* \author  Ahmad
	* \ingroup Common
	*/

	template <class TImageType>
	typename TImageType::Pointer
		StandardizeImage(  typename TImageType::Pointer const inputImage
                         , typename TImageType::Pointer const templateImage
                         , const unsigned int                 histogramBins
                         , const unsigned int                 histogramMatchPoints
                        )
	{
		// histogram match the images
		typedef itk::HistogramMatchingImageFilter< TImageType, TImageType > StandardizationType;
		typename StandardizationType::Pointer standardizer = StandardizationType::New();

		// set all the inputs
		standardizer->SetReferenceImage(templateImage);
		standardizer->SetInput(inputImage);
		standardizer->SetNumberOfHistogramLevels(histogramBins);
		standardizer->SetNumberOfMatchPoints(histogramMatchPoints);
		standardizer->ThresholdAtMeanIntensityOn();

		// calculate and return
		standardizer->Update();
		return standardizer->GetOutput();
		}

////////////////////////////////////////////////////////////////////////////////////////////////////



} // namespace
#endif // __ccipdIntensityStandardization_h