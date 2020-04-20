


//////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdIntensityStandardization.h"

#include "ccipdDisableWarningsMacro.h"
  // itk includes
  #include <itkHistogramMatchingImageFilter.h>
#include "ccipdEnableWarningsMacro.h"
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{

////////////////////////////////////////////////////////////////////////////////////////////////////
	VolumeImagePointer StandardizeImage(  VolumeImagePointer const inputImage
		                                , VolumeImagePointer const templateImage
                                        , const unsigned int            histogramBins
                                        , const unsigned int            histogramMatchPoints 
								       )
{
  
  // histogram match the images
  typedef itk::HistogramMatchingImageFilter<  VolumeImageType, VolumeImageType > StandardizationType;
  const auto standardizer = StandardizationType::New();

  // set all the inputs
  standardizer->SetReferenceImage         ( templateImage        );
  standardizer->SetInput                  ( inputImage           );
  standardizer->SetNumberOfHistogramLevels( histogramBins        );
  standardizer->SetNumberOfMatchPoints    ( histogramMatchPoints );
  standardizer->ThresholdAtMeanIntensityOn();

  // calculate and return
  standardizer->Update();
  return standardizer->GetOutput();
  
  

	//return StandardizeImage<VolumeImageType>(inputImage, templateImage, histogramBins, histogramMatchPoints);
  
} // StandardizeMRI
////////////////////////////////////////////////////////////////////////////////////////////////////



}
