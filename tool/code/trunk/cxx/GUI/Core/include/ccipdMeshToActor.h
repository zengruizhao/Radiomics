#ifndef __ccipdMeshToActor_h
#define __ccipdMeshToActor_h



//////////////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdVTKForwardDeclarations.h"
#include "ccipdMeshTypes.h"
//////////////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Generates an actor from a mesh.
 * 
 * Generates an actor from the tetrahedrons
 * of a mesh.
 * 
 * \param[in] mesh The mesh from which to generate vtkActor's
 * 
 * \returns a vtkActor representing the objects of that mesh.
 * 
 * \note
 *   This only uses the tetrahedron cells
 *   from the mesh, and ignores all other cells.
 * 
 * \date    2012-04-19
 * \author  Rob Toth
 * \see     GenerateTrianglesActorFromMesh()
 * \see     ConvertMeshToUnstructuredGrid()
 * \ingroup GUI
 */
vtkActorPointer GenerateTrianglesActorFromMesh( const VolumeMeshType * const mesh );
//////////////////////////////////////////////////////////////////////////////////////////////////



} // namespace ccipd
#endif // __ccipdMeshToActor_h
