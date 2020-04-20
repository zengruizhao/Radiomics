


//////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdBoundingBoxCoordinates.hxx"
#include "ccipdVolumeTypes.h"
#include "ccipdLandmarkTypes.h"
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////
template
VolumePointType GetMinBoundingBoxCoordinates< double, VolumeDimension >(
  const VolumeRegionType & boundingBox,
  const VolumeImageBase  & image );
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
template
VolumePointType GetMaxBoundingBoxCoordinates< double, VolumeDimension >(
  const VolumeRegionType & boundingBox,
  const VolumeImageBase  & image );
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
template
LandmarkPointType GetMinBoundingBoxCoordinates< float, VolumeDimension >(
  const VolumeRegionType & boundingBox,
  const VolumeImageBase  & image );
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
template
LandmarkPointType GetMaxBoundingBoxCoordinates< float, VolumeDimension >(
  const VolumeRegionType & boundingBox,
  const VolumeImageBase  & image );
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace ccipd