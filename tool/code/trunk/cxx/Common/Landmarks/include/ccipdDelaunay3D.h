#ifndef __ccipdDelaunay3D_h
#define __ccipdDelaunay3D_h


//////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdMeshTypes.h"
#include "ccipdLandmarkTypes.h"
#include "ccipdVTKForwardDeclarations.h"
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////
/**
 * The function that actual parforms the computations.
 */
vtkUnstructuredGridPointer Delaunay3D(
  const vtkPolyDataPointer & points,
  const bool                 removeDuplicatePoints = false );
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
/**
 * Overloads for different data types.
 */
vtkUnstructuredGridPointer Delaunay3D(
  const LandmarkSetType * const landmarks,
  const bool                    removeDuplicatePoints = false );
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace ccipd
#endif // __ccipdDelaunay3D_h
