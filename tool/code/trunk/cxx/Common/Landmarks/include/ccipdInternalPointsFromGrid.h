#ifndef __ccipdInternalPointsFromGrid_h
#define __ccipdInternalPointsFromGrid_h



/////////////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdVectorIndexTypes.h"
#include "ccipdVTKForwardDeclarations.h"
/////////////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



/////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Determine which points are on the inside of a grid.
 * 
 * This function will take a grid,
 * which presumably has a bunch of cells
 * such as tetrahedrons.
 * 
 * Then, it will determine which of its
 * points are inside (not on the surface).
 * 
 * \param[in] grid
 *   The grid containing the elements.
 * 
 * \param[in] surfacePoints
 *   The points on the surface
 *   which will be excluded from
 *   inclusion.
 * 
 * \returns
 *   A collection of indices
 *   indicating which points are
 *   internal points.
 *   Each index will be less than
 *   the total number of points
 *   in the grid.
 * 
 * \date    2012-07-14
 * \author  Rob Toth
 * \ingroup Common
 * 
 */
VectorIndexContainer ExtractInternalPointsIndices(
  const vtkUnstructuredGridPointer & grid,
  const VectorIndexContainer       & surfacePoints );
/////////////////////////////////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Determine which points are on the inside of a grid.
 * 
 * The same as ExtractInternalPointsIndices() but will
 * calculate the surface points for you.
 */
VectorIndexContainer ExtractInternalPointsIndices( const vtkUnstructuredGridPointer & grid);
/////////////////////////////////////////////////////////////////////////////////////////////////



} // namespace ccipd
#endif // __ccipdInternalPointsFromGrid_h
