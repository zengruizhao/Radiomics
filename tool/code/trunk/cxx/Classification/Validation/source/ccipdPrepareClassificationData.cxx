#include "ccipdPrepareClassificationData.h"


//#include "ccipdFileTypes.h"

#include "ccipdSplitParameters.h"


#include "ccipdExtractFeatureImage.hxx"
#include "ccipdFeatureCollection.h"
#include "ccipdFeatureID.h"
#include "ccipdFeatureTypes.h"

#include "ccipdTextureFeatureFirstOrderParameters.h"
#include "ccipdTextureFeatureGaborParameters.h"
#include "ccipdTextureFeatureSobelParameters.h"
#include "ccipdTextureFeatureHaralickParameters.h"
#include "ccipdTextureFeatureMapParametersType.h"  
#include "ccipdTextureFeatureParser.h"

#include "ccipdSplitParameters.h"
#include "ccipdFeatureCollectionTypes.h"

#include "SimpleINI.h"
#include <vector>
#include <stdlib.h>     /* atoi */


namespace ccipd

{

	// AA TODO: TrainingSetFileNames should be turned into a class
	TrainingSetFileNames LoadTrainingDataFileNames(string iniFilePath, bool bSortKeysAlphabetically, bool bSortValuesAlphabetically)
	{
		TrainingSetFileNames paths;
		// Each row will have five string vectors here: 
		// PatientID, InputVolumeT2, InputVolumeADC, InputMaskT2, CancerLabelsT2
		//vector<string> vecPatientIDs, vecVolumeImagesT2Paths, vecVolumeImagesADCPaths, vecMaskImagesPaths, vecLabelImagesPaths;

		CSimpleIniA ini;
		ini.SetMultiKey(true);
		ini.LoadFile(iniFilePath.data());

		CSimpleIniA::TNamesDepend datasetIDs, datasetPathValues; // After the for loop, datasetPathValues will ONLY hold the last datasetID's values
		ini.GetAllKeys("Datasets", datasetIDs);
		
		if(!bSortKeysAlphabetically)
			datasetIDs.sort(CSimpleIni::TNamesDepend::value_type::Entry::LoadOrder());
		else
			datasetIDs.sort(CSimpleIni::TNamesDepend::value_type::Entry::KeyOrder());


		for (CSimpleIniA::TNamesDepend::const_iterator i = datasetIDs.begin(); i != datasetIDs.end(); ++i)
		{
			vector<string> newDatasetFileCollection;
			newDatasetFileCollection.push_back(i->pItem);
			ini.GetAllValues("Datasets", i->pItem, datasetPathValues);
			
			if(!bSortValuesAlphabetically)
			    datasetPathValues.sort(CSimpleIni::TNamesDepend::value_type::Entry::LoadOrder());
		    else
			    datasetPathValues.sort(CSimpleIni::TNamesDepend::value_type::Entry::KeyOrder());
			
			
			for (CSimpleIniA::TNamesDepend::const_iterator j = datasetPathValues.begin(); j != datasetPathValues.end(); ++j)
			{
				newDatasetFileCollection.push_back(j->pItem);
			}
			paths.push_back(newDatasetFileCollection);
		}

		return paths;

	} // LoadTrainingDataFileNames

  FeatureIDCollection LoadFeaturesFromFile(string iniFilePath, string sectionName, bool bSortKeysAlphabetically, bool bSortValuesAlphabetically)
	{
		FeatureIDCollection featureIDs;
		// Each row will have five string vectors here: 
		// PatientID, InputVolumeT2, InputVolumeADC, InputMaskT2, CancerLabelsT2
		//vector<string> vecPatientIDs, vecVolumeImagesT2Paths, vecVolumeImagesADCPaths, vecMaskImagesPaths, vecLabelImagesPaths;

		CSimpleIniA ini;
		ini.SetMultiKey(true);
		ini.LoadFile(iniFilePath.data());
		

		CSimpleIniA::TNamesDepend FeatureNames, featureParams; // After the for loop, datasetPathValues will ONLY hold the last datasetID's values
		ini.GetAllKeys(sectionName.c_str(), FeatureNames);
		
		if(!bSortKeysAlphabetically)
			FeatureNames.sort(CSimpleIni::TNamesDepend::value_type::Entry::LoadOrder());
		else
			FeatureNames.sort(CSimpleIni::TNamesDepend::value_type::Entry::KeyOrder());
		
		for (CSimpleIniA::TNamesDepend::const_iterator i = FeatureNames.begin(); i != FeatureNames.end(); ++i)
		{
			  // Get the description
			  FeatureDescriptionType FeatureDescription = i->pItem/*"Mean"*/;
			  // Get the Parameters
			  ini.GetAllValues(sectionName.c_str(), i->pItem, featureParams);
		  
              if(!bSortValuesAlphabetically)
                  featureParams.sort(CSimpleIni::TNamesDepend::value_type::Entry::LoadOrder());
              else
                  featureParams.sort(CSimpleIni::TNamesDepend::value_type::Entry::KeyOrder());
      
			  FeatureIDPointer pFeatureID;
			  if(FeatureDescription == TEXTURE_FEATURE_INTENSITY)
			  {
				    for (CSimpleIniA::TNamesDepend::const_iterator j = featureParams.begin(); j != featureParams.end(); ++j)
				    {
				       ParameterPointer pFeatureIntensityParameters = TextureFeatureFirstOrderParameters::New();
				       FeatureIDPointer pIntensityFeatureID(FeatureID::New(TextureFeatureIntensity, FeatureDescription, pFeatureIntensityParameters));
				       featureIDs.push_back(pIntensityFeatureID);
				    }
			  }
			  else if(FeatureDescription == TEXTURE_FEATURE_MEAN)
			  {
				    for (CSimpleIniA::TNamesDepend::const_iterator j = featureParams.begin(); j != featureParams.end(); ++j)
				    {
				       TextureFeatureFirstOrderParameters::Pointer pFeatureMeanParameters = TextureFeatureFirstOrderParameters::New(atoi(j->pItem));
				       FeatureIDPointer pMeanFeatureID(FeatureID::New(TextureFeatureMean, FeatureDescription, pFeatureMeanParameters));
				       featureIDs.push_back(pMeanFeatureID);
				    }
			  }
        else if(FeatureDescription == TEXTURE_FEATURE_MEDIAN)
        {
            for (CSimpleIniA::TNamesDepend::const_iterator j = featureParams.begin(); j != featureParams.end(); ++j)
            {
               TextureFeatureFirstOrderParameters::Pointer pFeatureMedianParameters = TextureFeatureFirstOrderParameters::New(atoi(j->pItem));
               FeatureIDPointer pMedianFeatureID(FeatureID::New(TextureFeatureMedian, FeatureDescription, pFeatureMedianParameters));
               featureIDs.push_back(pMedianFeatureID);
            }
        }
        else if(FeatureDescription == TEXTURE_FEATURE_GABOR)
        {
          for (CSimpleIniA::TNamesDepend::const_iterator j = featureParams.begin(); j != featureParams.end(); ++j)
          {
            // Get The parameters (split the string) // Expecting two parameters
            std::vector<string> params = ccipd::SplitString(j->pItem, ',', true, true);
            TextureFeatureGaborParameters::Pointer pFeatureGaborParameters = TextureFeatureGaborParameters::New((float)(1.0f/((float)atof(params.at(0).c_str()))), (float)atof(params.at(1).c_str()));
            FeatureIDPointer pGaborFeatureID(FeatureID::New(TextureFeatureGabor, FeatureDescription, pFeatureGaborParameters));
            featureIDs.push_back(pGaborFeatureID); 
          }
        }
        else if(FeatureDescription == TEXTURE_FEATURE_HARALICK)
        {
            for (CSimpleIniA::TNamesDepend::const_iterator j = featureParams.begin(); j != featureParams.end(); ++j)
            {
              // Get The parameters (split the string) // Expecting 7 parameters
              std::vector<string> params = ccipd::SplitString(j->pItem, ',', true, true);
              TextureFeatureHaralickParameters::Pointer pFeatureHaralickParameters = 
                TextureFeatureHaralickParameters::New(atoi(params.at(0).c_str()), atoi(params.at(1).c_str()),
                                                      atoi(params.at(2).c_str()), atoi(params.at(3).c_str()),
                                                      atoi(params.at(4).c_str()), atoi(params.at(5).c_str()),
                                                      atoi(params.at(6).c_str()));

              FeatureIDPointer pHaralickFeatureID(FeatureID::New(ccipd::TextureFeatureHaralick, FeatureDescription, pFeatureHaralickParameters));
              featureIDs.push_back(pHaralickFeatureID); 
            }
        }
	      else if(FeatureDescription == TEXTURE_FEATURE_HARALICK_IMC1)
        {
		        for (CSimpleIniA::TNamesDepend::const_iterator j = featureParams.begin(); j != featureParams.end(); ++j)
            {
              // Get The parameters (split the string) // Expecting 7 parameters
              std::vector<string> params = ccipd::SplitString(j->pItem, ',', true, true);
              TextureFeatureHaralickParameters::Pointer pFeatureHaralickParameters = 
                TextureFeatureHaralickParameters::New(atoi(params.at(0).c_str()), atoi(params.at(1).c_str()),
                                                      atoi(params.at(2).c_str()), atoi(params.at(3).c_str()),
                                                      atoi(params.at(4).c_str()), atoi(params.at(5).c_str()),
                                                      atoi(params.at(6).c_str()));

              FeatureIDPointer pHaralickFeatureID(FeatureID::New(ccipd::TextureFeatureHaralickIMC1, FeatureDescription, pFeatureHaralickParameters));
              featureIDs.push_back(pHaralickFeatureID); 
            }
	      }
	      else if(FeatureDescription == TEXTURE_FEATURE_HARALICK_IMC2)
        {
		        for (CSimpleIniA::TNamesDepend::const_iterator j = featureParams.begin(); j != featureParams.end(); ++j)
            {
              // Get The parameters (split the string) // Expecting 7 parameters
              std::vector<string> params = ccipd::SplitString(j->pItem, ',', true, true);
              TextureFeatureHaralickParameters::Pointer pFeatureHaralickParameters = 
                TextureFeatureHaralickParameters::New(atoi(params.at(0).c_str()), atoi(params.at(1).c_str()),
                                                      atoi(params.at(2).c_str()), atoi(params.at(3).c_str()),
                                                      atoi(params.at(4).c_str()), atoi(params.at(5).c_str()),
                                                      atoi(params.at(6).c_str()));

              FeatureIDPointer pHaralickFeatureID(FeatureID::New(ccipd::TextureFeatureHaralickIMC2, FeatureDescription, pFeatureHaralickParameters));
              featureIDs.push_back(pHaralickFeatureID); 
            }
	      }
	      else if(FeatureDescription == TEXTURE_FEATURE_HARALICK_INERTIA)
        {
		        for (CSimpleIniA::TNamesDepend::const_iterator j = featureParams.begin(); j != featureParams.end(); ++j)
            {
              // Get The parameters (split the string) // Expecting 7 parameters
              std::vector<string> params = ccipd::SplitString(j->pItem, ',', true, true);
              TextureFeatureHaralickParameters::Pointer pFeatureHaralickParameters = 
                TextureFeatureHaralickParameters::New(atoi(params.at(0).c_str()), atoi(params.at(1).c_str()),
                                                      atoi(params.at(2).c_str()), atoi(params.at(3).c_str()),
                                                      atoi(params.at(4).c_str()), atoi(params.at(5).c_str()),
                                                      atoi(params.at(6).c_str()));

              FeatureIDPointer pHaralickFeatureID(FeatureID::New(ccipd::TextureFeatureHaralickInertia, FeatureDescription, pFeatureHaralickParameters));
              featureIDs.push_back(pHaralickFeatureID); 
            }
	      }
	      else if(FeatureDescription == TEXTURE_FEATURE_HARALICK_DIFFERENCEENTROPY)
        {
            for (CSimpleIniA::TNamesDepend::const_iterator j = featureParams.begin(); j != featureParams.end(); ++j)
            {
              // Get The parameters (split the string) // Expecting 7 parameters
              std::vector<string> params = ccipd::SplitString(j->pItem, ',', true, true);
              TextureFeatureHaralickParameters::Pointer pFeatureHaralickParameters = 
                TextureFeatureHaralickParameters::New(atoi(params.at(0).c_str()), atoi(params.at(1).c_str()),
                                                      atoi(params.at(2).c_str()), atoi(params.at(3).c_str()),
                                                      atoi(params.at(4).c_str()), atoi(params.at(5).c_str()),
                                                      atoi(params.at(6).c_str()));

              FeatureIDPointer pHaralickFeatureID(FeatureID::New(ccipd::TextureFeatureHaralickDifferenceEntropy, FeatureDescription, pFeatureHaralickParameters));
              featureIDs.push_back(pHaralickFeatureID); 
            }
	      }
        else if(FeatureDescription == TEXTURE_FEATURE_HARALICK_SUMENTROPY)
        {
            for (CSimpleIniA::TNamesDepend::const_iterator j = featureParams.begin(); j != featureParams.end(); ++j)
            {
              // Get The parameters (split the string) // Expecting 7 parameters
              std::vector<string> params = ccipd::SplitString(j->pItem, ',', true, true);
              TextureFeatureHaralickParameters::Pointer pFeatureHaralickParameters = 
                TextureFeatureHaralickParameters::New(atoi(params.at(0).c_str()), atoi(params.at(1).c_str()),
                                                      atoi(params.at(2).c_str()), atoi(params.at(3).c_str()),
                                                      atoi(params.at(4).c_str()), atoi(params.at(5).c_str()),
                                                      atoi(params.at(6).c_str()));

              FeatureIDPointer pHaralickFeatureID(FeatureID::New(ccipd::TextureFeatureHaralickSumEntropy, FeatureDescription, pFeatureHaralickParameters));
              featureIDs.push_back(pHaralickFeatureID); 
            }
	      }
        else if(FeatureDescription == TEXTURE_FEATURE_HARALICK_ENERGY)
        {
            for (CSimpleIniA::TNamesDepend::const_iterator j = featureParams.begin(); j != featureParams.end(); ++j)
            {
              // Get The parameters (split the string) // Expecting 7 parameters
              std::vector<string> params = ccipd::SplitString(j->pItem, ',', true, true);
              TextureFeatureHaralickParameters::Pointer pFeatureHaralickParameters = 
                TextureFeatureHaralickParameters::New(atoi(params.at(0).c_str()), atoi(params.at(1).c_str()),
                                                      atoi(params.at(2).c_str()), atoi(params.at(3).c_str()),
                                                      atoi(params.at(4).c_str()), atoi(params.at(5).c_str()),
                                                      atoi(params.at(6).c_str()));

              FeatureIDPointer pHaralickFeatureID(FeatureID::New(ccipd::TextureFeatureHaralickEnergy, FeatureDescription, pFeatureHaralickParameters));
              featureIDs.push_back(pHaralickFeatureID); 
            }
	      }
        else if(FeatureDescription == TEXTURE_FEATURE_HARALICK_INVDIFFMOMENT)
        {
            for (CSimpleIniA::TNamesDepend::const_iterator j = featureParams.begin(); j != featureParams.end(); ++j)
            {
              // Get The parameters (split the string) // Expecting 7 parameters
              std::vector<string> params = ccipd::SplitString(j->pItem, ',', true, true);
              TextureFeatureHaralickParameters::Pointer pFeatureHaralickParameters = 
                TextureFeatureHaralickParameters::New(atoi(params.at(0).c_str()), atoi(params.at(1).c_str()),
                                                      atoi(params.at(2).c_str()), atoi(params.at(3).c_str()),
                                                      atoi(params.at(4).c_str()), atoi(params.at(5).c_str()),
                                                      atoi(params.at(6).c_str()));

              FeatureIDPointer pHaralickFeatureID(FeatureID::New(ccipd::TextureFeatureHaralickInverseDifferenceMoment, FeatureDescription, pFeatureHaralickParameters));
              featureIDs.push_back(pHaralickFeatureID); 
            }
	      }
        else if(FeatureDescription == TEXTURE_FEATURE_HARALICK_VARIANCE)
        {
            for (CSimpleIniA::TNamesDepend::const_iterator j = featureParams.begin(); j != featureParams.end(); ++j)
            {
              // Get The parameters (split the string) // Expecting 7 parameters
              std::vector<string> params = ccipd::SplitString(j->pItem, ',', true, true);
              TextureFeatureHaralickParameters::Pointer pFeatureHaralickParameters = 
                TextureFeatureHaralickParameters::New(atoi(params.at(0).c_str()), atoi(params.at(1).c_str()),
                                                      atoi(params.at(2).c_str()), atoi(params.at(3).c_str()),
                                                      atoi(params.at(4).c_str()), atoi(params.at(5).c_str()),
                                                      atoi(params.at(6).c_str()));

              FeatureIDPointer pHaralickFeatureID(FeatureID::New(ccipd::TextureFeatureHaralickVariance, FeatureDescription, pFeatureHaralickParameters));
              featureIDs.push_back(pHaralickFeatureID); 
            }
	      }
        else if(FeatureDescription == TEXTURE_FEATURE_HARALICK_CLUSTERSHADE)
        {
            for (CSimpleIniA::TNamesDepend::const_iterator j = featureParams.begin(); j != featureParams.end(); ++j)
            {
              // Get The parameters (split the string) // Expecting 7 parameters
              std::vector<string> params = ccipd::SplitString(j->pItem, ',', true, true);
              TextureFeatureHaralickParameters::Pointer pFeatureHaralickParameters = 
                TextureFeatureHaralickParameters::New(atoi(params.at(0).c_str()), atoi(params.at(1).c_str()),
                                                      atoi(params.at(2).c_str()), atoi(params.at(3).c_str()),
                                                      atoi(params.at(4).c_str()), atoi(params.at(5).c_str()),
                                                      atoi(params.at(6).c_str()));

              FeatureIDPointer pHaralickFeatureID(FeatureID::New(ccipd::TextureFeatureHaralickClusterShade, FeatureDescription, pFeatureHaralickParameters));
              featureIDs.push_back(pHaralickFeatureID); 
            }
	      }
        else if(FeatureDescription == TEXTURE_FEATURE_CORRELATION)
        {
            for (CSimpleIniA::TNamesDepend::const_iterator j = featureParams.begin(); j != featureParams.end(); ++j)
            {
              // Get The parameters (split the string) // Expecting 7 parameters
              std::vector<string> params = ccipd::SplitString(j->pItem, ',', true, true);
              TextureFeatureHaralickParameters::Pointer pFeatureHaralickParameters = 
                TextureFeatureHaralickParameters::New(atoi(params.at(0).c_str()), atoi(params.at(1).c_str()),
                                                      atoi(params.at(2).c_str()), atoi(params.at(3).c_str()),
                                                      atoi(params.at(4).c_str()), atoi(params.at(5).c_str()),
                                                      atoi(params.at(6).c_str()));

              FeatureIDPointer pHaralickFeatureID(FeatureID::New(ccipd::TextureFeatureCorrelation, FeatureDescription, pFeatureHaralickParameters));
              featureIDs.push_back(pHaralickFeatureID); 
            }
	      }
        else if(FeatureDescription == TEXTURE_FEATURE_SOBEL)
        {
            for (CSimpleIniA::TNamesDepend::const_iterator j = featureParams.begin(); j != featureParams.end(); ++j)
            {
              // Get The parameters (split the string) // Expecting two integer parameters ()
              std::vector<string> params = ccipd::SplitString(j->pItem, ',', true, true);
              TextureFeatureSobelParameters::Pointer pFeatureSobelParameters = TextureFeatureSobelParameters::New(atoi(params.at(0).c_str()), atoi(params.at(1).c_str()));
              FeatureIDPointer pSobelFeatureID(FeatureID::New(TextureFeatureSobel, FeatureDescription, pFeatureSobelParameters));
              featureIDs.push_back(pSobelFeatureID); 
            }
        }
        else if(FeatureDescription == TEXTURE_FEATURE_STDEV)
        {
            for (CSimpleIniA::TNamesDepend::const_iterator j = featureParams.begin(); j != featureParams.end(); ++j)
            {
               TextureFeatureFirstOrderParameters::Pointer pFeatureStdDevParameters = TextureFeatureFirstOrderParameters::New(atoi(j->pItem));
               FeatureIDPointer pStdDevFeatureID(FeatureID::New(TextureFeatureStandardDeviation, FeatureDescription, pFeatureStdDevParameters));
               featureIDs.push_back(pStdDevFeatureID);
            }
        }
        else if(FeatureDescription == TEXTURE_FEATURE_RANGE)
        {
            for (CSimpleIniA::TNamesDepend::const_iterator j = featureParams.begin(); j != featureParams.end(); ++j)
            {
               TextureFeatureFirstOrderParameters::Pointer pFeatureRangeParameters = TextureFeatureFirstOrderParameters::New(atoi(j->pItem));
               FeatureIDPointer pRangeFeatureID(FeatureID::New(TextureFeatureRange, FeatureDescription, pFeatureRangeParameters));
               featureIDs.push_back(pRangeFeatureID);
            }
        }
        else if(FeatureDescription == TEXTURE_FEATURE_GRADMAGNITUDE)
        {
          for (CSimpleIniA::TNamesDepend::const_iterator j = featureParams.begin(); j != featureParams.end(); ++j)
          {
             TextureFeatureFirstOrderParameters::Pointer pFeatureGradMagParameters = TextureFeatureFirstOrderParameters::New();
             FeatureIDPointer pGradMagFeatureID(FeatureID::New(TextureFeatureGradientMagnitude, FeatureDescription, pFeatureGradMagParameters));
             featureIDs.push_back(pGradMagFeatureID);
          }
        }
        else // Invalid Name
        {
          cout << "\nWarning: Invalid or not supported Feature " << FeatureDescription << " will be ignored ...\n";
        }
		} // for

		return featureIDs;

	} // LoadTrainingDataFileNames
}