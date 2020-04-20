#ifndef __ccipdFEMToActor_h
#define __ccipdFEMToActor_h



//////////////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdVTKForwardDeclarations.h"
#include "ccipdFEMTypes.h"
//////////////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * \brief Generates actors from a FEM.
 * 
 * This function will generate a VTK
 * actor representing the landmarks,
 * the @trianglesActor, and the loads from
 * a defined finite element model (FEM).
 * 
 * \param[in] FEM
 *   The FEM from which to generate the actors.
 * 
 * \param[out] landmarksActor
 *   A vtkActor in which a colored
 *   point is placed at each landmark.
 * 
 * \param[out] trianglesActor
 *   A vtkActor in which a semi-transparent
 *   set of triangles are placed at each
 *   tetrahedron.
 * 
 * \param[out] loadsArrowsActor
 *   A vtkActor in which an arrow is
 *   shown at each load.
 * 
 * \param[out] loadsSpheresActor
 *   A vtkActor in which a spheres is
 *   shown at each load node.
 * 
 * \param[in] landmarkSphereRadius
 *   Each landmark will be represented
 *   by a sphere, and this
 *   controls the radii of those spheres.
 * 
 * \param[in] loadSphereRadius
 *   Each load node will be represented
 *   by a sphere, and this
 *   controls the radii of those spheres.
 * 
 * \param[in] loadArrowScalingFactor
 *   Each load node will be represented
 *   by an arrow. The length of that
 *   arrow will be the magnitude
 *   of the load multiplied by this
 *   scaling factor.
 * 
 * \param[in] verbose
 *   Whether or not to display progress
 *   to cout during computations.
 * 
 * \return All returns are given by non-const references.
 * 
 * \author  Rob Toth
 * \date    2012-04-17
 * \ingroup FEMRendering
 */
void GenerateVTKActorsFromFEM(
  const FEMObjectType * const FEM,
        vtkActorPointer & landmarksActor,    // output #1
        vtkActorPointer & trianglesActor,    // output #2   
        vtkActorPointer & loadsArrowsActor,  // output #3
        vtkActorPointer & loadsSpheresActor, // output #4
  const double            landmarkSphereRadius   = 1.0,
  const double            loadSphereRadius       = 2.0,
  const double            loadArrowScalingFactor = 10,
  const bool              verbose = true );

//////////////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Generates a load actor from FEM.
 * 
 * This function is a helper function for
 * GenerateVTKActorsFromFEM() which
 * will generate a vtkActor defining the
 * loads as arrows.
 * 
 * \param[in] FEM
 *   The FEM from which to generate the actor.
 * 
 * \param[in] scaleFactor
 *   The length of each arrow (in mm) is defined
 *   as the magnitude of the load multiplied
 *   by this scale factor.
 * 
 * \returns
 *   An actor containing glyph arrows
 *   pointing to each load node.
 *   The size of the arrow depends on
 *   the magnitude of the load.
 * 
 * \author  Rob Toth
 * \date    2012-04-17
 * \ingroup FEMRendering
 * \see     GenerateArrowActorFromFEMLoads()
 */
vtkActorPointer GenerateArrowsActorFromFEMLoads(
  const FEMObjectType * const FEM,
  const double                scaleFactor = 5.0 );
//////////////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Extracts vtkPolyData representing FEM loads.
 * 
 * This function is a helper function, in which
 * a vtkPolyData is created from the loads in a FEM.
 * 
 * \returns
 *   polyData->GetPoints() will represent the node
 *   locations at which a force is applied.
 *   polyData->GetPointData()->GetVectors() will
 *   represent a NEGATIVE force (i.e. a force
 *   pointing away from the point). The vectors
 *   will have the correct magnitude.
 * 
 * \date    2012-04-19
 * \author  Rob Toth
 * \ingroup FEMRendering
 * \see     GenerateArrowActorFromFEMLoads()
 */
vtkPolyDataPointer GeneratePolyDataFromFEMLoads( const FEMObjectType * const FEM );
//////////////////////////////////////////////////////////////////////////////////////////////////



} // namespace ccipd
#endif // __ccipdFEMToActor_h
