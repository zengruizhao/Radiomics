


//////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdTextureFeatureGetParametersByVariant.h"
//#include "ccipdTextureFeatureFirstOrderParameters.h"
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{

const ParameterPointer 
GetParametersByVariant::operator()( const ParameterPointer parameters ) const
{
  return parameters;
}

const TextureFeatureGaborParameterPointer 
GetParametersByVariant::operator()( const TextureFeatureGaborParameterPointer parameters ) const
{
  return parameters;
}
  
const TextureFeatureHaralickParameterPointer 
GetParametersByVariant::operator()( const TextureFeatureHaralickParameterPointer parameters ) const
{
	return parameters;
}

const TextureFeatureFirstOrderParameterPointer
GetParametersByVariant::operator()( const TextureFeatureFirstOrderParameterPointer parameters ) const
{
  return parameters;
}
  
const TextureFeatureSobelParameterPointer
GetParametersByVariant::operator()( const TextureFeatureSobelParameterPointer parameters ) const
{
  return parameters;
}
  

} // ccipd namespace


