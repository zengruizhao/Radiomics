
#ifndef __ccipdPrepareClassificationData_h
#define __ccipdPrepareClassificationData_h


#include "ccipdValidationTypes.h"
//#include "ccipdMatrixTypes.h"
#include "ccipdMatrixUtils.h"
#include "ccipdFeatureCollectionTypes.h"
#include "ccipdStatistics.h"

#include "ccipdDisableWarningsMacro.h"
	#include <itkImage.h>
	#include <itkVectorImage.h>
	#include <itkImageRegionConstIterator.h>
#include "ccipdEnableWarningsMacro.h"

using namespace std;

namespace ccipd
{
  TrainingSetFileNames LoadTrainingDataFileNames(string iniFilePath, bool bSortKeysAlphabetically = false, bool bSortValuesAlphabetically = false);

  FeatureIDCollection LoadFeaturesFromFile(string iniFilePath, string sectionName = "Features", bool bSortKeysAlphabetically = false, bool bSortValuesAlphabetically = false);

	/*
	This function converts a Vector Image of Features to an Eigen m x n Matrix
	m is the the number of pixels/voxels
	n is the number of features
  It also excludes all the pixels outside the given mask (if not null)
	*/
	template <class TVectorImageType, class TMaskImageType, class ScalarType, int Rows, int Cols, int Options>
	Eigen::Matrix<ScalarType, Rows, Cols, Options, Rows, Cols>
	ReshapeImage(typename TVectorImageType::Pointer pFeaturesImage, typename TMaskImageType::Pointer pMaskImage = NULL)
	{
		if (!pFeaturesImage)
		{
			cerr << "Error(19-13): Features Image is not valid ...";
			return Eigen::Matrix<ScalarType, Rows, Cols, Options, Rows, Cols>();
		}

		// ReForms the features and prepares them for classification - Converts from VolumeImageType to EigenMatrixTypeF 
		// Prepare the data container (m x n matrix, m:the number of pixels in the whole 3D Image, n number of components of each pixel in the 3D Image)
		unsigned int featuresNumber = pFeaturesImage->GetNumberOfComponentsPerPixel();
		typename TVectorImageType::SizeType FeatureImagSize = pFeaturesImage->GetBufferedRegion().GetSize(); // Gets the dimensions of the Feature Image

		unsigned int maskVoxelNum = 1;
				
		if (pMaskImage)
		{
			// Check Sizes
			if (FeatureImagSize != /*MaskImageSize*/ pMaskImage->GetBufferedRegion().GetSize())
			{
				cerr << "Error(38-13): Feature Image Size is not equal to Mask Size ...";
				return Eigen::Matrix<ScalarType, Rows, Cols, Options, Rows, Cols>();
			}

			// Get Non-Zero Voxel Count
			// This is the number of non-zero valued voxels in the given mask
			maskVoxelNum = ccipd::GetPixelCount(pMaskImage.GetPointer());

		}
		else
		{
		    //unsigned int imDim = typename TVectorImageType::ImageDimension;
			for (unsigned int i = 0; i < 3/*imDim*/; i++)
			{
				maskVoxelNum *= (unsigned int)FeatureImagSize[i];
			}
		}

		Eigen::Matrix<ScalarType, Rows, Cols, Options, Rows, Cols> dataToClassify(maskVoxelNum, featuresNumber);

		//float totalVoxelNum = 50.0f / (FeatureImagSize[0] * FeatureImagSize[1] * FeatureImagSize[2]);
		//float progressBarValue = 0.0f;


    //WriteITKImage<VolumeImageType>(  GetChannelByIndex<TVectorImageType, VolumeImageType>(const_cast<TVectorImageType*>(pFeaturesImage.GetPointer()), 48)
    //                               , "Z:/COPY/Copy/Temp/Alpha3T Visit/fast/Feature48_fast.mha");
    //WriteITKImage<VolumeImageType>(  GetChannelByIndex<TVectorImageType, VolumeImageType>(const_cast<TVectorImageType*>(pFeaturesImage.GetPointer()), 49)
    //                               , "Z:/COPY/Copy/Temp/Alpha3T Visit/fast/Feature49_fast.mha");
    //WriteITKImage<TMaskImageType>(  pMaskImage
    //                              , "Z:/COPY/Copy/Temp/Alpha3T Visit/fast/mask_fast.mha");


//////////////////////////
		//itk::ImageRegionConstIteratorWithIndex<TVectorImageType> in(pFeaturesImage, pFeaturesImage->GetLargestPossibleRegion());
		//if (pMaskImage) // This code snippet looks slightly redundant but it is the fastest
		//{
		//	itk::ImageRegionConstIteratorWithIndex<TMaskImageType> ms(pMaskImage , pFeaturesImage->GetLargestPossibleRegion());
		//	unsigned int n = 0;
  //    for (in.GoToBegin(), ms.GoToBegin(); !in.IsAtEnd(); ++in, ++ms)
		//	{
  //      typename TMaskImageType::PixelType maskPixelValue = ms.Get();
		//		if (maskPixelValue > 0) // Check if this pixel is above zero in the mask
		//		{
		//			typename TVectorImageType::PixelType pixelValue = in.Get();
		//			
  //        if(ms.GetIndex() != in.GetIndex())
  //        std::cout << "Mask Index: " << ms.GetIndex() << " Input Index: " << in.GetIndex() << " Status:" << (bool)(ms.GetIndex() == in.GetIndex()) << std::endl;
  //        
  //        for (unsigned int f = 0; f < featuresNumber; f++)
		//			{
		//				dataToClassify(n, f) = pixelValue[f];
		//			} // for
		//			n++;
		//		}
		//	}
		//}
		//else
		//{
  //    unsigned int n = 0;
		//	for(in.GoToBegin(); !in.IsAtEnd(); ++in, n++)
		//	{
		//		typename TVectorImageType::PixelType pixelValue = in.Get();
		//		for (unsigned int f = 0; f < featuresNumber; f++)
		//		{
		//			dataToClassify(n, f) = pixelValue[f];
		//		} // for
		//	}
		//}

    // AA Testing ONLY
		//MatrixUtils::WriteMatrixToFile(dataToClassify, "Z:/COPY/Copy/Temp/Alpha3T Visit/fast/dataToClassifyQ_fast.txt");
/////////////////////////
////////////////////////
////////////////////////

   //itk::Size<3> FeatureImagSize = pFeaturesImage->GetBufferedRegion().GetSize();
   for (unsigned int slice = 0, n = 0; slice < FeatureImagSize[2]; slice++)
   {
     for (unsigned int x = 0; x < FeatureImagSize[0]; x++)
     {
       for (unsigned int y = 0; y < FeatureImagSize[1]; y++)
       {
         // Check if this is not zero in the 
         VolumeFeaturesType::IndexType pixelIndex = {{x, y, slice}};
         // Check if this pixel is above zero in the mask
         
         if(!pMaskImage || pMaskImage->GetPixel(pixelIndex) > 0)
         {
           VolumeFeaturesType::PixelType pixelValue = pFeaturesImage->GetPixel(pixelIndex);
           for (unsigned int f = 0; f < featuresNumber; f++)
           {
             dataToClassify(n, f) = pixelValue[f];
           } // for
           n++;
         } // if
       }
     }
   }
    // AA Testing ONLY
		MatrixUtils::WriteMatrixToFile(dataToClassify, "Z:/COPY/Copy/Temp/Alpha3T Visit/fast/dataToClassifyQ_fast.txt");

		return dataToClassify;

	} // ReshapeImage




	

  


  template <class TVectorImageType, class TMaskImageType, class ScalarType, int Rows, int Cols, int Options>
	Eigen::Matrix<ScalarType, Rows, Cols, Options, Rows, Cols>
	PrepareForClassification(typename TVectorImageType::Pointer pFeaturesImage, typename TMaskImageType::Pointer pMaskImage = NULL)
	{
     Eigen::Matrix<ScalarType, Rows, Cols, Options, Rows, Cols> outputMatrix = 
       ReshapeImage<TVectorImageType, TMaskImageType, ScalarType, Rows, Cols, Options>(pFeaturesImage, pMaskImage);

     return MatrixUtils::WhitenMatrix(outputMatrix);

     }



} // namespace ccipd

#endif // __ccipdPrepareClassificationData_h