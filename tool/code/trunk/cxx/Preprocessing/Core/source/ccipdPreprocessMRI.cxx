



////////////////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdPreprocessMRI.h"
#include "ccipdPreprocessVolume.h"
#include "ccipdCohenMRIBiasCorrection.h"       // for bias field correction

// itk includes
#include "ccipdDisableWarningsMacro.h"
  #include <itkImage.h>
#include "ccipdEnableWarningsMacro.h"
////////////////////////////////////////////////////////////////////////////////////////////////////


namespace ccipd
{

////////////////////////////////////////////////////////////////////////////////////////////////////
MRIImagePointer PreprocessMRI(
  const MRIImageType * inputImage,
  const float biasSigma,
  const InternalPixelType intensityThreshold )
{
  
  // first, correct the bias field
  MRIImagePointer preprocessedImage;

  if (biasSigma !=0)
    preprocessedImage = CohenCorrectBiasField( inputImage, biasSigma );


  // then, normalize the image.
  if (biasSigma !=0)
     preprocessedImage = NormalizeVolume( preprocessedImage );
  else
     preprocessedImage = NormalizeVolume( inputImage );
  
  // the mean is 0 and variance is 1.  Crop intensities
  preprocessedImage = CropIntensitiesVolume( 
    preprocessedImage,
    -intensityThreshold, 
    intensityThreshold );

  // smooth the image
  preprocessedImage = ApplyBilateralFilterMRI( preprocessedImage );
 
  return preprocessedImage;

} // PreprocessMRI
////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////
MRIImagePointer ApplyBilateralFilterMRI(
  const MRIImageType * inputImage )
{

 return ApplyBilateralFilterVolume( inputImage, 0.3, 0.5 );

}
////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////
MRIImagePointer RescaleIntensitiesMRI(
  const MRIImageType * inputImage)
{

return RescaleIntensitiesVolume( inputImage, 0, 255 );

}

////////////////////////////////////////////////////////////////////////////////////////////////////



} // namespace
