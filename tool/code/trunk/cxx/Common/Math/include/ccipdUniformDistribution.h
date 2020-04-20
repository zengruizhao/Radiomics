#ifndef __ccipdUniformDistribution_h
#define __ccipdUniformDistribution_h



//////////////////////////////////////////////////////////////////////////////////////////
// ccipd includes
#include "ccipdStandardDistribution.h"
#include "ccipdFactory.h"

// std includes
#include <random> // for our base class's distribution
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{


  
/**
 * \brief   Uniform distribution.
 * \ingroup Math
 * \author  Rob Toth
 * \date    2012-10-28
 */
template< typename TDataType >
class UniformDistribution :
  virtual public StandardDistribution< TDataType,
    // define our distribution type
    typename std::conditional<
      std::is_floating_point< TDataType >::value,
      std::uniform_real_distribution< TDataType >,
      std::uniform_int_distribution < TDataType >
    >::type >,
  public Factory< UniformDistribution< TDataType > >
{

  

//////////////////////////////////////////////////////////////////////////////////////////
  ccipdConstructorDeclarationNoSelfMacro( UniformDistribution )
public:
  typedef UniformDistribution < TDataType > Self;
  ccipdFactorySuperclassTypedefsTemplatedMacro
//////////////////////////////////////////////////////////////////////////////////////////



public: // typedefs
//////////////////////////////////////////////////////////////////////////////////////////

  // Make sure we have a valid type.
  // If your code doesn't compile here,
  // make sure that TDataType is either an int, float, etc.
  static_assert(
    std::is_floating_point< TDataType >::value ||
    std::is_integral      < TDataType >::value,
    "You need either an integral or floating point type!" );

  // I can't figure our a way around copying and pasting this...
  typedef typename std::conditional<
    std::is_floating_point< TDataType >::value,
    std::uniform_real_distribution< TDataType >,
    std::uniform_int_distribution < TDataType >
  >::type DistributionType;
  
  typedef StandardDistribution< TDataType, DistributionType > Superclass;

  typedef typename Superclass::DataType                   DataType;
  typedef typename Superclass::DistributionVectorType     DistributionVectorType;
  typedef typename Superclass::DistributionVectorsType    DistributionVectorsType;
  typedef typename Superclass::DistributionMatrixType     DistributionMatrixType;
  typedef typename Superclass::DistributionVectorPointer  DistributionVectorPointer;
  typedef typename Superclass::DistributionVectorsPointer DistributionVectorsPointer;
  typedef typename Superclass::DistributionMatrixPointer  DistributionMatrixPointer;
  typedef typename Superclass::ProbabilityType            ProbabilityType;

//////////////////////////////////////////////////////////////////////////////////////////



public: // functions
//////////////////////////////////////////////////////////////////////////////////////////

  /// Define the minimum value of the random generator (default is 0)
  ccipdSetByvalDeclareMacro( MinimumValue, TDataType )

  /// Define the maximum value of the random generator (default is 1)
  ccipdSetByvalDeclareMacro( MaximumValue, TDataType )
  
  /// Define the mode
  ccipdGetConstDeclareMacro( ModeValue, DataType )

  /// Return the likelihood this value falls in the uniform distribution.
  virtual ProbabilityType GetPDF( const TDataType value ) const;
  
  /// Return the probability X falls from -infinity to value
  virtual ProbabilityType GetCDF( const TDataType value ) const;

//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
  /// PIMPL interface
  ccipdPIMPLMacro
//////////////////////////////////////////////////////////////////////////////////////////



}; // class UniformDistribution
} // namespace ccipd
#endif // __ccipdUniformDistribution_h
