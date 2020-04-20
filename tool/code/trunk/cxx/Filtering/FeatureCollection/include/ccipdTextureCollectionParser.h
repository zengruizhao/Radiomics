#ifndef __ccipdTextureCollectionParser_h
#define __ccipdTextureCollectionParser_h



//////////////////////////////////////////////////////////////////////////////////////////
// ccipd includes
#include "ccipdFeatureCollectionTypes.h"
#include "ccipdDisableWarningsMacro.h"
// std includes
#include <string>
#include <vector>
//////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdEnableWarningsMacro.h"


namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////
/**
 * Parser a collection of texture strings into enums.
 * 
 * Since not every string has an associated enum,
 * the output collection may have fewer elements
 * than the input collection.
 * 
 * \author  Rob Toth
 * \date    2013-01-23
 * \ingroup FeatureCollection
 */
template< class TStringCollection >
TextureFeatureEnumCollection GetTextureEnums( const TStringCollection & textureStrings );
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace ccipd
#endif // __ccipdTextureCollectionParser_h
