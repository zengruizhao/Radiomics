#ifndef __ccipdMaskToDistanceImage_hxx
#define __ccipdMaskToDistanceImage_hxx



//////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdMaskToDistanceImage.h"
#include "ccipdMaskToLevelset.h"
#include "ccipdResizeImage.h"
#include "ccipdLevelsetToSurface.h"
#include "ccipdDilateImage.h"
#include "ccipdMaskImage.h"

#include "ccipdDisableWarningsMacro.h"
  // itk includes
  #include <itkImage.h>
#include "ccipdEnableWarningsMacro.h"
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{


  
//////////////////////////////////////////////////////////////////////////////////////////
template< class TImagePixelType, class TMaskPixelType, unsigned int TDimension >
itk::SmartPointer< itk::Image< TImagePixelType, TDimension > >
MaskToDistanceImage( 
 const itk::Image< TMaskPixelType, TDimension >  * const segmentation,
 const itk::Image< TMaskPixelType, TDimension > *  const groundTruth,
 const float                                       smoothVariance,
 const float                                       increaseSizeInMM )
{
  // typedefs 
  typedef itk::Image< TMaskPixelType, TDimension >     MaskImageType;
  typedef itk::Image< TImagePixelType, TDimension >    LevelsetImageType;
  
  typedef itk::SmartPointer< const MaskImageType >     MaskImageConstPointer; 
  typedef itk::SmartPointer< const LevelsetImageType > LevelsetImageConstPointer; 


  // make sure we have everything
  if ( !segmentation ||  !groundTruth )
    return NULL;

  // calculate a levelset from segmentation  
  LevelsetImageConstPointer segmentationLevelset( 
    MaskToLevelset< TImagePixelType >( segmentation, false, false, 4, smoothVariance, false ) ); 

  // calculate a levelset from groundTruth  
  LevelsetImageConstPointer groundTruthLevelset( 
    MaskToLevelset< TImagePixelType >( groundTruth, false, false, 4, smoothVariance, false ) );
    
  // resize segmentation and ground truth levelset to be isotropic
  LevelsetImageConstPointer resizedSegmentation ( 
    ResizeImageIsotropically( segmentationLevelset.GetPointer(), true ) );
  
  LevelsetImageConstPointer resizedGroundTruth ( 
    ResizeImageIsotropically( groundTruthLevelset.GetPointer(), true ) );  

  MaskImageConstPointer surfaceMask( 
    GetSurfaceImage< TImagePixelType, TMaskPixelType >( resizedSegmentation.GetPointer(), increaseSizeInMM ) );  

  // now dilate so it is more than a single pixel
  MaskImageConstPointer dilatedSurfaceMask ( 
    DilateImage( surfaceMask.GetPointer() ) );

  return MaskImage< TImagePixelType >( resizedGroundTruth.GetPointer(), dilatedSurfaceMask.GetPointer(), -1e-4f, true );

} //MaskToDistanceImage

} // namespace ccipd

#endif // __ccipdMaskToDistanceImage_hxx