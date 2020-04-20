/*
 * This program tests out ImageGaussianModelEstimator
 */

//////////////////////////////////////////////////////////////////////////////////////////
// ccipd includes
#include "ccipdCore.h"      
#include "ccipdLoadVolume.h"
#include "ccipdFeatureTypes.h"
#include "ccipdFeatureID.h"
#include "ccipdFeatureCollection.h"
#include "ccipdSaveImage.h"
#include "ccipdResizeImage.h"
#include "ccipdTextureFeatureExtractor.h"
#include "ccipdVolumeFeaturesToGaussianModels.h"
#include "ccipdCalculateProbabilityImage.h"

// std includes
#include <fstream>

#include "ccipdDisableWarningsMacro.h"
  // itk includes
  #include <itkVectorImageToProbabilityImage.h>
  #include <itkGaussianMembershipFunction.h>
  #include <itkImage.h>
#include "ccipdEnableWarningsMacro.h"
//////////////////////////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////////////////////////
using std::string;
using std::cerr;
using std::endl;
using std::cout;
/////////////////////////////////////////////////////////////////////////////////////////

int main( int argc, char *argv[] )
{

  if ( argc < 2 )
    return EXIT_FAILURE;

  // going to need to get an image and a mask,
  const string imageFile ( argv[1] ),
		       maskFile  ( argv[2] );

  //get the image
  const ccipd::VolumeImagePointer prostateImage =
    ccipd::LoadVolume( imageFile );
  ccipd::VolumeMaskImagePointer prostateMask = 
    ccipd::LoadVolumeMask( maskFile);
  prostateMask->SetRequestedRegionToLargestPossibleRegion();

  //down sample the image
  const ccipd::VolumeImagePointer downSampleTemplate = ccipd::VolumeImageType::New();

  ccipd::VolumeImageType::SpacingType downSpacing = prostateImage->GetSpacing();
  ccipd::VolumeImageType::SizeType    downSize   = prostateImage->GetBufferedRegion().GetSize();

  for (unsigned int i = 0; i <ccipd::VolumeDimension; i++)
  {
	  downSpacing[i]= downSpacing[i]*10;
	  downSize[i]= downSize[i]/10;
  }

  downSampleTemplate->SetSpacing(downSpacing);
  downSampleTemplate->SetRegions(downSize);
  downSampleTemplate->SetOrigin(prostateImage->GetOrigin());
  downSampleTemplate->Update();


  ccipd::VolumeImagePointer downImage(ccipd::ResizeImage(prostateImage, downSampleTemplate));
  ccipd::VolumeMaskImagePointer downMask(ccipd::ResizeMask(prostateMask, downSampleTemplate));


  ccipd::SaveImage(downImage.GetPointer(),"DownsampledImage.mhd", true);
  ccipd::SaveImage(downMask.GetPointer(), "DownsampledMask.mhd", true);

  ccipd::FeatureIDPointer
    intensityID( ccipd::FeatureID::New( ccipd::TextureFeatureIntensity         ) ),
  //meanID     ( new FeatureID( ccipd::TextureFeatureMean                    ) ),
    stdID      ( ccipd::FeatureID::New( ccipd::TextureFeatureStandardDeviation ) ),
  //nakagamiID ( new FeatureID( ccipd::TextureFeatureNakagami                ) ),
  //sobelXID   ( new FeatureID( ccipd::TextureFeatureSobelX                  ) ),
  //sobelYID   ( new FeatureID( ccipd::TextureFeatureSobelY                  ) ),
    rayleighID( ccipd::FeatureID::New( ccipd::TextureFeatureRayleigh           ) );

  ccipd::FeatureIDCollection ids;
  ids.push_back(intensityID);
  //ids.push_back(intensityID);
  //ids.push_back( nakagamiID );
   ids.push_back( stdID       );
 // ids.push_back(rayleighID );
 // ids.push_back(sobelYID);
  ccipd::FeatureCollectionPointer myCollection( ccipd::FeatureCollection::New() );

  myCollection->SetVolume (  downImage);
  myCollection->SetVerbose( true          );

  // consider making VolumeFeaturesToGaussianModels take in a const VectorImage
  // so this trick isn't necessary; 
  //
  // Rachel's note: I am only doing it this way because the way ITK has the 
  // base class set up these inputs cannot be constant and I would rather not 
  // perform this casting in that function I am calling

  const ccipd::VolumeFeaturesPointer myVectorImage =
    const_cast< ccipd::VolumeFeaturesType * >(
      myCollection->GetFeatures(ids).GetPointer() );
  
  
  ccipd::SaveImage(myVectorImage.GetPointer(),"VectorImage.mhd",true); 

  /* Note that this breaks because ccipd::VolumeFeaturesType::PixelType
   * is not a member of itk::NumericTraits

  typedef itk::SquareImageFilter<
	ccipd::VolumeFeaturesType,
    ccipd::VolumeFeaturesType
  > SquareFeaturesFilterType;

  SquareFilterType::Pointer test = SquareFilterType::New();
  */

 /* typedef itk::SquareImageFilter<
    ccipd::VolumeImageType,
    ccipd::VolumeImageType
  > SquareImageFilterType;

  typedef itk::VectorImageToVectorImageFilter<
	  ccipd::VolumeFeaturesType,
	  ccipd::VolumeFeaturesType,
	  SquareImageFilterType
  > SquareFeaturesFilterType;

  SquareFeaturesFilterType::Pointer featureFilter = SquareFeaturesFilterType::New();
  
  
  SquareImageFilterType::Pointer  squareComponentFilter = SquareImageFilterType::New();
  featureFilter->SetComponentFilter( squareComponentFilter);

  featureFilter->SetInput( myVectorImage);
  ccipd::VolumeImageConstPointer test = featureFilter->GetComponent(1);
  ccipd::SaveImage(test, "testComponent.mhd",true);

  ccipd::VolumeFeaturesPointer vectorTest= featureFilter->GetOutput();  
  ccipd::SaveImage(vectorTest,"testVector.mhd",true);


  typedef itk::MultiResolutionPyramidImageFilter<
	  ccipd::VolumeFeaturesType,
	  ccipd::VolumeFeaturesType
  > VolumeFeaturesPyramidType;

  VolumeFeaturesPyramidType::Pointer pyramid = VolumeFeaturesPyramidType::New();

  pyramid->SetInput(myVectorImage);
  pyramid->SetNumberOfLevels(3);

  pyramid->Update();

  //print out each level of the pyramid
  for( unsigned int i = 0; i<3; i++)
    {
	ccipd::VolumeFeaturesPointer downsampledImage = pyramid->GetOutput(i);
	
    std::stringstream levelStr;
    levelStr << i;
    std::string fileName = levelStr.str(); 
	ccipd::SaveImage(downsampledImage,"test"+fileName+".mhd", true);
    }
	*/
  const ccipd::VolumeFeaturesGuassianFunctionContainer 
  trusContainer= ccipd::VolumeFeaturesToGaussianModels (
    myVectorImage,
    downMask,
    1 );
 
  trusContainer[0]->Print(cout);

  ccipd::VolumeImageConstPointer myProbImage = ccipd::CalculateProbabilityImage(myVectorImage, trusContainer[0]);
  ccipd::SaveImage(myProbImage.GetPointer(), "ProbabilityImage.mhd",true);
 
  return EXIT_SUCCESS;
}
