#ifndef __ccipdStandardDistribution_h
#define __ccipdStandardDistribution_h



//////////////////////////////////////////////////////////////////////////////////////////
// ccipd includes
#include "ccipdUnivariateDistribution.h"
#include "ccipdMacros.h"
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{
    


/**
 * \brief   Univariate standard distributions found in the C++ std library
 * \ingroup Math
 * \author  Rob Toth
 * \date    2012-10-28
 */
template< typename TDataType, typename TDistributionType >
class StandardDistribution :
  virtual public UnivariateDistribution< TDataType >
{

  

//////////////////////////////////////////////////////////////////////////////////////////
  ccipdConstructorDeclarationNoSelfMacro( StandardDistribution )
//////////////////////////////////////////////////////////////////////////////////////////



public: // typedefs
//////////////////////////////////////////////////////////////////////////////////////////

  typedef StandardDistribution< TDataType, TDistributionType > Self;
  typedef UnivariateDistribution< TDataType > Superclass;

  typedef TDistributionType DistributionType;

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

  /// Return a random value as a primitive type.
  virtual TDataType GetRandomValue() const;

//////////////////////////////////////////////////////////////////////////////////////////



protected: // functions
//////////////////////////////////////////////////////////////////////////////////////////
  /**
   * Child classes can set up custom inputs to the distribution
   * and pass it on. Otherwise, the default constructors are used.
   */
  ccipdSetByrefDeclareMacro( Distribution, TDistributionType )
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
  /// PIMPL interface
  ccipdPIMPLMacro
//////////////////////////////////////////////////////////////////////////////////////////



}; // class UniformDistribution
} // namespace ccipd
#endif // __ccipdStandardDistribution_h
