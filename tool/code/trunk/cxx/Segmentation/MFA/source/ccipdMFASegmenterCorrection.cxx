


//////////////////////////////////////////////////////////////////////////////////////////

// ccipd includes
#include "ccipdMFASegmenterCorrection.h"
#include "ccipdClosestPoints.h"
#include "ccipdApplyTransformation.h"
#include "ccipdThinPlateSplines.h"
#include "ccipdConcatenateLandmarks.h"
#include "ccipdGetCornerPoints.h"
#include "ccipdMFALevelsetExtractor.h"
#include "ccipdBoundingBoxCreation.h"
#include "ccipdBoundingBox.h"
#include "ccipdUniformDistribution.h"
#include "ccipdMaskToLandmarks.h" // for surface points
#include "ccipdDuplicateLandmarks.h"

// std includes
#include <unordered_map>

#include "ccipdDisableWarningsMacro.h"

// itk includes
#include <itkImage.h>
#include <itkTransform.h>
#include <itkPointSet.h>

// boost includes
#include <boost/foreach.hpp>

#ifdef Boost_SERIALIZATION_FOUND
  #include "ccipdUnorderedMapSerialization.h"
  #include <boost/serialization/nvp.hpp>
  #include <boost/archive/xml_iarchive.hpp>
  #include <boost/archive/xml_oarchive.hpp>
#endif // Boost_SERIALIZATION_FOUND

#include "ccipdEnableWarningsMacro.h"

//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
using std::cout;
using std::endl;
using std::cerr;
using std::string;
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////
class MFASegmenterCorrection::SecretData
{

public:
  SecretData(); ///< Constructor

  typedef std::unordered_map<
    MaskObjectType,
    LandmarkSetConstPointer
  > PointsMap;

  bool
    m_FixCorners;

  unsigned int
    m_NumberOfRandomFixedPoints;

  typedef std::unordered_map<
    MaskObjectType,
    unsigned int
  > NumberOfPointsMap;

  NumberOfPointsMap
    m_NumberOfAnchorSurfacePoints;

  TransformValueType
    m_ForbiddenRadius;

  PointsMap
    m_KnownSurfacePoints;

  LandmarkSetConstPointer
    m_RandomFixedPoints;

  VolumeTransformConstPointer
    m_CorrectionTransform;

}; // class SecretData
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
MFASegmenterCorrection::MFASegmenterCorrection() :
ccipdNewPIMPLMacro
{
}
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
MFASegmenterCorrection::~MFASegmenterCorrection()
{
}
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
MFASegmenterCorrection::SecretData::SecretData() :
m_FixCorners( true ),
m_NumberOfRandomFixedPoints  ( 0 ),
m_NumberOfAnchorSurfacePoints( 0 ),
m_ForbiddenRadius( 3 )
{
}
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
bool MFASegmenterCorrection::Load( const std::string & fileName )
{
  return this->SuperclassSerializable::Load( fileName );
}

bool MFASegmenterCorrection::Save( const std::string & fileName ) const
{
  return this->SuperclassSerializable::Save( fileName );
}
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
VolumeImagePointer MFASegmenterCorrection::GetReconstructionWithoutCorrection(
  const FeatureIDConstPointer & whichFeature ) const
{
  return this->Superclass::GetReconstruction( whichFeature );
}
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
VolumeImagePointer MFASegmenterCorrection::GetReconstructedLevelsetWithoutCorrection(
  const MaskObjectType & whichObject ) const
{

  // as per our convention, the object name is the description of the feature ID
  const FeatureIDConstPointer whichFeature =
    MFALevelsetExtractor::CreateFeatureIDWithObject( whichObject );
  
  // Calculate the reconstruction for that feature
  return this->GetReconstructionWithoutCorrection( whichFeature );

} // GetReconstructedLevelsetWithoutCorrection
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
VolumeMaskImagePointer MFASegmenterCorrection::GetSegmentationWithoutCorrection(
  const MaskObjectType & whichObject ) const
{

  // temporarily remove the transform
  const VolumeTransformConstPointer
    correctionTransform = this->m_SecretData->m_CorrectionTransform;
  this->m_SecretData->m_CorrectionTransform = VolumeTransformConstPointer();
  
  // output the segmentation now
  const VolumeMaskImagePointer
    segmentation = this->Superclass::GetSegmentation( whichObject );

  // restore the transform
  this->m_SecretData->m_CorrectionTransform = correctionTransform;

  // output
  return segmentation;

} // GetSegmentationWithoutCorrection
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
VolumeImagePointer MFASegmenterCorrection::GetReconstruction(
  const FeatureIDConstPointer & whichFeature ) const
{

  const bool verbose = this->GetVerbose();

  // get the original, uncorrected reconstruction image
  if ( verbose ) cout << "Getting original reconstruction:" << endl;
  const VolumeImagePointer originalReconstruction =
    this->GetReconstructionWithoutCorrection( whichFeature );
  if ( verbose ) cout << "Getting original reconstruction done." << endl;

  // check if we have a correct transformation to apply
  const VolumeTransformConstPointer
    correctionTransform = this->m_SecretData->m_CorrectionTransform;

  if ( correctionTransform )
    {

    // yes we do - apply it

    if ( verbose ) cout << "Correcting reconstruction:" << endl;

    const bool extrapolate = true;
    const VolumeImagePointer correctedReconstruction = 
      ApplyTransform(
        correctionTransform,    // tps output
        originalReconstruction, // input
        originalReconstruction, // template
        extrapolate );

    if ( verbose ) cout << "Correcting reconstruction done." << endl;

    // output
    return correctedReconstruction;

    } // correctionTransform
  else
    {
    
    // no correction transform;
    // just output the original
    return originalReconstruction;

    } // correctionTransform
  
} // GetReconstruction
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void MFASegmenterCorrection::RegenerateCorrection()
{

  const bool verbose = this->GetVerbose();

  // the fixed are the points given on the mask
  // the moving are the points from the segmentation output
  LandmarkSetConstPointer
    fixedLandmarks, movingLandmarks, anchorPoints;

  const unsigned int
    numberOfRandomFixedPoints = this->GetNumberOfRandomFixedPoints();
  const bool
    fixCorners     = this->GetFixCorners(),
    randomlySample = numberOfRandomFixedPoints > 0;

  // loop through each mask
  if ( verbose ) cout << "Getting all segmentation objects:" << endl;
  const MaskObjectCollectionType
    segmentationObjects = this->GetAllSegmentationObjects();
  if ( verbose ) cout << "Getting all segmentation objects done." << endl;

  // where can't we sample random points
  std::vector< VolumeRegionType > forbiddenBoundingBoxes;

  VolumeImageBaseConstPointer anySegmentation;

  BOOST_FOREACH( const MaskObjectType & segmentationObject, segmentationObjects )
    {

    if ( verbose ) cout << "Segmentation object = " << segmentationObject << endl;

    // extract the data for this mask
    const VolumeMaskImageConstPointer
      segmentation( this->GetSegmentationWithoutCorrection(
        segmentationObject ) );
    if ( !segmentation )
      {
      cout << "Skipping " << segmentationObject << " because no segmentation." << endl;
      continue;
      }
    anySegmentation = segmentation;

    // create a bounding box from this segmentation
    if ( randomlySample )
      {
      const VolumeRegionType segmentationBoundingBox =
        GetBoundingBox( segmentation );
      if ( verbose )
        {
        cout << "Bounding box for " <<
          segmentationObject << " = " << endl <<
          segmentationBoundingBox << endl;
        }
      forbiddenBoundingBoxes.push_back( segmentationBoundingBox );

      } // randomlySample

    const unsigned int numberOfAnchorSurfacePoints =
      this->GetNumberOfAnchorSurfacePoints( segmentationObject );

    // get random points on the surface
    if ( numberOfAnchorSurfacePoints > 0 )
      {

      if ( verbose )
        cout << "Sampling surface for " <<
          segmentationObject << ":" << endl;

      const unsigned int numberOfInternalPoints = 0;
      const InternalPixelType borderWidth = 1; // mm 
      const LandmarkSetConstPointer surfacePoints(
        ExtractLandmarkSetFromMask(
          segmentation,
          numberOfAnchorSurfacePoints,
          numberOfInternalPoints,
          borderWidth ) );

      // add them to our fixed points
      anchorPoints = ConcatenateLandmarkSets( anchorPoints, surfacePoints );

      if ( verbose ) cout << "Sampling surface done." << endl;

      } // surfaceSample

    // get known landmarks
    const LandmarkSetConstPointer
      knownLandmarks( this->GetKnownSurfacePoints( segmentationObject ) );
    if ( !knownLandmarks )
      {
      cout << "Skipping " << segmentationObject << " because no known landmarks." << endl;
      continue;
      }
    if ( !knownLandmarks ) continue;

    // get the closest landmarks
    const LandmarkSetConstPointer
      closestLandmarks( GetClosestSurfacePoints( *knownLandmarks, segmentation ) );
    if ( !closestLandmarks )
      {
      cout << "Skipping " << segmentationObject << " because no closest landmarks." << endl;
      continue;
      }
    if ( !closestLandmarks ) continue;

    // add them into our collection
    if ( verbose )
      cout << "Note: including landmarks from " << segmentationObject << endl;

    // add them into our current set
    fixedLandmarks  = ConcatenateLandmarkSets( fixedLandmarks , knownLandmarks   );
    movingLandmarks = ConcatenateLandmarkSets( movingLandmarks, closestLandmarks );
    
    } // for each segmentation object

  // add in other fixed points in the image

  // we first need to know where we can't sample
  // so we create a region of our landmarks
  if ( anySegmentation && randomlySample )
    {

    const LandmarkSetConstPointer allLandmarks(
      ConcatenateLandmarkSets( fixedLandmarks, movingLandmarks ) );

    if ( allLandmarks )
      {
      const VolumeRegionType landmarksBoundingBox =
        CreateBoundingBox( *allLandmarks, *anySegmentation );
      if ( verbose )
        cout << "Landmarks bounding box = " << endl << landmarksBoundingBox << endl;

      forbiddenBoundingBoxes.push_back( landmarksBoundingBox );
        
      } // allLandmarks

    // get the largest bounding box
    const VolumeRegionType boundingBoxToSample =
      anySegmentation->GetBufferedRegion();

    // now we randomly sample indices
    typedef UniformDistribution< unsigned int > DistributionType;
    const DistributionType::Pointer
      xDistribution = DistributionType::New(),
      yDistribution = DistributionType::New(),
      zDistribution = DistributionType::New();

    // get the index bounds
    const VolumeImageBase::IndexType
      minIndex = boundingBoxToSample.GetIndex(),
      maxIndex = boundingBoxToSample.GetUpperIndex();

    if ( verbose )
      cout << "Min Index = " << minIndex << endl <<
        "Max Index = " << maxIndex << endl;

    xDistribution->SetMinimumValue(
      static_cast< unsigned int >( minIndex[ 0 ] ) );
    yDistribution->SetMinimumValue(
      static_cast< unsigned int >( minIndex[ 1 ] ) );
    zDistribution->SetMinimumValue(
      static_cast< unsigned int >( minIndex[ 2 ] ) );
    
    xDistribution->SetMaximumValue(
      static_cast< unsigned int >( maxIndex[ 0 ] ) );
    yDistribution->SetMaximumValue(
      static_cast< unsigned int >( maxIndex[ 1 ] ) );
    zDistribution->SetMaximumValue(
      static_cast< unsigned int >( maxIndex[ 2 ] ) );

    const LandmarkSetPointer randomFixedPoints = LandmarkSetType::New();
    
    if ( verbose ) cout << "Maximum random points = "
      << numberOfRandomFixedPoints << endl;

    unsigned int currentNumberOfRandomPoints = 0;
    for ( unsigned int whichTry = 0;
      whichTry < ( numberOfRandomFixedPoints * 10000 ) &&
      currentNumberOfRandomPoints < numberOfRandomFixedPoints;
      ++whichTry )
      {

      // get a random index
      VolumeImageBase::IndexType randomIndex;
      randomIndex[ 0 ] = xDistribution->GetRandomValue();
      randomIndex[ 1 ] = yDistribution->GetRandomValue();
      randomIndex[ 2 ] = zDistribution->GetRandomValue();

      // see if it's in any of the forbidden bounding boxes
      bool validPoint = true;
      BOOST_FOREACH( const VolumeRegionType
        & forbiddenBoundingBox, forbiddenBoundingBoxes )
        {
        if ( !validPoint || forbiddenBoundingBox.IsInside( randomIndex ) )
          {
          validPoint = false;
          }
        } // for each forbidden bounding box

      // if it's valid, use it
      if ( validPoint )
        {

        // convert it to a point
        VolumeImageBase::PointType randomPoint;
        anySegmentation->TransformIndexToPhysicalPoint(
          randomIndex, randomPoint );

        randomFixedPoints->SetPoint( currentNumberOfRandomPoints++, randomPoint );
        } // validPoint

      } // whichTry
      
      // add in random points
      if ( verbose )
        cout << "Including " <<
        currentNumberOfRandomPoints << " random fixed points." << endl;

      anchorPoints = ConcatenateLandmarkSets( anchorPoints, randomFixedPoints );

      // fix the corner points
      if ( fixCorners )
        {

        if ( verbose ) cout << "Adding corner points:" << endl;

        const LandmarkSetConstPointer
          cornerPoints( GetCornerPoints( *anySegmentation ) );

        anchorPoints = ConcatenateLandmarkSets( anchorPoints, cornerPoints );

        if ( verbose ) cout << "Adding corner points done." << endl;

        } // fix corners

    } // anySegmentation

  if ( anchorPoints && fixedLandmarks )
    {
    // remove anchor points a certain radius from the given points
    const TransformValueType
      forbiddenRadius = this->GetForbiddenRadius(),
      forbiddenRadiusSquared = forbiddenRadius * forbiddenRadius;

    // loop through each anchor point
    const LandmarkSetType::PointIdentifier
      numberOfAnchorPoints = anchorPoints  ->GetNumberOfPoints(),
      numberOfManualPoints = fixedLandmarks->GetNumberOfPoints();
    LandmarkSetType::PointIdentifier
      newAnchorPointIndex = 0;
    const LandmarkSetPointer newAnchorPoints = LandmarkSetType::New();
  
    // reinsert all the points
    for ( LandmarkSetType::PointIdentifier anchorPointIndex = 0;
      anchorPointIndex < numberOfAnchorPoints; ++anchorPointIndex )
      {

      // get the input anchor point
      const LandmarkPointType
        tryingAnchorPoint = anchorPoints->GetPoint( anchorPointIndex );

      // are we a valid point?
      bool validPoint = true;

      // loop through each of our existing fixed and moving points
      for ( LandmarkSetType::PointIdentifier manualPointIndex = 0;
        manualPointIndex < numberOfManualPoints; ++manualPointIndex )
        {

        // we've already excluded this point
        if ( !validPoint ) continue;

        // extract the fixed and moving points
        const LandmarkPointType
          fixedPoint  = fixedLandmarks->GetPoint( manualPointIndex ),
          movingPoint = fixedLandmarks->GetPoint( manualPointIndex );

        // get the distance squared
        const TransformValueType
          fixedDistanceSquared = static_cast< TransformValueType >(
            tryingAnchorPoint.SquaredEuclideanDistanceTo(
              fixedPoint ) ),
          movingDistanceSquared = static_cast< TransformValueType >(
            tryingAnchorPoint.SquaredEuclideanDistanceTo(
              movingPoint ) );

        // see if it's too close to the given points
        if (
          fixedDistanceSquared  < forbiddenRadiusSquared ||
          movingDistanceSquared < forbiddenRadiusSquared )
          {
          validPoint = false;
          }

        } // for manualPointIndex

      if ( validPoint )
        {

        // include it in our final set
        newAnchorPoints->SetPoint(
          newAnchorPointIndex++,
          tryingAnchorPoint );

        } // validPoint

      } // for anchor point index

    // only include the valid points
    anchorPoints = newAnchorPoints;
    if ( verbose )
      cout << "# of valid anchor points = " <<
        anchorPoints->GetNumberOfPoints() << endl;
    } // anchorPoints

  // all anchor points
  fixedLandmarks  = ConcatenateLandmarkSets( fixedLandmarks , anchorPoints );
  movingLandmarks = ConcatenateLandmarkSets( movingLandmarks, anchorPoints );
  this->m_SecretData->m_RandomFixedPoints = anchorPoints;

  // calculate TPS given the landmarks
  if ( verbose ) cout << "Calculating TPS transformation:" << endl;
  const VolumeTransformConstPointer tpsTransform(
    CalculateThinPlateSplines3D( fixedLandmarks, movingLandmarks ) );
  if ( verbose ) cout << "Calculating TPS transformation done." << endl;

  // assign it
  if ( tpsTransform )
    {
    this->m_SecretData->m_CorrectionTransform = tpsTransform;
    }

} // RegenerateCorrection
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void MFASegmenterCorrection::SetKnownSurfacePoints(
    const MaskObjectType          & whichObject,
    const LandmarkSetConstPointer & points )
{
  this->m_SecretData->m_KnownSurfacePoints[ whichObject ] = points;
}
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void MFASegmenterCorrection::SetNumberOfAnchorSurfacePoints(
  const MaskObjectType & whichObject,
  const unsigned int     numberOfAnchorSurfacePoints )
{
  this->m_SecretData->m_NumberOfAnchorSurfacePoints[ whichObject ] =
    numberOfAnchorSurfacePoints;
}
//////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////
LandmarkSetConstPointer MFASegmenterCorrection::GetKnownSurfacePoints(
  const MaskObjectType & whichObject ) const
{

  // our map
  const auto & knownSurfacePoints = this->m_SecretData->m_KnownSurfacePoints;

  // search the map
  const auto foundPoints = knownSurfacePoints.find( whichObject );

  // output null, or the found result
  if ( foundPoints == knownSurfacePoints.cend() )
    return LandmarkSetConstPointer();
  else
    return foundPoints->second;

} // GetKnownSurfacePoints
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
unsigned int MFASegmenterCorrection::GetNumberOfAnchorSurfacePoints(
  const MaskObjectType & whichObject ) const
{

  // our map
  const auto & numberOfPointsMap = this->m_SecretData->m_NumberOfAnchorSurfacePoints;

  // search the map
  const auto foundPoints = numberOfPointsMap.find( whichObject );

  // output null, or the found result
  if ( foundPoints == numberOfPointsMap.cend() )
    return 0;
  else
    return foundPoints->second;

} // GetNumberOfAnchorSurfacePoints
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void MFASegmenterCorrection::ClearCorrectionAndKnownSurfacePoints()
{
  this->ClearKnownSurfacePoints();
  this->ClearCorrection();
}

void MFASegmenterCorrection::ClearCorrection()
{
  this->m_SecretData->m_CorrectionTransform = VolumeTransformConstPointer();
}

void MFASegmenterCorrection::ClearKnownSurfacePoints()
{
  this->m_SecretData->m_KnownSurfacePoints.clear();
}
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
ccipdSetByvalImplementMacro( MFASegmenterCorrection, FixCorners, bool )
ccipdGetConstImplementMacro( MFASegmenterCorrection, FixCorners, bool )
ccipdSetByvalImplementMacro( MFASegmenterCorrection,
  NumberOfRandomFixedPoints, unsigned int )
ccipdGetConstImplementMacro( MFASegmenterCorrection,
  NumberOfRandomFixedPoints, unsigned int )
ccipdSetByvalImplementMacro( MFASegmenterCorrection,
  ForbiddenRadius, TransformValueType )
ccipdGetConstImplementMacro( MFASegmenterCorrection,
  ForbiddenRadius, TransformValueType )
ccipdGetConstImplementMacro( MFASegmenterCorrection,
  RandomFixedPoints, LandmarkSetConstPointer )
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
#ifdef Boost_SERIALIZATION_FOUND

ccipdSerializationXMLMacro( MFASegmenterCorrection )

ccipdSerializationImplementationHeadingMacro( MFASegmenterCorrection )
{

  using boost::serialization::make_nvp;

  if ( version < 1 )
    {
    cerr << "Error: Unable to serialize versions prior to 1." << endl;
    return;
    }

  archive
    & BOOST_SERIALIZATION_BASE_OBJECT_NVP( Superclass )
    & make_nvp( "FixCorners", this->m_SecretData->m_FixCorners )
    & make_nvp( "NumberOfRandomFixedPoints",
      this->m_SecretData->m_NumberOfRandomFixedPoints )
    & make_nvp( "NumberOfAnchorSurfacePoints",
      this->m_SecretData->m_NumberOfAnchorSurfacePoints )
    & make_nvp( "ForbiddenRadius",
      this->m_SecretData->m_ForbiddenRadius );

} // serialize

#endif // Boost_SERIALIZATION_FOUND
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace
