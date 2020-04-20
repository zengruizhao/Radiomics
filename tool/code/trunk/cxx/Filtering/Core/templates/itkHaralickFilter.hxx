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
#ifndef __itkHaralickFilter_hxx
#define __itkHaralickFilter_hxx
#include "itkHaralickFilter.h"
#include <itkNeighborhoodAlgorithm.h>
#include <itkConstantBoundaryCondition.h>   
#include <itkScalarImageToCooccurrenceMatrixFilter.h>
#include <itkHistogramToTextureFeaturesFilter.h>
#include <itkStatisticsImageFilter.h>
#include "itkHistogramToHaralickFeaturesFilter.h"

namespace itk
{
template< class TInputImage, class TOutputImage >
HaralickFilter< TInputImage, TOutputImage >
::HaralickFilter()
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

  // allocate the data objects for the outputs which are
  // just decorators real types
  for ( int i = 0; i < 17; ++i )
  {
    this->ProcessObject::SetNthOutput(i, this->MakeOutput(i));
  }
} // end constructor

template< class TInputImage, class TOutputImage >
void
HaralickFilter< TInputImage, TOutputImage >
::ThreadedGenerateData(const OutputImageRegionType & outputRegionForThread, ThreadIdType threadId)
{
   // Retrieve Input
  typename  InputImageType::ConstPointer input  = this->GetInput();
  // Retrieve Output
  typename OutputImageType::Pointer pEnergyFeatureOutputImage               = this->GetOutput(0);
  typename OutputImageType::Pointer pEntropyOutputFeatureImage              = this->GetOutput(1); 
  typename OutputImageType::Pointer pCorrelationOutputFeatureImage          = this->GetOutput(2); 
  typename OutputImageType::Pointer pInvDiffMomentOutputFeatureImage        = this->GetOutput(3); 
  typename OutputImageType::Pointer pInertiaOutputFeatureImage              = this->GetOutput(4); 
  typename OutputImageType::Pointer pClusterShadeOutputFeatureImage         = this->GetOutput(5); 
  typename OutputImageType::Pointer pClusterProminenceOutputFeatureImage    = this->GetOutput(6);
  typename OutputImageType::Pointer pHaralickCorrelationOutputFeatureImage  = this->GetOutput(7);
  
  typename OutputImageType::Pointer pMeanOutputFeatureImage                 = this->GetOutput(8);
  typename OutputImageType::Pointer pVarianceOutputFeatureImage             = this->GetOutput(9);
  typename OutputImageType::Pointer pSumAverageOutputFeatureImage           = this->GetOutput(10);
  typename OutputImageType::Pointer pSumVarianceOutputFeatureImage          = this->GetOutput(11);
  typename OutputImageType::Pointer pSumEntropyOutputFeatureImage           = this->GetOutput(12);
  typename OutputImageType::Pointer pDifferenceEntropyOutputFeatureImage    = this->GetOutput(13);
  typename OutputImageType::Pointer pDifferenceVarianceOutputFeatureImage   = this->GetOutput(14);
  typename OutputImageType::Pointer pIMC1OutputFeatureImage                 = this->GetOutput(15);
  typename OutputImageType::Pointer pIMC2OutputFeatureImage                 = this->GetOutput(16);


  // support progress methods/callbacks
  ProgressReporter progress( this, threadId, outputRegionForThread.GetNumberOfPixels());

  // 
  ConstantBoundaryCondition<InputImageType,  OutputImageType> nbc;
  ConstNeighborhoodIterator< InputImageType > inputIt;
  ImageRegionIterator<OutputImageType>  energyIt
                                      , entropyIt
                                      , correlationIt
                                      , invDiffMomentIt
                                      , inertiaIt
                                      , clusterShadeIt
                                      , clusterProminence
                                      , haralicksCorrelationIt
                                      , meanIt               
                                      , varianceIt
                                      , sumAverageIt
                                      , sumVarianceIt
                                      , sumEntropyIt
                                      , differenceEntropyIt
                                      , differenceVarianceIt 
                                      , iMC1It
                                      , iMC2It
                                      ;


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


    energyIt                 = ImageRegionIterator< OutputImageType >(pEnergyFeatureOutputImage             , *faceListIt);
    entropyIt                = ImageRegionIterator< OutputImageType >(pEntropyOutputFeatureImage            , *faceListIt);
    correlationIt            = ImageRegionIterator< OutputImageType >(pCorrelationOutputFeatureImage        , *faceListIt);
    invDiffMomentIt          = ImageRegionIterator< OutputImageType >(pInvDiffMomentOutputFeatureImage      , *faceListIt);
    inertiaIt                = ImageRegionIterator< OutputImageType >(pInertiaOutputFeatureImage            , *faceListIt);
    clusterShadeIt           = ImageRegionIterator< OutputImageType >(pClusterShadeOutputFeatureImage       , *faceListIt);
    clusterProminence        = ImageRegionIterator< OutputImageType >(pClusterProminenceOutputFeatureImage  , *faceListIt);
    haralicksCorrelationIt   = ImageRegionIterator< OutputImageType >(pHaralickCorrelationOutputFeatureImage, *faceListIt);
    meanIt                   = ImageRegionIterator< OutputImageType >(pMeanOutputFeatureImage               , *faceListIt);
    varianceIt               = ImageRegionIterator< OutputImageType >(pVarianceOutputFeatureImage           , *faceListIt);
    sumAverageIt             = ImageRegionIterator< OutputImageType >(pSumAverageOutputFeatureImage         , *faceListIt);
    sumVarianceIt            = ImageRegionIterator< OutputImageType >(pSumVarianceOutputFeatureImage        , *faceListIt);
    sumEntropyIt             = ImageRegionIterator< OutputImageType >(pSumEntropyOutputFeatureImage         , *faceListIt);
    differenceEntropyIt      = ImageRegionIterator< OutputImageType >(pDifferenceEntropyOutputFeatureImage  , *faceListIt);
    differenceVarianceIt     = ImageRegionIterator< OutputImageType >(pDifferenceVarianceOutputFeatureImage , *faceListIt);
    iMC1It                   = ImageRegionIterator< OutputImageType >(pIMC1OutputFeatureImage               , *faceListIt);
    iMC2It                   = ImageRegionIterator< OutputImageType >(pIMC2OutputFeatureImage               , *faceListIt);



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

       typedef          Statistics::HistogramToTextureFeaturesFilter<HistogramType> Hist2FeaturesType;
       typedef typename Hist2FeaturesType::Pointer Hist2FeaturesPointerType;
       Hist2FeaturesPointerType          m_featureCalculator;
       m_featureCalculator = Hist2FeaturesType::New();
       m_featureCalculator->SetInput(m_glcmGenerator->GetOutput());
       m_featureCalculator->Update();

       

       typedef          Statistics::HistogramToHaralickFeaturesFilter<HistogramType> Hist2aralickFeaturesType;
       typedef typename Hist2aralickFeaturesType::Pointer Hist2HaralickFeaturesTypePointerType;
       Hist2HaralickFeaturesTypePointerType          m_advFeatureCalculator;
       m_advFeatureCalculator = Hist2aralickFeaturesType::New();
       m_advFeatureCalculator->SetInput(m_glcmGenerator->GetOutput());
       m_advFeatureCalculator->Update();

       //itk::Statistics::Histogram<double>* pH = const_cast<itk::Statistics::Histogram<double>*>(m_glcmGenerator->GetOutput());
       //m_advFeatureCalculator->/*SetInput*/SetHistogram(pH/*m_glcmGenerator->GetOutput()*/); // or ->SetHistogram
       //m_advFeatureCalculator->GenerateData(); // or ->Update



      // Fill outputs
      energyIt              .Set(static_cast< OutputImagePixelType >(m_featureCalculator->GetEnergy()));
      entropyIt             .Set(static_cast< OutputImagePixelType >(m_featureCalculator->GetEntropy()));
      correlationIt         .Set(static_cast< OutputImagePixelType >(m_featureCalculator->GetCorrelation()));
      invDiffMomentIt       .Set(static_cast< OutputImagePixelType >(m_featureCalculator->GetInverseDifferenceMoment()));
      inertiaIt             .Set(static_cast< OutputImagePixelType >(m_featureCalculator->GetInertia()));
      clusterShadeIt        .Set(static_cast< OutputImagePixelType >(m_featureCalculator->GetClusterShade()));
      clusterProminence     .Set(static_cast< OutputImagePixelType >(m_featureCalculator->GetClusterProminence()));
      haralicksCorrelationIt.Set(static_cast< OutputImagePixelType >(m_featureCalculator->GetHaralickCorrelation()));

      
      meanIt                 .Set(static_cast< OutputImagePixelType >(m_advFeatureCalculator->GetMean()));
      varianceIt             .Set(static_cast< OutputImagePixelType >(m_advFeatureCalculator->GetVariance()));
      sumAverageIt           .Set(static_cast< OutputImagePixelType >(m_advFeatureCalculator->GetSumAverage()));
      sumVarianceIt          .Set(static_cast< OutputImagePixelType >(m_advFeatureCalculator->GetSumVariance()));
      sumEntropyIt           .Set(static_cast< OutputImagePixelType >(m_advFeatureCalculator->GetSumEntropy()));
      differenceEntropyIt    .Set(static_cast< OutputImagePixelType >(m_advFeatureCalculator->GetDifferenceEntropy()));
      differenceVarianceIt   .Set(static_cast< OutputImagePixelType >(m_advFeatureCalculator->GetDifferenceVariance()));
      iMC1It                 .Set(static_cast< OutputImagePixelType >(m_advFeatureCalculator->GetIMC1()));
      iMC2It                 .Set(static_cast< OutputImagePixelType >(m_advFeatureCalculator->GetIMC2()));

      ++inputIt               ;
      ++energyIt              ;
      ++entropyIt             ;
      ++correlationIt         ;
      ++invDiffMomentIt       ;
      ++inertiaIt             ;
      ++clusterShadeIt        ;
      ++clusterProminence     ;
      ++haralicksCorrelationIt;
      ++meanIt                ;      
      ++varianceIt            ;    
      ++sumAverageIt          ;     
      ++sumVarianceIt         ;    
      ++sumEntropyIt          ;   
      ++differenceEntropyIt   ; 
      ++differenceVarianceIt  ;
      ++iMC1It                ;
      ++iMC2It                ;                               
                                       
      progress.CompletedPixel();

      } // while
    } // for
} // end generateThreadedData

template< class TInputImage, class TOutputImage >
void
  HaralickFilter< TInputImage, TOutputImage >
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
typename HaralickFilter<TInputImageType, TOutputImageType>::OutputImagePointerType
HaralickFilter<TInputImageType, TOutputImageType>
::GetEnergyOutput()
{
  if (this->GetNumberOfOutputs() < 1) return 0;
  return static_cast<OutputImagePointerType>(this->GetOutput(0));
}

template<class TInputImageType, class TOutputImageType>
typename HaralickFilter<TInputImageType, TOutputImageType>::OutputImagePointerType
  HaralickFilter<TInputImageType, TOutputImageType>
  ::GetEntropyOutput()
{
  if (this->GetNumberOfOutputs() < 1) return 0;
  return static_cast<OutputImagePointerType>(this->GetOutput(1));
}

template<class TInputImageType, class TOutputImageType>
typename HaralickFilter<TInputImageType, TOutputImageType>::OutputImagePointerType
  HaralickFilter<TInputImageType, TOutputImageType>
  ::GetCorrelationOutput()
{
  if (this->GetNumberOfOutputs() < 1) return 0;
  return static_cast<OutputImagePointerType>(this->GetOutput(2));
}

template<class TInputImageType, class TOutputImageType>
typename HaralickFilter<TInputImageType, TOutputImageType>::OutputImagePointerType
  HaralickFilter<TInputImageType, TOutputImageType>
  ::GetInverseDifferenceMomentOutput()
{
  if (this->GetNumberOfOutputs() < 1) return 0;
  return static_cast<OutputImagePointerType>(this->GetOutput(3));
}

template<class TInputImageType, class TOutputImageType>
typename HaralickFilter<TInputImageType, TOutputImageType>::OutputImagePointerType
  HaralickFilter<TInputImageType, TOutputImageType>
  ::GetInertiaOutput()
{
  if (this->GetNumberOfOutputs() < 1) return 0;
  return static_cast<OutputImagePointerType>(this->GetOutput(4));
}

template<class TInputImageType, class TOutputImageType>
typename HaralickFilter<TInputImageType, TOutputImageType>::OutputImagePointerType
  HaralickFilter<TInputImageType, TOutputImageType>
  ::GetClusterShadeOutput()
{
  if (this->GetNumberOfOutputs() < 1) return 0;
  return static_cast<OutputImagePointerType>(this->GetOutput(5));
}

template<class TInputImageType, class TOutputImageType>
typename HaralickFilter<TInputImageType, TOutputImageType>::OutputImagePointerType
HaralickFilter<TInputImageType, TOutputImageType>
::GetClusterProminenceOutput()
{
  if (this->GetNumberOfOutputs() < 1) return 0;
  return static_cast<OutputImagePointerType>(this->GetOutput(6));
}

template<class TInputImageType, class TOutputImageType>
typename HaralickFilter<TInputImageType, TOutputImageType>::OutputImagePointerType
HaralickFilter<TInputImageType, TOutputImageType>
::GetHaralickCorrelationOutput()
{
  if (this->GetNumberOfOutputs() < 1) return 0;
  return static_cast<OutputImagePointerType>(this->GetOutput(7));
}

template<class TInputImageType, class TOutputImageType>
typename HaralickFilter<TInputImageType, TOutputImageType>::OutputImagePointerType
HaralickFilter<TInputImageType, TOutputImageType>
::GetMeanOutput()
{
  if (this->GetNumberOfOutputs() < 1) return 0;
  return static_cast<OutputImagePointerType>(this->GetOutput(8));
}

template<class TInputImageType, class TOutputImageType>
typename HaralickFilter<TInputImageType, TOutputImageType>::OutputImagePointerType
HaralickFilter<TInputImageType, TOutputImageType>
::GetVarianceOutput()
{
  if (this->GetNumberOfOutputs() < 1) return 0;
  return static_cast<OutputImagePointerType>(this->GetOutput(9));
}

template<class TInputImageType, class TOutputImageType>
typename HaralickFilter<TInputImageType, TOutputImageType>::OutputImagePointerType
HaralickFilter<TInputImageType, TOutputImageType>
::GetSumAverageOutput()
{
  if (this->GetNumberOfOutputs() < 1) return 0;
  return static_cast<OutputImagePointerType>(this->GetOutput(10));
}

template<class TInputImageType, class TOutputImageType>
typename HaralickFilter<TInputImageType, TOutputImageType>::OutputImagePointerType
HaralickFilter<TInputImageType, TOutputImageType>
::GetSumVarianceOutput()
{
  if (this->GetNumberOfOutputs() < 1) return 0;
  return static_cast<OutputImagePointerType>(this->GetOutput(11));
}

template<class TInputImageType, class TOutputImageType>
typename HaralickFilter<TInputImageType, TOutputImageType>::OutputImagePointerType
HaralickFilter<TInputImageType, TOutputImageType>
::GetSumEntropyOutput()
{
  if (this->GetNumberOfOutputs() < 1) return 0;
  return static_cast<OutputImagePointerType>(this->GetOutput(12));
}

template<class TInputImageType, class TOutputImageType>
typename HaralickFilter<TInputImageType, TOutputImageType>::OutputImagePointerType
HaralickFilter<TInputImageType, TOutputImageType>
::GetDifferenceEntropyOutput()
{
  if (this->GetNumberOfOutputs() < 1) return 0;
  return static_cast<OutputImagePointerType>(this->GetOutput(13));
}

template<class TInputImageType, class TOutputImageType>
typename HaralickFilter<TInputImageType, TOutputImageType>::OutputImagePointerType
HaralickFilter<TInputImageType, TOutputImageType>
::GetDifferenceVarianceOutput()
{
  if (this->GetNumberOfOutputs() < 1) return 0;
  return static_cast<OutputImagePointerType>(this->GetOutput(14));
}

template<class TInputImageType, class TOutputImageType>
typename HaralickFilter<TInputImageType, TOutputImageType>::OutputImagePointerType
HaralickFilter<TInputImageType, TOutputImageType>
::GetIMC1Output()
{
  if (this->GetNumberOfOutputs() < 1) return 0;
  return static_cast<OutputImagePointerType>(this->GetOutput(15));
}

template<class TInputImageType, class TOutputImageType>
typename HaralickFilter<TInputImageType, TOutputImageType>::OutputImagePointerType
HaralickFilter<TInputImageType, TOutputImageType>
::GetIMC2Output()
{
  if (this->GetNumberOfOutputs() < 1) return 0;
  return static_cast<OutputImagePointerType>(this->GetOutput(16));
}

template<class TInputImageType, class TOutputImageType>
void 
HaralickFilter<TInputImageType, TOutputImageType>
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

#endif // __itkHaralickFilter_hxx


