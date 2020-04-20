

//////////////////////////////////////////////////////////////////////////////////////////
// ccipd includes
#include "ccipdTextureCollectionParser.hxx"
#include "ccipdDisableWarningsMacro.h"
// std includes
#include <vector>
#include <set>
//////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdEnableWarningsMacro.h"


namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////
template
TextureFeatureEnumCollection GetTextureEnums(
  const std::vector< std::string > & textureStrings );
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
template
TextureFeatureEnumCollection GetTextureEnums(
  const std::set< std::string > & textureStrings );
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace ccipd