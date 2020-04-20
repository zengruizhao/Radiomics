#ifndef __itkSkewImageFilter_hxx
#define __itkSkewImageFilter_hxx



////////////////////////////////////////////////////////////////////////////////////////////////////
// our own itk includes
#include "itkSkewImageFilter.h"

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
SkewImageFilter< TInputImage, TOutputImage >
::SkewImageFilter()
{}

template< class TInputImage, class TOutputImage >
void
SkewImageFilter< TInputImage, TOutputImage >
::ThreadedGenerateData(const OutputImageRegionType & outputRegionForThread,
                       ThreadIdType threadId)
{

  // Prepare our inputs and outputs
  typename OutputImageType::Pointer      output = this->GetOutput();
  typename InputImageType ::ConstPointer input  = this->GetInput ();
  InputPixelType nan = std::numeric_limits< InputPixelType >::quiet_NaN();

  // Find the data-set boundary "faces"
  NeighborhoodAlgorithm::ImageBoundaryFacesCalculator< InputImageType > bC;
  typename NeighborhoodAlgorithm::ImageBoundaryFacesCalculator< InputImageType >::FaceListType
  faceList = bC( input, outputRegionForThread, this->GetRadius() );

  // support progress methods/callbacks
  ProgressReporter progress( this, threadId, outputRegionForThread.GetNumberOfPixels() );

  // set up our boundary condition
  ZeroFluxNeumannBoundaryCondition<
    InputImageType >                nbc;

  // Process each of the boundary faces.  These are N-d regions which border
  // the edge of the buffer.
  typedef typename NeighborhoodAlgorithm::ImageBoundaryFacesCalculator<
    InputImageType >::FaceListType::iterator FaceIteratorType;
  for ( FaceIteratorType fit = faceList.begin(); fit != faceList.end(); ++fit )
    {

    // prepare our iterators
    ImageRegionIterator< OutputImageType > outputIter(
      output, *fit);
    ConstNeighborhoodIterator< InputImageType > bit(
      this->GetRadius(), input, *fit );

    // set the boundary condition
    bit.OverrideBoundaryCondition( &nbc );
    bit.GoToBegin();

    // prepare our vectors to store pixel values
    const SizeValueType neighborhoodSize = bit.Size();
    std::vector< InputPixelType > pixels;
    pixels.resize( neighborhoodSize );

    while ( !bit.IsAtEnd() )
      {

      // initialize my variables
      InputRealType
        sum               = NumericTraits< InputRealType >::Zero,
        variance          = NumericTraits< InputRealType >::Zero,
        skewNum           = NumericTraits< InputRealType >::Zero ;
                      

      // collect all the pixels in the neighborhood, note that we use
      // GetPixel on the NeighborhoodIterator to honor the boundary conditions
      for ( SizeValueType i = 0; i < neighborhoodSize; ++i )
        {

        const InputRealType pixelValue = bit.GetPixel( i );
        sum += static_cast< InputRealType >( pixelValue );
        pixels[i] = static_cast < InputPixelType >( pixelValue );

        } // i

      // get the standard deviation value
      // std = sqrt( 1/(N-1) sum ( x-mean(x) )^2 )

      const InputRealType
        realNeighborhoodSize = static_cast< InputRealType >( neighborhoodSize ),
        mean = sum / realNeighborhoodSize;
    
      for (unsigned int i=0; i < neighborhoodSize; ++i )
        {
        const InputRealType diffFromMean = pixels[i] - mean;
        variance += std::pow( diffFromMean, 2);
        skewNum += std::pow( diffFromMean, 3);
        }
    
      const InputRealType finalStandardDeviation = 
        std::sqrt( variance / (realNeighborhoodSize) );

      const InputRealType skew = ( finalStandardDeviation == 0 ) ?
        nan: 
        ( skewNum ) /( realNeighborhoodSize * std::pow ( finalStandardDeviation, 3) );

      // set the output value
      outputIter.Set(
        static_cast< typename OutputImageType::PixelType >(
         skew ) );

      // update the iterators
      ++bit;
      ++outputIter;
      progress.CompletedPixel();

      } // while not at end

    } // for face iterator

} // ThreadedGenerateData


} // end namespace itk

#endif
