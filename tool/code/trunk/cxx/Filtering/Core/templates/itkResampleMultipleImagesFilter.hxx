#ifndef __itkResampleMultipleImagesFilter_hxx
#define __itkResampleMultipleImagesFilter_hxx

#include "itkResampleMultipleImagesFilter.h"
#include "itkIdentityTransform.h"
#include "itkLinearInterpolateMultipleImagesFunction.h"
#include "itkSelectiveInterpolateMultipleImagesFunction.h"
#include "itkNearestNeighborInterpolateMultipleImagesFunction.h"
#include "itkProgressReporter.h"
#include "itkImageRegionIteratorWithIndex.h"

namespace itk
{
/**
 * Initialize new instance
 */
template< typename TInputImage, typename TOutputImage, typename TInterpolatorPrecisionType >
ResampleMultipleImagesFilter< TInputImage, TOutputImage, TInterpolatorPrecisionType >
::ResampleMultipleImagesFilter()
{
  m_OutputSpacing.Fill(1.0);
  m_OutputOrigin.Fill(0.0);
  m_OutputDirection.SetIdentity();
  m_Size.Fill(0);
  m_OutputStartIndex.Fill(0);

  TransformPointerType transform(IdentityTransform< TInterpolatorPrecisionType, ImageDimension >::New());
  m_Transforms.push_back(transform);

  //m_Interpolator = LinearInterpolateMultipleImagesFunction< InputImageType, TInterpolatorPrecisionType >::New();
  //m_Interpolator = NearestNeighborInterpolateMultipleImagesFunction< InputImageType, TInterpolatorPrecisionType >::New();
  m_Interpolator = SelectiveInterpolateMultipleImagesFunction< InputImageType, TInterpolatorPrecisionType >::New();

  m_DefaultPixelValue = NumericTraits< PixelType >::ZeroValue( m_DefaultPixelValue );
}

/**
 * Print out a description of self
 *
 * \todo Add details about this class
 */
template< typename TInputImage, typename TOutputImage, typename TInterpolatorPrecisionType >
void
ResampleMultipleImagesFilter< TInputImage, TOutputImage, TInterpolatorPrecisionType >
::PrintSelf(std::ostream & os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);

  os << indent << "DefaultPixelValue: "
     << static_cast< typename NumericTraits< PixelType >::PrintType >( m_DefaultPixelValue )
     << std::endl;
  os << indent << "Size: " << m_Size << std::endl;
  os << indent << "OutputStartIndex: " << m_OutputStartIndex << std::endl;
  os << indent << "OutputSpacing: " << m_OutputSpacing << std::endl;
  os << indent << "OutputOrigin: " << m_OutputOrigin << std::endl;
  os << indent << "OutputDirection: " << m_OutputDirection << std::endl;
  os << indent << "Transform: " << m_Transforms[0].GetPointer() << std::endl;
  os << indent << "Interpolator: " << m_Interpolator.GetPointer() << std::endl;
}

/**
 * Set the output image spacing.
 */
template< typename TInputImage, typename TOutputImage, typename TInterpolatorPrecisionType >
void
ResampleMultipleImagesFilter< TInputImage, TOutputImage, TInterpolatorPrecisionType >
::SetOutputSpacing(const double *spacing)
{
  SpacingType s(spacing);

  this->SetOutputSpacing(s);
}

/**
 * Set the output image origin.
 */
template< typename TInputImage, typename TOutputImage, typename TInterpolatorPrecisionType >
void
ResampleMultipleImagesFilter< TInputImage, TOutputImage, TInterpolatorPrecisionType >
::SetOutputOrigin(const double *origin)
{
  OriginPointType p(origin);

  this->SetOutputOrigin(p);
}
 
template< typename TInputImage, typename TOutputImage, typename TInterpolatorPrecisionType >
void
ResampleMultipleImagesFilter< TInputImage, TOutputImage, TInterpolatorPrecisionType >
::SetInput(const int index, const TInputImage* image)
{
  Superclass::SetInput(index, image);

  // add identity transforms to fill transform vector
  while (m_Transforms.size()<=index)
  {
      TransformPointerType transform(IdentityTransform< TInterpolatorPrecisionType, ImageDimension >::New());
      m_Transforms.push_back( transform ); 
  }

  //setIdentity
  m_Transforms[index] = IdentityTransform< TInterpolatorPrecisionType, ImageDimension >::New(); 
} 

/** Set Nth Transform */
template< typename TInputImage, typename TOutputImage, typename TInterpolatorPrecisionType >
void ResampleMultipleImagesFilter< TInputImage, TOutputImage, TInterpolatorPrecisionType >
::SetTransform( const int index, const TransformType * transf )
{
  
  while (m_Transforms.size()<=index)
  {
      TransformPointerType transform(IdentityTransform< TInterpolatorPrecisionType, ImageDimension >::New());
      m_Transforms.push_back( transform ); 
  }

  if (!transf) // not null
  {
    TransformPointerType transform(const_cast<TransformType *>( transf ));
    m_Transforms[index] = transform; 
  }
  
};


/**
 * Set up state of filter before multi-threading.
 * InterpolatorType::SetInputImage is not thread-safe and hence
 * has to be set up before ThreadedGenerateData
 */
template< typename TInputImage, typename TOutputImage, typename TInterpolatorPrecisionType >
void
ResampleMultipleImagesFilter< TInputImage, TOutputImage, TInterpolatorPrecisionType >
::BeforeThreadedGenerateData()
{
  if ( !m_Interpolator )
    {
    itkExceptionMacro(<< "Interpolator not set");
    }

  // Connect input image to interpolator
  for (unsigned int idx=0; idx < this->GetNumberOfIndexedInputs(); ++idx)
    m_Interpolator->AddInputImage( this->GetInput(idx) );

  NumericTraits<PixelType>::SetLength( m_DefaultPixelValue,
    this->GetInput()->GetNumberOfComponentsPerPixel() );
  m_DefaultPixelValue =  NumericTraits< PixelType >::ZeroValue( m_DefaultPixelValue );
}

/**
 * Set up state of filter after multi-threading.
 */
template< typename TInputImage, typename TOutputImage, typename TInterpolatorPrecisionType >
void
ResampleMultipleImagesFilter< TInputImage, TOutputImage, TInterpolatorPrecisionType >
::AfterThreadedGenerateData()
{
  // Disconnect input image from the interpolator
  m_Interpolator->ClearInputImages();
}

/**
 * ThreadedGenerateData
 */
template< typename TInputImage, typename TOutputImage, typename TInterpolatorPrecisionType >
void
ResampleMultipleImagesFilter< TInputImage, TOutputImage, TInterpolatorPrecisionType >
::ThreadedGenerateData(
  const OutputImageRegionType & outputRegionForThread,
  ThreadIdType threadId)
{
  itkDebugMacro(<< "Actually executing");

  // Get the output pointers
  OutputImagePointer outputPtr = this->GetOutput();

  // Get this input pointers
  std::vector<InputImageConstPointer> inputPtr;
  for (unsigned int idx=0; idx < this->GetNumberOfIndexedInputs(); ++idx)
    inputPtr.push_back(this->GetInput(idx));

  // Create an iterator that will walk the output region for this thread.
  typedef ImageRegionIteratorWithIndex< TOutputImage > OutputIterator;

  OutputIterator outIt(outputPtr, outputRegionForThread);

  // Define a few indices that will be used to translate from an input pixel
  // to an output pixel
  PointType outputPoint;         // Coordinates of current output pixel
  PointType inputPoint;          // Coordinates of current input pixel

  typedef ContinuousIndex< TInterpolatorPrecisionType, ImageDimension > ContinuousIndexType;
  ContinuousIndexType inputIndex;
  std::vector<ContinuousIndexType> inputIndexes;

  // Support for progress methods/callbacks
  ProgressReporter progress( this, threadId, outputRegionForThread.GetNumberOfPixels() );

  typedef typename InterpolatorType::OutputType OutputType;

  // Walk the output region
  outIt.GoToBegin();

  while ( !outIt.IsAtEnd() )
    {
    // Determine the index of the current output pixel
    outputPtr->TransformIndexToPhysicalPoint(outIt.GetIndex(), outputPoint);

    // Compute corresponding input pixel position
    inputIndexes.clear();
    for (unsigned int idx=0; idx < this->GetNumberOfIndexedInputs(); ++idx)
    {
      inputPoint = m_Transforms[idx]->TransformPoint(outputPoint);
      inputPtr[idx]->TransformPhysicalPointToContinuousIndex(inputPoint, inputIndex);
      inputIndexes.push_back(inputIndex);
      //if (threadId==0)
      //  std::cout << "Points " << inputIndex << std::endl;
    }

    PixelType        pixval;

    const OutputType value = m_Interpolator->EvaluateAtContinuousIndexes(inputIndexes);
    pixval = static_cast< PixelType >( value );

    outIt.Set(pixval);

/*
    // Evaluate input at right position and copy to the output
    if ( m_Interpolator->IsInsideBuffer(inputIndex) )
      {
      PixelType        pixval;

      const OutputType value =
        m_Interpolator->EvaluateAtContinuousIndex(inputIndex);
      pixval = static_cast< PixelType >( value );

      outIt.Set(pixval);
      }

    else
      {
      outIt.Set(m_DefaultPixelValue); // default background value
      }
*/
    progress.CompletedPixel();
    ++outIt;
    }
}

/**
 * Inform pipeline of necessary input image region
 *
 * Determining the actual input region is non-trivial, especially
 * when we cannot assume anything about the transform being used.
 * So we do the easy thing and request the entire input image.
 */
template< typename TInputImage, typename TOutputImage, typename TInterpolatorPrecisionType >
void
ResampleMultipleImagesFilter< TInputImage, TOutputImage, TInterpolatorPrecisionType >
::GenerateInputRequestedRegion()
{
  // call the superclass's implementation of this method
  Superclass::GenerateInputRequestedRegion();

  if ( !this->GetInput() )
    {
    return;
    }

  for (unsigned int idx=0; idx<this->GetNumberOfIndexedInputs(); ++idx)
  {
    // get pointers to the input and output
    InputImagePointer inputPtr  =
      const_cast< TInputImage * >( this->GetInput(idx) );

    // Request the entire input image
    InputImageRegionType inputRegion;
    inputRegion = inputPtr->GetLargestPossibleRegion();
    inputPtr->SetRequestedRegion(inputRegion);
  }
}

/**
 * Inform pipeline of required output region
 */
template< typename TInputImage, typename TOutputImage, typename TInterpolatorPrecisionType >
void
ResampleMultipleImagesFilter< TInputImage, TOutputImage, TInterpolatorPrecisionType >
::GenerateOutputInformation()
{
  // call the superclass' implementation of this method
  Superclass::GenerateOutputInformation();

  // get pointers to the input and output
  OutputImagePointer outputPtr = this->GetOutput();
  if ( !outputPtr )
    {
    return;
    }

  // Set the size of the output region
  typename TOutputImage::RegionType outputLargestPossibleRegion;
  outputLargestPossibleRegion.SetSize(m_Size);
  outputLargestPossibleRegion.SetIndex(m_OutputStartIndex);
  outputPtr->SetLargestPossibleRegion(outputLargestPossibleRegion);

  // Set spacing and origin
  outputPtr->SetSpacing(m_OutputSpacing);
  outputPtr->SetOrigin(m_OutputOrigin);
  outputPtr->SetDirection(m_OutputDirection);
}

/**
 * Verify if any of the components has been modified.
 */
template< typename TInputImage, typename TOutputImage, typename TInterpolatorPrecisionType >
ModifiedTimeType
ResampleMultipleImagesFilter< TInputImage, TOutputImage, TInterpolatorPrecisionType >
::GetMTime(void) const
{
  ModifiedTimeType latestTime = Object::GetMTime();

  if ( m_Transforms[0] )
    {
    if ( latestTime < m_Transforms[0]->GetMTime() )
      {
      latestTime = m_Transforms[0]->GetMTime();
      }
    }

  if ( m_Interpolator )
    {
    if ( latestTime < m_Interpolator->GetMTime() )
      {
      latestTime = m_Interpolator->GetMTime();
      }
    }

  return latestTime;
}
} // end namespace itk

#endif
