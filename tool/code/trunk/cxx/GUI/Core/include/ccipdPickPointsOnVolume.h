#ifndef __ccipdPickPointsOnVolume_h
#define __ccipdPickPointsOnVolume_h



////////////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdLandmarkTypes.h" // our output type
////////////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Display a volume on the screen and pick some landmarks.
 * 
 * This is very similar to the templated
 * PickPointsOnImage()
 * in which you
 * input the slice. However, this will use Qt to display
 * a dialog box with a slider, so that you can
 * pick points on multiple slices.
 * 
 * That function can be used when you want
 * to pick points from a single slice
 * of a volume.
 * 
 * For consistency, I kept the same function name.
 * 
 * If you press "Cancel" in the dialog,
 * or any other errors occur, a NULL
 * pointer will be returned, so be sure
 * to check for that.
 * 
 * \date    2013-03-23
 * \author  Rob Toth
 * \ingroup GUI
 */
LandmarkSetPointer PickPointsOnImage(
  const VolumeImageConstPointer & image );

LandmarkSetPointer PickPointsOnImage(
  const VolumeImageConstPointer & image,
  const LandmarkSetConstPointer & initialLandmarks );
////////////////////////////////////////////////////////////////////////////////////////////////



} // namespace ccipd
#endif // __ccipdPickPointsOnVolume_h
