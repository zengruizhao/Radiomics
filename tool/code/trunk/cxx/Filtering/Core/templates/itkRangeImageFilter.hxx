#ifndef __itkRangeImageFilter_hxx
#define __itkRangeImageFilter_hxx



////////////////////////////////////////////////////////////////////////////////////////////////////
// our own itk includes
#include "itkRangeImageFilter.h"

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
#include <algorithm>
////////////////////////////////////////////////////////////////////////////////////////////////////



namespace itk
{
template< class TInputImage, class TOutputImage >
RangeImageFilter< TInputImage, TOutputImage >
::RangeImageFilter()
{}

template< class TInputImage, class TOutputImage >
void
RangeImageFilter< TInputImage, TOutputImage >
::ThreadedGenerateData(const OutputImageRegionType & outputRegionForThread,
                       ThreadIdType threadId)
{
  // Allocate output
  typename OutputImageType::Pointer      output = this->GetOutput();
  typename  InputImageType::ConstPointer input  = this->GetInput();

  // Find the data-set boundary "faces"
  NeighborhoodAlgorithm::ImageBoundaryFacesCalculator< InputImageType > bC;
  typename NeighborhoodAlgorithm::ImageBoundaryFacesCalculator< InputImageType >::FaceListType
  faceList = bC( input, outputRegionForThread, this->GetRadius() );

  // support progress methods/callbacks
  ProgressReporter progress( this, threadId, outputRegionForThread.GetNumberOfPixels() );

  // specify the boundary condition (how will GetPixel work on the boundary?)
  ZeroFluxNeumannBoundaryCondition<
    InputImageType >                nbc;

  // Process each of the boundary faces.  These are N-d regions which border
  // the edge of the buffer.
  typedef typename NeighborhoodAlgorithm::ImageBoundaryFacesCalculator<
    InputImageType >::FaceListType::iterator FaceIteratorType;
  for ( FaceIteratorType fit = faceList.begin(); fit != faceList.end(); ++fit )
    {
    ImageRegionIterator< OutputImageType > it = ImageRegionIterator< OutputImageType >(output, *fit);

    ConstNeighborhoodIterator< InputImageType > bit =
      ConstNeighborhoodIterator< InputImageType >(this->GetRadius(), input, *fit);
    bit.OverrideBoundaryCondition(&nbc);
    bit.GoToBegin();
    const SizeValueType neighborhoodSize = bit.Size();

    while ( !bit.IsAtEnd() )
      {
    
      // initialize the min and max as the first pixel
      InputRealType
        min = static_cast< InputRealType >( bit.GetPixel(0) ),
        max = static_cast< InputRealType >( bit.GetPixel(0) );

      for ( SizeValueType i = 1; i < neighborhoodSize; ++i )
        {

        // get the pixel's value
        const InputRealType currentValue =
          static_cast< InputRealType >( bit.GetPixel(i) );

        // possibly update the min and max
        if ( currentValue < min )
          {
          min = currentValue;
          }
        if ( currentValue > max )
          {
          max = currentValue;
          }
    
        } // for i

      // get the range value
      const InputRealType range = max - min;

      // set it in the output
      it.Set( static_cast< typename OutputImageType::PixelType >( range ) );

      // move on to the next iteration
      ++bit;
      ++it;
      progress.CompletedPixel();

      } // while not at end

    } // for face iter

} // ThreadedGenerateData

} // end namespace itk

#endif
