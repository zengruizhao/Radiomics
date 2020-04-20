#ifndef __ccipdStandardDistribution_hxx
#define __ccipdStandardDistribution_hxx



//////////////////////////////////////////////////////////////////////////////////////////
// ccipd includes
#include "ccipdStandardDistribution.h"

// std includes
#include <random>
#include <type_traits>
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{


//////////////////////////////////////////////////////////////////////////////////////////
template< typename TDataType, typename TDistributionType >
class StandardDistribution< TDataType, TDistributionType >::SecretData
{
  
public:

  /// Make sure that TDistributionType returns TDataType
  static_assert(
    std::is_same< TDataType, typename TDistributionType::result_type >::value,
    "The data type and distribution return type must be equal!" );
  
  typedef std::default_random_engine GeneratorType;

  GeneratorType     m_Generator;
  TDistributionType m_Distribution;

}; // class SecretData
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
template< typename TDataType, typename TDistributionType >
StandardDistribution< TDataType, TDistributionType >
::StandardDistribution() :
ccipdNewPIMPLMacro
{

#if defined(_MSC_VER) && _MSC_VER <= 1600
  this->m_SecretData->m_Generator.seed();

#else

#ifdef NDEBUG
  // use a hardware device in release mode
  std::random_device hardwareRandom;
#endif

  const std::random_device::result_type randomSeed =
#ifdef NDEBUG
    hardwareRandom(); // use a hardware seed
#else
    1; // use the same seed in debug mode
#endif

  this->m_SecretData->m_Generator.seed( randomSeed );

#endif // version

} // constructor
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
template< typename TDataType, typename TDistributionType >
StandardDistribution< TDataType, TDistributionType >
::~StandardDistribution()
{
} // destructor
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
template< typename TDataType, typename TDistributionType >
TDataType
StandardDistribution< TDataType, TDistributionType >
::GetRandomValue() const
{

  return this->m_SecretData->m_Distribution(
    this->m_SecretData->m_Generator );

} // GetRandomValue
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
template< typename TDataType, typename TDistributionType >
void
StandardDistribution< TDataType, TDistributionType >
::SetDistribution( const TDistributionType & variable )
{
  ccipdSetBodyMacro( Distribution )
}
//////////////////////////////////////////////////////////////////////////////////////////


} // namespace ccipd
#endif // __ccipdStandardDistribution_hxx
