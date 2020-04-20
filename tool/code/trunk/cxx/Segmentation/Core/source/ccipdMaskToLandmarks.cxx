


////////////////////////////////////////////////////////////////////////////////////////////////////
// ccipd includes
#include "ccipdMaskToLandmarks.h"
#include "ccipdLevelsets.h"   // the distance to the surface decides on the pixel
#include "ccipdMaskToLevelset.h"
#include "ccipdBoundingBox.h" // we don't need to search the whole image
#include "ccipdCropImage.h"

#include "ccipdDisableWarningsMacro.h"
  // itk includes
  #include <itkImageRandomConstIteratorWithIndex.h> // for iterating through pixels
  #include <itkPointSet.h>
#include "ccipdEnableWarningsMacro.h"
////////////////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{
	////////////////////////////////////////////////////////////////////////////////////////////////////
LandmarkSetContainerPointer ExtractLandmarkContainerFromMask(
  const VolumeMaskImageType  *                  const mask,
        VectorIndexContainer &                  borderLandmarkIndices,
  const LandmarkSetContainer::ElementIdentifier numberOfBorderLandmarks,
  const LandmarkSetContainer::ElementIdentifier numberOfInternalLandmarks,
  const InternalPixelType                       borderWidth )
{

  // we don't need to sample the whole image, just near the mask
  const VolumeRegionType maskRegion = GetBoundingBox( mask, borderWidth * 2 );

  // crop the mask
  const VolumeMaskImageConstPointer croppedMask = CropMask( mask, maskRegion ).GetPointer();

  // extract a levelset
  const VolumeImagePointer levelset = MaskToLevelset< InternalPixelType, MaskPixelType, VolumeDimension>(croppedMask, false, false, 4.0, 0.0, false );

  // we will iterate through the points in the levelset
  typedef itk::ImageRandomConstIteratorWithIndex<VolumeImageType> IteratorType;

  // we will randomly sample in the mask region
  IteratorType imageIterator( levelset, levelset->GetBufferedRegion() );
  imageIterator.GoToBegin();

  // we will keep going until we have sampled this many
  LandmarkSetContainer::ElementIdentifier
    numberOfBorderLandmarksSoFar   = 0,
    numberOfInternalLandmarksSoFar = 0;

  // prepare the output
  LandmarkSetContainerPointer landmarks = LandmarkSetContainer::New();
  landmarks->Reserve( numberOfBorderLandmarks + numberOfInternalLandmarks );

  // we will iterate through the output
  typedef LandmarkSetContainer::Iterator LandmarkIteratorType;
  LandmarkIteratorType landmarkIterator = landmarks->Begin();
  
  // prepare the collection of the landmarks on the surface
  borderLandmarkIndices.clear();

  // initialize variables
  bool allDone = false;
  VectorIndexType currentLandmarkIndex = 0;
   
  // loop
  while ( !allDone )
    {

    // which pixels do we still need?
    const bool
      needBorderPixels   = numberOfBorderLandmarksSoFar   != numberOfBorderLandmarks,
      needInternalPixels = numberOfInternalLandmarksSoFar != numberOfInternalLandmarks;

    // extract the levelset value
    const InternalPixelType levelsetValue = imageIterator.Get();

    typedef IteratorType::IndexType IndexType;

    // how will we use the current pixel?
    const bool
      borderPixel   = needBorderPixels && std::abs( levelsetValue ) < borderWidth,
      internalPixel = !borderPixel && needInternalPixels && levelsetValue < 0,
      usingPixel    = borderPixel || internalPixel;

    // update our counts
    if ( borderPixel )
      ++numberOfBorderLandmarksSoFar;

    if ( internalPixel )
      ++numberOfInternalLandmarksSoFar;

    // if we are using the pixel, add it to our list
    if ( usingPixel )
      {
      // extract the index
      IndexType index = imageIterator.GetIndex();
      LandmarkPointType landmarkPoint;
      levelset->TransformIndexToPhysicalPoint( index, landmarkPoint );

      // add it to our list
      landmarkIterator->Value() = landmarkPoint;

      if ( borderPixel )
        {
        // we must add it to our collection of
        // surface landmarks

        // add it to our collection and increment
        borderLandmarkIndices.insert( currentLandmarkIndex );

        } // borderPixel

      // increment everything
      ++currentLandmarkIndex;
      ++landmarkIterator;

      } // usingPixel
     
    // have we finished everything?
    allDone =
      numberOfBorderLandmarksSoFar   == numberOfBorderLandmarks &&
      numberOfInternalLandmarksSoFar == numberOfInternalLandmarks;

    // iterate!
    ++imageIterator;

    } // while not done

  return landmarks;

} // ExtractLandmarksFromMask (with surface indices)
////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////
LandmarkSetContainerPointer ExtractLandmarkContainerFromMask(
  const VolumeMaskImageType  * const mask,
  const LandmarkSetContainer::ElementIdentifier numberOfBorderLandmarks,
  const LandmarkSetContainer::ElementIdentifier numberOfInternalLandmarks,
  const InternalPixelType            borderWidth )
{

  // just create a temporary, unused variable
  VectorIndexContainer borderLandmarkIndices; 

  return ExtractLandmarkContainerFromMask(
    mask,
    borderLandmarkIndices,
    numberOfBorderLandmarks,
    numberOfInternalLandmarks,
    borderWidth );

} // ExtractLandmarkContainerFromMask (without border indices)
////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////
LandmarkSetPointer ExtractLandmarkSetFromMask(
  const VolumeMaskImageType * const mask,
  const LandmarkSetContainer::ElementIdentifier numberOfBorderLandmarks,
  const LandmarkSetContainer::ElementIdentifier numberOfInternalLandmarks,
  const InternalPixelType           borderWidth )
{

  // just create a temporary, unused variable
  VectorIndexContainer borderLandmarkIndices; 

  return ExtractLandmarkSetFromMask(
    mask,
    borderLandmarkIndices,
    numberOfBorderLandmarks,
    numberOfInternalLandmarks,
    borderWidth );

} // ExtractLandmarkSetFromMask (without border indices)
////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////
LandmarkSetPointer ExtractLandmarkSetFromMask(
  const VolumeMaskImageType  * const mask,
        VectorIndexContainer &       borderLandmarkIndices,
  const LandmarkSetContainer::ElementIdentifier numberOfBorderLandmarks,
  const LandmarkSetContainer::ElementIdentifier numberOfInternalLandmarks,
  const InternalPixelType            borderWidth )
{

  // extract the landmarks
  const LandmarkSetContainerPointer landmarkData =
    ExtractLandmarkContainerFromMask(
      mask,
      borderLandmarkIndices,
      numberOfBorderLandmarks,
      numberOfInternalLandmarks,
      borderWidth );

  // give the landmarks to a LandmarkSet
  const LandmarkSetPointer landmarkSet = LandmarkSetType::New();
  landmarkSet->SetPoints( landmarkData );

  // return the LandmarkSet
  return landmarkSet;

} // ExtractLandmarksFromMask
////////////////////////////////////////////////////////////////////////////////////////////////////



} // namespace ccipd
