


//////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdPrepareMaskForEvaluation.h"
#include "ccipdResizeImage.h"
#include "ccipdLevelsets.h"
#include "ccipdMaskToLevelset.h"

#include "ccipdDisableWarningsMacro.h"
  #include <itkImage.h>
#include "ccipdEnableWarningsMacro.h"
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
using std::cout;
using std::endl;
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{

  
 
//////////////////////////////////////////////////////////////////////////////////////////
VolumeMaskImageConstPointer PrepareMaskForEvaluation(
  const VolumeMaskImageType * mask,
  const VolumeMaskImageType * templateMask,
  const float              smoothVariance,
  const float              increaseSizeInMM )
{

  // what are we doing to this mask?
  const bool
    smooth      = smoothVariance > 0,
    useLevelset = smooth || increaseSizeInMM != 0;

  if ( useLevelset )
    {

    // we first convert it to a levelset so we can resample
    VolumeImageConstPointer
      segmentationLevelset ( MaskToLevelset< InternalPixelType >(
        mask, false, false, 4, smoothVariance, false ) );
        
    // then, we must resample them so they are in the same coordinate frame
   VolumeImageConstPointer fitLevelset(
     ResizeImage( segmentationLevelset, templateMask ) );

    // finally, convert it back to a mask
    const VolumeMaskImageConstPointer finalMask(
        CalculateMaskFromLevelset( fitLevelset, increaseSizeInMM ) );

    return finalMask;
      
    }    
  else // no smoothing
    {

    const VolumeMaskImageConstPointer finalMask(
        ResizeMask( mask, templateMask ) );

    return finalMask;
    }

};
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace
