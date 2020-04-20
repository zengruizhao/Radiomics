#ifndef __ccipdPolyDataToArrows_h
#define __ccipdPolyDataToArrows_h



//////////////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdVTKForwardDeclarations.h"
//////////////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * \brief Generates arrows from polydata vectors.
 * 
 * This function will generate a VTK
 * actor representing arrows each each polydata
 * point.
 * 
 * \param[in] polyData
 *   polyData->GetPoints() contains the
 *   locations at which to draw each arrow.
 *   polyData->GetPointData()->GetVectors()
 *   contains the arrow vector to draw.
 * 
 * \param[in] pointAway
 *   Whether or not to point the arrows
 *   away from the point. For example,
 *   you might have a point at 0,0,0 with
 *   a vector of -1,0,0. If pointAway is
 *   set to true, then there will be an arrow
 *   pointing away from the origin, with its
 *   tip at -1,0,0. If pointAway is set to false,
 *   then there will be an arrow pointing towards
 *   the origin, with its tip at 0,0,0 and
 *   its base at -1,0,0.
 * 
 * \param[in] scaleFactor
 *   Each arrow will have length equal to
 *   its magnitude, times the scaleFactor.
 * 
 * \returns A vtkActor representing the arrows.
 * 
 * \author  Rob Toth
 * \date    2012-04-19
 * \ingroup GUI
 */
vtkActorPointer GenerateArrowsFromPolyData(
  const vtkPolyDataPointer & polyData,
  const bool                 pointAway   = true,
  const double               scaleFactor = 5.0 );

//////////////////////////////////////////////////////////////////////////////////////////////////



} // namespace ccipd
#endif // __ccipdPolyDataToArrows_h
