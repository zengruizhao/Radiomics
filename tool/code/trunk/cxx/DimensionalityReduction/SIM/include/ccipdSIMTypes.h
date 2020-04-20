#ifndef __ccipdSIMTypes_h
#define __ccipdSIMTypes_h




//////////////////////////////////////////////////////////////////////////////////////////

// ccipd includes
#include "ccipdVolumeTypes.h"
#include "ccipdMatrixTypes.h"
#include "ccipdSIMForwardDeclarations.h"

// std includes
#include <memory>

//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////
/**
 * \addtogroup SIM
 * @{
 */

typedef SIMStorage<
  InternalPixelType,
  VolumeDimension 
> SIMVolumeStorage;


typedef VolumeRegionType SIMBoundingBoxType;

typedef std::shared_ptr<       SIMVolumeStorage           > SIMVolumeStoragePointer;
typedef std::shared_ptr< const SIMVolumeStorage           > SIMVolumeStorageConstPointer;
typedef std::shared_ptr<       SIMBoundingBoxExtractor    > SIMBoundingBoxExtractorPointer;
typedef std::shared_ptr< const SIMBoundingBoxExtractor    > SIMBoundingBoxExtractorConstPointer;
typedef std::shared_ptr<       SIMFeatureExtractor        > SIMFeatureExtractorPointer;
typedef std::vector<           SIMFeatureExtractorPointer > SIMFeatureExtractorCollection;
typedef std::shared_ptr<       SIMStudiesAligner          > SIMStudiesAlignerPointer;
typedef std::shared_ptr< const SIMStudiesAligner          > SIMStudiesAlignerConstPointer;
typedef std::shared_ptr<       SIMStudiesMaskAligner      > SIMStudiesMaskAlignerPointer;
typedef std::shared_ptr< const SIMStudiesMaskAligner      > SIMStudiesMaskAlignerConstPointer;
//////////////////////////////////////////////////////////////////////////////////////////



/** @} */

} // namespace
#endif //__ccipdSIMTypes_h