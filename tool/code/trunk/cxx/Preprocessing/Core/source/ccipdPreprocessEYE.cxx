


////////////////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdPreprocessEYE.h"
#include "ccipdPreprocessVolume.h"  //for intensity normalization
#include "ccipdCohenMRIBiasCorrection.h"       // for bias field correction

// itk includes
#include "ccipdDisableWarningsMacro.h"
  #include <itkImage.h>
#include "ccipdEnableWarningsMacro.h"
////////////////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



////////////////////////////////////////////////////////////////////////////////////////////////////
TRUSImagePointer PreprocessRawEYE(
  const TRUSImageType * const inputImage,
  const InternalPixelType intensityThreshold,
  const InternalPixelType outputMinimum,
  const InternalPixelType outputMaximum )
{

  // Correct the bias field
  TRUSImagePointer preprocessedImage = CorrectAttenuationRawEye( inputImage, 5 );

  // Normalize the image. Put this before bilateral filter so the RangeSigma is correct
  preprocessedImage = NormalizeVolume( preprocessedImage ); 

  // Crop intensities
  preprocessedImage = CropIntensitiesVolume( 
    preprocessedImage,  
    -intensityThreshold,
    intensityThreshold );

  // Smooth the image

  TRUSImageType::SpacingType pixelSpacing = inputImage->GetSpacing();
  preprocessedImage = ApplyBilateralFilterVolume( 
    preprocessedImage,  
    (InternalPixelType) pixelSpacing[0]/10, 
     0.5 );

  preprocessedImage = RescaleIntensitiesVolume( 
    preprocessedImage, 
    outputMinimum, 
    outputMaximum );

  return preprocessedImage;

} // PreprocessMRI
////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////
TRUSImagePointer CorrectAttenuationRawEye(
  const TRUSImageType * const inputImage,
  const float biasCorrectionSigma )
{

  //apply the bias field correction
  itk::FixedArray<ccipd::InternalPixelType, ccipd::VolumeDimension> sigmaArray;
    sigmaArray[0] = biasCorrectionSigma; // this is in mm
    sigmaArray[1] = 0.2; //this is in radians
    sigmaArray[2] = 0.5; // this is in radians

  TRUSImagePointer biasCorrect = CohenCorrectBiasField(inputImage, sigmaArray);

  return biasCorrect;

}
////////////////////////////////////////////////////////////////////////////////////////////////////




} // namespace
