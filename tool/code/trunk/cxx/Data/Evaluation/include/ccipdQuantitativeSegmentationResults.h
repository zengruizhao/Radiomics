#ifndef __ccipdQuantitativeSegmentationResults_h
#define __ccipdQuantitativeSegmentationResults_h



//////////////////////////////////////////////////////////////////////////////////////////
// ccipd includes
#include "ccipdVolumeTypes.h"

// std includes
#include <string>
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Save quantitative segmentation results to a file.
 * 
 * \param[in] segmentation
 *   The segmentation result.
 * 
 * \param[in] groundTruth
 *   The ground truth mask.
 * 
 * \param[in] fileName
 *   A file (e.g. "quantitative.txt")
 *   to save the results.
 * 
 * \param[in] midglandTemplateMask
 *   If you give a mask here, then
 *   the midgland will be extracted
 *   from this mask and used to calculate
 *   the Dice separately.
 *   If you don't input a mask here,
 *   then the groundTruth mask will
 *   be used.
 * 
 * \param[in] midglandSlicesPercentage
 *   If you enter a value between
 *   0.0 and 1.0 (exclusive!)
 *   then a certain percentage of the mask
 *   slices will be extract from the center
 *   of the mask (representing the "midgland").
 *   Then, the Dice value of just
 *   these slices will also be calculated.
 *   If you enter an invalid value
 *   (or leave the default), then this
 *   is ignored.
 *   
 * \returns false if unsuccessful
 * 
 * \date  2012-06-21
 * \author Rob Toth
 * \ingroup Segmentation
 */
bool SaveQuantitativeSegmentationResults(
  const VolumeMaskImageType * const segmentation,
  const VolumeMaskImageType * const groundTruth,
  const std::string         &       fileName,
  const VolumeMaskImageType * const midglandTemplateMask = 0,
  const float                       midglandSlicesPercentage = 0.0,
  const bool                        verbose = false );
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace ccipd
#endif // __ccipdQuantitativeSegmentationResults_h
