/*=========================================================================
 *
 *  Copyright CCIPD
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *
 *=========================================================================*/
/*=========================================================================
 *
 *  Portions of this file are subject to the ITK Toolkit Version 4 copyright.
 *
 *  Copyright (c) Ahmad Algohary
 *
 *  For complete copyright, license and disclaimer of warranty information
 *  please refer to the NOTICE file at the top of the ITK source tree.
 *
 *=========================================================================*/
#ifndef __itkHaralickIMC2Filter_hxx
#define __itkHaralickIMC2Filter_hxx
#include "itkHaralickIMC2Filter.h"
#include <itkNeighborhoodAlgorithm.h>
#include <itkConstantBoundaryCondition.h>   
#include <itkScalarImageToCooccurrenceMatrixFilter.h>
#include <itkHistogramToTextureFeaturesFilter.h>
#include <itkStatisticsImageFilter.h>
#include "itkHistogramToHaralickFeaturesFilter.h"

namespace itk
{
template< class TInputImage, class TOutputImage >
HaralickIMC2Filter< TInputImage, TOutputImage >
::HaralickIMC2Filter()
{
  // Default values of member variables
//     m_bFeaturesCalculated = false;
  m_NumberOfBinsPerAxis = 16;
  this->SetRadius(1); // m_Radius = {{1, 1, 1}}
  m_Offset.Fill(0);
  m_Offset[0] = 1; // m_Offset = {{1, 0, 0}};

//  m_OffsetVector.push_back(m_Offset);

  this->ProcessObject::SetNumberOfRequiredInputs(1);
//this->ProcessObject::SetNumberOfRequiredOutputsputs(8);

  this->ProcessObject::SetNthOutput(0, this->MakeOutput(0));

} // end constructor

template< class TInputImage, class TOutputImage >
void
HaralickIMC2Filter< TInputImage, TOutputImage >
::ThreadedGenerateData(const OutputImageRegionType & outputRegionForThread, ThreadIdType threadId)
{
   // Retrieve Input
  typename  InputImageType::ConstPointer input  = this->GetInput();
  // Retrieve Output
  typename OutputImageType::Pointer pIMC2OutputFeatureImage                 = this->GetOutput(0);



  // support progress methods/callbacks
  ProgressReporter progress( this, threadId, outputRegionForThread.GetNumberOfPixels());

  // 
  ConstantBoundaryCondition<InputImageType,  OutputImageType> nbc;
  ConstNeighborhoodIterator< InputImageType > inputIt;
  ImageRegionIterator<OutputImageType>  IMC2It  ;

  // Find the data-set boundary "faces"
  typename NeighborhoodAlgorithm::ImageBoundaryFacesCalculator< InputImageType >::FaceListType faceList;
  NeighborhoodAlgorithm::ImageBoundaryFacesCalculator< InputImageType > bC;
  faceList = bC( input, outputRegionForThread, this->GetRadius());

  typename NeighborhoodAlgorithm::ImageBoundaryFacesCalculator< InputImageType >::FaceListType::iterator faceListIt;

  typedef itk::ImageRegionIterator< InputImageType > InputImageIterator;
  InputImagePointerType pNeighborhoodImage = InputImageType::New();

  // Process each of the boundary faces.  These are N-d regions which border the edge of the buffer.
  for ( faceListIt = faceList.begin(); faceListIt != faceList.end(); ++faceListIt )
  {
    inputIt = ConstNeighborhoodIterator< InputImageType >(this->GetRadius(), input, *faceListIt);
    auto neighborhoodSize = inputIt.Size();
    IMC2It                   = ImageRegionIterator< OutputImageType >(pIMC2OutputFeatureImage               , *faceListIt);

    inputIt.OverrideBoundaryCondition(&nbc);
    inputIt.GoToBegin();

    while (!inputIt.IsAtEnd())
    {
      pNeighborhoodImage->SetRegions(inputIt.GetBoundingBoxAsImageRegion());
      pNeighborhoodImage->Allocate();

      // setup the iterator
      InputImageIterator imageIt( pNeighborhoodImage, pNeighborhoodImage->GetBufferedRegion());

      imageIt.GoToBegin();
      for (unsigned int x = 0; x < neighborhoodSize; x++, ++imageIt )
      {
        imageIt.Set(inputIt.GetPixel(x));
      }

      // Initiating the GLCM generator
      typedef itk::Statistics::ScalarImageToCooccurrenceMatrixFilter<InputImageType> InputImage2GLCMType;
      typedef typename InputImage2GLCMType::Pointer InputImage2GLCMPointerType;
      typedef typename InputImage2GLCMType::HistogramType HistogramType;
      typedef typename InputImage2GLCMType::OffsetVector InputImage2GLCMOffsetVectorType;
      
      
      InputImage2GLCMPointerType       m_glcmGenerator;
      m_glcmGenerator = InputImage2GLCMType::New();
      m_glcmGenerator->ResetPipeline();
      m_glcmGenerator->SetInput(pNeighborhoodImage);
/////////////////////////////////////////////////////////////////////////////////////////
      //////////////////////////////////////////////////////////////////////////////////
      typename InputImage2GLCMOffsetVectorType::Pointer offsetV = InputImage2GLCMOffsetVectorType::New();
      
      InputImageOffsetType offset1 = {{0, 0, 1}}; offsetV->push_back(offset1);
      InputImageOffsetType offset2 = {{0, 1, 0}}; offsetV->push_back(offset2);
      InputImageOffsetType offset3 = {{0, 1, 1}}; offsetV->push_back(offset3);
      InputImageOffsetType offset4 = {{1, 0, 0}}; offsetV->push_back(offset4);
      InputImageOffsetType offset5 = {{1, 0, 1}}; offsetV->push_back(offset5);
      InputImageOffsetType offset6 = {{1, 1, 0}}; offsetV->push_back(offset6);
      InputImageOffsetType offset7 = {{1, 1, 1}}; offsetV->push_back(offset7);
      
      m_glcmGenerator->SetOffsets(offsetV);
////////////////////////////////////////////////////////////////////////////////////////////////
      ///////////////////////////////////////////////////////////////////////////////////////////////////////
      //m_glcmGenerator->SetOffset(m_Offset);
/////////////////////////////////////////////////////////////////////////////////////////////////////////
      /////////////////////////////////////////////////////////////////////////////////////////////////////////
      ///////////////////////////////////////////////////////////////////////////////////
      m_glcmGenerator->SetNumberOfBinsPerAxis(m_NumberOfBinsPerAxis); //This will be the dimensions of the GLCM
      m_glcmGenerator->SetPixelValueMinMax(m_InputImageMinimum, m_InputImageMaximum);
      m_glcmGenerator->SetNormalize(false); // Why This is not working - reported as bug
      // The input histogram will be forcibly normalized! This algorithm takes three passes through the input histogram if the histogram was already normalized, and four if not.
      m_glcmGenerator->Update();
      
      // Compute textures indices
       typedef          Statistics::HistogramToHaralickFeaturesFilter<HistogramType> Hist2aralickFeaturesType;
       typedef typename Hist2aralickFeaturesType::Pointer Hist2HaralickFeaturesTypePointerType;
       Hist2HaralickFeaturesTypePointerType          m_advFeatureCalculator;
       m_advFeatureCalculator = Hist2aralickFeaturesType::New();
       m_advFeatureCalculator->SetInput(m_glcmGenerator->GetOutput());
       m_advFeatureCalculator->Update();


      // Fill outputs
      IMC2It                 .Set(static_cast< OutputImagePixelType >(m_advFeatureCalculator->GetIMC2()));
	  ++inputIt               ;
      ++IMC2It                ;
                                       
      progress.CompletedPixel();

      } // while
    } // for
} // end generateThreadedData

template< class TInputImage, class TOutputImage >
void
  HaralickIMC2Filter< TInputImage, TOutputImage >
  ::SetInput(InputImagePointerType pInputImage)
{
  // First, call superclass implementation
  Superclass::SetInput(pInputImage);

  typedef          StatisticsImageFilter<InputImageType> ImageStatisticsFilterType;
  typedef typename ImageStatisticsFilterType::Pointer ImageStatisticsFilterPointerType;
  ImageStatisticsFilterPointerType  m_imageStatisticsCalculator;
  m_imageStatisticsCalculator = ImageStatisticsFilterType::New();

  m_imageStatisticsCalculator->SetInput(pInputImage);
  m_imageStatisticsCalculator->Update();

  m_InputImageMinimum = m_imageStatisticsCalculator->GetMinimum();
  m_InputImageMaximum = m_imageStatisticsCalculator->GetMaximum();
  
  if(m_InputImageMaximum < m_NumberOfBinsPerAxis && m_InputImageMaximum < 3)
  m_NumberOfBinsPerAxis = (unsigned int)(m_InputImageMaximum + 1);

} // end SetInput



template<class TInputImageType, class TOutputImageType>
void 
HaralickIMC2Filter<TInputImageType, TOutputImageType>
::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  //os << indent << "Radius:" << this->GetRadius() << std::endl; // Already in the super class
  os << indent << "Offset: " << m_Offset << std::endl;
  os << indent << "NumberOfBinsPerAxis: " << m_NumberOfBinsPerAxis << std::endl;
  os << indent << "InputImageMinimum: " << m_InputImageMinimum << std::endl;
  os << indent << "InputImageMaximum: " << m_InputImageMaximum << std::endl;
}

} // end namespace itk

#endif // __itkHaralickIMC2Filter_hxx


