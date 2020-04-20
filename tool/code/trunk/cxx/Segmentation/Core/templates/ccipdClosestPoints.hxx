#ifndef __ccipdClosestPoints_hxx
#define __ccipdClosestPoints_hxx



//////////////////////////////////////////////////////////////////////////////////////////
// ccipd includes
#include "ccipdCore.h"
#include "ccipdClosestPoints.h"
#include "ccipdSampleImage.h"
#include "ccipdLevelsets.h"
#include "ccipdMaskToLevelset.h"
#include "ccipdLevelsetToSurface.h"
#include "ccipdResizeImage.h"
#include "ccipdNormalDirectionCalculator.h"
#include "ccipdClosestPoint.h"

// std includes
#include <unordered_map>

#include "ccipdDisableWarningsMacro.h"
  // itk includes
  #include <itkImage.h>
  #include <itkPointSet.h>
  #include <itkImageRegionConstIteratorWithIndex.h> // for looping
#include "ccipdEnableWarningsMacro.h"
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{
  


//////////////////////////////////////////////////////////////////////////////////////////
template< typename TLandmarksType >
itk::SmartPointer< TLandmarksType >
GetClosestSurfacePoints(
  const TLandmarksType & inputPoints,
  const itk::SmartPointer< const
    itk::Image< MaskPixelType, TLandmarksType::PointDimension >
  > & mask )
{

  typedef itk::SmartPointer< TLandmarksType > OutputPointer;
  if ( !mask ) return OutputPointer();

  // create our normal calculator
  typedef NormalDirectionCalculator<
    TLandmarksType::PointDimension
  > CalculatorType;

  const typename CalculatorType::ConstPointer calculator =
    CalculatorType::ConstNew( mask, false /* verbose */ );

  if ( !calculator ) return OutputPointer();

  return GetClosestSurfacePoints( inputPoints, *calculator );

} // GetClosestSurfacePoints
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
template< typename TLandmarksType >
itk::SmartPointer< TLandmarksType >
GetClosestSurfacePoints(
  const TLandmarksType & inputPoints,
  const NormalDirectionCalculator<
    TLandmarksType::PointDimension
  > & calculator )
{

  // prepare the output
  typedef itk::SmartPointer< TLandmarksType > OutputLandmarksPointer;
  const OutputLandmarksPointer outputPoints = TLandmarksType::New();
  outputPoints->Initialize();

  // loop through each point
  const auto points = inputPoints.GetPoints();
  if ( points )
    {

    const auto inputEnd = points->End();
    unsigned int outputPointIndex = 0;
    for ( auto pointIterator = points->Begin();
      pointIterator != inputEnd;
      ++pointIterator, ++outputPointIndex )
      {

      // calculate the closest point
      const typename TLandmarksType::PointType
        inputPoint  = pointIterator.Value();
      const auto
        outputPoint = GetClosestSurfacePoint( inputPoint, calculator );

      // add it into our output
      outputPoints->SetPoint( outputPointIndex, outputPoint );

      } // for point iterator

    } // points

  // output
  return outputPoints;

} // GetClosestSurfacePoints
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
template< typename TLandmarkType, typename TMaskPixelType, unsigned int TDimension >
itk::SmartPointer< itk::PointSet<
  TLandmarkType,
  TDimension,
  itk::DefaultStaticMeshTraits<
    TLandmarkType,
    TDimension,
    TDimension, 
    TLandmarkType,
    TLandmarkType,
    TLandmarkType
> > >
GetClosestPoints(
  const itk::PointSet<
    TLandmarkType,
    TDimension,
    itk::DefaultStaticMeshTraits<
      TLandmarkType,
      TDimension,
      TDimension,
      TLandmarkType,
      TLandmarkType,
      TLandmarkType > >                            &       inputPoints,
  const itk::Image< TMaskPixelType, TDimension >   * const inputMask,
  const itk::Image< TMaskPixelType, TDimension >   * const outputMask )
{

  // we first calculate a levelset of the input and output masks
  typedef ccipd::InternalPixelType LevelsetValueType;
  typedef itk::Image< LevelsetValueType, TDimension > LevelsetImageType;
  
    const typename LevelsetImageType::ConstPointer inputLevelset(MaskToLevelset< InternalPixelType, MaskPixelType, VolumeDimension>(inputMask, false, false, 4.0, 0.0, false));																
	const typename LevelsetImageType::ConstPointer outputLevelset(MaskToLevelset< InternalPixelType, MaskPixelType, VolumeDimension>(outputMask, false, false, 4.0, 0.0, true));
  
  typedef itk::PointSet< TLandmarkType, TDimension >  PointSetType;
  typedef typename PointSetType::PointsContainer      PointsContainerType;
  typedef typename PointsContainerType::Pointer       PointsContainerPointer;
  typedef typename PointsContainerType::ConstPointer  PointsContainerConstPointer;
  
  // prepare the output
  const PointsContainerPointer outputPointsContainer =
    PointsContainerType::New();

  const PointsContainerConstPointer
    inputPointsContainer = inputPoints.GetPoints();

  const typename PointsContainerType::ElementIdentifier
    numberOfPoints = inputPointsContainer->Size();
  outputPointsContainer->Reserve( numberOfPoints );

  // now we loop through each point

  const typename PointsContainerType::ConstIterator
    inputPointsEnd = inputPointsContainer->End();
  typename PointsContainerType::ConstIterator
    inputPointIter = inputPointsContainer->Begin();

  // we will store the levelset value for each point
  typedef typename LevelsetImageType::PointType PointType;
  typedef std::vector< PointType >              LevelsetPointsContainer;
  LevelsetPointsContainer inputLevelsetPoints;

  for ( ; inputPointIter != inputPointsEnd; ++inputPointIter )
    {
    // store that location
    inputLevelsetPoints.push_back( inputPointIter->Value() );
    } // for inputPoint

  // now sample the levelset image at those points
  typedef std::vector< LevelsetValueType > LevelsetValueContainer;
  LevelsetValueContainer inputLevelsetValues;
  inputLevelsetValues.resize( numberOfPoints );
  const bool extrapolate = true;
  SampleImage(
    inputLevelsetPoints,
    inputLevelset.GetPointer(),
    inputLevelsetValues.begin(),
    extrapolate );

  // we will loop through the output
  typename PointsContainerType::Iterator
    outputPointIter = outputPointsContainer->Begin();

  // in parallel, we will loop through the levelset values
  const typename LevelsetValueContainer::const_iterator
    inputLevelsetValuesEnd = inputLevelsetValues.cend();

  typename LevelsetPointsContainer::const_iterator
    levelsetPointsIter = inputLevelsetPoints.cbegin();

  // we will cache the mask values
  typedef itk::Image< TMaskPixelType, TDimension > MaskImageType;
  typedef typename MaskImageType::ConstPointer MaskConstPointer;
  typedef std::unordered_map<
    LevelsetValueType,
    MaskConstPointer
  > MaskCacheType;
  MaskCacheType maskCache;

  inputPointIter = inputPointsContainer->Begin();
  for ( LevelsetValueContainer::const_iterator
    inputLevelsetValueIter = inputLevelsetValues.begin();
    inputLevelsetValueIter != inputLevelsetValuesEnd;
    ++inputLevelsetValueIter, ++levelsetPointsIter, ++outputPointIter )
    {

    // extract the levelset value
    // to a 1-pixel precision
    const LevelsetValueType levelsetPrecision =
#ifdef NDEBUG
      static_cast< LevelsetValueType >( inputLevelset->GetSpacing()[ 0 ] );
#else
      1e-1f;
#endif
    const LevelsetValueType levelsetValue =
      static_cast< LevelsetValueType >(
        floor( *inputLevelsetValueIter / levelsetPrecision + 0.5 )
          * levelsetPrecision );

    // get the surface points at that value
    // we now need to make that levelset value equal to 0
    // on the output levelset image

    const TMaskPixelType
      backgroundValue = 0,
      foregroundValue = 1;

    // see if it's in the cache
    MaskConstPointer cachedMask = maskCache[ levelsetValue ];
    if ( !cachedMask )
      {

      // extract the mask image
      const MaskConstPointer newMaskImage(
        GetSurfaceImage< LevelsetValueType, TMaskPixelType >(
          outputLevelset.GetPointer(),
          levelsetValue,
          backgroundValue,
          foregroundValue ) );

      // delete large caches
      const std::size_t maxCacheSize = 100;
      if ( maskCache.size() > maxCacheSize )
        maskCache.clear();

      // update the cache
      maskCache[ levelsetValue ] = newMaskImage;

      } // cached mask

    const MaskConstPointer maskImage = maskCache[ levelsetValue ];

    // okay, look through every pixel in that surface
    typedef itk::ImageRegionConstIteratorWithIndex<
      MaskImageType
    > MaskIteratorType;

    MaskIteratorType maskIterator(
      maskImage, maskImage->GetBufferedRegion() );

    typedef typename PointType::ValueType PointDistanceType;

    // start out really far away
    PointType         closestPoint( 0.0 );
    PointDistanceType closestPointDistance = 
      std::numeric_limits< PointDistanceType >::max();

    for ( ; !maskIterator.IsAtEnd(); ++maskIterator )
      {

      // see if this was found or else skip it
      if ( maskIterator.Get() == backgroundValue )
        continue;

      // get the index
      const auto currentPixelIndex = maskIterator.GetIndex();

      // get the point
      PointType currentPixelPoint;
      maskImage->TransformIndexToPhysicalPoint(
        currentPixelIndex, currentPixelPoint );

      // get the distance
      PointDistanceType currentPointDistance = 0;
      for ( unsigned int dimension = 0; dimension < TDimension; ++dimension )
        {
        const PointDistanceType signedDistance =
          currentPixelPoint[ dimension ] - ( *levelsetPointsIter )[ dimension ];

        currentPointDistance += ( signedDistance * signedDistance );

        } // dimension

      // if we found a closer point, use it
      if ( currentPointDistance < closestPointDistance )
        {
        closestPointDistance = currentPointDistance;
        closestPoint = currentPixelPoint;
        }

      } // maskIterator

    // now that we have the closest point, assign it
    // to the output
    outputPointIter->Value() = closestPoint;
    
    } // for inputLevelsetValueIter and outputPointIter

  // output
  const typename PointSetType::Pointer outputPoints = PointSetType::New();
  outputPoints->SetPoints( outputPointsContainer );
  return outputPoints;

} // GetClosestPoints
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace ccipd
#endif // __ccipdClosestPoints_hxx
