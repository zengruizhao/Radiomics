//////////////////////////////////////////////////////////////////////////////////////////
// ccipd includes
#include "ccipdExtractSubMask.h"
#include "ccipdResizeImage.h"                // for fitting it onto the template
#include "ccipdBoundingBox.h"                // for determining the organ location

#include "ccipdDisableWarningsMacro.h"
  // itk includes
  #include <itkRegionOfInterestImageFilter.h> // for cropping the image
#include "ccipdEnableWarningsMacro.h"
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////
VolumeMaskImageConstPointer ExtractSubMask(
  const VolumeMaskImageType * const inputMask,
  const VolumeMaskImageType * const templateMask,
  const float firstSlicePercentage,
  const float slicesPercentage )
{

  // the first step is to move the input mask onto the template
  const auto fitMask = ResizeMask( inputMask, templateMask );

  // now, we must figure out a bounding box containing the prostate
  // in the template mask
  VolumeRegionType boundingBox = GetBoundingBox( templateMask, 0 );

  // which slices are we currently considering?
  const auto
    currentSize = boundingBox.GetSize( 2 );
  const float
    currentSizeFloat = static_cast< float >( currentSize ),
    newSizeFloat     = currentSizeFloat * slicesPercentage;
  const unsigned long
    newSize = static_cast< unsigned long >( newSizeFloat );
  const float
    currentIndexFloat = static_cast< float >( boundingBox.GetIndex( 2 ) ),
    newIndexFloat     = currentIndexFloat +  firstSlicePercentage * currentSizeFloat;

  boundingBox.SetSize ( 2, newSize < 1 ? 1 : newSize );
  boundingBox.SetIndex( 2,
    static_cast< VolumeRegionType::IndexValueType >( newIndexFloat ) );

  // now, extract only the portion of the mask containing the
  // extract midgland region
  typedef itk::RegionOfInterestImageFilter<
    VolumeMaskImageType, VolumeMaskImageType > CroppingFilterType;
  const auto croppingFilter = CroppingFilterType::New();
  croppingFilter->SetInput( fitMask );
  croppingFilter->SetRegionOfInterest( boundingBox );
  croppingFilter->Update();

  // output the cropped image
  return croppingFilter->GetOutput();

} // ExtractSubMask
//////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////
VolumeMaskImageConstPointer ExtractMidglandMask(
  const VolumeMaskImageType * const inputMask,
  const VolumeMaskImageType * const templateMask,
  const float slicesPercentage )
{
  const float firstSlicePercentage = 0.5f * ( 1.0f - slicesPercentage );

  return ExtractSubMask(inputMask, templateMask, firstSlicePercentage, slicesPercentage);
} // ExtractMidglandMask

//////////////////////////////////////////////////////////////////////////////////////////
VolumeMaskImageConstPointer ExtractApexMask(
  const VolumeMaskImageType * const inputMask,
  const VolumeMaskImageType * const templateMask,
  const float slicesPercentage )
{
  const float firstSlicePercentage = 0; // 0.5 * ( 1 - slicesPercentage );

  return ExtractSubMask(inputMask, templateMask, firstSlicePercentage, slicesPercentage);
} // ExtractApexMask

//////////////////////////////////////////////////////////////////////////////////////////
VolumeMaskImageConstPointer ExtractBaseMask(
  const VolumeMaskImageType * const inputMask,
  const VolumeMaskImageType * const templateMask,
  const float slicesPercentage )
{
  const float firstSlicePercentage = 1 - slicesPercentage;

  return ExtractSubMask(inputMask, templateMask, firstSlicePercentage, slicesPercentage);
} // ExtractBaseMask

} // namespace
