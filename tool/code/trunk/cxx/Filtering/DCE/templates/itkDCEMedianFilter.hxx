#ifndef __itkDCEMedianFilter_hxx
#define __itkDCEMedianFilter_hxx



////////////////////////////////////////////////////////////////////////////////////////////////////
// our own itk includes
#include "itkDCEMedianFilter.h"


  // itk includes
  #include <itkConstNeighborhoodIterator.h>
  #include <itkConstantBoundaryCondition.h>  
  #include <itkNeighborhoodInnerProduct.h>
  #include <itkImageRegionIterator.h>
  #include <itkNeighborhoodAlgorithm.h>
  #include <itkOffset.h>
  #include <itkProgressReporter.h>


// std includes
#include <iostream>
#include <vector>
#include <algorithm>
////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////
namespace itk
{
template< class TInputImage, class TOutputImage >
DCEMedianFilter< TInputImage, TOutputImage >
::DCEMedianFilter()
{
  // Default values of member variables
  this->SetRadius(0); // m_Radius = {{0, 0, 0}} // This means that the neighbourhood box is 1 x 1 size
  this->ProcessObject::SetNumberOfRequiredInputs(1);
  // allocate the data objects for the outputs which are
  // just decorators real types
  for ( int i = 0; i < m_NumberOfSupportedDCEFeatures; ++i ) // Iterate over the number of Supported DCE Features
  {
    this->ProcessObject::SetNthOutput(i, this->MakeOutput(i));
  }
}

template< class TInputImage, class TOutputImage >
void
DCEMedianFilter< TInputImage, TOutputImage >
::ThreadedGenerateData(const OutputImageRegionType & outputRegionForThread, ThreadIdType threadId)
{
  // Allocate output
  typename OutputImageType::Pointer     output = this->GetOutput();
  typename InputImageType::ConstPointer input  = this->GetInput();
  const    unsigned int     componentsPerPixel = input->GetNumberOfComponentsPerPixel();

   
  // Find the data-set boundary "faces"
  NeighborhoodAlgorithm::ImageBoundaryFacesCalculator< InputImageType > bC;
  typename NeighborhoodAlgorithm::ImageBoundaryFacesCalculator< InputImageType >::FaceListType
  faceList = bC( input, outputRegionForThread, this->GetRadius() );

  // support progress methods/callbacks
  ProgressReporter progress( this, threadId, outputRegionForThread.GetNumberOfPixels() );

  // All of our neighborhoods have an odd number of pixels, so there is
  // always a median index (if there where an even number of pixels
  // in the neighborhood we have to average the middle two values).

  ConstantBoundaryCondition< InputImageType > nbc;
  std::vector<OutputPixelType > pixels;
  // Process each of the boundary faces.  These are N-d regions which border
  // the edge of the buffer.

  for ( typename NeighborhoodAlgorithm::ImageBoundaryFacesCalculator< InputImageType >::FaceListType::iterator
        fit = faceList.begin(); fit != faceList.end(); ++fit )
    {
    ImageRegionIterator< OutputImageType > it = ImageRegionIterator< OutputImageType >(output, *fit);
  
    ConstNeighborhoodIterator< InputImageType > bit = ConstNeighborhoodIterator< InputImageType >(this->GetRadius(), input, *fit);
    bit.OverrideBoundaryCondition(&nbc);
    bit.GoToBegin();
    const unsigned int neighborhoodSize = (unsigned int) bit.Size();
    pixels.resize(componentsPerPixel);
    while ( !bit.IsAtEnd() )
    {
      // collect all the pixels in the neighborhood, note that we use
      // GetPixel on the NeighborhoodIterator to honor the boundary conditions
      
      for (unsigned int indexComponents = 0; indexComponents < componentsPerPixel; ++indexComponents )
      { 
        OutputPixelType sum = NumericTraits< OutputPixelType >::Zero;     
        for ( unsigned int i = 0; i < neighborhoodSize; ++i )
        {
          sum += (OutputPixelType) ( bit.GetPixel(i))[indexComponents];
        }
        //Get the mean of the whole pixel neigborhood
        pixels[indexComponents] = (sum / neighborhoodSize);
      }
      
      // Calculate the output value
      // Get the maximum value
      it.Set(GetMedian(pixels));

      // Add the Other Features here 

      ++bit;
      ++it;
      progress.CompletedPixel();
      }
      
    }

} // ThreadedGenerateData

template< class TInputImage, class TOutputImage >
typename TOutputImage::PixelType
DCEMedianFilter< TInputImage, TOutputImage >
::GetMax(const std::vector<OutputPixelType> & vec)
{
  return *std::max_element(vec.begin(), vec.end());
}

template< class TInputImage, class TOutputImage >
typename TOutputImage::PixelType
DCEMedianFilter< TInputImage, TOutputImage >
::GetMin(const std::vector<OutputPixelType> & vec)
{
  return *std::min_element(vec.begin(), vec.end());
}

template< class TInputImage, class TOutputImage >
typename TOutputImage::PixelType
DCEMedianFilter< TInputImage, TOutputImage >
::GetRange(const std::vector<OutputPixelType> & vec)
{
  return ((*std::max_element(vec.begin(), vec.end())) - (*std::min_element(vec.begin(), vec.end())));
}

template< class TInputImage, class TOutputImage >
typename TOutputImage::PixelType
DCEMedianFilter< TInputImage, TOutputImage >
::GetMedian(/*const*/ std::vector<OutputPixelType> /*&*/ vec)
{
	auto size = vec.size();

	if(size == 0)
	{
		std::cerr << "Empty vector ..." << __FILE__ << __LINE__ << "\n";
		return 0;
	}

	typename std::vector< InputPixelType >::iterator medianIterator = vec.begin() + (size / 2);
    std::nth_element(vec.begin(), medianIterator, vec.end());

	if(size % 2 == 0) // even
	{
		return ((*(medianIterator)+*(medianIterator-1))/2);
	}
  
    return *medianIterator;
}

template< class TInputImage, class TOutputImage >
typename TOutputImage::PixelType
DCEMedianFilter< TInputImage, TOutputImage >
::GetSum(const std::vector<OutputPixelType> & vec)
{
  OutputPixelType sum = 0.0;
  typename std::vector< OutputPixelType >::const_iterator iterator = vec.begin(), endIt = vec.end();
  while(iterator != endIt)
  {
    sum += *iterator;
    iterator++;
  }

  return sum;
}

template< class TInputImage, class TOutputImage >
typename TOutputImage::PixelType
DCEMedianFilter< TInputImage, TOutputImage >
::GetMean(const std::vector<OutputPixelType> & vec)
{
  return (GetSum(vec) / vec.size());
}

template< class TInputImage, class TOutputImage >
typename TOutputImage::PixelType
DCEMedianFilter< TInputImage, TOutputImage >
::GetVariance(const std::vector<OutputPixelType> & vec)
{
  OutputPixelType mean = GetMean(vec);
  auto len = vec.size();
  if(len <= 1)
  {
    std::cout << "\nError: Length of input vector is less than 2.";
    return 0.0;
  }

  OutputPixelType variance = 0.0;
  const typename std::vector< OutputPixelType >::const_iterator iterator = vec.begin(), endIt = vec.end();
  while(iterator != endIt)
  {
    variance += std::pow(*iterator - mean, 2);
    iterator++;
  }

  return (variance / (vec.size() - 1));
}

template< class TInputImage, class TOutputImage >
typename TOutputImage::PixelType
DCEMedianFilter< TInputImage, TOutputImage >
::GetStdDeviation(const std::vector<OutputPixelType> & vec)
{
  return std::sqrt(GetVariance(vec));
}

} // end namespace itk

#endif
