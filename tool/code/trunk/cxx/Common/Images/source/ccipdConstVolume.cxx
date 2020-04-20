

//////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdConstVolume.h"
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////
/** For multiple images. */
VolumeConstImagesContainer ConvertVolumeImagesToConst(
  const VolumeImagesContainer images )
{
  // prepare the output
  VolumeConstImagesContainer constImages;

  // convert each image
  for ( VolumeImagesContainer::const_iterator imageIter = images.cbegin();
    imageIter != images.cend(); ++imageIter)
    {

    constImages.push_back( static_cast< const VolumeImageType * >( *imageIter ) );

    } // each image

  return constImages;

}
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
/** For multiple masks. */
VolumeConstMaskImagesContainer ConvertVolumeMasksToConst(
  const VolumeMaskImagesContainer masks )
{
  // prepare the output
  VolumeConstMaskImagesContainer constMasks;

  // convert each image
  for ( VolumeMaskImagesContainer::const_iterator maskIter = masks.cbegin();
    maskIter != masks.cend(); ++maskIter)
    {

    constMasks.push_back( static_cast< const VolumeMaskImageType * >( *maskIter ) );

    } // each image

  return constMasks;

}
//////////////////////////////////////////////////////////////////////////////////////////




//////////////////////////////////////////////////////////////////////////////////////////
/**For multiple trasnforms.*/
VolumeConstTransformsContainer ConvertVolumeTransformsToConst(
  const VolumeTransformsContainer transforms)
{
  //prepare the output
  VolumeConstTransformsContainer constTransforms;

  //convert each transform
  for( VolumeTransformsContainer::const_iterator transformIter=transforms.cbegin();
    transformIter!= transforms.cend(); ++transformIter)
  {
    constTransforms.push_back( static_cast< const VolumeTransformType * >(* transformIter) );
  }

  return constTransforms;
}
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace