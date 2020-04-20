#include "ccipdPrepareClassificationData.h"
#include "ccipdLoadImage.h"
#include "ccipdCohenMRIBiasCorrection.h"
#include "ccipdVolumeTypes.h"
#include "ccipdFileTypes.h"
#include "ccipdLoadImage.h"
#include "ccipdPreprocessT2.h"
#include "ccipdFeatureCollectionTypes.h"
#include "ccipdFeatureCollection.h"
#include "ccipdResizeImage.h"
#include "ccipdMaskPixelValue.h"


#include "ccipdDisableWarningsMacro.h"
    #include <itkComposeImageFilter.h>
    #include <iostream>
#include "SimpleINI.h"
#include "ccipdEnableWarningsMacro.h"

using namespace ccipd;
using namespace std;

int main(int argc, char* argv[])
{
  // First  argument is the executable name
  // Second argument argv[1] is the full path of the input ini file
  // Third  argument argv[2] is the full path of the output folder
  
      if(argc < 3)
      {
        std::cerr << "Usage of : " << argv[0] << " requires the following parameters: "<< std::endl;
        std::cerr << "argv[1]: Name/Path of the Input INI File, \n"  ;
        std::cerr << "argv[2]: Name/Path of the Output Directory. \n";
        return EXIT_FAILURE;
      }

     // 1. Get the INI file path
     string iniFilePath  = argv[1];
     string outputFolder = argv[2];


      //iniFilePath = "J:/Data/Alpha3T/Results/15778/Training.ini";
      //outputFolder = "J:/Data/Alpha3T/Results/15778"; 
	    
     TrainingSetFileNames files = LoadTrainingDataFileNames(iniFilePath);
     cout << "\n 1. Done Reading INI File " << iniFilePath << "\n"; 

	// 2. Load All Volumes Available
	// Function: LoadTrainingSet(TrainingSetFileNames files)
	VolumeImagesContainer      vecVolumeImagesT2, vecVolumeImagesADC;
	VolumeMaskImagesContainer  vecMaskImages, vecLabelImages;
	for (unsigned int i = 0; i < files.size(); i++)
	{
        //cout << "\n Started Loading Image(s): " << files.at(i).at(1) << "\n";
		vecVolumeImagesT2 .push_back(ccipd::LoadITKImage<VolumeImageType    >(files.at(i).at(1)));
		vecVolumeImagesADC.push_back(ccipd::LoadITKImage<VolumeImageType    >(files.at(i).at(2)));
		vecMaskImages     .push_back(ccipd::LoadITKImage<VolumeMaskImageType>(files.at(i).at(3)));
		vecLabelImages    .push_back(ccipd::LoadITKImage<VolumeMaskImageType>(files.at(i).at(4)));
    }
  cout << "\n 2. Done Loading Image(s): " << files.size() << "\n";

  
  // 3. Force Masks to be binary
  for (unsigned int i = 0; i < files.size(); i++)
	{
    vecMaskImages.at(i) = MaskPixelValue<MaskPixelType, VolumeDimension>(vecMaskImages.at(i), 0, 0, 1);
    vecLabelImages.at(i) =  MaskPixelValue<MaskPixelType, VolumeDimension>(vecLabelImages.at(i), 1, 1, 0);
  }

	// 3. Load the Standardization Reference Image/Histogram  
	CSimpleIniA ini;
	ini.LoadFile(iniFilePath.data());
	std::string refVolumeImageFilePath(ini.GetValue("Standardization", "refVolumeImage", ""));
	VolumeImagePointer refVolumeImage = LoadITKImage<VolumeImageType>(refVolumeImageFilePath);
  cout << "\n 3. Done Loading Ref Volume " << iniFilePath << "\n"; 

 
	// 4. Preprocess All loaded volumes  a. Bias Field, b. Crop, c. Standardize
	for (unsigned int i = 0; i < files.size(); i++)
	{
    // Resize All to the mask, we dont need to apply this to the T2 as it is done inside the preprocessT2 function
    vecVolumeImagesADC.at(i) = ResizeImage(vecVolumeImagesADC.at(i), vecMaskImages.at(i));
    vecLabelImages.at(i) = ResizeMask(vecLabelImages.at(i), vecMaskImages.at(i));


    VolumeRegionType boundingBox = 
		PreprocessT2<VolumeImageType, VolumeMaskImageType>(vecVolumeImagesT2.at(i), vecVolumeImagesT2.at(i), 
                                                       refVolumeImage, vecMaskImages.at(i), true, 100, 25, 
                                                       true, 5.0f, false, true, 6.0f);
    // Input T2 image and mask were cropped, so we need to crop the remaining inputs
		vecVolumeImagesADC.at(i) = CropVolume(vecVolumeImagesADC.at(i), boundingBox);

    vecLabelImages.at(i) = CropMask(vecLabelImages.at(i), boundingBox);
  }

  cout << "\n 4. Done Preprocessing \n"; 
	
  // 5. Load a set of provisional features
  FeatureIDCollection FeatureIDs = LoadFeaturesFromFile(iniFilePath);
  cout << "\n 5. Done Loading Feature IDs: " << FeatureIDs.size() << " Features were loaded. \n";

 
  // 6. Extract Features
  VolumeFeaturesConstImagesContainer vecVolumeFeaturesT2, vecVolumeFeaturesADC;
  FeatureCollectionPointer pFeatureCollection = ccipd::FeatureCollection::New();
  for (unsigned int i = 0; i < files.size(); i++)
	{
      pFeatureCollection->SetVolume(vecVolumeImagesT2.at(i));
      VolumeFeaturesConstPointer pFeatureVectorImage = VolumeFeaturesConstPointer(pFeatureCollection->GetFeatures(FeatureIDs));
      vecVolumeFeaturesT2.push_back(pFeatureVectorImage);
  
      pFeatureCollection->SetVolume(vecVolumeImagesADC.at(i));
      pFeatureVectorImage = VolumeFeaturesConstPointer(pFeatureCollection->GetFeatures(FeatureIDs));
      vecVolumeFeaturesADC.push_back(pFeatureVectorImage);

      cout << "\n Features Extraction for Study:" << files.at(i).at(0) << " was done ...\n" ;
  }
  
  cout << "\n 6. Done Ectracting Features ... " << "\n";

  
  // 7. Prepare for Classification  a. segment (load prostate masks), b. reshape to matrix, c.whiten data
  EigenMatrixTypeF featuresMatrixT2(0, 0), featuresMatrixADC(0, 0), labelsMatrix(0, 0);
  for (unsigned int i = 0; i < files.size(); i++)
	{
    EigenMatrixTypeF tempfeaturesMatrixT2 = 
    PrepareForClassification<VolumeVectorType, VolumeMaskImageType, float, -1, -1, 1>(
      const_cast<VolumeVectorType*>(vecVolumeFeaturesT2.at(i).GetPointer()), vecMaskImages.at(i));
    featuresMatrixT2.resize(featuresMatrixT2.rows() + tempfeaturesMatrixT2.rows(), tempfeaturesMatrixT2.cols());
    featuresMatrixT2.bottomRows(tempfeaturesMatrixT2.rows()) = tempfeaturesMatrixT2;

    EigenMatrixTypeF tempfeaturesMatrixADC = 
    PrepareForClassification<VolumeVectorType, VolumeMaskImageType, float, -1, -1, 1>(
      const_cast<VolumeVectorType*>(vecVolumeFeaturesADC.at(i).GetPointer()), vecMaskImages.at(i));
    featuresMatrixADC.resize(featuresMatrixADC.rows() + tempfeaturesMatrixADC.rows(), tempfeaturesMatrixADC.cols());
    featuresMatrixADC.bottomRows(tempfeaturesMatrixADC.rows()) = tempfeaturesMatrixADC;


    // Convert the label image to a vector image // Should be moved to some separate function
    typedef itk::ComposeImageFilter<VolumeMaskImageType>   ScalarImageToVectorImageFilterType;
    ScalarImageToVectorImageFilterType::Pointer imageToVectorImageFilter = ScalarImageToVectorImageFilterType::New();
    imageToVectorImageFilter->SetInput(0, vecLabelImages.at(i));
    imageToVectorImageFilter->Update();
    
    EigenMatrixTypeF templabelsMatrix = 
    ReshapeImage<MaskVectorType, VolumeMaskImageType, float, -1, -1, 1>(
      const_cast<MaskVectorType*>(/*vecLabelImages.at(i)*/imageToVectorImageFilter->GetOutput()), vecMaskImages.at(i));
    labelsMatrix.resize(labelsMatrix.rows() + templabelsMatrix.rows(), templabelsMatrix.cols());
    labelsMatrix.bottomRows(templabelsMatrix.rows()) = templabelsMatrix;

	}
  cout << "\n 7. Done Reforming Matrices ...\n" ;

  
	// 8. Save reshaped matrices for matlab
  MatrixUtils::WriteMatrixToFile(featuresMatrixT2 , outputFolder + "/featuresMatrixT2.txt" );
  MatrixUtils::WriteMatrixToFile(featuresMatrixADC, outputFolder + "/featuresMatrixADC.txt");
  MatrixUtils::WriteMatrixToFile(labelsMatrix     , outputFolder + "/labelsMatrix.txt"     );
  cout << "\n 8. Done Saving Started ...\n" ;

	// 9. Train

	// 10. Classify

	// 11. Validate




	return EXIT_SUCCESS;
}