#ifndef __ccipdSurfacePointsFromGrid_h
#define __ccipdSurfacePointsFromGrid_h



/////////////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdVectorIndexTypes.h"
#include "ccipdVTKForwardDeclarations.h"
/////////////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



/////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Determine which points are on the surface.
 * 
 * This function will take a grid,
 * which presumably has a bunch of cells
 * such as tetrahedrons.
 * 
 * Then, it will determine which of its
 * points actually lie on the surface.
 * 
 * \param[in] grid
 *   The grid containing the elements.
 * 
 * \returns
 *   A collection of indices
 *   indicating which points are
 *   surface points.
 *   Each index will be less than
 *   the total number of points
 *   in the grid.
 * 
 * \date    2012-04-20
 * \author  Rob Toth
 * \ingroup Common
 * 
 */
VectorIndexContainer ExtractSurfacePointsIndices( const vtkUnstructuredGridPointer & grid );
/////////////////////////////////////////////////////////////////////////////////////////////////


} // namespace ccipd
#endif // __ccipdSurfacePointsFromGrid_h
