#ifndef __ccipdTextureFeatureParser_h
#define __ccipdTextureFeatureParser_h



//////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdFeatureCollectionTypes.h"
#include "ccipdMacros.h"
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



/**
 * \brief Parse texture features from either enum or string
 * 
 * Since all the methods are static, you simply have to
 * call TextureFeatureParser::PrintAllTextureFeatures() for example
 * and don't have to actually create an object.
 * 
 * This class throws an exception
 * if the given string isn't found, for example.
 * 
 * \author  Rachel Sparks
 * \ingroup FeatureCollection
 */
class TextureFeatureParser
{



public:
//////////////////////////////////////////////////////////////////////////////////////////

  /** Return the TextureFeatureEnum that corresonds to a string */
  static TextureFeatureEnum GetTextureFeatureEnum(
    const std::string & whichFeatureName ); 

  /** Return the string that corresonds to a TextureFeatureEnum */
  static std::string GetTextureFeatureString(
    const TextureFeatureEnum whichFeature );
  
  /** Print out all possible Texture Feature strings */
  static void PrintAllTextureFeatures();
  
  /** Get a collection of texture feature strings. */
  static std::vector< std::string > GetAllTextureFeatures();

//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
  /// To store our internal data types.
  class SecretData;
//////////////////////////////////////////////////////////////////////////////////////////



}; //TextureFeatureParser



} // ccipd namespace
#endif // __ccipdTextureFeatureParser_h