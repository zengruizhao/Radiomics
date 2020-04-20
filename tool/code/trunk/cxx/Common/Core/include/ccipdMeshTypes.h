#ifndef __ccipdMeshTypes_h
#define __ccipdMeshTypes_h



//////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdCore.h"
#include "ccipdVolumeTypes.h" // for the dimension
#include "ccipdITKForwardDeclarations.h"
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



/**
 * \addtogroup Common
 * @{
 */


  
//////////////////////////////////////////////////////////////////////////////////////////
typedef itk::DefaultStaticMeshTraits<
  MeshPixelType,
  VolumeDimension,
  VolumeDimension,
  MeshPixelType,
  MeshPixelType,
  MeshPixelType
> VolumeMeshTraits;

typedef itk::Mesh<
  MeshPixelType,
  VolumeDimension,
  VolumeMeshTraits
> VolumeMeshType;

typedef itk::SmartPointer<
  VolumeMeshType
> VolumeMeshPointer;

typedef itk::SmartPointer<
  const VolumeMeshType
> VolumeMeshConstPointer;
//////////////////////////////////////////////////////////////////////////////////////////



/** @} */
} // namespace ccipd
#endif // __ccipdMeshTypes_h
