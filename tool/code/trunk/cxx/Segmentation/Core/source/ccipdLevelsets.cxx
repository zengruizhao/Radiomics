
 

//////////////////////////////////////////////////////////////////////////////////////////
// ccipd includes
#include "ccipdLevelsets.h"
#include "ccipdResizeImage.h"               // for resizing masks and levelsets
#include "ccipdApplyTransformation.h"
#include "ccipdSaveImage.h"

 // our own itk includes
#include "ccipdDisableWarningsMacro.h"
#include "itkMaskToLevelsetFilter.h"       // for computing the levelset

// boost includes
#include <boost/foreach.hpp>

#include "ccipdDisableWarningsMacro.h"
  // itk includes
  #include <itkBinaryThresholdImageFilter.h> // for compute the mask
  #include <itkImageSliceIteratorWithIndex.h>
  #include <itkPointSet.h>
#include "ccipdEnableWarningsMacro.h"
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{

//////////////////////////////////////////////////////////////////////////////////////////
VolumeImagePointer CalculateLevelsetFromMask(
  const VolumeMaskImageType * mask,
  const bool                  normalize,
  const bool                  sigmoid,
  const float                 sigmoidWidth,
  const float                 smoothingVariance )
{

  auto levelsetFilter = itk::MaskToLevelsetFilter<
    VolumeMaskImageType, VolumeImageType >::New();

  // there are issues with anisotropic voxels,
  // so we first make sure all the voxels are isostropic
  const VolumeMaskImageConstPointer resizedMask =
#ifdef NDEBUG
    static_cast<const ccipd::VolumeMaskImageType *>(
      ResizeMaskIsotropically( mask ) );
#else
    mask;
#endif

  const bool smooth = smoothingVariance > 0;

  levelsetFilter->SetInput            ( resizedMask       );
  levelsetFilter->SetNormalize        ( normalize         );
  levelsetFilter->SetSigmoid          ( sigmoid           );
  levelsetFilter->SetSigmoidWidth     ( sigmoidWidth      );
  levelsetFilter->SetSmooth           ( smooth            );
  levelsetFilter->SetSmoothingVariance( smoothingVariance );

  levelsetFilter->Update();

  const VolumeImagePointer
    levelset = levelsetFilter->GetOutput(),
    // resize the levelset back to the correct voxel sizes
    resizedLevelset = ResizeImage( levelset, mask );

  return resizedLevelset;

} // CalculateLevelsetFromMask
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
VolumeImagesContainer CalculateLevelsetsFromMasks(
  const VolumeConstMaskImagesContainer masks,
  const bool                        normalize,
  const bool                        sigmoid,
  const float                       sigmoidWidth,
  const float                       smoothingVariance )
{

  // prepare our output
  VolumeImagesContainer levelsets;
  levelsets.reserve( masks.size() );

  // go through and calculate
  BOOST_FOREACH( const auto & mask, masks )
    levelsets.push_back(
      CalculateLevelsetFromMask(
        mask,
        normalize,
        sigmoid,
        sigmoidWidth,
        smoothingVariance ) );

  // output
  return levelsets;

} // CalculateLevelsetsFromMasks
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
VolumeMaskImagePointer CalculateMaskFromLevelset(
  const VolumeImageType * levelset,
  const float threshold )
{
  // create a thresholder
  typedef itk::BinaryThresholdImageFilter<
    VolumeImageType, VolumeMaskImageType > ThresholdFilter;
  ThresholdFilter::Pointer thresholder = ThresholdFilter::New();

  // give it our distance
  thresholder->SetInput( levelset );

  // tell it how to threshold
  const MaskPixelType
    trueValue  = 1, // in the future, this could represent an organ's "mask value"
    falseValue = 0;
  thresholder->SetUpperThreshold( threshold  ); // anything <= threshold is converted
  thresholder->SetInsideValue   ( trueValue  ); // to a "1".
  thresholder->SetOutsideValue  ( falseValue ); // anything > 0 is a "0"

  // calculate and output
  thresholder->Update();
  return thresholder->GetOutput();

}

//////////////////////////////////////////////////////////////////////////////////////////
VolumeImagePointer UpdateLevelset(
  const VolumeImageType     * levelset,
  const float               threshold,
  const bool                normalize,
  const bool                sigmoid,
  const float               sigmoidWidth,
  const float               smoothingVariance)
{

  // in release mode make the levelset isotropic
  // so thresholding is more accurate,
  // rather than thresholding, and then
  // making the resulting mask isotropic
  // (better interpolation this way)
  VolumeImageConstPointer isotropicLevelset =
#ifdef NDEBUG
    ResizeImageIsotropically( levelset, false, 3.0).GetPointer();
#else
    levelset; // don't waste time in debug mode
#endif // NDEBUG

  // then, threshold to get a mask
  const VolumeMaskImagePointer isotropicMask =
    CalculateMaskFromLevelset( isotropicLevelset, threshold );
  
  // recompute the levelset from this mask
  isotropicLevelset = CalculateLevelsetFromMask(
    isotropicMask, normalize, sigmoid, sigmoidWidth, smoothingVariance );

  // resize back to its original, correct size
  const VolumeImagePointer anisotropicLevelset = ResizeImage(isotropicLevelset, levelset);

  // output
  return anisotropicLevelset;

} // UpdateLevelset
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
MedialAxisSetPointer CalculateSimpleMedialAxisFromLevelset(
  const VolumeImageType * levelset)
{

  //prepare PointsContainer
  MedialAxisSetPointer            pointSet        = MedialAxisSetType     ::New();
  MedialAxisSetContainer::Pointer MedialAxis      = MedialAxisSetContainer::New();
  MedialAxisPointType             MedialPoint;

	//slice interator
  typedef itk::ImageSliceIteratorWithIndex< const ccipd::VolumeImageType > IteratorType;
  IteratorType it( levelset, levelset->GetLargestPossibleRegion() );

  it.GoToBegin();
  it.SetFirstDirection( 0 );  // 0=x, 1=y, 2=z
  it.SetSecondDirection( 1 ); // 0=x, 1=y, 2=z

  VolumeImageType::IndexType indexMin;
  float min;
  while( !it.IsAtEnd() ) // going through slice on z
  {
    min = 0;
    indexMin[0] = -1;
    indexMin[1] = -1;
    indexMin[2] = -1;
    while( !it.IsAtEndOfSlice() )
    {
      while( !it.IsAtEndOfLine() )
      {

        if (it.Get() < min)
        {
          min = it.Get();
          indexMin = it.GetIndex();
        }
        ++it;
      }
      it.NextLine();
    }

		for ( unsigned int i = 0; i < 3; ++i )
      MedialPoint[i] = static_cast< float >(
        indexMin[i]*levelset->GetSpacing()[i] + levelset->GetOrigin()[i] );
    MedialPoint[3] = min;
    MedialAxis->InsertElement (it.GetIndex()[2], MedialPoint );
    it.NextSlice();
  }

  pointSet->SetPoints(MedialAxis);

  return pointSet;

}
//////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////
LandmarkSetPointer ExtractSimpleMedialAxisIntoLandmark(
  const VolumeImageType * levelset)
{

  //prepare PointsContainer
  LandmarkSetPointer            	pointSet        = LandmarkSetType     ::New();
  LandmarkSetContainer::Pointer 	MedialAxis      = LandmarkSetContainer::New();
  LandmarkPointType             	MedialPoint;

  //slice interator
  typedef itk::ImageSliceIteratorWithIndex< const ccipd::VolumeImageType > IteratorType;
  
  IteratorType it( levelset, levelset->GetLargestPossibleRegion() );
  it.GoToBegin();
  it.SetFirstDirection( 0 );  // 0=x, 1=y, 2=z
  it.SetSecondDirection( 1 ); // 0=x, 1=y, 2=z

  VolumeImageType::IndexType indexMin;
  VolumeImageType::IndexType indexAvg;
  double min;
  int 
    countPointsAdded = 0,
    countPointsAvg;
  while( !it.IsAtEnd() ) // going through slice on z
  {
    min = 1e-6;  //very very tiny number so that 0 can be considered as part of the; // should also eliminate the points at the corner
    indexMin[0] = -1;
    indexMin[1] = -1;
    indexMin[2] = -1;
    //it.GoToBeginOfSlice();
    while( !it.IsAtEndOfSlice() )
    {
      while( !it.IsAtEndOfLine() )
      {

        if (it.Get() <= min)
        {
          min = it.Get();
          indexMin = it.GetIndex();
        }
        ++it;
      }
      it.NextLine();
    }

    if (min <= 0  && indexMin[0]>=0 && indexMin[1]>=0 && indexMin[2]>=0 ) // within surface
    {
      countPointsAvg = 0;
      indexAvg[0] = 0;
      indexAvg[1] = 0;
      indexAvg[2] = 0;

      it.GoToBeginOfSlice();
      while( !it.IsAtEndOfSlice() )
      {
        while( !it.IsAtEndOfLine() )
        {

          if (it.Get() == min)
          {
            indexMin = it.GetIndex();
            indexAvg[0] += indexMin[0];
            indexAvg[1] += indexMin[1];
            indexAvg[2] += indexMin[2];
            countPointsAvg++;
          }
          ++it;
        }
        it.NextLine();
      }

      if (countPointsAvg>0)
      {
        indexAvg[0] /= countPointsAvg;
        indexAvg[1] /= countPointsAvg;
        indexAvg[2] /= countPointsAvg;
      }
      else
      {
        std::cout << "Ups!!! Division by zero!! No Point found? " <<  std::endl;
        indexAvg[0] = -1;
        indexAvg[1] = -1;
        indexAvg[2] = -1;
      }

      for ( unsigned int i = 0; i < 3; ++i )
        MedialPoint[i] = static_cast< float >(
          indexAvg[i]*levelset->GetSpacing()[i] + levelset->GetOrigin()[i] );
      MedialAxis->InsertElement (countPointsAdded, MedialPoint );
      countPointsAdded++;
    }
    it.NextSlice();
  }

  pointSet->SetPoints(MedialAxis);

  return pointSet;
}
//////////////////////////////////////////////////////////////////////////////////////////

} // namespace
