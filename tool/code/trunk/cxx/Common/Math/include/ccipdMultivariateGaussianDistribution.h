#ifndef __ccipdMultivariateGaussianDistribution_h
#define __ccipdMultivariateGaussianDistribution_h



//////////////////////////////////////////////////////////////////////////////////////////
// ccipd includes
#include "ccipdGaussianDistribution.h"
#include "ccipdFactory.h"
//////////////////////////////////////////////////////////////////////////////////////////
// http://bit.ly/WiKKfd
#ifdef _MSC_VER
#pragma warning( disable: 4250 )
#pragma warning( disable: 4263 )
#endif


namespace ccipd
{

  

/**
 * \brief   Multivariate gaussian distribution.
 * \ingroup Math
 * \author  Rob Toth
 * \date    2013-02-12
 */
template< typename TDataType, int TDimensionality >
class MultivariateGaussianDistribution :
  virtual public GaussianDistribution< TDataType, TDimensionality >,
  public Factory< MultivariateGaussianDistribution< TDataType, TDimensionality > >
{
  


//////////////////////////////////////////////////////////////////////////////////////////
  ccipdConstructorDeclarationNoSelfMacro( MultivariateGaussianDistribution )
public:
  typedef MultivariateGaussianDistribution<
    TDataType, TDimensionality
  > Self;
  ccipdFactorySuperclassTypedefsTemplatedMacro
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
  // Make sure we are, indeed, multivariate.
  static_assert(
    TDimensionality >= 2 || TDimensionality == -1,
    "For 1-dimensional datasets, please use ccipd::UnivariateGaussianDistribution." );
//////////////////////////////////////////////////////////////////////////////////////////
  
    

public: // templates
//////////////////////////////////////////////////////////////////////////////////////////

  typedef GaussianDistribution<
    TDataType, TDimensionality
  > Superclass;

  typedef typename Superclass::DataType                   DataType;
  typedef typename Superclass::ProbabilityType            ProbabilityType;
  typedef typename Superclass::DistributionVectorType     DistributionVectorType;
  typedef typename Superclass::DistributionVectorsType    DistributionVectorsType;
  typedef typename Superclass::DistributionVectorPointer  DistributionVectorPointer;
  typedef typename Superclass::DistributionVectorsPointer DistributionVectorsPointer;
  typedef typename Superclass::MeanVectorType             MeanVectorType;
  typedef typename Superclass::MeanVectorPointer          MeanVectorPointer;
  typedef typename Superclass::CovarianceMatrixType       CovarianceMatrixType;
  typedef typename Superclass::CovarianceMatrixPointer    CovarianceMatrixPointer;

  typedef Eigen::Matrix<
    TDataType,
    TDimensionality, TDimensionality,
    Eigen::RowMajor,
    TDimensionality, TDimensionality
  > PseudoInverseType;

  typedef std::shared_ptr< PseudoInverseType >   PseudoInversePointer;
//////////////////////////////////////////////////////////////////////////////////////////

  

public: // functions
//////////////////////////////////////////////////////////////////////////////////////////
  
  /// Overload so we can recompute the inverse when we set the variance
  ccipdSetByrefDeclareMacro( Variance, CovarianceMatrixPointer )

  /// Output a random vector from the distribution
  virtual DistributionVectorPointer GetRandomVector();

  /// Output a random vector as each column
  virtual DistributionVectorsPointer GetRandomVectors(
    const unsigned int numberOfVectors );

  /// Return the probability this value falls in the gaussian distribution.
  virtual ProbabilityType GetPDF(
    const DistributionVectorType & value ) const;
  
  /// Return log( PDF ) of this value (overriding for accuracy)
  virtual ProbabilityType GetLogPDF(
    const DistributionVectorType & value ) const;

  /// Return the probability X falls from -infinity to value
  virtual ProbabilityType GetCDF(
    const DistributionVectorType & value ) const;

  /// Return the probability X falls from value to +infinity
  virtual ProbabilityType GetCDFComplement(
    const DistributionVectorType & value ) const;

  /// Return the Z Scores for this value
  virtual DistributionVectorPointer GetZScores(
    const DistributionVectorType & value ) const;

  /// Return the Z Scores for thjs value (overloaded assuming data is centered from mean)
  virtual DistributionVectorPointer GetZScoresFromMeanSubtracted(
    const MeanVectorPointer & meanSubtracted ) const;

  /// Return the Mahalanobis Distance for this value
  virtual ProbabilityType GetMahalanobisDistanceSquared(
    const DistributionVectorType & value ) const;

//////////////////////////////////////////////////////////////////////////////////////////


 
protected: // functions
//////////////////////////////////////////////////////////////////////////////////////////
  
  // Reset the inverse and related variables;
  virtual void ClearInverse();

  // Calculate determinant from the covariance xatrix
  virtual void CalculateDeterminant(); 

  // Set the PseduoInverse directly.
  virtual void SetPseudoInverse( const PseudoInversePointer & pseudoInverse );

  // Calculate the inverse of the covariance matrix using the LDLT method
  virtual void CalculateInverse();
  
  /// Return the Determinant if set; otherwise return null
  ccipdGetConstDeclareMacro( Determinant, ProbabilityType )

  /// Set the determinant
  ccipdSetByvalDeclareMacro( Determinant, ProbabilityType )
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
  /// PIMPL interface
  ccipdPIMPLMacro
//////////////////////////////////////////////////////////////////////////////////////////



private:
//////////////////////////////////////////////////////////////////////////////////////////
  /** Serialization */
#ifdef Boost_SERIALIZATION_FOUND
  /** We are serializable */
  ccipdSerializationDeclarationMacro
#endif // Boost_SERIALIZATION_FOUND
//////////////////////////////////////////////////////////////////////////////////////////



}; // class MultivariateGaussianDistribution
} // namespace ccipd



//////////////////////////////////////////////////////////////////////////////////////////
#ifdef Boost_SERIALIZATION_FOUND
// http://bit.ly/11HPC2J
namespace boost {
namespace serialization {
 
template< typename TDataType, int TDimensionality >
struct version< ccipd::MultivariateGaussianDistribution< TDataType, TDimensionality > >
{
  // This is version 1
  typedef mpl::int_< 1 > type;
  typedef mpl::integral_c_tag tag;
  BOOST_STATIC_CONSTANT( unsigned int, value = version::type::value );
};

template< typename TDataType, int TDimensionality >
struct tracking_level< ccipd::MultivariateGaussianDistribution< TDataType, TDimensionality > >
{
  typedef mpl::integral_c_tag tag;
  typedef mpl::int_< track_always > type;
  BOOST_STATIC_CONSTANT( int, value = tracking_level::type::value );
};

    /*
     * Note you must do the following:
    
        template
        struct boost::serialization::version<
          ccipd::MultivariateGaussianDistribution< MyType, Dimensionality > >;
    
        template
        struct boost::serialization::tracking_level<
          ccipd::MultivariateGaussianDistribution< MyType, Dimensionality > >;
    
     */

} } // namespaces
#endif // Boost_SERIALIZATION_FOUND
//////////////////////////////////////////////////////////////////////////////////////////



#endif // __ccipdMultivariateGaussianDistribution_h
