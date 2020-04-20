#ifndef __ccipdStatistics_hxx
#define __ccipdStatistics_hxx



////////////////////////////////////////////////////////////////////////////////////////////////////
// ccipd includes
#include "ccipdCore.h"

//std incldues
#include <iostream>

#include "ccipdDisableWarningsMacro.h"
  // itk includes
  #include <itkBinaryThresholdImageFilter.h>
  #include <itkStatisticsImageFilter.h>
  #include <itkImageToHistogramFilter.h>
#include "ccipdEnableWarningsMacro.h"
////////////////////////////////////////////////////////////////////////////////////////////////////

using std::cout;
using std::endl;

namespace ccipd
{
template< class TInputImage>
unsigned int GetPixelCount(  const TInputImage * inImage
                           , const float         threshold 
                          )
{
  //first threshold
  typedef itk::BinaryThresholdImageFilter <TInputImage, TInputImage> ThresholdFilterType;
 
  typename ThresholdFilterType::Pointer pThresholdFilter = ThresholdFilterType::New();
  pThresholdFilter->SetInput(inImage);
  pThresholdFilter->SetLowerThreshold( static_cast< typename TInputImage::PixelType >( threshold) );
  pThresholdFilter->SetInsideValue ( 1 );
  pThresholdFilter->SetOutsideValue( 0 ); 
  pThresholdFilter->Update();

  //then compute occupied voxels
  typedef itk::StatisticsImageFilter<TInputImage> StatisticsFilterType;
  typename StatisticsFilterType::Pointer pStatisticsFilter = StatisticsFilterType::New();
  pStatisticsFilter->SetInput( pThresholdFilter->GetOutput() );
  pStatisticsFilter->Update();

  //compute and return pixel Count
  return (unsigned int)pStatisticsFilter->GetSum();
}









template< class TInputImage>
EvaluationValueType GetVolume(
	const TInputImage     * inImage,
	const float           threshold )
{
  //first threshold
  typedef itk::BinaryThresholdImageFilter <TInputImage, TInputImage>
    BinaryThresholdImageFilterType;
 
  typename BinaryThresholdImageFilterType::Pointer thresholdFilter
    = BinaryThresholdImageFilterType::New();
  thresholdFilter->SetInput(inImage);
  thresholdFilter->SetLowerThreshold(
    static_cast< typename TInputImage::PixelType >( threshold) );
  thresholdFilter->SetInsideValue ( 1 );
  thresholdFilter->SetOutsideValue( 0 ); 
  thresholdFilter->Update();

  //then compute occupied voxels
  typedef itk::StatisticsImageFilter<TInputImage> StatisticsImageFilterType;
  typename StatisticsImageFilterType::Pointer statisticsImageFilter
          = StatisticsImageFilterType::New ();
  statisticsImageFilter->SetInput( thresholdFilter->GetOutput() );
  statisticsImageFilter->Update();

  float voxelVol = 1;
  for (int i=0; i<TInputImage::ImageDimension; i++)
    voxelVol *= static_cast< float >( inImage->GetSpacing()[i] );

  //compute and return volume
  return statisticsImageFilter->GetSum()*voxelVol;
};

/**
 * computes the min of a image 
 * \param inImage the input image
 * 
 * \author  Mirabela Rusu
 * \ingroup Filtering
 */
template<class TInputImage>
typename TInputImage::PixelType GetMin(const TInputImage * inImage)
{
	typedef itk::StatisticsImageFilter<TInputImage> StatisticsImageFilterType;
	typename StatisticsImageFilterType::Pointer statisticsImageFilter =  StatisticsImageFilterType::New ();
	statisticsImageFilter->SetInput( inImage );
	statisticsImageFilter->Update();

	return statisticsImageFilter->GetMinimum();
};

/**
 * computes the max of a image 
 * \param inImage the input image
 * 
 * \author  Mirabela Rusu
 * \ingroup Filtering
 */
template<class TInputImage>
typename TInputImage::PixelType GetMax(const TInputImage * inImage)
{
	typedef itk::StatisticsImageFilter<TInputImage> StatisticsImageFilterType;
	typename StatisticsImageFilterType::Pointer statisticsImageFilter =  StatisticsImageFilterType::New ();
	statisticsImageFilter->SetInput( inImage );
	statisticsImageFilter->Update();

	return statisticsImageFilter->GetMaximum();
};

/**
 * computes the sum of a image 
 * \param inImage the input image
 * 
 * \author  Mirabela Rusu
 * \ingroup Filtering
 */
template< class TInputImage>
typename TInputImage::PixelType GetSum(const TInputImage * inImage)
{
	typedef itk::StatisticsImageFilter<TInputImage> StatisticsImageFilterType;
	typename StatisticsImageFilterType::Pointer statisticsImageFilter =  StatisticsImageFilterType::New ();
	statisticsImageFilter->SetInput( inImage );
	statisticsImageFilter->Update();

	return static_cast< typename TInputImage::PixelType >( statisticsImageFilter->GetSum() );

};

/**
 * computes the sum of a image 
 * \param inImage the input image
 * 
 * \author  Mirabela Rusu
 * \ingroup Filtering
 */
template< class TInputImage>
typename TInputImage::PixelType GetSD(const TInputImage * inImage)
{
	typedef itk::StatisticsImageFilter<TInputImage> StatisticsImageFilterType;
	typename StatisticsImageFilterType::Pointer statisticsImageFilter =  StatisticsImageFilterType::New ();
	statisticsImageFilter->SetInput( inImage );
	statisticsImageFilter->Update();

	return static_cast< typename TInputImage::PixelType >( statisticsImageFilter->GetSigma() );

};

/**
 * computes the quantile  
 * \param inImage the input image
 * \param index whick quantile (should be a value between 0 and 1)
 * \author  Mirabela Rusu
 * \ingroup Filtering
 */
template< class TInputImage>
typename TInputImage::PixelType GetQuantile(const TInputImage * inImage, float percentile)
{

	//First compute the histogram 
 	const unsigned int MeasurementVectorSize = 1; // Grayscale
	const unsigned int binsPerDimension = 1000;
 
	typedef itk::Statistics::ImageToHistogramFilter< TInputImage > ImageToHistogramFilterType;
 
 	typename ImageToHistogramFilterType::HistogramType::MeasurementVectorType lowerBound(binsPerDimension);
  lowerBound.Fill( GetMin(inImage) );
 
  typename ImageToHistogramFilterType::HistogramType::MeasurementVectorType upperBound(binsPerDimension);
  upperBound.Fill( GetMax(inImage) );
 
  typename ImageToHistogramFilterType::HistogramType::SizeType size(MeasurementVectorSize);
  size.Fill(binsPerDimension);
 
  typename ImageToHistogramFilterType::Pointer imageToHistogramFilter = ImageToHistogramFilterType::New();
  imageToHistogramFilter->SetInput(inImage);
  imageToHistogramFilter->SetHistogramBinMinimum(lowerBound);
  imageToHistogramFilter->SetHistogramBinMaximum(upperBound);
  imageToHistogramFilter->SetHistogramSize(size);
  imageToHistogramFilter->Update();
 
  typename ImageToHistogramFilterType::HistogramType* histogram = imageToHistogramFilter->GetOutput();
 
	if (percentile>1)
		percentile /= 100;
	
	return static_cast< typename TInputImage::PixelType >(
    histogram->Quantile(0, percentile) );
};


}//namespace ccipd


#endif //__ccipdStatistics_hxx
