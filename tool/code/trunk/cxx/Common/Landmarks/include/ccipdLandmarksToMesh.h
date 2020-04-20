#ifndef __ccipdLandmarksToMesh_h
#define __ccipdLandmarksToMesh_h



//////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdCore.h"
#include "ccipdLandmarkTypes.h"
#include "ccipdMeshTypes.h" // for mesh types
#include "ccipdITKForwardDeclarations.h"

#include "ccipdDisableWarningsMacro.h"
  #include <string>
#include "ccipdEnableWarningsMacro.h"
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Convert landmarks to an itk mesh.
 * 
 * \ingroup Common
 */
template< class PointSetType, unsigned int PointDimension, class MeshTraits >
itk::SmartPointer< itk::Mesh<
  typename PointSetType::PixelType,
  PointDimension,
  MeshTraits
> >
LandmarksToMesh( const PointSetType * landmarks );
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Convert landmarks to an itk mesh in 3D.
 * 
 * \ingroup Common
 */
VolumeMeshPointer ConvertLandmarksToMesh( const LandmarkSetType * landmarks );
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace


#endif // __ccipdLandmarksToMesh_h