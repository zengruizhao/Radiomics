#ifndef __ccipdStandardizeImage_hxx
#define __ccipdStandardizeImage_hxx

#include "ccipdStandardizeImage.h"
//#include "ccipdVolumeTypes.h"
//#include "ccipdHistogram.h"
#include "ccipdDisableWarningsMacro.h"
  #include <itkHistogramToIntensityImageFilter.h>
  #include <itkHistogramMatchingImageFilter.h>



#include <itkInterpolateImageFunction.h>




#include "ccipdEnableWarningsMacro.h"

namespace ccipd
{

template<class TImageType, class THistogramType>
typename TImageType::Pointer
StandardizeImage(  typename TImageType::Pointer     pInputImage
                 , typename THistogramType::Pointer pReferenceHistogram
                )
{
  //// Get the Reference Image from the given Reference Histogram
  typedef itk::HistogramToIntensityImageFilter<THistogramType, TImageType> HistogramToImageFilterType;
  typename HistogramToImageFilterType::Pointer pHistogramToImageFilter = HistogramToImageFilterType::New();
  
  // Check
  if(0 == pReferenceHistogram->GetBinMax(0, 0) - pReferenceHistogram->GetBinMin(0, 0)) //should not be zero
  {
    cout << "\nError(39-456): Histogram bin spacing error ...";
    return typename TImageType::Pointer();
  }


  pHistogramToImageFilter->SetInput(pReferenceHistogram);
  pHistogramToImageFilter->Update();
  typename TImageType::Pointer pReferenceImage = pHistogramToImageFilter->GetOutput();

  //// Get the histogram matched image
  typedef itk::HistogramMatchingImageFilter<TImageType, TImageType > StandardizationFilterType;
  typename StandardizationFilterType::Pointer pStandardizationFilter = StandardizationFilterType::New();

  pStandardizationFilter->SetReferenceImage         (pHistogramToImageFilter->GetOutput()  );
  pStandardizationFilter->SetInput                  ( pInputImage                           );
  pStandardizationFilter->SetNumberOfHistogramLevels( pReferenceHistogram->GetSize()[0]);
  pStandardizationFilter->SetNumberOfMatchPoints    ( pReferenceHistogram->GetSize()[0]/4);
  pStandardizationFilter->ThresholdAtMeanIntensityOn();
  pStandardizationFilter->Update();

  // Return the histogram standardized image
  return pStandardizationFilter->GetOutput();

}




} // namespace ccipd





#endif