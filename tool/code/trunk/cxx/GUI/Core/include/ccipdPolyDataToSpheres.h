#ifndef __ccipdPolyDataToSpheres_h
#define __ccipdPolyDataToSpheres_h



//////////////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdVTKForwardDeclarations.h"
//////////////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Generates spheres from polydata points.
 * 
 * This will generate a vtkActor of spheres
 * at each polydata point.
 * 
 * \param[in] polyData
 *   A sphere will be created at
 *   each polyData->GetPoints()
 * 
 * \returns
 *   A vtkActor representing the
 *   spheres.
 * 
 * \date    2012-04-19
 * \author  Rob Toth
 * \ingroup GUI
 */
vtkActorPointer GenerateSpheresFromPolyData(
  const vtkPolyDataPointer & polyData,
  const double               sphereRadius = 1.0 );
//////////////////////////////////////////////////////////////////////////////////////////////////



} // namespace ccipd
#endif // __ccipdPolyDataToSpheres_h
