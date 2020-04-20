#ifndef __ccipdSIMBoundingBoxUnion_h
#define __ccipdSIMBoundingBoxUnion_h



//////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdAlignerForwardDeclarations.h"
#include "ccipdSIMTypes.h"
#include "ccipdPatientTypes.h"
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////
/**
 * Take a collection
 * of studies,
 * and get the final
 * bounding box
 * for the SIM model
 * from those studies.
 * 
 * The bounding box will
 * be in terms of 
 * the template volume.
 * 
 * \param[in] extractor
 *   To get a bounding box from each study.
 * 
 * \param[in] aligner
 *   How to transform each study.
 *   
 */
SIMBoundingBoxType GetBoundingBox(
  const VolumeImageBaseConstPointer & templateImage,
  const StudyConstCollectionType    & studies,
  const SIMBoundingBoxExtractor     & extractor,
  const StudyAligner                & aligner,
  const float                         padding = 0.0 );
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace ccipd
#endif // __ccipdSIMBoundingBoxUnion_h
