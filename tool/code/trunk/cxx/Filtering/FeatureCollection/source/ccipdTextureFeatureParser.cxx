

//////////////////////////////////////////////////////////////////////////////////////////
// ccipd includes
#include "ccipdTextureFeatureParser.h"

#include "ccipdDisableWarningsMacro.h"
  // boost includes
  #include <boost/bimap/unordered_set_of.hpp>
  #include <boost/bimap/bimap.hpp>
  #include <boost/assign.hpp>
  #include <boost/foreach.hpp>


// std includes
#include <iostream>
//////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdEnableWarningsMacro.h"


namespace ccipd
{

	

//////////////////////////////////////////////////////////////////////////////////////////
class TextureFeatureParser::SecretData
{

public: // public to the TextureFeatureParser class

  /* Type Defs */
  struct featureEnum{};
  struct featureName{};

  typedef boost::bimaps::bimap<
    boost::bimaps::unordered_set_of< boost::bimaps::tagged< TextureFeatureEnum, featureEnum> >,
    boost::bimaps::unordered_set_of< boost::bimaps::tagged < std::string, featureName> >
  > TextureFeatureEnumStringMap;

  /* Variables */
  static const TextureFeatureEnumStringMap m_TextureEnumString;

 }; // TextureFeatureParser::SecretData
//////////////////////////////////////////////////////////////////////////////////////////


    
//////////////////////////////////////////////////////////////////////////////////////////
const TextureFeatureParser::SecretData::TextureFeatureEnumStringMap
  TextureFeatureParser::SecretData::m_TextureEnumString =
  boost::assign::list_of< TextureFeatureEnumStringMap::relation>
    (TextureFeatureGabor             , "TextureFeatureGabor"                )
    (TextureFeatureHaralick          , "TextureFeatureHaralick"             )

	(TextureFeatureHaralick          , "TextureFeatureHaralickEnergy"             )
	(TextureFeatureHaralick          , "TextureFeatureHaralickInertia"             )
	(TextureFeatureHaralick          , "TextureFeatureHaralickDifferenceEntropy"   )
	(TextureFeatureHaralick          , "TextureFeatureHaralickSumEntropy"          )
	(TextureFeatureHaralick          , "TextureFeatureHaralickIMC1"                )
	(TextureFeatureHaralick          , "TextureFeatureHaralickIMC2"                )

	(TextureFeatureHaralick          , "TextureFeatureCorrelation"          )


    (TextureFeatureGradientMagnitude , "TextureFeatureGradientMagnitude"    )
    (TextureFeatureIntensity         , "TextureFeatureIntensity"            )
    (TextureFeatureMean              , "TextureFeatureMean"                 )
    (TextureFeatureMedian            , "TextureFeatureMedian"               )
    (TextureFeatureNakagami          , "TextureFeatureNakagami"             )
    (TextureFeatureRange             , "TextureFeatureRange"                )
    (TextureFeatureRayleigh          , "TextureFeatureRayleigh"             )
  	(TextureFeatureSobel             , "TextureFeatureSobel"                )
    (TextureFeatureStandardDeviation , "TextureFeatureStandardDeviation"    )
    (TextureFeatureSkew              , "TextureFeatureSkew"                 );
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
TextureFeatureEnum TextureFeatureParser::GetTextureFeatureEnum(
  const std::string & whichFeatureName )
{
 return SecretData::m_TextureEnumString.right.at( whichFeatureName );
} // GetTextureFeatureEnum
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
std::string TextureFeatureParser::GetTextureFeatureString(
  const TextureFeatureEnum whichFeature )
{
  return SecretData::m_TextureEnumString.left.at( whichFeature );
} // GetTextureFeatureEnum
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void TextureFeatureParser::PrintAllTextureFeatures()
{

  BOOST_FOREACH( const std::string & textureFeatureString, GetAllTextureFeatures() )
    {
    std::cout << textureFeatureString << std::endl;
    } // for each string

} // PrintAllTextureFeatures
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
std::vector< std::string > TextureFeatureParser::GetAllTextureFeatures()
{

  // prepare the output
  std::vector< std::string > textureFeatureStrings;

  SecretData::TextureFeatureEnumStringMap::const_iterator 
    mapItr = SecretData::m_TextureEnumString.begin();
  const SecretData::TextureFeatureEnumStringMap::const_iterator 
    endItr = SecretData::m_TextureEnumString.end();

  for( ; mapItr != endItr; ++mapItr )
    {
    textureFeatureStrings.push_back( 
      mapItr->get<SecretData::featureName>() );
    }

  // output
  return textureFeatureStrings;

} //GetAllTextureFeatures
//////////////////////////////////////////////////////////////////////////////////////////



} //namespace ccipd
