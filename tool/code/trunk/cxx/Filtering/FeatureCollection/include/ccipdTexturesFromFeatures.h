#ifndef __ccipdTexturesFromFeatures_h
#define __ccipdTexturesFromFeatures_h



//////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdFeatureCollectionTypes.h"
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////
/**
 * Return the associated texture enum
 * for each feature that represents
 * a texture.
 * 
 * It's therefore possible that
 * the size of the output is
 * less than the size of the input.
 * 
 * \ingroup FeatureCollection
 * \author  Rob Toth
 * \date    2013-01-21
 */
TextureFeatureEnumCollection GetTexturesFromFeatures(
  const FeatureIDConstCollection & whichFeatures );
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace ccipd
#endif // __ccipdTexturesFromFeatures_h
