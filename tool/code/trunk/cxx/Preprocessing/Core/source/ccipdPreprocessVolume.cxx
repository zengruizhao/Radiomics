


////////////////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdPreprocessVolume.h"

// itk includes
#include "ccipdDisableWarningsMacro.h"
  #include <itkIntensityWindowingImageFilter.h> // for cropping the histogram
  #include <itkBilateralImageFilter.h>          // for smoothing
  #include <itkNormalizeImageFilter.h>          // for normalizing image intensities
  #include <itkRescaleIntensityImageFilter.h>   // to set intensity ranges
#include "ccipdEnableWarningsMacro.h"
////////////////////////////////////////////////////////////////////////////////////////////////////


namespace ccipd
{

////////////////////////////////////////////////////////////////////////////////////////////////////
VolumeImagePointer NormalizeVolume(
  const VolumeImageType * inputImage )
{
  if ( !inputImage )
    return VolumeImagePointer();

  VolumeImagePointer preprocessedImage;

  typedef itk::NormalizeImageFilter<
    VolumeImageType, VolumeImageType > NormalizeFilterType;
  NormalizeFilterType::Pointer normalizer = NormalizeFilterType::New();
  normalizer->SetInput(inputImage);

  normalizer->Update(); 
  preprocessedImage = normalizer->GetOutput();

  return  preprocessedImage;
}
////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////
VolumeImagePointer CropIntensitiesVolume(
  const VolumeImageType * inputImage,
  const InternalPixelType outputMinimum,
  const InternalPixelType outputMaximum )
{
  if ( !inputImage )
    return VolumeImagePointer();

  VolumeImagePointer preprocessedImage;

  const InternalPixelType intensityCrop = 3;
  typedef itk::IntensityWindowingImageFilter<
    VolumeImageType > WindowingFilterType;
  WindowingFilterType::Pointer windowingFilter = WindowingFilterType::New();
  windowingFilter->SetInput( inputImage );
  windowingFilter->SetWindowMaximum( +intensityCrop );
  windowingFilter->SetWindowMinimum( -intensityCrop );
  windowingFilter->SetOutputMaximum( outputMaximum  );
  windowingFilter->SetOutputMinimum( outputMinimum  );
  
  windowingFilter->Update();
  preprocessedImage = windowingFilter->GetOutput();

  return preprocessedImage;

}
////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////
VolumeImagePointer ApplyBilateralFilterVolume(
  const VolumeImageType * inputImage,
  const float domainSigma,
  const float rangeSigma                  )
{
  if ( !inputImage )
    return VolumeImagePointer();

  VolumeImagePointer preprocessedImage;

  // smooth the image
  typedef itk::BilateralImageFilter<
    VolumeImageType, VolumeImageType > SmoothFilterType;
  SmoothFilterType::Pointer smoothFilter = SmoothFilterType::New();
  smoothFilter->SetInput( inputImage );
  smoothFilter->SetAutomaticKernelSize( true );
  smoothFilter->SetDomainSigma( domainSigma ); // in mm
  smoothFilter->SetRangeSigma ( rangeSigma ); // in intensities (which should have a var of 1)

  smoothFilter->Update();
  preprocessedImage = smoothFilter->GetOutput();

  return preprocessedImage;
}
////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////
VolumeImagePointer RescaleIntensitiesVolume(
  const VolumeImageType * inputImage,
  const InternalPixelType minIntensity,
  const InternalPixelType maxIntensity)
{
  if ( !inputImage )
    return VolumeImagePointer();

  VolumeImagePointer preprocessedImage;
  
  typedef itk::RescaleIntensityImageFilter< VolumeImageType, VolumeImageType > RescaleFilterType;
  RescaleFilterType::Pointer rescaleFilter = RescaleFilterType::New();
  rescaleFilter->SetInput( inputImage );
  rescaleFilter->SetOutputMinimum( minIntensity );
  rescaleFilter->SetOutputMaximum( maxIntensity );

  rescaleFilter->Update();
  preprocessedImage = rescaleFilter->GetOutput();

  return preprocessedImage;

}

////////////////////////////////////////////////////////////////////////////////////////////////////



} // namespace
