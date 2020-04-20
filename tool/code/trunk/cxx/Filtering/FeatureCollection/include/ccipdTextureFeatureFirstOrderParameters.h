#ifndef __ccipdTextureFeatureFirstOrderParameters_h
#define __ccipdTextureFeatureFirstOrderParameters_h



//////////////////////////////////////////////////////////////////////////////////////////
// ccipd includes
#include "ccipdParameterBase.h"      // base class
#include "ccipdFeatureTypes.h"
#include "ccipdFilteringFeatureCollectionOptions.h"   // serialization options
#include "ccipdMacros.h"            // for commonly typed code   
#include "ccipdDisableWarningsMacro.h"
// std includes
#include <memory> // for smart pointers

// serialization includes
#ifdef Boost_SERIALIZATION_FOUND
  #include <boost/serialization/version.hpp>
  #include <boost/archive/xml_iarchive.hpp>
  #include <boost/archive/xml_oarchive.hpp> 
#endif // Boost_SERIALIZATION_FOUND
///////////////////////////////////i///////////////////////////////////////////////////////
#include "ccipdEnableWarningsMacro.h"


//////////////////////////////////////////////////////////////////////////////////////////
// serialization includes
#ifdef Boost_SERIALIZATION_FOUND
  ccipdSerializationForwardDeclarationMacro
#endif // Boost_SERIALIZATION_FOUND
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{

/** 
  * \brief A set of parameters for first order filters.
  *
  * This function provides the set of parameters (window size)
  * required for first order filters.
  *
  * First order filters: TextureFeatureMean, TextureFeatureMedian, TextureFeatureStandardDeviation, 
  * TextureFeatureRange, TextureFeatureNakagami, TextureFeatureRayleigh
  *
  * \author  Rachel Sparks
  * \date    2013-15-05
  * \ingroup FeatureCollection
  */
class TextureFeatureFirstOrderParameters : 
  public ParameterBase,
  public Factory< TextureFeatureFirstOrderParameters >,
  public Serializable< TextureFeatureFirstOrderParameters >
{



//////////////////////////////////////////////////////////////////////////////////////////
  /** Constructors/Desctructors */
  ccipdConstructorDeclarationMacro( TextureFeatureFirstOrderParameters )
  ccipdFactorySuperclassTypedefsMacro
//////////////////////////////////////////////////////////////////////////////////////////



public:
//////////////////////////////////////////////////////////////////////////////////////////
  /** Constructor with user defined parameters */
  static Pointer New( const VolumeRadiusType windowSize );
//////////////////////////////////////////////////////////////////////////////////////////



public:
//////////////////////////////////////////////////////////////////////////////////////////
    /** Add functions for saving / loading */
  virtual bool Load( const std::string & fileName);
  virtual bool Save( const std::string & fileName) const;
  
  /* == operator */
  virtual bool operator==( const Hashable & other ) const;
//////////////////////////////////////////////////////////////////////////////////////////



protected:
//////////////////////////////////////////////////////////////////////////////////////////
  virtual HashValueType ReHash() const;
//////////////////////////////////////////////////////////////////////////////////////////



public:
//////////////////////////////////////////////////////////////////////////////////////////
 /** Set/Get variables */
  ccipdSetByvalDeclareMacro( WindowSize, VolumeRadiusType )
  ccipdGetConstDeclareMacro( WindowSize, VolumeRadiusType )
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
#ifdef Boost_SERIALIZATION_FOUND
  /** We are serializable */
  ccipdSerializationDeclarationMacro
#endif // Boost_SERIALIZATION_FOUND
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
  ccipdPIMPLMacro
//////////////////////////////////////////////////////////////////////////////////////////



}; // class
} //namespace



#ifdef Boost_SERIALIZATION_FOUND
  BOOST_CLASS_VERSION( ccipd::TextureFeatureFirstOrderParameters, 1 )
  ccipdBoostVirtualBaseFalse(
    ccipd::ParameterBase,
    ccipd::TextureFeatureFirstOrderParameters )
#endif // Boost_SERIALIZATION_FOUND



#endif //__ccipdTextureFeatureFirstOrderParameters_h
