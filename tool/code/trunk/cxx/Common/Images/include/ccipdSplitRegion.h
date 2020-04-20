#ifndef __ccipdSplitRegion_h
#define __ccipdSplitRegion_h



///////////////////////////////////////////////////////////////////////////
// ccipd includes
#include "ccipdITKForwardDeclarations.h"

#include "ccipdDisableWarningsMacro.h"
  // std includes
  #include <vector>
#include "ccipdEnableWarningsMacro.h"
///////////////////////////////////////////////////////////////////////////



namespace ccipd
{



///////////////////////////////////////////////////////////////////////////
/**
 * \brief Split an image region into smaller subregions.
 * 
 * This class is a wrapper for
 * itk::ImageRegionSplitter,
 * in which an image region
 * is split up into multiple, smaller
 * subregions.
 * 
 * This is highly useful for multi-threaded
 * applications, as each thread
 * can process a single subregion
 * in parallel.
 * 
 * \param[in] region
 *   The requested region
 *   to be split.
 * 
 * \param[in] numberOfRequestedSubRegions
 *   How many sub-regions would you
 *   like to split the region into?
 * 
 * \returns
 *   A std::vector of subregions,
 *   where the size of the std::vector
 *   is less than or equal to
 *   the number of requested regions,
 *   depending on how the input region
 *   was able to be split up.
 * 
 * \note
 *   The number of subregions returned
 *   might be less than the number of 
 *   subregions requested, based
 *   on the shape of the input region.
 * 
 * \date    2012-04-24
 * \author  Rob Toth
 * \ingroup Common
 */
template< unsigned int Dimension >
std::vector< itk::ImageRegion< Dimension > >
SplitRegion(
  const itk::ImageRegion< Dimension > & region,
  const unsigned int                    numberOfRequestedSubRegions );
///////////////////////////////////////////////////////////////////////////



} // namespace ccipd
#endif // __ccipdSplitRegion_h
