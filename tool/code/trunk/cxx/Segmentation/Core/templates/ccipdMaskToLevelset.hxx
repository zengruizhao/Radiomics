#ifndef __ccipdMaskToLevelset_hxx 
#define __ccipdMaskToLevelset_hxx



//////////////////////////////////////////////////////////////////////////////////////////
// ccipd includes
//#include "ccipdMaskToLevelset.h"
#include "ccipdCore.h"
#include "ccipdResizeImage.h"
#include "ccipdStatistics.h"

// our own itk includes
#include "itkMaskToLevelsetFilter.h"       // for computing the levelset

#include "ccipdDisableWarningsMacro.h"
  // itk includes
  #include <itkNearestNeighborInterpolateImageFunction.h>
  #include <itkImage.h>
#include "ccipdEnableWarningsMacro.h"
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////
template< class TImagePixelType, class TMaskPixelType, unsigned int TDimension >
itk::SmartPointer< itk::Image< TImagePixelType, TDimension > >
 MaskToLevelset(  const itk::Image< TMaskPixelType, TDimension >  * const mask
				, const bool  normalize
				, const bool  sigmoid
				, const float sigmoidWidth
				, const float smoothingVariance
				, const bool  resizeIsotropically
			   )
{

  // set out typedefs
  typedef itk::Image< TMaskPixelType, TDimension >  MaskImageType;
  typedef itk::Image< TImagePixelType, TDimension > LevelsetImageType;
  typedef itk::SmartPointer< LevelsetImageType >    LevelsetImagePointer; 

  // for resizing the mask
  typedef itk::NearestNeighborInterpolateImageFunction<MaskImageType, TransformValueType> InterpolatorType;  
  
  itk::SmartPointer< InterpolatorType > interpolator = InterpolatorType::New();

  typedef itk::MaskToLevelsetFilter< MaskImageType, LevelsetImageType >   LevelsetFilterType;
  
  itk::SmartPointer< const MaskImageType> resizedMask = mask;

  if (resizeIsotropically)
  {
	  resizedMask = static_cast<const MaskImageType *>(
		  ResizeImageIsotropically< TMaskPixelType, TDimension >(
		  mask,
		  false,
		  TMaskPixelType(0.0),
		  interpolator));
  }

  const bool smooth = smoothingVariance > 0;

  typename LevelsetFilterType::Pointer levelsetFilter = LevelsetFilterType::New();
  
  levelsetFilter->SetInput            ( resizedMask       );
  levelsetFilter->SetNormalize        ( normalize         );
  levelsetFilter->SetSigmoid          ( sigmoid           );
  levelsetFilter->SetSigmoidWidth     ( sigmoidWidth      );
  levelsetFilter->SetSmooth           ( smooth            );
  levelsetFilter->SetSmoothingVariance( smoothingVariance );

  levelsetFilter->Update();
 

  const LevelsetImagePointer
    levelset = levelsetFilter->GetOutput();

  TImagePixelType max = TImagePixelType(0.1);
    //TImagePixelType(GetMax(levelset.GetPointer())); // get max to identify resizing default value

    // resize the levelset back to the correct voxel sizes
  const LevelsetImagePointer
    resizedLevelset = ResizeImage( levelset.GetPointer(), mask, false, max ); 
 
  
  return resizedLevelset;
  
} // MaskToLevelset

} // namespace ccipd
#endif // __ccipdMaskToLevelset_hxx
