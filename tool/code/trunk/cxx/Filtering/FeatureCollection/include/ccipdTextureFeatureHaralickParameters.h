#ifndef __ccipdTextureFeatureHaralickParameters_h
#define __ccipdTextureFeatureHaralickParameters_h

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
  * \brief A set of parameters for Haralick filters.
  *  
  * This class provides the set of parameters required for Haralick filter.
  *
  * Haralick filters: TextureFeatureHaralick
  *
  * \author  Ahmad Algohary
  * \date    10/20/2013
  * \ingroup FeatureCollection
  */
class TextureFeatureHaralickParameters : public ParameterBase, 
                                         public Factory< TextureFeatureHaralickParameters>,
                                         public Serializable< TextureFeatureHaralickParameters>
{
//////////////////////////////////////////////////////////////////////////////////////////
  /** Constructors/Destructors */
  ccipdConstructorDeclarationMacro( TextureFeatureHaralickParameters )
  ccipdFactorySuperclassTypedefsMacro
//////////////////////////////////////////////////////////////////////////////////////////



public: 
//////////////////////////////////////////////////////////////////////////////////////////
  /** Constructor with user defined parameters */
  static Pointer New(  const VolRadiusType Radius, const VolOffsetType Offset, const unsigned int NumberOfHistogramBins);
  static Pointer New(  const unsigned int RadiusX, const unsigned int RadiusY, const unsigned int RadiusZ
                     , const unsigned int OffsetX, const unsigned int OffsetY, const unsigned int OffsetZ
                     , const unsigned int NumberOfHistogramBins
                    );
//////////////////////////////////////////////////////////////////////////////////////////


public: 
//////////////////////////////////////////////////////////////////////////////////////////  
  /** Add functions for saving / loading */
  virtual bool Load(const std::string & fileName);
  virtual bool Save(const std::string & fileName) const;

  /** == operator */
  virtual bool operator==( const Hashable & other ) const;
////////////////////////////////////////////////////////////////////////////////////////////
//
//
//
protected:
////////////////////////////////////////////////////////////////////////////////////////////
  virtual HashValueType ReHash() const;
////////////////////////////////////////////////////////////////////////////////////////////
//
//

public:
//////////////////////////////////////////////////////////////////////////////////////////
  /** Set/Get radius (in voxels) */  
  ccipdSetByvalDeclareMacro(Radius, VolRadiusType)
  ccipdGetConstDeclareMacro(Radius, VolRadiusType)


  /** Set/Get Offset */ 
  ccipdSetByvalDeclareMacro(Offset, VolOffsetType) 
  ccipdGetConstDeclareMacro(Offset, VolOffsetType)

  /** Set/Get NumberOfHistogramBins */  
  ccipdSetByvalDeclareMacro(NumberOfHistogramBins, unsigned int) 
  ccipdGetConstDeclareMacro(NumberOfHistogramBins, unsigned int)
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
  BOOST_CLASS_VERSION( ccipd::TextureFeatureHaralickParameters, 1 )
  ccipdBoostVirtualBaseFalse( ccipd::ParameterBase, ccipd::TextureFeatureHaralickParameters )
#endif // Boost_SERIALIZATION_FOUND



#endif //__ccipdTextureFeatureHaralickParameters_h
