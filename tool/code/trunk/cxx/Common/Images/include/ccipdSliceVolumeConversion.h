/**
 * This file aims to create a slice from a volume and create a volume from a slice.
 */
#ifndef __ccipdSliceVolumeConversion_h
#define __ccipdSliceVolumeConversion_h



//////////////////////////////////////////////////////////////////////////////////////////
// ccipd includes
#include "ccipdVolumeTypes.h"
#include "ccipdSliceTypes.h"
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////

/**
 * \brief Create a slice pointer from a volume.
 * 
 * \date    2013-03-15
 * \author  Eileen Hwuang
 * \ingroup Common
 */
template< typename TPixelType >
itk::SmartPointer< itk::Image< TPixelType, ccipd::SliceDimension > >
GetSliceFromVolume(
  const itk::Image< TPixelType, ccipd::VolumeDimension > * const volume,
  const unsigned int whichSlice );
//////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Create a volume pointer from a slice.
 * 
 * \date    2013-03-15
 * \author  Eileen Hwuang
 * \ingroup Common
 */
template< typename TPixelType >
itk::SmartPointer< itk::Image< TPixelType, ccipd::VolumeDimension > >
GetVolumeFromSlice(
  const itk::Image< TPixelType, ccipd::SliceDimension > * const slice );
//////////////////////////////////////////////////////////////////////////////////////////


} // namespace
#endif // __ccipdSliceVolumeConversion_h
