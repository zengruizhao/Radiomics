/**
 *   \author Mirabela Rusu
 *   \date 03/19/2013
 *   \brief functionality to extract and merge labels/masks 
 */
//////////////////////////////////////////////////////////////////////////////////////////

#ifndef __ccipdProcessLabels_h
#define __ccipdProcessLabels_h


////////////////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdVolumeTypes.h"
#include "ccipdDisableWarningsMacro.h"
  // itk includes
  #include "itkImage.h"
#include "ccipdEnableWarningsMacro.h"
////////////////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{
 /**
  * Extract one label from a file with multiple labels
  **/
VolumeMaskImageConstPointer ExtractLabels(const VolumeImageType * MultiLabelVol, std::vector<int> inLabels, int outLabel);

/**
 * Merge two labels in the same file
 **/
VolumeMaskImageConstPointer MergeLabels(const VolumeMaskImageConstPointer src1, const VolumeMaskImageConstPointer src2, int outValue1 = 1, int outValue2 = 2 );

} // namespace
#endif // __ccipdProcessLabels_h
