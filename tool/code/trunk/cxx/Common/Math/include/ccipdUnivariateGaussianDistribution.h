#ifndef __ccipdUnivariateGaussianDistribution_h
#define __ccipdUnivariateGaussianDistribution_h



//////////////////////////////////////////////////////////////////////////////////////////

// ccipd includes
#include "ccipdStandardDistribution.h"
#include "ccipdGaussianDistribution.h"
#include "ccipdFactory.h"

// std includes
#include <random>

// http://bit.ly/WiKKfd
#ifdef _MSC_VER
#pragma warning( disable: 4250 )
#pragma warning( disable: 4263 )
#endif
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



/**
 * \brief Univariate (1d) distribution gaussian distribution.
 * \ingroup Math
 * \author  Rob Toth
 * \date    2012-10-28
 */
template< typename TDataType >
class UnivariateGaussianDistribution :
  virtual public StandardDistribution<
    TDataType,
    std::normal_distribution< TDataType > >,
  virtual public GaussianDistribution<
    TDataType, 1 >,
  public Factory< UnivariateGaussianDistribution< TDataType > >
{



//////////////////////////////////////////////////////////////////////////////////////////
  ccipdConstructorDeclarationNoSelfMacro( UnivariateGaussianDistribution )
public:
  typedef UnivariateGaussianDistribution< TDataType > Self;
  ccipdFactorySuperclassTypedefsTemplatedMacro
//////////////////////////////////////////////////////////////////////////////////////////



public: // typedefs
//////////////////////////////////////////////////////////////////////////////////////////

  // I can't figure our a way around copying and pasting this...
  typedef std::normal_distribution< TDataType > DistributionType;
  

  typedef StandardDistribution< TDataType, DistributionType > Superclass1;
  typedef GaussianDistribution< TDataType, 1                > Superclass2;

  typedef typename Superclass1::DataType                   DataType;
  typedef typename Superclass1::DistributionVectorType     DistributionVectorType;
  typedef typename Superclass1::DistributionVectorsType    DistributionVectorsType;
  typedef typename Superclass1::DistributionMatrixType     DistributionMatrixType;
  typedef typename Superclass1::DistributionVectorPointer  DistributionVectorPointer;
  typedef typename Superclass1::DistributionVectorsPointer DistributionVectorsPointer;
  typedef typename Superclass1::DistributionMatrixPointer  DistributionMatrixPointer;
  typedef typename Superclass1::ProbabilityType            ProbabilityType;
  typedef typename Superclass2::MeanVectorType             MeanVectorType;
  typedef typename Superclass2::MeanVectorPointer          MeanVectorPointer;
  typedef typename Superclass2::CovarianceMatrixType       CovarianceMatrixType;
  typedef typename Superclass2::CovarianceMatrixPointer    CovarianceMatrixPointer;

//////////////////////////////////////////////////////////////////////////////////////////



public: // functions
//////////////////////////////////////////////////////////////////////////////////////////

  /// Define the minimum value of the random generator (default is 0)
  ccipdSetByvalDeclareMacro( Mean, DataType )

  /// Define the variance of the gaussian (default is 1)
  ccipdSetByvalDeclareMacro( Variance, DataType )
  
  ccipdSetByrefDeclareMacro( Mean, MeanVectorPointer )

  ccipdSetByrefDeclareMacro( Variance, CovarianceMatrixPointer )
  
  ccipdGetConstDeclareMacro( Mode, DistributionVectorPointer )

  ccipdGetConstDeclareMacro( ModeValue, DataType )

  /// Return the likelihood this value falls in the gaussian distribution.
  virtual ProbabilityType GetPDF( const DataType value ) const;

  /// Return the probability this value falls from -infinity to value
  virtual ProbabilityType GetCDF( const DataType value ) const;
  
  /// Return the probability this value falls from value to +infinity
  virtual ProbabilityType GetCDFComplement( const DataType value ) const;

//////////////////////////////////////////////////////////////////////////////////////////



protected: // functions
//////////////////////////////////////////////////////////////////////////////////////////

  /// Semi-Overload for primitive types
  ccipdGetConstDeclareMacro( MeanPrimitive, TDataType )

  /// Semi-Overload for primitive types
  ccipdGetConstDeclareMacro( VariancePrimitive, TDataType )

  /// Regenerate internal distributions when something changes
  virtual void Regenerate();

//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
  /// PIMPL interface
  ccipdPIMPLMacro
//////////////////////////////////////////////////////////////////////////////////////////



}; // class UnivariateGaussianDistribution
} // namespace ccipd
#endif // __ccipdUnivariateGaussianDistribution_h
