#ifndef __itkMeanVectorImageFilter_hxx
#define __itkMeanVectorImageFilter_hxx



////////////////////////////////////////////////////////////////////////////////////////////////////
// our own itk includes
#include "itkMeanVectorImageFilter.h"
#include "ccipdDuplicateImage.h"

#include "ccipdDisableWarningsMacro.h"
  // itk includes
  #include <itkConstNeighborhoodIterator.h>
  #include <itkNeighborhoodInnerProduct.h>
  #include <itkImageRegionIterator.h>
  #include <itkNeighborhoodAlgorithm.h>
  #include <itkOffset.h>
  #include <itkProgressReporter.h>
#include "ccipdEnableWarningsMacro.h"

// std includes
#include <vector>
#include <algorithm>
////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////
namespace itk
{
template< class TInputImage, class TOutputImage >
MeanVectorImageFilter< TInputImage, TOutputImage >
::MeanVectorImageFilter()
{ 
  SetMultiplicationFactor (1.0);
}

template< class TInputImage, class TOutputImage >
void MeanVectorImageFilter< TInputImage, TOutputImage >
::BeforeThreadedGenerateData()
{
  Superclass::BeforeThreadedGenerateData();
  if (!this->m_WasFactorImageAssigned)
    CreateMultiplicationFactorImage ( );
};


template< class TInputImage, class TOutputImage >
void
MeanVectorImageFilter< TInputImage, TOutputImage >
::ThreadedGenerateData(const OutputImageRegionType & outputRegionForThread,
                       ThreadIdType threadId)
{
  // Allocate output
  typename OutputImageType::Pointer       output = this->GetOutput();
  typename  InputImageType::ConstPointer  input  = this->GetInput();

  // Find the data-set boundary "faces"
  NeighborhoodAlgorithm::ImageBoundaryFacesCalculator< InputImageType > bC;
  typename NeighborhoodAlgorithm::ImageBoundaryFacesCalculator< InputImageType >::FaceListType
  faceList = bC( input, outputRegionForThread, this->GetRadius() );

  // support progress methods/callbacks
  ProgressReporter progress( this, threadId, outputRegionForThread.GetNumberOfPixels() );

  // All of our neighborhoods have an odd number of pixels, so there is
  // always a median index (if there where an even number of pixels
  // in the neighborhood we have to average the middle two values).

  ZeroFluxNeumannBoundaryCondition< InputImageType > nbc;
  std::vector< InputPixelType >                      pixels;
  // Process each of the boundary faces.  These are N-d regions which border
  // the edge of the buffer.
  const unsigned int componentsPerPixel = input->GetNumberOfComponentsPerPixel();
  for ( typename NeighborhoodAlgorithm::ImageBoundaryFacesCalculator< InputImageType >::FaceListType::iterator
        fit = faceList.begin(); fit != faceList.end(); ++fit )
    {
    ImageRegionIterator< OutputImageType > 
      it      = ImageRegionIterator< OutputImageType >(output, *fit),
     itRatio = ImageRegionIterator< OutputImageType >(m_MultiplicationFactorImage, *fit);
  
    ConstNeighborhoodIterator< InputImageType > bit =
      ConstNeighborhoodIterator< InputImageType >(this->GetRadius(), input, *fit);
    bit.OverrideBoundaryCondition(&nbc);
    bit.GoToBegin();
    const unsigned int neighborhoodSize = (int) bit.Size();
    while ( !bit.IsAtEnd() )
      {
      // collect all the pixels in the neighborhood, note that we use
      // GetPixel on the NeighborhoodIterator to honor the boundary conditions

      OutputPixelType sum = NumericTraits< OutputPixelType >::Zero;
      for ( unsigned int i = 0; i < neighborhoodSize; ++i )
        {
         for (unsigned int indexComponents = 0; indexComponents < componentsPerPixel; ++indexComponents )
            {
            sum += (OutputPixelType) ( bit.GetPixel(i))[indexComponents];
            }
        }
      
      // get the median value
      OutputPixelType scaleFactor = (OutputPixelType)  itRatio.Get() * (OutputPixelType) (neighborhoodSize*componentsPerPixel);

      if (scaleFactor>1e-9) // essentially epsilon
        it.Set( sum / scaleFactor );
      else
        it.Set(  OutputPixelType (0.0) );

      ++bit;
      ++it;
      ++itRatio;
      progress.CompletedPixel();
      }     
    }

} // ThreadedGenerateData

/* user defined factor allows to use the filter as frequency */
template< class TInputImage, class TOutputImage >
void MeanVectorImageFilter< TInputImage, TOutputImage >
::SetMultiplicationFactor (float factor)
{
  this->m_WasFactorImageAssigned  = false;
  m_MultiplicationFactor          = factor;
}



template< class TInputImage, class TOutputImage >
void MeanVectorImageFilter< TInputImage, TOutputImage >
::CreateMultiplicationFactorImage()
{
  m_MultiplicationFactorImage = OutputImageType::New();
  m_MultiplicationFactorImage->SetRegions( this->GetOutput()->GetRequestedRegion() );
  m_MultiplicationFactorImage->Allocate();
  m_MultiplicationFactorImage->FillBuffer( m_MultiplicationFactor );
}

/* user defined factor allows to use the filter as frequency */
template< class TInputImage, class TOutputImage >
void MeanVectorImageFilter< TInputImage, TOutputImage >
::SetMultiplicationFactorImage (OutputImageType * image){ 
  this->m_WasFactorImageAssigned    = true; 
  m_MultiplicationFactorImage       = image;
} 

} // end namespace itk

#endif
