


//////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdMFATextureExtractorClone.h"
#include "ccipdMFATextureExtractor.h"
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////
MFATextureExtractorPointer Clone( const MFATextureExtractor & extractor )
{

  // first create a new extractor
  const MFATextureExtractorPointer clonedExtractor( MFATextureExtractor::New() );

  // now set all the properties
  clonedExtractor->SetAligner             ( extractor.GetAligner             () );
  clonedExtractor->SetBoundingBoxExtractor( extractor.GetBoundingBoxExtractor() );
  clonedExtractor->SetWhichFeatureObjects ( extractor.GetWhichFeatureObjects () );

  // output
  return clonedExtractor;

} // Clone
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace ccipd