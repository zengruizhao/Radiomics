


////////////////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdPreprocessGIPL.h"
#include "ccipdPreprocessVolume.h"
#include "ccipdCohenMRIBiasCorrection.h" // for bias field correction
#include "ccipdApplyCylindricalToCartesianTransform.h"
#include "ccipdApplyCartesianToCylindricalTransform.h"

// itk includes
#include "ccipdDisableWarningsMacro.h"
  #include <itkImage.h>
#include "ccipdEnableWarningsMacro.h"
////////////////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



////////////////////////////////////////////////////////////////////////////////////////////////////
TRUSImagePointer PreprocessGIPL(
  const TRUSImageType * const inputImage,
  const InternalPixelType intensityThreshold ,
  const InternalPixelType outputMinimum,
  const InternalPixelType outputMaximum )
{

  // Correct the bias field
  TRUSImagePointer preprocessedImage = CorrectBiasFieldGIPL( inputImage, 5 );

  // Normalize the image -- for GIPL this is just the same as MRI
  preprocessedImage = NormalizeVolume( preprocessedImage );

  // Crop intensities
  preprocessedImage = CropIntensitiesVolume( 
    preprocessedImage, 
    -intensityThreshold, 
     intensityThreshold  );

  // Smooth the image -- for GIPL this is just the same as MRI
  preprocessedImage = ApplyBilateralFilterVolume( preprocessedImage, 0.5, 0.3 );
 
  preprocessedImage = RescaleIntensitiesVolume( 
    preprocessedImage, 
    outputMinimum, 
    outputMaximum );

  return preprocessedImage;

} // PreprocessMRI
////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////
TRUSImagePointer CorrectBiasFieldGIPL(
  const TRUSImageType * const inputImage,
  const float biasCorrectionSigma )
{

  

//////////////////////////////////////////////////////////////////////////////////////////
  //apply the Cylindrical transformation
  TRUSImagePointer templateImage = TRUSImageType::New();

  //get the values for our inputImage
  TRUSImageType::SpacingType imageSpacing = inputImage->GetSpacing();
  TRUSImageType::PointType   imageOrigin  = inputImage->GetOrigin();
  TRUSImageType::SizeType    imageSize    = inputImage->GetLargestPossibleRegion().GetSize();

  //initialize the templateImage parameters
  TRUSImageType::PointType   templateOrigin  = imageOrigin;
  TRUSImageType::SpacingType templateSpacing = imageSpacing; 
  TRUSImageType::SizeType    templateSize    = imageSize;
		
  templateOrigin[0] = 0;
  templateOrigin[1] = -360;
  templateSpacing [1] = .1;
  templateSize[1]= static_cast< TRUSImageType::SizeValueType >( 360/templateSpacing[1] );

  templateImage->SetSpacing(templateSpacing);
  templateImage->SetOrigin(templateOrigin);
  templateImage->SetRegions(templateSize);
  
  TRUSImagePointer CylindricalImage = ApplyCartesianToCylindricalTransform( inputImage, templateImage, true );
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////  
  //apply the bias field correction
  itk::FixedArray<ccipd::InternalPixelType, ccipd::VolumeDimension> sigmaArray;
    sigmaArray[0] = biasCorrectionSigma; // this is in mm
    sigmaArray[1] = 20; // this is in degrees 
    sigmaArray[2] = 1; // this is in mm

  TRUSImagePointer biasCorrectCylindrical = CohenCorrectBiasField(CylindricalImage, sigmaArray);
//////////////////////////////////////////////////////////////////////////////////////////
  


//////////////////////////////////////////////////////////////////////////////////////////
  // transform back to Cartesian Coordinates
  TRUSImagePointer preprocessedImage(ApplyCylindricalToCartesianTransform(biasCorrectCylindrical, inputImage) );
  return preprocessedImage;
}
////////////////////////////////////////////////////////////////////////////////////////////////////



} // namespace
