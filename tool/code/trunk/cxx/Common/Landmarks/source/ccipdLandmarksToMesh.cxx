


//////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdLandmarksToMesh.hxx"
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{


  
//////////////////////////////////////////////////////////////////////////////////////////
/*
template VolumeMeshPointer ConvertLandmarksToMesh<
  LandmarkSetType,
  LandmarkDimension,
  VolumeMeshTraits
>( const LandmarkSetType * );
*/
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
VolumeMeshPointer ConvertLandmarksToMesh( const LandmarkSetType * landmarks )
{

  return ConvertLandmarksToMesh<
    LandmarkSetType,
    LandmarkDimension,
    VolumeMeshTraits
  >( landmarks );

} // LandmarksToMesh
//////////////////////////////////////////////////////////////////////////////////////////



}//namespace