#ifndef __ccipdMFAGetReconstruction_h
#define __ccipdMFAGetReconstruction_h



//////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdVolumeTypes.h"
#include "ccipdMFATypes.h"
#include "ccipdPatientTypes.h"
#include "ccipdFeatureCollectionTypes.h"
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////
VolumeImagePointer GetReconstruction(
  const MFAVolumeReconstructionFilter & filter,
  const FeatureIDConstPointer         & whichFeature,
  const VolumeImageBaseConstPointer   & outputTemplateImage,
  const VolumeTransformConstPointer   & finalTransform );
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace ccipd
#endif // __ccipdMFAGetReconstruction_h
