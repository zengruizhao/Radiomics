#ifndef __ccipdTextureFeatureExtractor_hxx
#define __ccipdTextureFeatureExtractor_hxx



//////////////////////////////////////////////////////////////////////////////////////////
// ccipd includes
#include "ccipdTextureFeatureExtractor.h"
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
using std::cerr;
using std::cout;
using std::endl;
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////
template< TextureFeatureEnum TWhichFeature, 
          unsigned int       TParameterIndex >
void 
TextureFeatureExtractor::SetParameter( const typename MapParametersType< TWhichFeature,
	                           TParameterIndex 
	                         >::ParameterType & parameter)
{

  typedef MapParametersType< TWhichFeature, TParameterIndex > whichParameterMap;

  // check to make sure we are not trying to input an index that is out of bounds
    static_assert( TParameterIndex < whichParameterMap::ParameterSize,
      "Invalid index for this feature." );

  // get the parameters tuple
  whichParameterMap::AllParametersType whichParameters = 
    boost::any_cast< whichParameterMap::AllParametersType >( this->m_SecretData->m_ParameterMap.at( TWhichFeature ) );

  // set the parameter value
  std::get< TParameterIndex > (whichParameters) = parameter;

} // SetParameter
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
template< TextureFeatureEnum TWhichFeature,
             unsigned int      TParameterIndex >
typename MapParametersType< TWhichFeature, TParameterIndex >::ParameterType
TextureFeatureExtractor::GetParameter() const
{

  typedef MapParametersType< TWhichFeature, TParameterIndex > whichParameterMap;

  // check to make sure we are not trying to get an index that is out of bounds
    static_assert( TParameterIndex < whichParameterMap::ParameterSize,
      "Invalid index for this feature." );

  // get the parameters tuple
  whichParameterMap::AllParametersType whichParameters = 
    boost::any_cast< whichParameterMap::AllParametersType >( this->m_SecretData->m_ParameterMap.at( TWhichFeature ) );

  // get the parameter value 
  return std::get< TParameterIndex > (whichParameters);
} // GetParameter
//////////////////////////////////////////////////////////////////////////////////////////



} // ccipd namespace
#endif // __ccipdTextureFEatureExtractor_hxx