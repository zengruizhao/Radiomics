


////////////////////////////////////////////////////////////////////////////////////////////////////
// ccipd includes
#include "ccipdN4MRIBiasCorrection.h"
#include "ccipdResizeImage.h"

// itk includes
#include "ccipdDisableWarningsMacro.h"

#include <itkN4BiasFieldCorrectionImageFilter.h> // for actually correcting
#include <itkBinaryThresholdImageFilter.h>       // for the mask
#include <itkAddImageFilter.h>                   // for removing "0"s

#include "ccipdEnableWarningsMacro.h"
////////////////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



////////////////////////////////////////////////////////////////////////////////////////////////////
VolumeImagePointer N4CorrectBiasField(
  const VolumeImageType     * const inputImage,
  const VolumeMaskImageType * const maskImage,
  const InternalPixelType           addValue,
  const unsigned int                resolutions,
  const unsigned int                iterationsPerResolution,
  const unsigned int                histogramBins,
  const bool                        verbose )
{
  
  using std::cout;
  using std::cerr;
  using std::endl;

  // first we must remove low values in the image
  // by adding a constant to the image
  typedef itk::AddImageFilter< VolumeImageType > AddFilterType;
  const AddFilterType::Pointer addFilter = AddFilterType::New();

  addFilter->SetInput1   ( inputImage );
  addFilter->SetConstant2( addValue   );
  if ( verbose ) cout << "Adding constant to image:" << endl;
  addFilter->Update();
  if ( verbose ) cout << "Adding constant to image done." << endl;

  // create the filter
  typedef itk::N4BiasFieldCorrectionImageFilter<
    VolumeImageType,
    VolumeMaskImageType,
    VolumeImageType
  > N4FilterType;

  const N4FilterType::Pointer biasCorrectionFilter = N4FilterType::New();

  // give it the image
  biasCorrectionFilter->SetInput( addFilter->GetOutput() );
  
  // give it some parameters
  
  N4FilterType::VariableSizeArrayType maximumIterations( resolutions );
  maximumIterations.Fill( iterationsPerResolution );
  biasCorrectionFilter->SetNumberOfFittingLevels( resolutions );
  biasCorrectionFilter->SetMaximumNumberOfIterations( maximumIterations );

  biasCorrectionFilter->SetNumberOfHistogramBins( histogramBins );

  VolumeMaskImageConstPointer thresholdedMask;

  if ( maskImage )
    {
    if ( verbose ) cout << "Using mask image = true." << endl;

    // resize the mask
    if ( verbose ) cout << "Resizing mask:" << endl;
    const VolumeMaskImageConstPointer resizedMask(
      ResizeMask( maskImage, inputImage ) );
    if ( verbose ) cout << "Resizing mask done." << endl;

    if ( verbose ) cout << "Thresholding mask:" << endl;

    // we will threshold so any positive
    // mask value will be used
    typedef itk::BinaryThresholdImageFilter<
      VolumeMaskImageType,
      VolumeMaskImageType
    > ThresholdFilterType;

    const ThresholdFilterType::Pointer
      thresholdFilter = ThresholdFilterType::New();

    // give it all the inputs
    thresholdFilter->SetInput( resizedMask );

    const MaskPixelType maskValue = 1;

    // we want the positive values (>= 1) to be "1", or maskValue
    thresholdFilter->SetLowerThreshold( 1 );
    thresholdFilter->SetUpperThreshold(
      itk::NumericTraits< MaskPixelType >::max() );
    thresholdFilter->SetInsideValue   ( maskValue );
    thresholdFilter->SetOutsideValue  ( 0 );

    // threshold
    try
      {
      thresholdFilter->Update();
      biasCorrectionFilter->SetMaskImage(
        thresholdFilter->GetOutput() );
      biasCorrectionFilter->SetMaskLabel( maskValue );

      cout << "Thresholding mask done." << endl;

      } // try
    catch( ... )
      {
      cerr << "Thresholding mask failed. Ignoring mask." << endl;
      }

    } // maskImage
  else
    {
    if ( verbose ) cout << "Using mask image = false." << endl;
    }

  // actually calculate
  if ( verbose ) std::cout << "Correcting bias field:" << endl;

  bool success = false;
  try
    {
    biasCorrectionFilter->Update();
    success = true;
    }
  catch( itk::ExceptionObject & exception )
    {

    if ( verbose )
      {
      cerr << "Bias correction failed." << endl <<
        exception.GetDescription() << endl;
      } // verbose

    success = false;
    }
  catch ( std::exception & exception )
    {

    if ( verbose )
      {
      cerr << "Bias correction failed." << endl <<
        exception.what() << endl;
      } // verbose

    success = false;
    }
  catch( ... )
    {

    if ( verbose )
      {
      cerr << "Bias correction failed." << endl <<
        "Unknown error." << endl;
      } // verbose

    success = false;

    } // catch

  // output
  if ( success )
    {

    // actually calculate
    if ( verbose ) std::cout << "Correcting bias field done." << endl;

    return biasCorrectionFilter->GetOutput();
    }
  else
    {
    // return null on any errors
    return VolumeImagePointer();
    }
  
} // N4CorrectBiasField
////////////////////////////////////////////////////////////////////////////////////////////////////


} // namespace ccipd
