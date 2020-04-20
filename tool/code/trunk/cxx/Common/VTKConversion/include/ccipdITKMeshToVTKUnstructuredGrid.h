#ifndef __ccipdITKMeshToVTKUnstructuredGrid_h
#define __ccipdITKMeshToVTKUnstructuredGrid_h



//////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdMeshTypes.h"
#include "ccipdVTKForwardDeclarations.h"
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////
/**
 * \brief ITK to VTK conversion of mesh cells.
 * 
 * This function extracts the tetrahedrons
 * from a mesh and converts it to a vtk data type.
 * 
 * \param[in] mesh The mesh from which to extract tetrahedron cells.
 * 
 * \returns A vtkUnstructuredGrid containing a collection of vtk tetrahedrons.
 * 
 * \date    2012-04-19
 * \author  Rob Toth
 * \ingroup VTKConversion
 */
vtkUnstructuredGridPointer ConvertMeshToUnstructuredGrid( const VolumeMeshType * mesh );
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace ccipd


#endif // __ccipdITKMeshToVTKUnstructuredGrid_h
