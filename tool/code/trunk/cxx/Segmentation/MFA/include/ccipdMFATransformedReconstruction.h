#ifndef __ccipdMFATransformedReconstruction_h
#define __ccipdMFATransformedReconstruction_h



//////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdMFATypes.h"
#include "ccipdVolumeTypes.h"
#include "ccipdFeatureCollectionTypes.h"
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////
VolumeImagePointer GetTransformedReconstruction(
  const MFAVolumeReconstructionFilter & filter,
  const FeatureIDConstPointer         & whichFeature );
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace ccipd
#endif // __ccipdMFATransformedReconstruction_h
