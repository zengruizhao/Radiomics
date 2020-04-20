#ifndef __ccipdExtractMedialAxis_h
#define __ccipdExtractMedialAxis_h



////////////////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdCore.h"
#include "ccipdVolumeTypes.h"
#include "ccipdLandmarkTypes.h"
////////////////////////////////////////////////////////////////////////////////////////////////////


namespace ccipd
{

////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * This function will extract the medial axis of a distance map (input must be levelset).
 *
 * \param distanceVolume is a level set where 0 is the surface
 * \param a gaussian filter is applied - give small sigma 
 * 
 * \author Mirabela Rusu - using Third party classes
 * \ingroup Common
 */
VolumeMaskImageConstPointer ExtractMedialAxis(
  const VolumeImageType * distanceVolume,
  float sigma = 0.5,
  float threshold = 0.0	);
////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * This function will extract the medial axis of a distance map (input must be levelset) and 
 * output a pointset
 *
 * \param distanceVolume is a level set where 0 is the surface
 * \param a gaussian filter is applied - give small sigma 
 * 
 * \author Mirabela Rusu - using Third party classes
 * \ingroup Common
 */
LandmarkSetPointer ExtractMedialAxisIntoLandmark(
  const VolumeImageType * distanceVolume,
  float sigma = 0.5,
  float threshold = 0.0	);

} // namespace
#endif // __ccipdExtractMedialAxis_h
