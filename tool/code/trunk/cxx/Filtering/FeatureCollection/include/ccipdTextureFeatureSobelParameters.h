#ifndef __ccipdTextureFeatureSobelParameters_h
#define __ccipdTextureFeatureSobelParameters_h



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
//////////////////////////////////////////////////////////////////////////////////////////
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
  * \brief A set of parameters for sobel filters.
  *  
  * This function provides the set of parameters (window size, direction)
  * required for sobel filters.
  *
  * Sobel filters: TextureFeatureSobel
  *
  * \author  Rachel Sparks
  * \date    2013-15-05
  * \ingroup FeatureCollection
  */
class TextureFeatureSobelParameters : 
  public ParameterBase,
  public Factory< TextureFeatureSobelParameters >,
  public Serializable< TextureFeatureSobelParameters >
{



//////////////////////////////////////////////////////////////////////////////////////////
  /** Constructors/Desctructors */
  ccipdConstructorDeclarationMacro( TextureFeatureSobelParameters )
  ccipdFactorySuperclassTypedefsMacro
//////////////////////////////////////////////////////////////////////////////////////////



public: 
//////////////////////////////////////////////////////////////////////////////////////////
  /** Constructor with user defined parameters */
  static Pointer New( const VolumeRadiusType windowSize, const unsigned int direction );
//////////////////////////////////////////////////////////////////////////////////////////



public: 
//////////////////////////////////////////////////////////////////////////////////////////  
  /** Add functions for saving / loading */
  virtual bool Load( const std::string & fileName);
  virtual bool Save( const std::string & fileName) const;

  /** == operator */
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
  
  ccipdSetByvalDeclareMacro( Direction, unsigned int )
  ccipdGetConstDeclareMacro( Direction, unsigned int )
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
  BOOST_CLASS_VERSION( ccipd::TextureFeatureSobelParameters, 1 )
    ccipdBoostVirtualBaseFalse(
    ccipd::ParameterBase,
    ccipd::TextureFeatureSobelParameters )
#endif // Boost_SERIALIZATION_FOUND



#endif //__ccipdTextureFeatureSobelParameters_h
