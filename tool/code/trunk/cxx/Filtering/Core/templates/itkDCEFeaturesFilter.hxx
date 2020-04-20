#ifndef __itkDCEFeaturesFilter_hxx
#define __itkDCEFeaturesFilter_hxx



////////////////////////////////////////////////////////////////////////////////////////////////////
// our own itk includes
#include "itkDCEFeaturesFilter.h"


  // itk includes
  #include <itkConstNeighborhoodIterator.h>
  #include <itkConstantBoundaryCondition.h>  
  #include <itkNeighborhoodInnerProduct.h>
  #include <itkImageRegionIterator.h>
  #include <itkNeighborhoodAlgorithm.h>
  #include <itkOffset.h>
  #include <itkProgressReporter.h>


// std includes
#include <vector>
#include <algorithm>
////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////
namespace itk
{
template< class TInputImage, class TOutputImage >
DCEFeaturesFilter< TInputImage, TOutputImage >
::DCEFeaturesFilter()
{
  // Default values of member variables
  this->SetRadius(0); // m_Radius = {{0, 0, 0}} // This means that the neighbourhood box is 1 x 1 size
  m_DeltaT = 1.0;
  
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
DCEFeaturesFilter< TInputImage, TOutputImage >
::ThreadedGenerateData(const OutputImageRegionType & outputRegionForThread, ThreadIdType threadId)
{
  // Allocate output
  typename OutputImageType::Pointer     pMaxUptake        = this->GetOutput(0);
  typename OutputImageType::Pointer     pTimeToPeak       = this->GetOutput(1);
  typename OutputImageType::Pointer     pRateOfUptake     = this->GetOutput(2);
  typename OutputImageType::Pointer     pRateOfWashout    = this->GetOutput(3);
  typename OutputImageType::Pointer     pEnhancement      = this->GetOutput(4);
  typename OutputImageType::Pointer     pEnhancementRatio = this->GetOutput(5);


  typename InputImageType::ConstPointer input  = this->GetInput();
  const    unsigned int     componentsPerPixel = input->GetNumberOfComponentsPerPixel();

  // Find the data-set boundary "faces"
  NeighborhoodAlgorithm::ImageBoundaryFacesCalculator< InputImageType > bC;
  typename NeighborhoodAlgorithm::ImageBoundaryFacesCalculator< InputImageType >::FaceListType
  faceList = bC( input, outputRegionForThread, this->GetRadius() );

  // support progress methods/callbacks
  ProgressReporter progress( this, threadId, outputRegionForThread.GetNumberOfPixels() );

  ConstantBoundaryCondition< InputImageType > nbc;
  std::vector<OutputPixelType > pixels;
  // Process each of the boundary faces.  These are N-d regions which border
  // the edge of the buffer.

  for ( typename NeighborhoodAlgorithm::ImageBoundaryFacesCalculator< InputImageType >::FaceListType::iterator
        fit = faceList.begin(); fit != faceList.end(); ++fit )
    {
        ImageRegionIterator< OutputImageType > maxUptakeIt        = ImageRegionIterator< OutputImageType >(pMaxUptake       , *fit);
		ImageRegionIterator< OutputImageType > timeToPeakIt       = ImageRegionIterator< OutputImageType >(pTimeToPeak      , *fit);
		ImageRegionIterator< OutputImageType > rateOfUptakeIt     = ImageRegionIterator< OutputImageType >(pRateOfUptake    , *fit);
		ImageRegionIterator< OutputImageType > rateOfWashoutIt    = ImageRegionIterator< OutputImageType >(pRateOfWashout   , *fit);
		ImageRegionIterator< OutputImageType > enhancementIt      = ImageRegionIterator< OutputImageType >(pEnhancement     , *fit);
		ImageRegionIterator< OutputImageType > enhancementRatioIt = ImageRegionIterator< OutputImageType >(pEnhancementRatio, *fit);



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
      
      // Calculate the output values

	  OutputPixelType Max      = GetMax(pixels);
	  OutputPixelType idxMax   = GetIndexOfMax(pixels);
	  OutputPixelType first    = pixels[0];
	  OutputPixelType last     = pixels.back();
	  OutputPixelType idxlast  = componentsPerPixel - 1;

	  OutputPixelType MU  = (Max - first) /*/ first*/;
	  OutputPixelType TtP = m_DeltaT * idxMax;

	  OutputPixelType En  = (pixels[idxMax / 2] - first) /*/ first*/;

	  maxUptakeIt.Set(MU);

	  timeToPeakIt.Set(TtP);

	  if(TtP == 0) TtP += (m_DeltaT);

	  rateOfUptakeIt.Set(MU / TtP);

	  if(idxlast == idxMax) idxlast++;

	  rateOfWashoutIt.Set((Max - last)/(m_DeltaT * (idxlast - idxMax)));

	  enhancementIt.Set(En);

	  if(MU == 0) MU++;

	  enhancementRatioIt.Set(En / MU);

      // Add the Other Features here 

      ++bit               ;
	  ++maxUptakeIt       ;
	  ++timeToPeakIt      ;
	  ++rateOfUptakeIt    ;
	  ++rateOfWashoutIt   ;
	  ++enhancementIt     ;
	  ++enhancementRatioIt;

      progress.CompletedPixel();
      }
      
    }

} // ThreadedGenerateData

template< class TInputImage, class TOutputImage >
typename TOutputImage::PixelType
DCEFeaturesFilter< TInputImage, TOutputImage >
::GetMax(const std::vector<OutputPixelType> & vec)
{
  return *std::max_element(vec.begin(), vec.end());
}

template< class TInputImage, class TOutputImage >
typename TOutputImage::PixelType
DCEFeaturesFilter< TInputImage, TOutputImage >
::GetIndexOfMax(const std::vector<OutputPixelType> & vec)
{
  return std::distance(vec.begin(), std::max_element(vec.begin(), vec.end()));
}

template< class TInputImage, class TOutputImage >
typename TOutputImage::Pointer
DCEFeaturesFilter< TInputImage, TOutputImage >
::GetMaximumUptake()
{
  if (this->GetNumberOfOutputs() < 1) return 0;
  return static_cast<TOutputImage*>(this->GetOutput(0));
}

template< class TInputImage, class TOutputImage >
typename TOutputImage::Pointer
DCEFeaturesFilter< TInputImage, TOutputImage >
::GetTimeToPeak()
{
  if (this->GetNumberOfOutputs() < 1) return 0;
  return static_cast<TOutputImage*>(this->GetOutput(1));
}

template< class TInputImage, class TOutputImage >
typename TOutputImage::Pointer
DCEFeaturesFilter< TInputImage, TOutputImage >
::GetRateOfUptake()
{
  if (this->GetNumberOfOutputs() < 1) return 0;
  return static_cast<TOutputImage*>(this->GetOutput(2));
}

template< class TInputImage, class TOutputImage >
typename TOutputImage::Pointer
DCEFeaturesFilter< TInputImage, TOutputImage >
::GetRateOfWashout()
{
  if (this->GetNumberOfOutputs() < 1) return 0;
  return static_cast<TOutputImage*>(this->GetOutput(3));
}

template< class TInputImage, class TOutputImage >
typename TOutputImage::Pointer
DCEFeaturesFilter< TInputImage, TOutputImage >
::GetEnhancement()
{
  if (this->GetNumberOfOutputs() < 1) return 0;
  return static_cast<TOutputImage*>(this->GetOutput(4));
}

template< class TInputImage, class TOutputImage >
typename TOutputImage::Pointer
DCEFeaturesFilter< TInputImage, TOutputImage >
::GetEnhancementRatio()
{
  if (this->GetNumberOfOutputs() < 1) return 0;
  return static_cast<TOutputImage*>(this->GetOutput(5));
}


} // end namespace itk

#endif
