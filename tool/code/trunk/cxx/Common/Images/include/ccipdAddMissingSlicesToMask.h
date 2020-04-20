#ifndef __ccipdAddMissingSlicesToMask_h
#define __ccipdAddMissingSlicesToMask_h

#include "ccipdVolumeTypes.h"

//////////////////////////////////////////////////////////////////////////////////////////
//#include "ccipdITKForwardDeclarations.h"
//////////////////////////////////////////////////////////////////////////////////////////

#include "ccipdDisableWarningsMacro.h"
  // itk includes
  #include <itkImage.h> // for loading masks
#include "ccipdEnableWarningsMacro.h"


namespace ccipd
{

/**
 * Used to fill missing data: when cancer annotations are available on slice
 * n-1 and n+1 but not on n, one can assume that the annotation on n is an
 * interpolation of between n-1 and n+1 information
 */
std::vector<int> AddMissingData(VolumeMaskImagePointer mask, std::vector<int> slices);



//////////////////////////////////////////////////////////////////////////////////////////



} // namespace ccipd
#endif // __ccipdAddMissingSlicesToMask_h
