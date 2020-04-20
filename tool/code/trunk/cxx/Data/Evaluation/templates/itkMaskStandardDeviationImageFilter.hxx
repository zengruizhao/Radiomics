#ifndef __itkMeanAbsoluteDistanceImageFilter_hxx
#define __itkMeanAbsoluteDistanceImageFilter_hxx



/**
 * Modeled after itkMeanAbsoluteDistanceImageFilter
 */



//////////////////////////////////////////////////////////////////////////////////////////
// ccipd includes
//#include "ccipdVolumeTypes.h"

#include "ccipdDisableWarningsMacro.h"
  // itk includes
  #include "itkImage.h"
  #include "itkCastImageFilter.h"
  #include "itkImageMaskSpatialObject.h"
  #include "itkImageRegionConstIterator.h"
  #include "itkZeroFluxNeumannBoundaryCondition.h"
  #include "itkConstNeighborhoodIterator.h"
  #include "itkImageRegionIterator.h"
  #include "itkNeighborhoodAlgorithm.h"
  #include "itkProgressReporter.h"
#include "ccipdEnableWarningsMacro.h"

//////////////////////////////////////////////////////////////////////////////////////////



namespace itk
{



//////////////////////////////////////////////////////////////////////////////////////////
template< class TInputImage1, class TInputImage2 >
MaskStandardDeviationImageFilter< TInputImage1, TInputImage2 >
::MaskStandardDeviationImageFilter():
m_CountMaskPixels          ( 0 )
{

  // this filter requires two input images
  this->SetNumberOfRequiredInputs(2);

  this->m_MaskStandardDeviation = NumericTraits< RealType >::Zero;

}
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
template< class TInputImage1, class TInputImage2 >
void
MaskStandardDeviationImageFilter< TInputImage1, TInputImage2 >
::SetMask(const MaskImageType * image)
{
  this->SetNthInput( 1, const_cast< MaskImageType * >( image ) );
  
  // create a bounding box out of the mask
  typedef itk::ImageMaskSpatialObject< ccipd::VolumeDimension >
    ImageMaskSpatialObject;

  ImageMaskSpatialObject::Pointer maskSO = ImageMaskSpatialObject::New();

  typedef itk::Image<unsigned char, 3>  UnsignedCharImageType;
 
  typedef itk::CastImageFilter< MaskImageType, UnsignedCharImageType > CastFilterType;
  typename CastFilterType::Pointer castFilter = CastFilterType::New();
  castFilter->SetInput( image );
  castFilter->Update();

  maskSO->SetImage ( castFilter->GetOutput() );

  m_MaskBoundingBox  = maskSO->GetAxisAlignedBoundingBoxRegion();

  std::cout << "Bounding Box Region: " << m_MaskBoundingBox << std::endl;
}
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
template< class TInputImage1, class TInputImage2 >
const typename MaskStandardDeviationImageFilter< TInputImage1, TInputImage2 >
::MaskImageType *
MaskStandardDeviationImageFilter< TInputImage1, TInputImage2 >
::GetMask()
{

  return static_cast< const MaskImageType * >
         ( this->ProcessObject::GetInput(1) );

}
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
template< class TInputImage1, class TInputImage2 >
void
MaskStandardDeviationImageFilter< TInputImage1, TInputImage2 >
::GenerateInputRequestedRegion()
{

  this->Superclass::GenerateInputRequestedRegion();

  if ( this->GetDisplacementField() )
    {

    DisplacementFieldPointer displacementField =
      const_cast< DisplacementFieldType * >( this->GetDisplacementField() );
    displacementField->SetRequestedRegion( m_MaskBoundingBox );
    //displacementField->SetRequestedRegionToLargestPossibleRegion();

    if ( this->GetMask() )
      {

      const MaskImagePointer mask =
        const_cast< MaskImageType * >( this->GetMask() );
      mask->SetRequestedRegion(
        this->GetDisplacementField()->GetRequestedRegion() );

      } // GetMask

    } // GetDisplacementField

} // GenerateInputRequestedRegion
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
template< class TInputImage1, class TInputImage2 >
void
MaskStandardDeviationImageFilter< TInputImage1, TInputImage2 >
::EnlargeOutputRequestedRegion( DataObject *data )
{

  this->Superclass::EnlargeOutputRequestedRegion(data);
  data->SetRequestedRegionToLargestPossibleRegion();

} // EnlargeOutputRequestedRegion
//////////////////////////////////////////////////////////////////////////////////////////




//////////////////////////////////////////////////////////////////////////////////////////
//template< class TInputImage1, class TInputImage2 >
//void
//MaskStandardDeviationImageFilter< TInputImage1, TInputImage2 >
//::AllocateOutputs()
//{
//
//  // Pass the first input through as the output
//  const MaskImagePointer image =
//    const_cast< MaskImageType * >( this->GetMask() );
//
//  this->GraftOutput(image);
//
//} // AllocateOutputs
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
template< class TInputImage1, class TInputImage2 >
void
MaskStandardDeviationImageFilter< TInputImage1, TInputImage2 >
::BeforeThreadedGenerateData()
{

  typename MaskImageType::IndexType start;
  start = this->GetMask()->GetLargestPossibleRegion().GetIndex();
 
  typename MaskImageType::SizeType size;
  size = this->GetMask()->GetLargestPossibleRegion().GetSize();
 
  typename MaskImageType::RegionType region( start, size );
 
  m_Output = MaskImageType::New();
  m_Output->SetRegions( region );
  m_Output->Allocate();
  m_Output->FillBuffer( 0 );

} // BeforeThreadedGenerateData
//////////////////////////////////////////////////////////////////////////////////////////




//////////////////////////////////////////////////////////////////////////////////////////
template< class TInputImage1, class TInputImage2 >
void
MaskStandardDeviationImageFilter< TInputImage1, TInputImage2 >
::AfterThreadedGenerateData()
{
} // AfterThreadedGenerateData
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
template< class TInputImage1, class TInputImage2 >
void
MaskStandardDeviationImageFilter< TInputImage1, TInputImage2 >
::ThreadedGenerateData(const RegionType & outputRegionForThread,
                       ThreadIdType threadId)
{

  std::cout << threadId << std::endl;
  std::cout << outputRegionForThread << std::endl;

  itk::ImageRegionIterator< MaskImageType >               outputIter;

  typename itk::ConstNeighborhoodIterator< DisplacementFieldType >::RadiusType  dispFieldRadius;
    dispFieldRadius.Fill( this->GetRadius() );
  
  itk::ConstNeighborhoodIterator< DisplacementFieldType >              dispFieldNeighborhoodIter(
    dispFieldRadius, this->GetDisplacementField(), m_MaskBoundingBox );

  typename itk::ConstNeighborhoodIterator< MaskImageType >::RadiusType          maskRadius;
    maskRadius.Fill( this->GetRadius() );
  
  itk::ConstNeighborhoodIterator< MaskImageType >                      maskNeighborhoodIter(
    maskRadius, this->GetMask(), m_MaskBoundingBox );

  // support progress methods/callbacks
  itk::ProgressReporter progress( this, threadId,
    outputRegionForThread.GetNumberOfPixels() );

  RealType value;
  RealType sum;
  RealType sumOfSquares;
  RealType var;
  RealType num;

  unsigned int pixelIndex;
  unsigned int vectorIndex;

  //for ( dispFieldNeighborhoodIter.GoToBegin();
  //      !dispFieldNeighborhoodIter.IsAtEnd();
  //      ++dispFieldNeighborhoodIter )
  //  {
  value = 0; sum = 0; sumOfSquares = 0; var = 0; num = 0;
  //  }

  num = static_cast< RealType >( dispFieldNeighborhoodIter.Size() );

  outputIter = itk::ImageRegionIterator< MaskImageType >(m_Output, m_MaskBoundingBox);

  dispFieldNeighborhoodIter.GoToBegin();
  maskNeighborhoodIter.GoToBegin();
  outputIter.GoToBegin();

  while ( !dispFieldNeighborhoodIter.IsAtEnd() )
    {
    if ( maskNeighborhoodIter.GetCenterPixel() != 0 )
      this->m_CountMaskPixels += 1;
    var = NumericTraits< RealType >::Zero;
    for ( vectorIndex = 0; vectorIndex < ImageDimension; ++vectorIndex )
      {
      sum = NumericTraits< RealType >::Zero;
      sumOfSquares = NumericTraits< RealType >::Zero;       
      for ( pixelIndex = 0; pixelIndex < num; ++pixelIndex )
        {
        value = static_cast< RealType >( dispFieldNeighborhoodIter.GetPixel( pixelIndex )[ vectorIndex ] );
        sum += value;
        sumOfSquares += ( value * value );           
        }
      // calculate the standard deviation value
      var += ( sumOfSquares - ( sum * sum / num ) ) / ( num - 1.0 );
      }
    var /= ImageDimension;
    if ( maskNeighborhoodIter.GetCenterPixel() != 0 )
      {
      outputIter.Set( static_cast< MaskImagePixelType >( vcl_sqrt(var) ) );
      this->m_MaskStandardDeviation += static_cast< MaskImagePixelType >( vcl_sqrt(var) ) / static_cast< MaskImagePixelType >( m_CountMaskPixels );
      }

    ++dispFieldNeighborhoodIter;
    ++maskNeighborhoodIter;
    ++outputIter;
    progress.CompletedPixel();
    }

} // ThreadedGenerateData
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
template< class TInputImage1, class TInputImage2 >
void
MaskStandardDeviationImageFilter< TInputImage1, TInputImage2 >
::PrintSelf(std::ostream & os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);

  os << indent << "MaskStandardDeviation: " 
    << m_MaskStandardDeviation << std::endl;
}
//////////////////////////////////////////////////////////////////////////////////////////



} // end namespace itk


#endif // __itkMaskStandardDeviationImageFilter_hxx
