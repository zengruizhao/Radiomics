#ifndef __itkMeanAbsoluteDistanceImageFilter_hxx
#define __itkMeanAbsoluteDistanceImageFilter_hxx



/**
 * Modeled after itkMeanAbsoluteDistanceImageFilter
 */



//////////////////////////////////////////////////////////////////////////////////////////
// our own itk includes
#include "itkMaskToLevelsetFilter.h"

#include "ccipdDisableWarningsMacro.h"
  // itk includes
  #include <itkImageRegionConstIterator.h>
  #include <itkProgressReporter.h>
#include "ccipdEnableWarningsMacro.h"

//////////////////////////////////////////////////////////////////////////////////////////



/**
 * Note from Rob 2013-02-13:
 * 
 * SEE OUR WIKI PAGE HERE: http://bit.ly/Y8VCKv
 * 
 *    "Never "use" an entire namespace.
 *     This forces you to actually know what
 *     you are including, and is a good
 *     way to avoid conflicts."
 * 
 * Do NOT include an entire namespace!
 * 
 */
//using namespace std;



/**
 * Note from Rob 2013-02-13:
 * 
 * Use "this->" when you can.
 * 
 */
 


namespace itk
{



//////////////////////////////////////////////////////////////////////////////////////////
template< class TInputImage1, class TInputImage2 >
MeanAbsoluteDistanceImageFilter< TInputImage1, TInputImage2 >
::MeanAbsoluteDistanceImageFilter():
m_SegmentationBorderThickness( .01  ),
m_ComputeLevelsets           ( true ),
m_AbsoluteDistanceSums       ( 1    ),
m_CountBorderPixels          ( 1    )
{

  // this filter requires two input images
  this->SetNumberOfRequiredInputs(2);

  this->m_MeanAbsoluteDistance = NumericTraits< RealType >::Zero;

}
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
template< class TInputImage1, class TInputImage2 >
void
MeanAbsoluteDistanceImageFilter< TInputImage1, TInputImage2 >
::SetGroundTruth(const GroundTruthImageType * image)
{
  this->SetNthInput( 1, const_cast< GroundTruthImageType * >( image ) );
}
//////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////
template< class TInputImage1, class TInputImage2 >
void
MeanAbsoluteDistanceImageFilter< TInputImage1, TInputImage2 >
::SetGroundTruthLevelset(LevelsetImageType  *levelset)
{
  this->m_GroundTruthLevelset = levelset;
}
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
template< class TInputImage1, class TInputImage2 >
void
MeanAbsoluteDistanceImageFilter< TInputImage1, TInputImage2 >::
SetSegmentationLevelset(LevelsetImageType  *levelset)
{
  this->m_SegmentationLevelset = levelset;
}
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
template< class TInputImage1, class TInputImage2 >
const typename MeanAbsoluteDistanceImageFilter< TInputImage1, TInputImage2 >
::GroundTruthImageType *
MeanAbsoluteDistanceImageFilter< TInputImage1, TInputImage2 >
::GetGroundTruth()
{

  return static_cast< const GroundTruthImageType * >
         ( this->ProcessObject::GetInput(1) );

}
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
template< class TInputImage1, class TInputImage2 >
void
MeanAbsoluteDistanceImageFilter< TInputImage1, TInputImage2 >
::GenerateInputRequestedRegion()
{

  this->Superclass::GenerateInputRequestedRegion();

  // this filter requires:
  // - the largeset possible region of the first image
  // - the corresponding region of the second image
  if ( this->GetSegmentation() )
    {

    SegmentationImagePointer segmentation =
      const_cast< SegmentationImageType * >( this->GetSegmentation() );
    segmentation->SetRequestedRegionToLargestPossibleRegion();

    if ( this->GetGroundTruth() )
      {

      const GroundTruthImagePointer groundTruth =
        const_cast< GroundTruthImageType * >( this->GetGroundTruth() );
      groundTruth->SetRequestedRegion(
        this->GetSegmentation()->GetRequestedRegion() );

      } // GetGroundTruth

    } // GetSegmentation

} // GenerateInputRequestedRegion
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
template< class TInputImage1, class TInputImage2 >
void
MeanAbsoluteDistanceImageFilter< TInputImage1, TInputImage2 >
::EnlargeOutputRequestedRegion( DataObject *data )
{

  this->Superclass::EnlargeOutputRequestedRegion(data);
  data->SetRequestedRegionToLargestPossibleRegion();

} // EnlargeOutputRequestedRegion
//////////////////////////////////////////////////////////////////////////////////////////




//////////////////////////////////////////////////////////////////////////////////////////
template< class TInputImage1, class TInputImage2 >
void
MeanAbsoluteDistanceImageFilter< TInputImage1, TInputImage2 >
::AllocateOutputs()
{

  // Pass the first input through as the output
  const SegmentationImagePointer image =
    const_cast< SegmentationImageType * >( this->GetSegmentation() );

  this->GraftOutput(image);

} // AllocateOutputs
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
template< class TInputImage1, class TInputImage2 >
void
MeanAbsoluteDistanceImageFilter< TInputImage1, TInputImage2 >
::BeforeThreadedGenerateData()
{

  const ThreadIdType numberOfThreads = this->GetNumberOfThreads();

  // Resize the thread temporaries
  this->m_AbsoluteDistanceSums.SetSize( numberOfThreads );
  this->m_CountBorderPixels   .SetSize( numberOfThreads );

  // Initialize the temporaries
  this->m_AbsoluteDistanceSums.Fill( 0 );
  this->m_CountBorderPixels   .Fill( 0 );

    // calculate our levelsets
  typedef MaskToLevelsetFilter< SegmentationImageType, LevelsetImageType >
    SegmentationLevelsetFilterType;
  typedef MaskToLevelsetFilter< GroundTruthImageType, LevelsetImageType >
    GroundTruthLevelsetFilterType;

  const typename SegmentationLevelsetFilterType::Pointer
    segmentationLevelsetFilter = SegmentationLevelsetFilterType::New();
  const typename GroundTruthLevelsetFilterType::Pointer
    groundTruthLevelsetFilter  = GroundTruthLevelsetFilterType ::New();

  if (m_ComputeLevelsets)
    {

    segmentationLevelsetFilter->SetInput( this->GetSegmentation() );
    segmentationLevelsetFilter->SetNormalize        ( false );
    segmentationLevelsetFilter->SetSigmoid          ( false );
    segmentationLevelsetFilter->SetSigmoidWidth     ( 0     );
    segmentationLevelsetFilter->SetSmooth           ( false );
    segmentationLevelsetFilter->SetSmoothingVariance( 0     );

    groundTruthLevelsetFilter->SetInput( this->GetGroundTruth() );
    groundTruthLevelsetFilter->SetNormalize        ( false );
    groundTruthLevelsetFilter->SetSigmoid          ( false );
    groundTruthLevelsetFilter->SetSigmoidWidth     ( 0     );
    groundTruthLevelsetFilter->SetSmooth           ( false );
    groundTruthLevelsetFilter->SetSmoothingVariance( 0     );

    segmentationLevelsetFilter->Update();
    groundTruthLevelsetFilter->Update();

    // set our calculated levelsets
    m_SegmentationLevelset = static_cast< LevelsetImageConstPointer >(
      segmentationLevelsetFilter->GetOutput() );
    m_GroundTruthLevelset  = static_cast< LevelsetImageConstPointer >(
      groundTruthLevelsetFilter ->GetOutput() );
  }

} // BeforeThreadedGenerateData
//////////////////////////////////////////////////////////////////////////////////////////




//////////////////////////////////////////////////////////////////////////////////////////
template< class TInputImage1, class TInputImage2 >
void
MeanAbsoluteDistanceImageFilter< TInputImage1, TInputImage2 >
::AfterThreadedGenerateData()
{

  ThreadIdType i;
  RealType      absoluteDistanceSum = 0;
  SizeValueType countBorderPixels   = 0;

  const ThreadIdType numberOfThreads = this->GetNumberOfThreads();

  // Accumulate counts over all threads
  for ( i = 0; i < numberOfThreads; i++ )
    {
    absoluteDistanceSum += this->m_AbsoluteDistanceSums[i];
    countBorderPixels   += this->m_CountBorderPixels   [i];
    }

  // have we found any border pixels?
  if ( !countBorderPixels )
    {
    this->m_MeanAbsoluteDistance = itk::NumericTraits< RealType >::max();
    return;
    }

  // compute MAD
  this->m_MeanAbsoluteDistance = absoluteDistanceSum / 
    static_cast< RealType >( countBorderPixels );

} // AfterThreadedGenerateData
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
template< class TInputImage1, class TInputImage2 >
void
MeanAbsoluteDistanceImageFilter< TInputImage1, TInputImage2 >
::ThreadedGenerateData(const RegionType & outputRegionForThread,
                       ThreadIdType threadId)
{

  // prepare iterators
  ImageRegionConstIterator< LevelsetImageType > levelsetIter (
    this->m_SegmentationLevelset, outputRegionForThread );
  ImageRegionConstIterator< LevelsetImageType > groundTruthIter (
    this->m_GroundTruthLevelset, outputRegionForThread );

  // support progress methods/callbacks
  ProgressReporter progress( this, threadId,
    outputRegionForThread.GetNumberOfPixels() );

  // do the work
  while ( !levelsetIter.IsAtEnd() )
    {
    const bool onBorder = 
      std::abs( levelsetIter.Get() ) < m_SegmentationBorderThickness;

    if ( onBorder )
      {

      // okay, we're on the border
      // say we've found another border pixel
      this->m_CountBorderPixels[ threadId ]++;

      // okay calculate the distance to the ground truth border
      const RealType distanceToBorder = std::abs( groundTruthIter.Get() );
      this->m_AbsoluteDistanceSums[ threadId ] += distanceToBorder;
      
      } // onBorder

    // move on to the next pixel
    ++levelsetIter;
    ++groundTruthIter;

    progress.CompletedPixel();

    } // not at end

} // ThreadedGenerateData
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
template< class TInputImage1, class TInputImage2 >
void
MeanAbsoluteDistanceImageFilter< TInputImage1, TInputImage2 >
::PrintSelf(std::ostream & os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);

  os << indent << "MeanAbsoluteDistance: " 
    << m_MeanAbsoluteDistance << std::endl;
}
//////////////////////////////////////////////////////////////////////////////////////////



} // end namespace itk


#endif // __itkMeanAbsoluteDistanceImageFilter_hxx
