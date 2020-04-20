#ifndef __ccipdUnivariateDistribution_h
#define __ccipdUnivariateDistribution_h



//////////////////////////////////////////////////////////////////////////////////////////
// ccipd includes
#include "ccipdDistribution.h"
//////////////////////////////////////////////////////////////////////////////////////////

// http://bit.ly/WiKKfd
#ifdef _MSC_VER
#pragma warning( disable: 4250 )
#pragma warning( disable: 4263 )
#endif

namespace ccipd
{



/**
 * \brief Univariate (1d) distribution base class.
 * \ingroup Math
 * \author  Rob Toth
 * \date    2012-10-28
 */
template< typename TDataType >
class UnivariateDistribution :
  virtual public Distribution< TDataType, 1 >
{

  

//////////////////////////////////////////////////////////////////////////////////////////
  ccipdConstructorDeclarationNoSelfMacro( UnivariateDistribution )
//////////////////////////////////////////////////////////////////////////////////////////



public:
//////////////////////////////////////////////////////////////////////////////////////////

  typedef UnivariateDistribution< TDataType > Self;
  typedef Distribution< TDataType, 1 > Superclass;

  typedef typename Superclass::DataType                   DataType;
  typedef typename Superclass::DistributionVectorType     DistributionVectorType;
  typedef typename Superclass::DistributionVectorsType    DistributionVectorsType;
  typedef typename Superclass::DistributionVectorPointer  DistributionVectorPointer;
  typedef typename Superclass::DistributionVectorsPointer DistributionVectorsPointer;
  typedef typename Superclass::ProbabilityType            ProbabilityType;

  /// Define a variable-sized matrix
  typedef Eigen::Matrix<
    TDataType,
    -1, -1, Eigen::RowMajor, -1, -1
  > DistributionMatrixType;

  /// Smart pointer
  typedef std::shared_ptr<
    DistributionMatrixType
  > DistributionMatrixPointer;

//////////////////////////////////////////////////////////////////////////////////////////



public:
//////////////////////////////////////////////////////////////////////////////////////////

  /// Return a random value smart pointer.
  virtual DistributionVectorPointer GetRandomVector();

  /// Return a random value as a primitive type.
  virtual TDataType GetRandomValue() const = 0;

  /// Return a matrix full of random values.
  virtual DistributionMatrixPointer GetRandomMatrix(
    const unsigned int rows, const unsigned int cols );

  /// Return a value as the mode
  virtual DataType GetModeValue() const = 0;

  // the pointer to the mode
  ccipdGetConstDeclareMacro( Mode, DistributionVectorPointer )
  
  /**
   * \brief Return random values.
   * 
   * Each column represents
   * a random vector.
   * 
   * \param[in] numberOfVectors
   *   How many random vectors (cols) to generate.
   * 
   */
  virtual DistributionVectorsPointer GetRandomVectors(
    const unsigned int numberOfVectors );

  /// We will call the get probability function with primitive type.
  virtual ProbabilityType GetPDF(
    const DistributionVectorType & value ) const;

  /// Overloading for use with a primitive type.
  virtual ProbabilityType GetPDF(
    const TDataType value ) const = 0;

  virtual ProbabilityType GetCDF(
    const DistributionVectorType & value ) const;

  /// Overloading for use with a primitive type.
  virtual ProbabilityType GetCDF(
    const TDataType value ) const = 0;

  virtual ProbabilityType GetCDFComplement(
    const DistributionVectorType & value ) const;

  /// Overloading for use with a primitive type.
  virtual ProbabilityType GetCDFComplement(
    const TDataType value ) const;

  /// Overloading for use with a primitive type.
  virtual ProbabilityType GetProbability(
    const TDataType value1, const TDataType value2 ) const;

//////////////////////////////////////////////////////////////////////////////////////////



}; // class UnivariateDistribution
} // namespace ccipd
#endif // __ccipdUnivariateDistribution_h
