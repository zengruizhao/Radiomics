#ifndef __ccipdTextureFeatureGaborParameters_h
#define __ccipdTextureFeatureGaborParameters_h



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
  * \brief A set of parameters for gabor filters.
  *  
  * This class provides the set of parameters (Frequency, direction)
  * required for gabor filters.
  *
  * Gabor filters: TextureFeatureGabor
  *
  * \author  Rachel Sparks. Ahmad Algohary
  * \date    2013-25-06 - 08/15/2013
  * \ingroup FeatureCollection
  */
class TextureFeatureGaborParameters : public ParameterBase,
                                      public Factory< TextureFeatureGaborParameters >,
                                      public Serializable< TextureFeatureGaborParameters >
{



//////////////////////////////////////////////////////////////////////////////////////////
  /** Constructors/Destructors */
  ccipdConstructorDeclarationMacro( TextureFeatureGaborParameters )
  ccipdFactorySuperclassTypedefsMacro
//////////////////////////////////////////////////////////////////////////////////////////



public: 
//////////////////////////////////////////////////////////////////////////////////////////
  /** Constructor with user defined parameters */
  static Pointer New(const float frequency, const float direction);
//////////////////////////////////////////////////////////////////////////////////////////



public: 
//////////////////////////////////////////////////////////////////////////////////////////  
  /** Add functions for saving / loading */
  virtual bool Load(const std::string & fileName);
  virtual bool Save(const std::string & fileName) const;

  /** == operator */
  virtual bool operator==( const Hashable & other ) const;
//////////////////////////////////////////////////////////////////////////////////////////



protected:
//////////////////////////////////////////////////////////////////////////////////////////
  virtual HashValueType ReHash() const;
//////////////////////////////////////////////////////////////////////////////////////////



public:
//////////////////////////////////////////////////////////////////////////////////////////
  /** Set/Get Direction (in radians) */
  ccipdSetByvalDeclareMacro(Direction, float)
  ccipdGetConstDeclareMacro(Direction, float)


  /** Set/Get Frequency */
  ccipdSetByvalDeclareMacro(Frequency, float) // this controls windowSize too
  ccipdGetConstDeclareMacro(Frequency, float)
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
  BOOST_CLASS_VERSION( ccipd::TextureFeatureGaborParameters, 1 )
    ccipdBoostVirtualBaseFalse(ccipd::ParameterBase, ccipd::TextureFeatureGaborParameters )
#endif // Boost_SERIALIZATION_FOUND



#endif //__ccipdTextureFeatureGaborParameters_h
