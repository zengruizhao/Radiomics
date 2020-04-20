#ifndef __ccipdRenderPointPicker_h
#define __ccipdRenderPointPicker_h



////////////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdLandmarkTypes.h"          // our output type
#include "ccipdVTKForwardDeclarations.h" // for vtkRenderWindow
////////////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Update an existing render window to pick points.
 * 
 * This class is a helper function for
 * PickPointsOnImage()
 * in which you already have a render window
 * and want to load an image onto it,
 * with a point picker.
 * 
 * \returns
 *   Nothing. The final call is to "Render()"
 *   and you must start the interaction if
 *   you so desire.
 * 
 * \date    2012-05-08
 * \author  Rob Toth
 * \ingroup GUI
 */
template< class TImageType >
void RenderPointPicker(
  const TImageType * const image,
  const unsigned int       slice,
  const bool               verbose,
  vtkRenderWindow * const  renderWindow );
////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * This version of RenderPointPicker()
 * allows you to specify some initial
 * landmarks which will be remembered
 * and drawn on the scene.
 * 
 * \date    2012-05-15
 * \author  Rob Toth
 * \ingroup GUI
 */
template< class TImageType >
void RenderPointPicker(
  const TImageType * const        image,
  const unsigned int              slice,
  const bool                      verbose,
  vtkRenderWindow * const         renderWindow,
  const LandmarkSetConstPointer & initialLandmarks );
////////////////////////////////////////////////////////////////////////////////////////////////



} // namespace ccipd
#endif // __ccipdPickPointsOnImage_h
