#ifndef __ccipdGammaDistribution_h
#define __ccipdGammaDistribution_h



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
 * \brief   Gamma distribution.
 * \ingroup Math
 * \author  Rob Toth
 * \date    2013-02-12
 */
template< typename TDataType >
class GammaDistribution :
  virtual public StandardDistribution< TDataType,
    std::gamma_distribution< TDataType > >,
  public Factory< GammaDistribution< TDataType > >
{

  

//////////////////////////////////////////////////////////////////////////////////////////
  ccipdConstructorDeclarationNoSelfMacro( GammaDistribution )
public:
  typedef GammaDistribution< TDataType > Self;
  ccipdFactorySuperclassTypedefsTemplatedMacro
//////////////////////////////////////////////////////////////////////////////////////////



public: // typedefs
//////////////////////////////////////////////////////////////////////////////////////////

  // Make sure we have a valid type.
  // If your code doesn't compile here,
  // make sure that TDataType is either a float, double, etc.
  static_assert(
    std::is_floating_point< TDataType >::value,
    "You need a floating point type!" );

  /// Our base distribution type
  typedef std::gamma_distribution< TDataType > DistributionType;
  
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

  /**
   * \see http://en.wikipedia.org/wiki/Gamma_distribution
   * 
   * default = 1.0 for both.
   */
  virtual void SetShapeAndScale(
    const TDataType shape,
    const TDataType scale );
  
  /**
   * This will compute alpha and beta
   * based on the mode and variance
   * of the distribution, which
   * may be more "reasonable"
   * or "interpretable" parameters
   * to use to define the distribution.
   */
  virtual void SetModeAndVariance(
    const TDataType mode, 
    const TDataType variance );

  /// Output the statistics of this distribution
  ccipdGetConstDeclareMacro( ModeValue     , DataType )
  ccipdGetConstDeclareMacro( Variance      , DataType )
  ccipdGetConstDeclareMacro( Mean          , DataType )
  ccipdGetConstDeclareMacro( Skewness      , DataType )
  ccipdGetConstDeclareMacro( ExcessKurtosis, DataType )

  /// Return the likelihood this value falls in the Gamma distribution.
  virtual ProbabilityType GetPDF( const TDataType value ) const;
  
  /// Return the probability X falls from -infinity to value
  virtual ProbabilityType GetCDF( const TDataType value ) const;
  
  /// Return the probability X falls from value to +infinity (overriding for accuracy)
  virtual ProbabilityType GetCDFComplement( const TDataType value ) const;

//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
  /// PIMPL interface
  ccipdPIMPLMacro
//////////////////////////////////////////////////////////////////////////////////////////



}; // class GammaDistribution
} // namespace ccipd
#endif // __ccipdGammaDistribution_h
