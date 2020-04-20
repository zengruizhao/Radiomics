#ifndef __ccipdLandmarksToMesh_hxx
#define __ccipdLandmarksToMesh_hxx



//////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdLandmarksToMesh.h"      // declarations
#include "ccipdDuplicateLandmarks.hxx" // for duplicating points to place into the mesh

#include "ccipdDisableWarningsMacro.h"
  // itk includes
  #include <itkMesh.h>
#include "ccipdEnableWarningsMacro.h"
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{


  
//////////////////////////////////////////////////////////////////////////////////////////
template< class PointSetType, unsigned int PointDimension, class MeshTraits >
itk::SmartPointer< itk::Mesh<
  typename PointSetType::PixelType,
  PointDimension,
  MeshTraits
> >
ConvertLandmarksToMesh( const PointSetType * landmarks )
{

  // first create a mesh to place the points into
  typedef itk::Mesh<
    typename PointSetType::PixelType,
    PointDimension,
    MeshTraits
  > MeshType;
  typename MeshType::Pointer mesh = MeshType::New();

  // get a set of unconstant points
  typedef typename PointSetType::PointsContainer PointSetContainer;  
  typename PointSetContainer::Pointer 
    unconstContainer = DuplicateLandmarkContainer< typename PointSetType::PixelType, PointDimension, MeshTraits > (  landmarks->GetPoints() );

  typename PointSetType::Pointer unconstLandmarks = PointSetType::New();
    unconstLandmarks->SetPoints( unconstContainer );
							  
  mesh->SetPoints(unconstLandmarks->GetPoints());

  return mesh;

}//SaveLandmarks
////////////////////////////////////////////////////////////////////////////////////////// 



} // namespace



#endif // __ccipdLandmarksToMesh_hxx
