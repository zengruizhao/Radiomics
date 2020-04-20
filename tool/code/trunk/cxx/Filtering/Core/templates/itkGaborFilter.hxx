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
#ifndef __itkGaborFilter_hxx
#define __itkGaborFilter_hxx
#include "itkGaborFilter.h"
#include <itkGaborImageSource.h>
#include <itkAffineTransform.h>
#include <itkResampleImageFilter.h>
#include <itkNearestNeighborExtrapolateImageFunction.h>


#include <iostream>
using namespace std;

namespace itk
{
template< class TInputImage, class TKernelImage, class TOutputImage, class TInternalPrecision>
GaborFilter<TInputImage, TKernelImage, TOutputImage, TInternalPrecision>
::GaborFilter()
{
  // Default values of member variables
  m_Frequency = 0.01f;
  m_RotationZ = 0.00f;
  Superclass::SetKernelImage(CreateGaborKernel()); // A default Kernel Should be Created
} // end constructor

template< class TInputImage, class TKernelImage, class TOutputImage, class TInternalPrecision>
void
GaborFilter<TInputImage, TKernelImage, TOutputImage, TInternalPrecision>
::GenerateData()
{
  // Create The Kernel
  // Set the Kernel
  Superclass::SetKernelImage(CreateGaborKernel());
  // call the SuperClass Implementation
  Superclass::GenerateData();
} // end generateData


template< class TInputImage, class TKernelImage, class TOutputImage, class TInternalPrecision>
void 
GaborFilter<TInputImage, TKernelImage, TOutputImage, TInternalPrecision>
::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << indent << "Frequency: " << m_Frequency << std::endl;
  os << indent << "RotationZ: " << m_RotationZ << std::endl;
}


template<class TInputImage, class TKernelImage, class TOutputImage, class TInternalPrecision>
typename TKernelImage::Pointer 
GaborFilter<TInputImage, TKernelImage, TOutputImage, TInternalPrecision>
::CreateGaborKernel()
{
  // Reference
  // http://matlabserver.cs.rug.nl/edgedetectionweb/web/edgedetection_params.html#params

  // Construct the gabor image kernel
  typedef itk::GaborImageSource<TKernelImage> GaborSourceType;
  typename GaborSourceType::Pointer gabor = GaborSourceType::New();

  // The ConvolutionImageFilter ignores the spacing, origin, and orientation of the kernel image and treats them as identical to those in the input image.
  // http://www.itk.org/Doxygen/html/classitk_1_1ConvolutionImageFilter.html

  // Frequency
  gabor->SetFrequency(m_Frequency);

  // Sigma (Standard Deviation)
  typename GaborSourceType::ArrayType sigma;
  sigma.Fill(0.56 / m_Frequency); // This means bandwidth = 1 (Ref.: http://matlabserver.cs.rug.nl/edgedetectionweb/web/edgedetection_params.html#references)
  gabor->SetSigma( sigma );

  // Spacing - Will be ignored
  typename TKernelImage::SpacingType spacing;
  spacing.Fill( 1.0 );
  gabor->SetSpacing( spacing );

  // Kernel Size 
  typename TKernelImage::RegionType::SizeType size;
  unsigned int windowSize = (unsigned int)ceil(7 * 0.56 / m_Frequency);
  if(windowSize % 2 == 0) windowSize++;  // make window size odd
  size.Fill(windowSize);
  for (unsigned int i = 2; i < TKernelImage::ImageDimension; i++) size[i] = 1;
  gabor->SetSize( size );

  // Origin - Will be ignored
  typename TKernelImage::PointType origin;
  for( unsigned int i = 0; i < TKernelImage::ImageDimension; i++)
    origin[i] = (-0.5 * size[i] * spacing[i]); 
  gabor->SetOrigin(origin);

  // Mean
  typename GaborSourceType::ArrayType mean;
  //   for( unsigned int d = 0; d < TOutputImageType::ImageDimension; d++ )
  //     mean[d] = origin[d] + 0.5 * spacing[d] * size[d];
  mean.Fill(0.0);
  gabor->SetMean( mean );

  // Direction
   typename TKernelImage::DirectionType direction;
   direction.SetIdentity();
   gabor->SetDirection(direction);

  // Phase Offset
  //   double phaseOffset = 0.0;
  //   gabor->SetPhaseOffset(phaseOffset);

  // Finalize Gabor
  gabor->SetCalculateImaginaryPart(true);
  gabor->Update();

   // Rotate ONLY around z-axis
   typedef itk::AffineTransform< double, TKernelImage::ImageDimension > TransformType;
   typename TransformType::Pointer      transform     = TransformType::New();    
   typename  TransformType::CenterType center;
   center.Fill(0.0);
   transform->SetCenter(center     );
   transform->Rotate2D (m_RotationZ);

   typedef itk::ResampleImageFilter<TKernelImage, TKernelImage> ResampleImageFilterType;
   typename ResampleImageFilterType::Pointer resampleFilter = ResampleImageFilterType::New();
   resampleFilter->SetInput(gabor->GetOutput());
   resampleFilter->SetTransform(transform);
  
   resampleFilter->SetOutputOrigin     ( gabor->GetOrigin   () );
   resampleFilter->SetOutputDirection  ( gabor->GetDirection() );
   resampleFilter->SetOutputSpacing    ( gabor->GetSpacing  () );
   resampleFilter->SetSize             ( gabor->GetSize() );
   resampleFilter->SetDefaultPixelValue( 0.0                   );

   // use a linear interpolator
   typedef itk::LinearInterpolateImageFunction<TKernelImage, double> InterpolatorType;

   resampleFilter->SetInterpolator( InterpolatorType::New() );

   // use a nearest neighbor extrapolator; 
   // note that pixels outside the extrapolator should be close to zero.
   typedef itk::NearestNeighborExtrapolateImageFunction<TKernelImage, double> ExtrapolateType;

   resampleFilter->SetExtrapolator( ExtrapolateType::New() );

   resampleFilter->Update();
 
   return resampleFilter->GetOutput();

} // CreateGaborKernel


} // end namespace itk

#endif // __itkGaborFilter_h

