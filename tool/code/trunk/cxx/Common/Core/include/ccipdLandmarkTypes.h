#ifndef __ccipdLandmarkTypes_h
#define __ccipdLandmarkTypes_h



//////////////////////////////////////////////////////////////////////////////////////////

// ccipd includes
#include "ccipdCore.h"
#include "ccipdITKForwardDeclarations.h"
#include "ccipdMeshTypes.h"

// itk includes
#include "ccipdDisableWarningsMacro.h"
#include <itkIntTypes.h> // for size value type
#include "ccipdEnableWarningsMacro.h"

//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



/**
 * \addtogroup Common
 * @{
 */
  

  
//////////////////////////////////////////////////////////////////////////////////////////
// landmark types
const unsigned int LandmarkDimension = 3;

typedef itk::PointSet< 
  LandmarkType, 
  LandmarkDimension,
  VolumeMeshTraits
> LandmarkSetType;

typedef itk::SmartPointer<
  LandmarkSetType
> LandmarkSetPointer;

typedef itk::SmartPointer<
  const LandmarkSetType
> LandmarkSetConstPointer;

typedef itk::Point<
  LandmarkType,  
  LandmarkDimension 
> LandmarkPointType;

typedef itk::VectorContainer<
  itk::SizeValueType,
  LandmarkPointType
> LandmarkSetContainer;

typedef itk::SmartPointer<
  LandmarkSetContainer
> LandmarkSetContainerPointer;

typedef itk::SmartPointer<
  const LandmarkSetContainer
> LandmarkSetContainerConstPointer;

//////////////////////////////////////////////////////////////////////////////////////////
  

  
//////////////////////////////////////////////////////////////////////////////////////////
// medial Axis types
const unsigned int MedialAxisDimension = 4; // x,y,z and a distance to surface as the forth dimension 

typedef itk::DefaultStaticMeshTraits<
  MeshPixelType,
  MedialAxisDimension,
  MedialAxisDimension,
  MeshPixelType,
  MeshPixelType,
  MeshPixelType
> MedialAxisMeshTraits;

typedef itk::PointSet< 
  MedialAxisType, 
  MedialAxisDimension,
  MedialAxisMeshTraits
> MedialAxisSetType;

typedef itk::SmartPointer<
  MedialAxisSetType
> MedialAxisSetPointer;

typedef itk::SmartPointer<
  const MedialAxisSetType
> MedialAxisSetConstPointer;

typedef itk::Point<
  MedialAxisType,  
  MedialAxisDimension 
> MedialAxisPointType;


typedef itk::VectorContainer<
  itk::SizeValueType,
  MedialAxisPointType
> MedialAxisSetContainer;
//////////////////////////////////////////////////////////////////////////////////////////



/** @} */
} // namespace
#endif // __ccipdLandmarkTypes_h
