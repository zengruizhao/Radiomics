#ifndef __ccipdLandmarksToActor_h
#define __ccipdLandmarksToActor_h



//////////////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdVTKForwardDeclarations.h"
#include "ccipdLandmarkTypes.h"
//////////////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Generates an actor from landmarks.
 * 
 * Generates an actor from a PointSet.
 * Note that this includes itk meshes.
 * 
 * \param[in] landmarks The mesh from which to generate vtkActor's
 * 
 * \returns a vtkActor representing the objects of that mesh.
 * 
 * \date 2012-04-19
 * \author Rob Toth
 * \see ConvertLandmarksToPolyData()
 * \see GenerateSpheresFromPolyData()
 * \ingroup GUI
 */
vtkActorPointer GenerateActorFromLandmarks(
  const LandmarkSetType * const landmarks,
  const double                  sphereRadius = 1.0 );
//////////////////////////////////////////////////////////////////////////////////////////////////



} // namespace ccipd
#endif // __ccipdLandmarksToActor_h
