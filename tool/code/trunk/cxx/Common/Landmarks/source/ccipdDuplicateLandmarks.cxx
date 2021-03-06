


//////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdDuplicateLandmarks.hxx"

#include "ccipdDisableWarningsMacro.h"
  #include <itkPointSet.h>
#include "ccipdEnableWarningsMacro.h"
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////
LandmarkSetPointer DuplicateLandmarkSet  ( const LandmarkSetType     * landmarks  )
{

  return DuplicateLandmarkSet< LandmarkType, LandmarkDimension, VolumeMeshTraits >( landmarks );

} // DuplicateLandmarkSet
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
LandmarkSetContainerPointer DuplicateLandmarkContainer  ( const LandmarkSetContainer * landmarkContainer   )
{

  return DuplicateLandmarkContainer< LandmarkType, LandmarkDimension, VolumeMeshTraits  >( landmarkContainer );

} // DuplicateLandmarkContainer
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace
