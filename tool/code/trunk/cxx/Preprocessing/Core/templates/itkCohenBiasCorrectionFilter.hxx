#ifndef __itkCohenBiasCorrectionFilter_hxx
#define __itkCohenBiasCorrectionFilter_hxx



////////////////////////////////////////////////////////////////////////////////////////////////////
// our own ITK file
#include <itkCohenBiasCorrectionFilter.h>

// std includes 
#include <numeric>                            // for epsilon

#include "ccipdDisableWarningsMacro.h"
  // itk includes
  #include <itkDivideImageFilter.h>             // for dividing 2 images
  #include <itkRecursiveGaussianImageFilter.h>  // for smoothing
  #include <itkMultiplyImageFilter.h>           // for multiplying by a constant
  #include <itkAddImageFilter.h>                // so the divisor isn't 0, we add epsilon to it
  #include <itkStatisticsImageFilter.h>         // to compute the mean
  #include <itkSmoothingRecursiveGaussianImageFilter.h>   // to blur
#include "ccipdEnableWarningsMacro.h"
////////////////////////////////////////////////////////////////////////////////////////////////////



namespace itk
{



////////////////////////////////////////////////////////////////////////////////////////////////////
template < class TInputImage, class TOutputImage >
CohenBiasCorrectionFilter < TInputImage, TOutputImage >
::CohenBiasCorrectionFilter()
{
	m_Sigma = 6.0;
	m_SigmaArray.Fill(m_Sigma);
} // constructor
////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////
/*
 * Set m_SigmaArray with the sigma value
 */
template < class TInputImage, class TOutputImage >
void
CohenBiasCorrectionFilter < TInputImage, TOutputImage >
::SetSigma(const ScalarRealType sigma)
{
	m_Sigma = sigma;
	m_SigmaArray.Fill(sigma);
}
////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////
/*
 * Set m_SigmaArray with the sigma value
 */
template < class TInputImage, class TOutputImage >
void
CohenBiasCorrectionFilter < TInputImage, TOutputImage >
::SetSigmaArray(const SigmaArrayType & sigmaArray)
{
	/* Rachel's Note: I am doing what itkSmoothingRecursiveGaussianImageFilter 
	*  does and setting sigma to the first value. */
	m_Sigma = sigmaArray[0];  
	m_SigmaArray = sigmaArray;
}
////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////
/*
 * This is where the processing actually goes
 */
template < class TInputImage, class TOutputImage >
void
CohenBiasCorrectionFilter < TInputImage, TOutputImage >
::GenerateData()
{
  // initialize
  //typename InputImageType::IndexType index; // to hold the index of the first pixel
  const InputImageType * input =
    this->GetInput();                       // the input image
  //const InputImagePixelType totalPixels =   // how many pixels are there
  //  input->GetLargestPossibleRegion().GetNumberOfPixels();

  //get the average 
  typedef itk::StatisticsImageFilter<InputImageType> StatisticsImageFilterType;
  typename StatisticsImageFilterType::Pointer statisticsImageFilter
          = StatisticsImageFilterType::New ();
  statisticsImageFilter->SetInput(input);
  statisticsImageFilter->Update();
 
  const InputImagePixelType
    epsilon = std::numeric_limits< InputImagePixelType >::epsilon(),
    average = static_cast< InputImagePixelType >(
      statisticsImageFilter->GetMean() );

  typedef itk::SmoothingRecursiveGaussianImageFilter<
    InputImageType, InputImageType >  FilterType;
 
  typename FilterType::Pointer smoother = FilterType::New();
  smoother->SetInput(input);
  smoother->SetSigmaArray(m_SigmaArray);
  smoother->Update();
 
  // make sure the divisor isn't zero of the final smoothing filter
  typedef AddImageFilter <
    InputImageType, InputImageType, InputImageType > AddFilterType;
  typename AddFilterType::Pointer addFilter = AddFilterType::New();;
  addFilter->SetInput1( smoother->GetOutput() );
  addFilter->SetConstant2( epsilon );

  // then, you take the raw input and divide by the smoothed image
  typedef DivideImageFilter <
    InputImageType, InputImageType, InputImageType > DivideFilterType;
  typename DivideFilterType::Pointer divideFilter = DivideFilterType::New();
  divideFilter->SetInput1( input                  ); // numerator
  divideFilter->SetInput2( addFilter->GetOutput() ); // denominator

  // multiply by the average pixel value and you're done
  typedef MultiplyImageFilter <
    InputImageType, InputImageType, OutputImageType > MultiplyFilterType;
  typename MultiplyFilterType::Pointer multiplyFilter = MultiplyFilterType::New();
  multiplyFilter->SetInput1   ( divideFilter->GetOutput() );
  multiplyFilter->SetConstant2( average                   );

  // finally, we must give the last filter as the output
  multiplyFilter->GraftOutput( this->GetOutput() );
  multiplyFilter->Update();
  this->GraftOutput( multiplyFilter->GetOutput() );
} // GenerateData()
////////////////////////////////////////////////////////////////////////////////////////////////////



} // namespace itk



#endif // __itkCohenBiasCorrectionFilter_hxx
