#ifndef __ccipdLandmarkToCursor_h
#define __ccipdLandmarkToCursor_h



//////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdVTKForwardDeclarations.h"
#include "ccipdLandmarkTypes.h"
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Create a crosshair actor at a specific point.
 * 
 * At a point in 3D space, create a crosshair.
 * 
 * \param[in] landmark
 *   The point in 3D space at which to center the crosshair.
 * 
 * \param[in] ?Radius
 *   A crosshair will be created in which
 *   the length of each axis (x, y, z)
 *   is twice the radius specified.
 * 
 * \returns
 *   A vtkActor representing
 *   the crosshair polygon data.
 * 
 * \date    2012-05-07
 * \author  Rob Toth
 * \ingroup GUI
 */
vtkActorPointer CreateCursorAtLandmark(
  const LandmarkPointType & landmark,
  const LandmarkType        xRadius = 10,
  const LandmarkType        yRadius = 10,
  const LandmarkType        zRadius = 10 );
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace ccipd
#endif // __ccipdLandmarkToCursor_h
