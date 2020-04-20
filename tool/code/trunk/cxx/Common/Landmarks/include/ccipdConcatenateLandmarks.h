//////////////////////////////////////////////////////////////////////////////////////////
/**
 * \brief   Simple wrapper to dupcliate our common landmark types.
 * \ingroup Common 
 * \author  Mirabela Rusu
 * \date    03/25/2013 
 */
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef __ccipdConcatenateLandmarks_h
#define __ccipdConcatenateLandmarks_h

//////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdLandmarkTypes.h"
//////////////////////////////////////////////////////////////////////////////////////////

namespace ccipd
{

/**
 * \brief   Concatenate two landmark lists into one list
 */
LandmarkSetPointer ConcatenateLandmarkSets( const LandmarkSetType * const landmarkSet1, const LandmarkSetType * const landmarkSet2 );

/**
 * \brief   Concatenate two landmarks into a list 
 */
LandmarkSetPointer ConcatenateLandmarks( const LandmarkPointType landmark1, const LandmarkPointType landmark2 );
} // namespace
#endif // __ccipdConcatenate_h
