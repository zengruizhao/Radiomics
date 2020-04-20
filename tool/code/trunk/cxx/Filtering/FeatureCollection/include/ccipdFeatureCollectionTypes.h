#ifndef __ccipdFeatureCollectionTypes_h
#define __ccipdFeatureCollectionTypes_h



/**
 * This file contains common types for our
 * texture and non-texture features.
 */



//////////////////////////////////////////////////////////////////////////////////////////
// ccipd includes 
#include "ccipdFeatureTypes.h"
#include "ccipdDisableWarningsMacro.h"
// std includes
#include <set>
#include <vector>
#include <memory> // for smart pointers
#include <string> // for feature description

// boost includes
#include <boost/variant.hpp>
//////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdEnableWarningsMacro.h"

#define  TEXTURE_FEATURE_INTENSITY           "Intensity"        
#define  TEXTURE_FEATURE_MEAN                "Mean"             
#define  TEXTURE_FEATURE_MEDIAN              "Median"           
#define  TEXTURE_FEATURE_SKEW                "Skew"             
#define  TEXTURE_FEATURE_STDEV               "StandardDeviation"
#define  TEXTURE_FEATURE_RANGE               "Range"            
#define  TEXTURE_FEATURE_GABOR               "Gabor"            
#define  TEXTURE_FEATURE_HARALICK            "Haralick"         
#define  TEXTURE_FEATURE_GRADMAGNITUDE       "GradientMagnitude"
#define  TEXTURE_FEATURE_NAKAGAMI            "Nakagami"         
#define  TEXTURE_FEATURE_RAYLEIGH            "Rayleigh"         
#define  TEXTURE_FEATURE_SOBEL               "Sobel"  
#define  TEXTURE_FEATURE_CORRELATION         "Correlation"  

#define  TEXTURE_FEATURE_HARALICK_ENERGY             "Haralick_Energy" 
#define  TEXTURE_FEATURE_HARALICK_ENTROPY            "Haralick_Entropy"
#define  TEXTURE_FEATURE_HARALICK_INVDIFFMOMENT      "Haralick_InverseDifferenceMoment"
#define  TEXTURE_FEATURE_HARALICK_INERTIA            "Haralick_Inertia"
#define  TEXTURE_FEATURE_HARALICK_CLUSTERSHADE       "Haralick_ClusterShade"            
#define  TEXTURE_FEATURE_HARALICK_CLUSTERPROMINENCE  "Haralick_ClusterProminence"       
#define  TEXTURE_FEATURE_HARALICK_CORRELATION        "Haralick_HaralickCorrelation"     
#define  TEXTURE_FEATURE_HARALICK_MEAN               "Haralick_Mean"                    
#define  TEXTURE_FEATURE_HARALICK_VARIANCE           "Haralick_Variance"                
#define  TEXTURE_FEATURE_HARALICK_SUMAVERAGE         "Haralick_SumAverage"              
#define  TEXTURE_FEATURE_HARALICK_SUMVARIANCE        "Haralick_SumVariance"             
#define  TEXTURE_FEATURE_HARALICK_SUMENTROPY         "Haralick_SumEntropy"
#define  TEXTURE_FEATURE_HARALICK_DIFFERENCEENTROPY  "Haralick_DifferenceEntropy"
#define  TEXTURE_FEATURE_HARALICK_DIFFERENCEVARIANCE "Haralick_DifferenceVariance"      
#define  TEXTURE_FEATURE_HARALICK_IMC1               "Haralick_IMC1"
#define  TEXTURE_FEATURE_HARALICK_IMC2               "Haralick_IMC2"


namespace ccipd
{

//////////////////////////////////////////////////////////////////////////////////////////
/**
 * This lists all our implemented texture features. 
 * Please add to this when you implement a new feature.
 */
enum TextureFeatureEnum
{
  TextureFeatureIntensity,
  TextureFeatureMean,
  TextureFeatureMedian,
  TextureFeatureSkew,
  TextureFeatureStandardDeviation,
  TextureFeatureRange,
  TextureFeatureGabor,
  TextureFeatureHaralick,

  TextureFeatureHaralickEnergy                  ,
  TextureFeatureHaralickEntropy                 ,
  TextureFeatureHaralickInverseDifferenceMoment ,
  TextureFeatureHaralickInertia                 ,
  TextureFeatureHaralickClusterShade            ,
  TextureFeatureHaralickClusterProminence       ,
  TextureFeatureHaralickCorrelation             ,
  TextureFeatureHaralickMean                    ,
  TextureFeatureHaralickVariance                ,
  TextureFeatureHaralickSumAverage              ,
  TextureFeatureHaralickSumVariance             ,
  TextureFeatureHaralickSumEntropy              ,
  TextureFeatureHaralickDifferenceEntropy       ,
  TextureFeatureHaralickDifferenceVariance      ,
  TextureFeatureHaralickIMC1                    ,
  TextureFeatureHaralickIMC2                    ,

  TextureFeatureCorrelation                     ,
  TextureFeatureGradientMagnitude               ,
  TextureFeatureNakagami                        ,
  TextureFeatureRayleigh                        ,
  TextureFeatureSobel
};
//////////////////////////////////////////////////////////////////////////////////////////
          


//////////////////////////////////////////////////////////////////////////////////////////
// forward declare parameter types for our implemented texture features
class ParameterBase                     ;
class TextureFeatureFirstOrderParameters;
class TextureFeatureSobelParameters     ; 
class TextureFeatureGaborParameters     ;
class TextureFeatureHaralickParameters  ;

typedef std::shared_ptr< ParameterBase                      >   ParameterPointer                        ;
typedef std::shared_ptr< const ParameterBase                >   ParameterCostPointer                    ;
typedef std::shared_ptr< TextureFeatureFirstOrderParameters >   TextureFeatureFirstOrderParameterPointer;
typedef std::shared_ptr< TextureFeatureSobelParameters      >   TextureFeatureSobelParameterPointer     ;
typedef std::shared_ptr< TextureFeatureGaborParameters      >   TextureFeatureGaborParameterPointer     ;
typedef std::shared_ptr< TextureFeatureHaralickParameters   >   TextureFeatureHaralickParameterPointer  ;
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
/* A list of all the possible parameter types for our texture features. 
 * Please add to it when you introduce a new parameter class.
 */
typedef boost::variant< ParameterPointer,
                        TextureFeatureFirstOrderParameterPointer,
                        TextureFeatureSobelParameterPointer,
                        TextureFeatureGaborParameterPointer,
						TextureFeatureHaralickParameterPointer
                      > AnyTextureFeatureParameters;
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
/// Define how to hash a texture feature enum.
inline
std::size_t hash_value( const TextureFeatureEnum & textureFeature )
{
  return static_cast< std::size_t >( textureFeature );
} // hash_value
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
typedef std::string                           FeatureDescriptionType   ;
typedef std::shared_ptr< TextureFeatureEnum > TextureFeatureEnumPointer;
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
// forward declare our classes
class TextureFeatureExtractor;
class FeatureID;
class FeatureCollection;

typedef std::shared_ptr< FeatureCollection > FeatureCollectionPointer;
typedef std::shared_ptr<       FeatureID   > FeatureIDPointer        ;
typedef std::shared_ptr< const FeatureID   > FeatureIDConstPointer   ;
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
// containers
typedef std::vector< FeatureIDPointer            > FeatureIDCollection               ;
typedef std::vector< FeatureIDConstPointer       > FeatureIDConstCollection          ;
typedef std::vector< TextureFeatureEnum          > TextureFeatureEnumCollection      ;
typedef std::vector< AnyTextureFeatureParameters > TextureFeatureParametersCollection;
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace
#endif // __ccipdFeatureTypes_h
