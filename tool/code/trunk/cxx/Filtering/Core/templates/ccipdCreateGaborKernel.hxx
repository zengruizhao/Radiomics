#ifndef __ccipdCreateGaborKernel_hxx
#define __ccipdCreateGaborKernel_hxx



/**
 * \addtogroup Filtering
 * @{
 */

 

//////////////////////////////////////////////////////////////////////////////////////////
// ccipd includes
#include "ccipdCreateGaborKernel.h"
#include "ccipdVolumeTypes.h"

#include "ccipdDisableWarningsMacro.h"
  // itk includes
  #include <itkGaborImageSource.h>
  #include <itkAffineTransform.h>
  #include <itkResampleImageFilter.h>
  #include <itkLinearInterpolateImageFunction.h>
  #include <itkNearestNeighborExtrapolateImageFunction.h>
  #include <itkImage.h>
#include "ccipdEnableWarningsMacro.h"

#if ( __GNUC__ >= 5 || ( __GNUC__ == 4 && __GNUC_MINOR__ >= 7) )
#pragma GCC diagnostic ignored "-Wstrict-overflow"
#endif // GNUC

//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{


template<class TOutputImageType>
typename TOutputImageType::Pointer 
  CreateGaborKernel(  float frequency
                    , float ThetaZ/* = 0.0*/
                   )
{
  // Reference
  // http://matlabserver.cs.rug.nl/edgedetectionweb/web/edgedetection_params.html#params

  // Construct the gabor image kernel
  typedef itk::GaborImageSource<TOutputImageType> GaborSourceType;
  typename GaborSourceType::Pointer gabor = GaborSourceType::New();

  // The ConvolutionImageFilter ignores the spacing, origin, and orientation of the kernel image and treats them as identical to those in the input image.
  // http://www.itk.org/Doxygen/html/classitk_1_1ConvolutionImageFilter.html

  // Frequency
  gabor->SetFrequency(frequency);

  // Sigma (Standard Deviation)
  typename GaborSourceType::ArrayType sigma;
  sigma.Fill(0.56 / frequency); // This means bandwidth = 1 (Ref.: http://matlabserver.cs.rug.nl/edgedetectionweb/web/edgedetection_params.html#references)
  gabor->SetSigma( sigma );

  // Spacing - Will be ignored
  typename TOutputImageType::SpacingType spacing;
  spacing.Fill( 1.0 );
  gabor->SetSpacing( spacing );

  // Kernel Size 
  typename TOutputImageType::RegionType::SizeType size;
  unsigned int windowSize = (unsigned int)ceil(7 * 0.56 / frequency);
  if(windowSize % 2 == 0) windowSize++;  // make window size odd
  size.Fill(windowSize);
  for (unsigned int i = 2; i < TOutputImageType::ImageDimension; i++) size[i] = 1;
  gabor->SetSize( size );

  // Origin - Will be ignored
  typename TOutputImageType::PointType origin;
  for( unsigned int i = 0; i < TOutputImageType::ImageDimension; i++)
    origin[i] = (-0.5 * size[i] * spacing[i]); 
  gabor->SetOrigin(origin);

  // Mean
  typename GaborSourceType::ArrayType mean;
  //   for( unsigned int d = 0; d < TOutputImageType::ImageDimension; d++ )
  //     mean[d] = origin[d] + 0.5 * spacing[d] * size[d];
  mean.Fill(0.0);
  gabor->SetMean( mean );

  // Direction
  typename TOutputImageType::DirectionType direction;
  direction.SetIdentity();
  gabor->SetDirection(direction);

  // Phase Offset
  //   double phaseOffset = 0.0;
  //   gabor->SetPhaseOffset(phaseOffset);

  // Finalize Gabor
  gabor->SetCalculateImaginaryPart(true);
  gabor->Update();

   // Rotate ONLY around z-axis
   typedef itk::AffineTransform< double, TOutputImageType::ImageDimension > TransformType;
   typename TransformType::Pointer      transform     = TransformType::New();    
   typename TransformType::CenterType center;
   center.Fill(0.0);
   transform->SetCenter( center   );
   transform->Rotate2D ( ThetaZ );

   typedef itk::ResampleImageFilter<TOutputImageType, TOutputImageType> ResampleImageFilterType;
   typename ResampleImageFilterType::Pointer resampleFilter = ResampleImageFilterType::New();
   resampleFilter->SetInput(gabor->GetOutput());
   resampleFilter->SetTransform(transform);
  
   resampleFilter->SetOutputOrigin     ( gabor->GetOrigin   () );
   resampleFilter->SetOutputDirection  ( gabor->GetDirection() );
   resampleFilter->SetOutputSpacing    ( gabor->GetSpacing  () );
   resampleFilter->SetSize             ( gabor->GetSize()      );
   resampleFilter->SetDefaultPixelValue( 0.0                   );
   
   // use a linear interpolator
   typedef itk::LinearInterpolateImageFunction<TOutputImageType, double> InterpolatorType;
   resampleFilter->SetInterpolator( InterpolatorType::New() );
   // use a nearest neighbor extrapolator; 
   // note that pixels outside the extrapolator should be close to zero.
   typedef itk::NearestNeighborExtrapolateImageFunction<TOutputImageType, double> ExtrapolateType;
   resampleFilter->SetExtrapolator( ExtrapolateType::New() );
   resampleFilter->Update();
   return resampleFilter->GetOutput();
}



} // namespace
/** @} */

#endif // __ccipdCreateGaborKernel_hxx
