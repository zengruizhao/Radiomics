#ifndef __itkMaskToLevelsetFilter_hxx
#define __itkMaskToLevelsetFilter_hxx



////////////////////////////////////////////////////////////////////////////////////////////////////
// our own itk includes
#include "itkMaskToLevelsetFilter.h"

#include "ccipdDisableWarningsMacro.h"
  // itk includes
  #include <itkSignedMaurerDistanceMapImageFilter.h> // for calculated signed distances
  #include <itkSigmoidImageFilter.h>                 // for computing a sigmoid levelset
  #include <itkStatisticsImageFilter.h>              // for calculating the variance of the levelset
  #include <itkShiftScaleImageFilter.h>              // for normalizing the levelset
  #include <itkDiscreteGaussianImageFilter.h>        // for smoothing the levelset
#include "ccipdEnableWarningsMacro.h"
////////////////////////////////////////////////////////////////////////////////////////////////////



namespace itk
{



////////////////////////////////////////////////////////////////////////////////////////////////////
/*
 * Constructor (called when the class is created).
 */
template < class TMaskImage, class TLevelsetImage >
MaskToLevelsetFilter < TMaskImage, TLevelsetImage >
::MaskToLevelsetFilter() :
m_Sigmoid  ( false ), // by default just use signed distance
m_Normalize( true  ), // by default, normalize the levelset
m_Smooth   ( true  ), // by default, smooth the levelset
m_SmoothingVariance( 4 ),
m_SigmoidWidth     ( 4 )
{

  // we need at least one feature to reconstruct
  this->SetNumberOfRequiredInputs( 1 );

  // we will have one output
  this->SetNumberOfRequiredOutputs( 1 );
  this->SetNthOutput( 0, this->MakeOutput( 0 ) );

} // constructor
////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////
/*
 * This is where the processing actually goes
 */
template            < class TMaskImage, class TLevelsetImage >
void MaskToLevelsetFilter < TMaskImage,       TLevelsetImage >
::GenerateData()
{

  // make sure we have an input
  const auto mask = this->GetInput();
  if ( !mask )
    itkExceptionMacro( << "You must supply an image mask." );

  // prepare our final output filter
  typename itk::ImageSource< LevelsetImageType >::Pointer
    lastFilter;
  PipelineOutput
    distanceOutput, sigmoidOutput, normalizationOutput, smoothingOutput;

  distanceOutput = this->CalculateDistances( mask );
  lastFilter = distanceOutput;

  // optionally use a sigmoid filter
  if ( this->GetSigmoid() )
    {

    sigmoidOutput = this->CalculateSigmoid(
      lastFilter->GetOutput(), this->GetSigmoidWidth() );
    lastFilter = sigmoidOutput;

    } // sigmoid

  // optionally smooth
  if ( this->GetSmooth() )
    {
    
    smoothingOutput = this->SmoothLevelset(
      lastFilter->GetOutput(), this->GetSmoothingVariance() );
    lastFilter = smoothingOutput;
    
    } // smooth
    
  // optionally normalize
  if ( this->GetNormalize() )
    {

    normalizationOutput = this->NormalizeLevelset( lastFilter->GetOutput() );
    lastFilter = normalizationOutput;

    } // normalize
  
  // graft to the last filter in our pipeline?
  lastFilter->GraftOutput( this->GetOutput() );
  lastFilter->Update();
  this->GraftOutput( lastFilter->GetOutput() );

} // GenerateData()
////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////
template < class TMaskImage, class TLevelsetImage > typename
MaskToLevelsetFilter< TMaskImage,  TLevelsetImage >::PipelineOutput
MaskToLevelsetFilter< TMaskImage,  TLevelsetImage >
::CalculateDistances( const MaskImageType * mask )
{

  // prepare our distance map filter
  typedef itk::SignedMaurerDistanceMapImageFilter<
    MaskImageType,
    LevelsetImageType
  > DistanceMapFilter;
  auto distanceFilter = DistanceMapFilter::New();
  
  // set the inputs to the distance filter
  distanceFilter->SetInput          ( mask  );
  distanceFilter->SetSquaredDistance( false ); // don't use signed squared euclidean
  distanceFilter->SetUseImageSpacing( true  ); // calculate the distance in mm (not pixels)

  return static_cast< itk::ImageSource< TLevelsetImage > * >( distanceFilter );

} // CalculateDistances
////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////
template < class TMaskImage, class TLevelsetImage > typename
MaskToLevelsetFilter< TMaskImage,  TLevelsetImage >::PipelineOutput
MaskToLevelsetFilter< TMaskImage,  TLevelsetImage >
::CalculateSigmoid(
  const LevelsetImageType * levelset,
  const SigmoidWidthType    width )
{
  
  typedef itk::SigmoidImageFilter<
    LevelsetImageType, LevelsetImageType > SigmoidFilterType;
  auto sigmoidFilter = SigmoidFilterType::New();

  sigmoidFilter->SetInput( levelset );
  sigmoidFilter->SetOutputMinimum( -1    );
  sigmoidFilter->SetOutputMaximum( +1    );
  sigmoidFilter->SetBeta         ( 0     ); // no shifting the levelset boundary
  sigmoidFilter->SetAlpha        ( width );

  return static_cast< itk::ImageSource< TLevelsetImage > * >( sigmoidFilter );

} // CalculateSigmoid
////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////
template < class TMaskImage, class TLevelsetImage > typename
MaskToLevelsetFilter< TMaskImage,  TLevelsetImage >::PipelineOutput
MaskToLevelsetFilter< TMaskImage,  TLevelsetImage >
::NormalizeLevelset( const LevelsetImageType * levelset )
{

  const auto inputLevelset = levelset;

  // we first calculate the standard deviation (sigma) of the levelset
  typedef itk::StatisticsImageFilter< LevelsetImageType >
    StatisticsFilterType;
  auto statistics = StatisticsFilterType::New();
  statistics->SetInput( inputLevelset );
  statistics->Update();
  const typename LevelsetImageType::PixelType
    sigma = static_cast< typename LevelsetImageType::PixelType >( statistics->GetSigma() ),
    one   = 1,
    scale = one / sigma;

  // Normalize the variance.
  // We don't shift the intensities because
  // we want the levelset to be at 0 still.
  typedef itk::ShiftScaleImageFilter<
    LevelsetImageType, LevelsetImageType > ShiftFilterType;
  auto shiftFilter = ShiftFilterType::New();
  shiftFilter->SetInput( inputLevelset );
  shiftFilter->SetShift( 0 );
  shiftFilter->SetScale( scale );

  return static_cast< itk::ImageSource< TLevelsetImage > * >( shiftFilter );

} //NormalizeLevelset
////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////
template < class TMaskImage, class TLevelsetImage > typename
MaskToLevelsetFilter< TMaskImage,  TLevelsetImage >::PipelineOutput
MaskToLevelsetFilter< TMaskImage,  TLevelsetImage >
::SmoothLevelset(
  const LevelsetImageType   * levelset,
  const SmoothingVarianceType variance )
{

  // create a smoothing filter
  typedef itk::DiscreteGaussianImageFilter<
    LevelsetImageType, LevelsetImageType > SmootherType;
  auto smoother = SmootherType::New();

  // inputs to the smoothing filter
  smoother->SetUseImageSpacing( true     );
  smoother->SetInput          ( levelset );
  smoother->SetVariance       ( variance );
  
  return static_cast< itk::ImageSource< TLevelsetImage > * >( smoother );

} // SmoothLevelset


} // namespace itk



#endif // __itkMaskToLevelsetFilter_hxx
