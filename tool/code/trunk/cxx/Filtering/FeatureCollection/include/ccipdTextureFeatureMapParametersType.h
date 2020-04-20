#ifndef __ccipdTextureFeatureMapParametersType_h
#define __ccipdTextureFeatureMapParametersType_h



//////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdFeatureTypes.h"
#include "ccipdMacros.h"
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



/**
 * \brief To store and retrieve ParameterTypes for our TextureFeatureEnum Types
 * 
 *
 * \author  Rachel Sparks
 * \ingroup FeatureCollection
 */



//////////////////////////////////////////////////////////////////////////////////////////
/* Template to map parameter types for each enum */
template< TextureFeatureEnum TWhichFeature > struct MapParametersType;
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
/// Implementation for each specific TextureFeature.

/* TextureFeatureGradientMagnitue has no parameters */
template <>
struct MapParametersType< TextureFeatureGradientMagnitude > 
{
  typedef ParameterBase AllParametersType; //Default type for no parameters
  static const unsigned int ParameterSize = 0;
};

/* TextureFeatureIntensity has no parameters. */
template <>
struct MapParametersType< TextureFeatureIntensity > 
{
  typedef ParameterBase AllParametersType;
  static const unsigned int ParameterSize = 0;
};

/* TextureFeatureMean has a double to control window size (in mm) */
template <>
struct MapParametersType< TextureFeatureMean > 
{
  typedef ccipd::TextureFeatureFirstOrderParameters AllParametersType;
  static const unsigned int ParameterSize = 1;
};

/* TextureFeatureMedian has a double to control window size (in mm) */
template <>
struct MapParametersType< TextureFeatureMedian > 
{
  typedef ccipd::TextureFeatureFirstOrderParameters AllParametersType;
  static const unsigned int ParameterSize = 1;
};

/* TextureFeatureNakagami has a double to control window size (in mm) */
template <>
struct MapParametersType< TextureFeatureNakagami > 
{
  typedef ccipd::TextureFeatureFirstOrderParameters AllParametersType;
  static const unsigned int ParameterSize = 1;
};

/* TextureFeatureRange has a double to control window size (in mm) */
template <>
struct MapParametersType< TextureFeatureRange > 
{
  typedef ccipd::TextureFeatureFirstOrderParameters AllParametersType;
  static const unsigned int ParameterSize = 1;
};

/* TextureFeatureRayleigh has a double to control window size (in mm) */
template <>
struct MapParametersType< TextureFeatureRayleigh > 
{
  typedef ccipd::TextureFeatureFirstOrderParameters AllParametersType;
  static const unsigned int ParameterSize = 1;
};

/* TextureFeatureSkew has a double to control window size (in mm) */
template <>
struct MapParametersType< TextureFeatureSkew > 
{  
  typedef ccipd::TextureFeatureFirstOrderParameters AllParametersType;
  static const unsigned int ParameterSize = 1;
};

/* TextureFeatureStandardDeviation has a double to control window size (in mm) */
template <>
struct MapParametersType< TextureFeatureStandardDeviation > 
{  
  typedef ccipd::TextureFeatureFirstOrderParameters AllParametersType;
  static const unsigned int ParameterSize = 1;
};

/* TextureFeatureSobel has (1) double to control window size (in mm)  
                           (2) unsigned int to control direction (0 corresponds to 1st direction, 1 to 2nd direction etc.) */
template <>
struct MapParametersType< TextureFeatureSobel > 
{
  typedef ccipd::TextureFeatureSobelParameters AllParametersType;
  static const unsigned int ParameterSize = 2;
};


/* TextureFeatureGabor has (1) double to control window size (in mm)  
                           (2) float to control direction (in radians) */
template <>
struct MapParametersType< TextureFeatureGabor > 
{
  typedef ccipd::TextureFeatureGaborParameters AllParametersType;
  static const unsigned int ParameterSize = 2;
};
//////////////////////////////////////////////////////////////////////////////////////////
/* TextureFeatureHaralick has 
   (1) VolumeRadiusType to control radius (in voxels) along the 3 directions 
   (2) VolumeOffsetType to control offset (in voxels) along the 3 directions
   (3) unsigned int     to control number of histogram bins per axis
 */
template <>
struct MapParametersType< TextureFeatureHaralick > 
{
	typedef ccipd::TextureFeatureHaralickParameters AllParametersType;
	static const unsigned int ParameterSize = 3;
};


} // ccipd namespace



#endif // __ccipdTextureFeatureMapParametersType

