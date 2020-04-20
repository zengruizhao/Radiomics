#ifndef __ccipdTextureCollectionParser_hxx
#define __ccipdTextureCollectionParser_hxx



//////////////////////////////////////////////////////////////////////////////////////////
// ccipd includes
#include "ccipdTextureCollectionParser.h"
#include "ccipdTextureFeatureParser.h"
#include "ccipdDisableWarningsMacro.h"
// boost includes
#include <boost/foreach.hpp>
//////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdEnableWarningsMacro.h"


namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////
template< class TStringCollection >
TextureFeatureEnumCollection GetTextureEnums( const TStringCollection & textureStrings )
{

  // initialize the output
  TextureFeatureEnumCollection enums;

  // loop through each string
  BOOST_FOREACH( const std::string & textureString, textureStrings )
  {
    // try to parse it
    try
    {
      const TextureFeatureEnum textureEnum = TextureFeatureParser::GetTextureFeatureEnum( textureString );
      enums.push_back( textureEnum );
    } // try
    catch( ... ) 
	{

	}
  } // for each string

  // output
  return enums;

} // GetTextureEnums
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace ccipd
#endif // __ccipdTextureCollectionParser_hxx
