#ifndef __ccipdGaussianDistribution_h
#define __ccipdGaussianDistribution_h



//////////////////////////////////////////////////////////////////////////////////////////
// ccipd includes
#include "ccipdDistribution.h"

// std includes
#include <memory> // for smart pointers
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{

  

/**
 * \brief   Gaussian distribution.
 * \ingroup Math
 * \author  Rob Toth
 * \date    2013-02-12
 */
template< typename TDataType, int TDimensionality >
class GaussianDistribution:
  virtual public Distribution< TDataType, TDimensionality >
{
  


//////////////////////////////////////////////////////////////////////////////////////////
  ccipdConstructorDeclarationNoSelfMacro( GaussianDistribution )
//////////////////////////////////////////////////////////////////////////////////////////
  
    

public: // templates
//////////////////////////////////////////////////////////////////////////////////////////

  typedef GaussianDistribution< TDataType, TDimensionality > Self;
  typedef Distribution        < TDataType, TDimensionality > Superclass;

  typedef typename Superclass::DataType                   DataType;
  typedef typename Superclass::ProbabilityType            ProbabilityType;
  typedef typename Superclass::DistributionVectorType     DistributionVectorType;
  typedef typename Superclass::DistributionVectorsType    DistributionVectorsType;
  typedef typename Superclass::DistributionVectorPointer  DistributionVectorPointer;
  typedef typename Superclass::DistributionVectorsPointer DistributionVectorsPointer;

  // define our types for the mean and covariance
  typedef DistributionVectorType    MeanVectorType;
  typedef  std::shared_ptr<
     MeanVectorType >               MeanVectorPointer;
  typedef  std::shared_ptr<
    const MeanVectorType >          MeanVectorConstPointer;

  typedef Eigen::Matrix<
    TDataType,
    TDimensionality, TDimensionality,
    Eigen::RowMajor,
    TDimensionality, TDimensionality
  > CovarianceMatrixType;

  typedef std::shared_ptr<
    CovarianceMatrixType > CovarianceMatrixPointer;

  typedef std::shared_ptr<
    const CovarianceMatrixType > CovarianceMatrixConstPointer;

//////////////////////////////////////////////////////////////////////////////////////////

  

public: // functions
//////////////////////////////////////////////////////////////////////////////////////////
  
  /// Define the mean (default is {0, ... 0})
  ccipdSetByrefDeclareMacro( Mean, MeanVectorPointer )

  /// Define the covariance (default is identity)
  ccipdSetByrefDeclareMacro( Variance, CovarianceMatrixPointer )

  /// Return the mean
  ccipdGetConstDeclareMacro( Mean, MeanVectorPointer )

  /// Return the variance
  ccipdGetConstDeclareMacro( Variance, CovarianceMatrixPointer )

  /// return the mode
  ccipdGetConstDeclareMacro( Mode, DistributionVectorPointer );

  /// Estimate mean and covariance from a collection of sampled vectors.
  virtual void EstimateDistributionFromSamples(
    const DistributionVectorsType & inputSamples );

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



}; // class GaussianDistribution
} // namespace ccipd


  
//////////////////////////////////////////////////////////////////////////////////////////
#ifdef Boost_SERIALIZATION_FOUND
// http://bit.ly/11HPC2J
namespace boost {
namespace serialization {
 
template< typename TDataType, int TDimensionality >
struct version< ccipd::GaussianDistribution< TDataType, TDimensionality > >
{
  // This is version 1
  typedef mpl::int_< 1 > type;
  typedef mpl::integral_c_tag tag;
  BOOST_STATIC_CONSTANT( unsigned int, value = version::type::value );
};

template< typename TDataType, int TDimensionality >
struct tracking_level< ccipd::GaussianDistribution< TDataType, TDimensionality > >
{
  typedef mpl::integral_c_tag tag;
  typedef mpl::int_< track_always > type;
  BOOST_STATIC_CONSTANT( int, value = tracking_level::type::value );
};

   /*
    * Note you must do the following:
   
       template
       struct boost::serialization::version<
         ccipd::GaussianDistribution< MyType, Dimensionality > >;
   
       template
       struct boost::serialization::tracking_level<
         ccipd::GaussianDistribution< MyType, Dimensionality > >;
   
    */

} } // namespaces
#endif // Boost_SERIALIZATION_FOUND
//////////////////////////////////////////////////////////////////////////////////////////



#endif // __ccipdGaussianDistribution_h
