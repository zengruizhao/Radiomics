#ifndef __ccipdPickPointsOnImage_h
#define __ccipdPickPointsOnImage_h



////////////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdLandmarkTypes.h" // our output type
#include "ccipdVolumeTypes.h"   // our image type
#include "ccipdRGBSliceTypes.h" // another image type
#include "ccipdVTKForwardDeclarations.h" // for vtkRenderWindow
////////////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Display an image on the screen and pick some landmarks.
 * 
 * An image will pop up on the screen using
 * VTK, and every time you press Ctrl+(Right Button),
 * the point will be remembered (in real world coordinates).
 * 
 * \returns
 *   The landmarks you selected on the image.
 * 
 * \date    2012-05-05
 * \author  Rob Toth
 * \ingroup GUI
 */
template< class TImageType >
LandmarkSetPointer PickPointsOnImage(
  const TImageType * const image,
  const unsigned int       slice,
  const bool               verbose );
////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * \brief   Pick points on RGB image.
 * \see     PickPointsOnImage( const VolumeImageType * const, const unsigned int, const bool )
 * \note    The third dimension of the output is undefined.
 * \date    2012-05-07
 * \author  Rob Toth
 * \ingroup GUI
 */
LandmarkSetPointer PickPointsOnImage(
  const RGBSliceType * const image,
  const bool                 verbose );
////////////////////////////////////////////////////////////////////////////////////////////////



} // namespace ccipd
#endif // __ccipdPickPointsOnImage_h
