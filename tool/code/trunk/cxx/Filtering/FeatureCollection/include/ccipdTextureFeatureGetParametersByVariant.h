#ifndef __ccipdTextureFeatureGetParametersByVariant_h
#define __ccipdTextureFeatureGetParametersByVariant_h



//////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdFeatureCollectionTypes.h"
#include "ccipdMacros.h"

#include <boost/variant/static_visitor.hpp>
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{


/* Deduce the class type for the AnyTextureFeatureParameters variant */
class GetParametersByVariant :
  public boost::static_visitor< ParameterPointer >
{



//////////////////////////////////////////////////////////////////////////////////////////
public: 
  const ParameterPointer operator()( const ParameterPointer parameters ) const;
  
  const TextureFeatureFirstOrderParameterPointer
    operator()( const TextureFeatureFirstOrderParameterPointer parameters ) const;
  
  const TextureFeatureSobelParameterPointer
    operator()( const TextureFeatureSobelParameterPointer parameters ) const;
  
  const TextureFeatureGaborParameterPointer operator()( const TextureFeatureGaborParameterPointer parameters ) const;

  const TextureFeatureHaralickParameterPointer operator()( const TextureFeatureHaralickParameterPointer parameters ) const;
//////////////////////////////////////////////////////////////////////////////////////////



};

} // ccipd namespace
//////////////////////////////////////////////////////////////////////////////////////////


#endif // __ccipdTextureFeatureGetParametersByVariant_h
