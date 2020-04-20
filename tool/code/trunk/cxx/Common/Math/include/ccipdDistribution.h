#ifndef __ccipdDistribution_h
#define __ccipdDistribution_h



//////////////////////////////////////////////////////////////////////////////////////////
// ccipd includes
#include "ccipdDistributionTypes.h"
#include "ccipdMatrixTypes.h"
#include "ccipdMacros.h"
#include "ccipdDisableWarningsMacro.h"

// boost includes
#include <boost/static_assert.hpp>

// std includes
#include <memory> // for smart pointers

// serialization includes
#include "ccipdCommonMathOptions.h"
#ifdef Boost_SERIALIZATION_FOUND
  ccipdSerializationForwardDeclarationMacro
  #include <boost/serialization/version.hpp>
  #include <boost/serialization/tracking.hpp>
  //#include <boost/config/suffix.hpp>
#endif // Boost_SERIALIZATION_FOUND
//////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdEnableWarningsMacro.h"


namespace ccipd
{



/**
 * \brief Abstract distribution base class.
 * \ingroup Math
 * \author  Rob Toth
 * \date    2012-10-28
 */
template< typename TDataType, int TDimensionality >
class Distribution
{
  


//////////////////////////////////////////////////////////////////////////////////////////
  ccipdConstructorDeclarationNoSelfMacro( Distribution )
//////////////////////////////////////////////////////////////////////////////////////////



public:
//////////////////////////////////////////////////////////////////////////////////////////
  BOOST_STATIC_CONSTEXPR int Dimensionality = TDimensionality;
  typedef TDataType DataType;

  typedef Distribution< DataType, Dimensionality > Self;
  
  /// make sure we are at least 1 dimensional (or dynamic)!

  static_assert(
    TDimensionality >= 1 || TDimensionality == -1,
    "You need at least a 1-dimensional distribution!" );
  

  /// Define a single value of the distribution
  typedef Eigen::Matrix<
    TDataType,
    TDimensionality, 1, Eigen::ColMajor,
    TDimensionality, 1
  > DistributionVectorType;

  /**
   * \brief
   *   Define multiple values of the distribution
   *   where each column is a value.
   * 
   * \note I couldn't compile with ColMajor!
   */
  typedef Eigen::Matrix<
    TDataType,
    TDimensionality, Eigen::Dynamic, Eigen::RowMajor,
    TDimensionality, Eigen::Dynamic
  > DistributionVectorsType;

  /// Smart pointer
  typedef std::shared_ptr<
    DistributionVectorType
  > DistributionVectorPointer;

  /// Smart pointer
  typedef std::shared_ptr<
    DistributionVectorsType
  > DistributionVectorsPointer;

//////////////////////////////////////////////////////////////////////////////////////////



public:
//////////////////////////////////////////////////////////////////////////////////////////
  /**
   * \brief Return a random vector.
   * 
   * Create a random vector (of the correct
   * dimensionality) and return it.
   * 
   * This represents a SINGLE value
   * in the distribution.
   * 
   */
  virtual DistributionVectorPointer GetRandomVector() = 0;

  /**
   * \brief Return random vectors.
   * 
   * Each column represents
   * a random vector.
   * 
   * \param[in] numberOfVectors
   *   How many random vectors (cols) to generate.
   * 
   */
  virtual DistributionVectorsPointer GetRandomVectors(
    const unsigned int vectorSize, const unsigned int numberOfVectors );

  /**
   * \brief Return the mode of the distribution.
   * 
   * \note If this distribution is multi-model,
   * just return one mode.
   * 
   * If no mode exists, return an empty pointer.
   */
  virtual DistributionVectorPointer GetMode() const = 0;

  /// Typedef for probability
  typedef DistributionProbabilityType ProbabilityType;

  /**
   * \brief Return the probability density of a value in this distribution.
   * 
   * You give it a multi-dimensional value.
   * This should return a floating point value between
   * 0 and 1 representing the likelihood of seeing
   * this value.
   * 
   * \see GetCDF
   * \see GetCDFComplement
   */
  virtual ProbabilityType GetPDF(
    const DistributionVectorType & value ) const = 0;

  /**
   * \brief Return the log( PDF ) of a value in this distribution.
   */
  virtual ProbabilityType GetLogPDF(
    const DistributionVectorType & value ) const;

  /**
   * \brief Return the probability that X falls from -infinity to value for any X.
   * 
   * \note The GetPDF() function
   *   returns a density, but this actually returns a probability
   *   and can be interpreted as as percent.
   * 
   * \see GetCDFComplement()
   */
  virtual ProbabilityType GetCDF(
    const DistributionVectorType & value ) const = 0;

  /**
   * \brief Return the probability that X falls from value to +infinity for any X.
   * 
   * \see GetCDF()
   * 
   * \warning
   *   The default implementation of 1.0 - GetCDF()
   *   may be inaccurate for some distributions,
   *   and it is recommended, although not required,
   *   to override this method.
   */
  virtual ProbabilityType GetCDFComplement(
    const DistributionVectorType & value ) const;

  /** 
   * \brief Returns the probability that X falls between the two values.
   */
  virtual ProbabilityType GetProbability(
    const DistributionVectorType & value1,
    const DistributionVectorType & value2 ) const;

//////////////////////////////////////////////////////////////////////////////////////////


  
private:
//////////////////////////////////////////////////////////////////////////////////////////
  /** Serialization */
#ifdef Boost_SERIALIZATION_FOUND
  /** We are serializable */
  ccipdSerializationDeclarationMacro
#endif // Boost_SERIALIZATION_FOUND
//////////////////////////////////////////////////////////////////////////////////////////



}; // class Distribution
} // namespace ccipd



//////////////////////////////////////////////////////////////////////////////////////////
#ifdef Boost_SERIALIZATION_FOUND
// http://bit.ly/11HPC2J
namespace boost {
namespace serialization {
 
template< typename TDataType, int TDimensionality >
struct version< ccipd::Distribution< TDataType, TDimensionality > >
{
  // This is version 1
  typedef mpl::int_< 1 > type;
  typedef mpl::integral_c_tag tag;
  BOOST_STATIC_CONSTANT( unsigned int, value = version::type::value );
};

template< typename TDataType, int TDimensionality >
struct tracking_level< ccipd::Distribution< TDataType, TDimensionality > >
{
  typedef mpl::integral_c_tag tag;
  typedef mpl::int_< track_always > type;
  BOOST_STATIC_CONSTANT( int, value = tracking_level::type::value );
};

   /*
    * Note you must do the following:
   
       template
       struct boost::serialization::version<
         ccipd::Distribution< MyType, Dimensionality > >;
   
       template
       struct boost::serialization::tracking_level<
         ccipd::Distribution< MyType, Dimensionality > >;
   
    */

} } // namespaces
#endif // Boost_SERIALIZATION_FOUND
//////////////////////////////////////////////////////////////////////////////////////////



#endif // __ccipdDistribution_h
