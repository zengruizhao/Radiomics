#ifndef __ccipdMFABoundingBoxTraining_h
#define __ccipdMFABoundingBoxTraining_h



//////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdAffineTypes.h"
#include "ccipdPatientTypes.h"
#include "ccipdMFATypes.h"
#include "ccipdAlignerTypes.h"
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////
AffineDistributionPointer GetAffineDistribution(
  const StudyCollectionType & studies,
  const MFABoundingBoxExtractor  & extractor,
  const StudyAffineAligner       & aligner,
  const unsigned int               repeatsPerStudy = 0 );
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
AffineDistributionPointer GetAffineDistribution(
  const StudyConstCollectionType & studies,
  const MFABoundingBoxExtractor  & extractor,
  const StudyAffineAligner       & aligner,
  const unsigned int               repeatsPerStudy = 0 );
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
AffineGaussianDistributionPointer GetAffineGaussianDistribution(
  const StudyCollectionType & studies,
  const MFABoundingBoxExtractor  & extractor,
  const StudyAffineAligner       & aligner,
  const unsigned int               repeatsPerStudy = 0 );
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
/**
 * For a collection of studies which
 * have bounding boxes defined, and transforms
 * defined,
 * we will learn a distribution of
 * transformations WITHING each bounding box.
 * 
 * For example, for a given study,
 * you can first transform the template
 * bounding box onto the study's bounding box.
 * Then, you will know what the expected
 * transformation is to apply once
 * you are within the bounding box.
 * 
 * \author  Rob Toth
 * \date    2013-02-08
 * \ingroup MFA
 */
AffineGaussianDistributionPointer GetAffineGaussianDistribution(
  const StudyConstCollectionType & studies,
  const MFABoundingBoxExtractor  & extractor,
  const StudyAffineAligner       & aligner,
  const unsigned int               repeatsPerStudy = 0 );
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace ccipd
#endif // __ccipdMFABoundingBoxTraining_h
