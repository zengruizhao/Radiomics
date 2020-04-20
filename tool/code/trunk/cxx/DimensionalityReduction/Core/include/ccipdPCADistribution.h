#ifndef __ccipdPCADistribution_h
#define __ccipdPCADistribution_h



//////////////////////////////////////////////////////////////////////////////////////////
// ccipd includes
#include "ccipdMultivariateGaussianDistribution.h"
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{

/*
* It is assumed that for a PCA Distribution the matrix should be assigned dynamically,
* as this will vary according to the number of pixels in your image.
*/
template< typename TDataType > 
class PCADistribution : 
  virtual public  MultivariateGaussianDistribution< TDataType, -1 >
{



//////////////////////////////////////////////////////////////////////////////////////////
  ccipdConstructorDeclarationNoSelfMacro( PCADistribution )
/////////////////////////////////////////////////////////////////////////////////////////

    

public: // templates
//////////////////////////////////////////////////////////////////////////////////////////

  typedef PCADistribution<
    TDataType
  > Self;

  typedef MultivariateGaussianDistribution<
    TDataType, -1
  > Superclass;

  typedef typename Superclass::DataType                   DataType;
  typedef typename Superclass::MeanVectorPointer          MeanVectorPointer;
  typedef typename Superclass::PseudoInverseType          PseudoInverseType;
  
  typedef Eigen::Matrix<
    TDataType,
    -1, -1,
    Eigen::RowMajor,
    -1, -1
  > EigenVectorMatrixType;

  typedef std::shared_ptr<
    EigenVectorMatrixType > EigenVectorMatrixPointer;

  typedef Eigen::Matrix<
    TDataType,
    -1, 1,
    Eigen::ColMajor,
    -1, 1
  > EigenValueType;

  typedef std::shared_ptr<
    EigenValueType > EigenValuePointer;
//////////////////////////////////////////////////////////////////////////////////////////

    

public: // templates
//////////////////////////////////////////////////////////////////////////////////////////
  virtual void SetEigenVectorsAndValues(
    const MeanVectorPointer        & mean, 
    const EigenVectorMatrixPointer & eigenVectors,
    const EigenValuePointer        & eigenValues );
   
   /// Set the eigenvectors; Assumes that eigenvectors = eigenvectors*sqrt(eigenvalues) 
  virtual void SetEigenVectors(
    const MeanVectorPointer        & mean, 
    const EigenVectorMatrixPointer & eigenVectors);
    
//////////////////////////////////////////////////////////////////////////////////////////
 
 
 
protected: // functions
//////////////////////////////////////////////////////////////////////////////////////////
  
  /// Overridden from parent class; PCA cannot clear pseudoInverse that must remain set. 
  virtual void ClearInverse();

  /// Overridden from parent class; Determinant of PCA is always 1 because we multiple eigenvalues into eigenvectors.
  virtual void CalculateDeterminant();   
  
  /// Calculate and set the PseduoInverse;  
  virtual void CalculatePseudoInverse(); // 
 
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



  
}; // class PrincipalComponentAnalysisDistrbution

} // namespace ccipd



//////////////////////////////////////////////////////////////////////////////////////////
#ifdef Boost_SERIALIZATION_FOUND
// http://bit.ly/11HPC2J
namespace boost {
namespace serialization {
 
template< typename TDataType >
struct version< ccipd::PCADistribution< TDataType > >
{
  // This is version 1
  typedef mpl::int_< 1 > type;
  typedef mpl::integral_c_tag tag;
  BOOST_STATIC_CONSTANT( unsigned int, value = version::type::value );
};

template< typename TDataType >
struct tracking_level< ccipd::PCADistribution< TDataType > >
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



#endif // __ccipdPCADistribution_h