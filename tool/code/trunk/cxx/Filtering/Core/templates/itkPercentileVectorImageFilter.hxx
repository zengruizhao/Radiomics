#ifndef __itkPercentileVectorImageFilter_hxx
#define __itkPercentileVectorImageFilter_hxx



////////////////////////////////////////////////////////////////////////////////////////////////////
// our own itk includes
#include "itkPercentileVectorImageFilter.h"

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
PercentileVectorImageFilter< TInputImage, TOutputImage >
::PercentileVectorImageFilter():
  m_percentile(0.5) // apply a median filter
{}

template< class TInputImage, class TOutputImage > 
void
PercentileVectorImageFilter< TInputImage, TOutputImage >
::SetPercentile( RealType percentile)
{
  if (percentile<0 || percentile>1 )
    {
      std::cout << "Percentile should be between 0 and 1!" << std::endl;
      return;
    }

  m_percentile = percentile;
}

template< class TInputImage, class TOutputImage >
void
PercentileVectorImageFilter< TInputImage, TOutputImage >
::ThreadedGenerateData(const OutputImageRegionType & outputRegionForThread,
                       ThreadIdType threadId)
{
  // Allocate output
  typename OutputImageType::Pointer output = this->GetOutput();
  typename  InputImageType::ConstPointer input  = this->GetInput();

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
    ImageRegionIterator< OutputImageType > it = ImageRegionIterator< OutputImageType >(output, *fit);
  
    ConstNeighborhoodIterator< InputImageType > bit =
      ConstNeighborhoodIterator< InputImageType >(this->GetRadius(), input, *fit);
    bit.OverrideBoundaryCondition(&nbc);
    bit.GoToBegin();
    const unsigned int neighborhoodSize = (unsigned int) bit.Size();
    const unsigned int position = (unsigned int)(neighborhoodSize * componentsPerPixel * m_percentile);
    pixels.resize(neighborhoodSize*componentsPerPixel);

    while ( !bit.IsAtEnd() )
      {
      // collect all the pixels in the neighborhood, note that we use
      // GetPixel on the NeighborhoodIterator to honor the boundary conditions

      for ( unsigned int i = 0; i < neighborhoodSize; ++i )
        {
         for (unsigned int indexComponents = 0; indexComponents < componentsPerPixel; ++indexComponents )
            {
            pixels[i*componentsPerPixel+indexComponents] = ( bit.GetPixel(i))[indexComponents];
            }
        }
      
      // get the median value
      const typename std::vector< InputPixelType >::iterator medianIterator = pixels.begin() + position;
      std::nth_element( pixels.begin(), medianIterator, pixels.end() );
      it.Set( static_cast< typename OutputImageType::PixelType >( *medianIterator ) );

      ++bit;
      ++it;
      progress.CompletedPixel();
      }
      
    }

} // ThreadedGenerateData


} // end namespace itk

#endif
