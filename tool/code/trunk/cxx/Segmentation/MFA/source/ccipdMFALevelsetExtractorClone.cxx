


//////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdMFALevelsetExtractorClone.h"
#include "ccipdMFALevelsetExtractor.h"
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////
MFALevelsetExtractorPointer Clone( const MFALevelsetExtractor & extractor )
{

  // first create a new extractor
  const MFALevelsetExtractorPointer clonedExtractor( MFALevelsetExtractor::New() );

  // now set all the properties
  clonedExtractor->SetAligner             ( extractor.GetAligner             () );
  clonedExtractor->SetBoundingBoxExtractor( extractor.GetBoundingBoxExtractor() );
  clonedExtractor->SetNormalizeLevelset   ( extractor.GetNormalizeLevelset   () );
  clonedExtractor->SetSigmoidLevelset     ( extractor.GetSigmoidLevelset     () );
  clonedExtractor->SetSigmoidWidth        ( extractor.GetSigmoidWidth        () );
  clonedExtractor->SetSmoothingVariance   ( extractor.GetSmoothingVariance   () );
  clonedExtractor->SetWhichLevelsetObjects( extractor.GetWhichLevelsetObjects() );

  // output
  return clonedExtractor;

} // Clone
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace ccipd