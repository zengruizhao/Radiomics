#ifndef __ccipdExtractUniqueValues_h
#define __ccipdExtractUniqueValues_h



//////////////////////////////////////////////////////////////////////////////////////////

// ccipd includes
#include "ccipdITKForwardDeclarations.h"

// std includes
#include <unordered_set> // for the output
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Find unique pixel values in an image.
 * 
 * This will iterate through an image and
 * find all unique pixel values.
 * 
 * \returns
 *   An unordered set of pixel
 *   values found in the image.
 * 
 * \author  Rob Toth
 * \date    2012-06-21
 * \ingroup Common
 */
template< class TPixelType, unsigned int Dimension >
std::unordered_set< TPixelType > // output
ExtractUniqueValues( const itk::Image< TPixelType, Dimension > * const image );
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace ccipd
#endif // __ccipdExtractUniqueValues_h
