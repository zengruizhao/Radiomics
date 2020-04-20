#ifndef __ccipdMultivariateGaussianDistribution_hxx
#define __ccipdMultivariateGaussianDistribution_hxx



//////////////////////////////////////////////////////////////////////////////////////////
// ccipd includes
#include "ccipdMultivariateGaussianDistribution.h"
#include "ccipdUnivariateGaussianDistribution.h" // for a standard normal

#include "ccipdDisableWarningsMacro.h"
  // std includes
  #include <iostream>

  // Eigen includes
  #include <Eigen/Core>
  #include <Eigen/Cholesky> // for covariance decompositions
  #include <Eigen/LU>
  #include <Eigen/Eigen>    // for singular values

  // boost includes
  #include <boost/math/constants/constants.hpp>
  #include <boost/math/distributions/chi_squared.hpp>

  #ifdef Boost_SERIALIZATION_FOUND
    #include <boost/archive/xml_iarchive.hpp>
    #include <boost/archive/xml_oarchive.hpp>
  #endif // Boost_SERIALIZATION_FOUND


#include "ccipdEnableWarningsMacro.h"

//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
using std::cerr;
using std::endl;
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////
template< typename TDataType, int TDimensionality >
class MultivariateGaussianDistribution< TDataType, TDimensionality >::SecretData
{
  
public:

  typedef MultivariateGaussianDistribution<
    TDataType,
    TDimensionality
  > EncapsulatingClass;
  
  typedef typename EncapsulatingClass::SecretData Self;
  typename EncapsulatingClass::SecretData & operator=(
    const Self & other ){ return const_cast< Self & >( other ); }

  //SecretData();
  SecretData( const EncapsulatingClass * const encapsulatingClass ); // constructor

  typedef typename EncapsulatingClass::CovarianceMatrixType    CovarianceMatrixType;
  typedef typename EncapsulatingClass::ProbabilityType         ProbabilityType;
  typedef typename EncapsulatingClass::PseudoInversePointer    PseudoInversePointer;

  typedef Eigen::LDLT< CovarianceMatrixType >               InverseDecompositionType;
  typedef boost::math::chi_squared                          ChiSquaredType;

  InverseDecompositionType m_CovarianceInverse;
  PseudoInversePointer     m_CovariancePseudoInverse; // taken from ccipdPseudoInverse.h

  ProbabilityType          m_Determinant;
  ChiSquaredType           m_ChiSquared;
  unsigned int             m_Rank;
  bool                     m_UsePseudoInverse;
  const EncapsulatingClass * const
    m_EncapsulatingClass;

  // we will reuse the same function for one or many random vectors
  template< int TNumberOfVectors >
  std::shared_ptr< Eigen::Matrix<
    TDataType,
    TDimensionality,
    TNumberOfVectors,
    ( TNumberOfVectors == 1 ? Eigen::ColMajor : Eigen::RowMajor ),
    TDimensionality,
    TNumberOfVectors
  > >
  GetRandomVectors( const unsigned int numberOfVectors ) const;

}; // class SecretData
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
template< typename TDataType, int TDimensionality >
MultivariateGaussianDistribution< TDataType, TDimensionality >
::SecretData::SecretData(
  const EncapsulatingClass * const encapsulatingClass ) :
m_ChiSquared        ( TDimensionality    ),
m_Rank              ( TDimensionality    ),
m_UsePseudoInverse  ( false              ),
m_EncapsulatingClass( encapsulatingClass )
{
} // constructor
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
template< typename TDataType, int TDimensionality >
MultivariateGaussianDistribution< TDataType, TDimensionality >
::MultivariateGaussianDistribution() :
m_SecretData( new SecretData( this ) )
{
} // constructor
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
template< typename TDataType, int TDimensionality >
MultivariateGaussianDistribution< TDataType, TDimensionality >
::~MultivariateGaussianDistribution()
{
} // destructor
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
template< typename TDataType, int TDimensionality >
typename
MultivariateGaussianDistribution< TDataType, TDimensionality >
::ProbabilityType
MultivariateGaussianDistribution< TDataType, TDimensionality >
::GetMahalanobisDistanceSquared(
  const DistributionVectorType & value ) const
{

  const MeanVectorPointer meanSubtracted ( new MeanVectorType );
    *meanSubtracted = value - *this->GetMean();

    // get the zscores for meanSubtracted
    const DistributionVectorPointer zscores = this->GetZScoresFromMeanSubtracted( meanSubtracted ); 
  
    // if our zscores are smaller than meanSubtracted; meanSubtracted needs to be cropped.
    const MeanVectorPointer croppedMeanSubtracted ( new MeanVectorType );

      *croppedMeanSubtracted = ( zscores->size() < meanSubtracted->size() ) ?
      meanSubtracted->head( zscores->size() )
      : *meanSubtracted;
  
    const ProbabilityType
      mahalanobisSquared = croppedMeanSubtracted->dot( *zscores ); // croppedMeanSubtracted.dot(zscores);

    return mahalanobisSquared;

} // GetMahalanobisDistanceSquared
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
template< typename TDataType, int TDimensionality >
typename
MultivariateGaussianDistribution< TDataType, TDimensionality >
::DistributionVectorPointer
MultivariateGaussianDistribution< TDataType, TDimensionality >
::GetZScores(
  const DistributionVectorType & value ) const
{

  const MeanVectorPointer meanSubtracted ( new MeanVectorType );
    *meanSubtracted = value - *this->GetMean();

  return this->GetZScoresFromMeanSubtracted( meanSubtracted );

} // GetZScores
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
template< typename TDataType, int TDimensionality >
typename
MultivariateGaussianDistribution< TDataType, TDimensionality >
::DistributionVectorPointer
MultivariateGaussianDistribution< TDataType, TDimensionality >
::GetZScoresFromMeanSubtracted(
  const MeanVectorPointer & meanSubtracted ) const
{

  
  const DistributionVectorPointer zscores( new DistributionVectorType );

  // if we are using a pseudo inverse than simply multiple by mean subtracted;
  // otherwise use the ldlt solver;
  if ( this->m_SecretData->m_UsePseudoInverse )
    {
    typename SecretData::PseudoInversePointer
      pseudoInverse = this->m_SecretData->m_CovariancePseudoInverse;
    
    // * inverse * ( X - mu ) // this is the z-scores 
    if ( pseudoInverse->cols() != meanSubtracted->size() )
      {
      cerr << "Pseudoinverse has " << pseudoInverse->cols() << " columns" << endl
        << "meanSubtracted has " << meanSubtracted->size() << " rows." << endl
        << "Unable to multiply!" << endl;
      return DistributionVectorPointer();
      }
      *zscores = (*pseudoInverse)*(*meanSubtracted);

    return zscores;

    } // using the pseduo inverse
  else
    {
    typedef typename SecretData::InverseDecompositionType InverseDecompositionType;
      const InverseDecompositionType
      & inverse = this->m_SecretData->m_CovarianceInverse;

      // make sure m_CovarianceInverse and meanSubtracted are the correct size
      if( inverse.cols() != meanSubtracted->size() )
      {
      cerr << "covarianceInverse has " << inverse.cols() << " columns" << endl
        << "meanSubtracted has " << meanSubtracted->size() << " rows." << endl
        << "Unable to multiply!" << endl;
      return DistributionVectorPointer();
      }
    // get the zscores for meanSubtracted
    *zscores = inverse.solve( *meanSubtracted ); 
  
    return zscores;
      
    } // using the ldlt inverse

} // GetZScores
//////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////
template< typename TDataType, int TDimensionality >
typename
MultivariateGaussianDistribution< TDataType, TDimensionality >
::ProbabilityType
MultivariateGaussianDistribution< TDataType, TDimensionality >
::GetPDF( const DistributionVectorType & value ) const
{

  // We provide the general (singular or
  // non-singular) case based on this:
  // http://bit.ly/VUvdWj

  // calculate constants
  BOOST_CONSTEXPR_OR_CONST ProbabilityType
//    two                = 2,
    twoPi              = boost::math::constants::two_pi< ProbabilityType >(),
    negativeHalf       = static_cast< ProbabilityType >( -0.5 );
  const ProbabilityType
    partialMantissa    = std::pow( twoPi, static_cast<ProbabilityType>(this->m_SecretData->m_Rank) ),
    determinant        = this->GetDeterminant(),
    mantissaISquared   = partialMantissa * determinant, // ^-2
    mantissaInverse    = std::sqrt( mantissaISquared ), // ^-1
    mahalanobisSquared = this->GetMahalanobisDistanceSquared( value ),
    exponent           = negativeHalf * mahalanobisSquared,
    pdf                = std::exp( exponent ) / mantissaInverse;

  return pdf;

} // GetPDF
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
template< typename TDataType, int TDimensionality >
typename
MultivariateGaussianDistribution< TDataType, TDimensionality >
::ProbabilityType
MultivariateGaussianDistribution< TDataType, TDimensionality >
::GetLogPDF( const DistributionVectorType & value ) const
{

  // calculate constants
  BOOST_CONSTEXPR_OR_CONST ProbabilityType
    twoPi              = boost::math::constants::two_pi< ProbabilityType >(),
    negativeHalf       = static_cast< ProbabilityType >( -0.5 ),
    logTwoPi           = std::log( twoPi );
  const ProbabilityType
    rank               = static_cast< ProbabilityType >( this->m_SecretData->m_Rank ),
    rankLogTwoPi       = rank * logTwoPi,
    mahalanobisSquared = this->GetMahalanobisDistanceSquared( value ),
    determinant        = this->GetDeterminant(),
    logDeterminant     = std::log( determinant ),
    logPDF             = negativeHalf * ( mahalanobisSquared + logDeterminant + rankLogTwoPi );

#ifndef NDEBUG
  const ProbabilityType superclassLog = this->Superclass::GetLogPDF( value );
  assert( std::abs( logPDF - superclassLog ) < 1e-4 &&
    "Invalid log( PDF ) calculation result!" );
#endif

  return logPDF;

} // GetLogPDF
//////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////
template< typename TDataType, int TDimensionality >
typename
MultivariateGaussianDistribution< TDataType, TDimensionality >
::ProbabilityType
MultivariateGaussianDistribution< TDataType, TDimensionality >
::GetCDF( const DistributionVectorType & value ) const
{
  const ProbabilityType
    mahalanobisSquared = this->GetMahalanobisDistanceSquared( value );

  return boost::math::cdf(
    this->m_SecretData->m_ChiSquared, mahalanobisSquared );

} // GetCDF
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
template< typename TDataType, int TDimensionality >
typename
MultivariateGaussianDistribution< TDataType, TDimensionality >
::ProbabilityType
MultivariateGaussianDistribution< TDataType, TDimensionality >
::GetCDFComplement( const DistributionVectorType & value ) const
{
  const ProbabilityType
    mahalanobisSquared = this->GetMahalanobisDistanceSquared( value );

  return boost::math::cdf( boost::math::complement(
    this->m_SecretData->m_ChiSquared, mahalanobisSquared ) );

} // GetCDFComplement
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
template< typename TDataType, int TDimensionality >
void
MultivariateGaussianDistribution< TDataType, TDimensionality >
::SetVariance( const CovarianceMatrixPointer & variance )
{

  if ( variance->rows() != variance->cols() )
    {
    cerr << "Variance must be a square matrix" << endl;
    return;
    }

  // update the variance
  this->Superclass::SetVariance( variance );

  // update the inverse of the variance
  this->CalculateInverse();
   
  // now calculate the determinant 
  this->CalculateDeterminant();
  
} // SetVariance
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
template< typename TDataType, int TDimensionality >
typename
MultivariateGaussianDistribution< TDataType, TDimensionality >
::DistributionVectorsPointer
MultivariateGaussianDistribution< TDataType, TDimensionality >
::GetRandomVectors( const unsigned int numberOfVectors )
{

  // http://stackoverflow.com/a/6250498
  return (*this->m_SecretData).template
    GetRandomVectors< Eigen::Dynamic >( numberOfVectors );

} // GetRandomVectors
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
template< typename TDataType, int TDimensionality >
typename
MultivariateGaussianDistribution< TDataType, TDimensionality >
::DistributionVectorPointer
MultivariateGaussianDistribution< TDataType, TDimensionality >
::GetRandomVector()
{

  // http://stackoverflow.com/a/6250498
  return (*this->m_SecretData).template GetRandomVectors< 1 >( 1 );
  
} // GetRandomVector
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
template< typename TDataType, int TDimensionality >
void
MultivariateGaussianDistribution< TDataType, TDimensionality >
::ClearInverse()
{
  // reset which inverse we are using
  this->m_SecretData->m_UsePseudoInverse = false;
  
  // reset each individual inverse type;
  this->m_SecretData->m_CovariancePseudoInverse = NULL;

  typename SecretData::InverseDecompositionType newCovarianceInverse;
  this->m_SecretData->m_CovarianceInverse = newCovarianceInverse;

} // ClearInverse
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
template< typename TDataType, int TDimensionality >
void
MultivariateGaussianDistribution< TDataType, TDimensionality >
::CalculateDeterminant()
{

  const CovarianceMatrixPointer variance = this->Superclass::GetVariance();

  // we need the rank of this matrix
  typedef Eigen::FullPivLU< CovarianceMatrixType > InverseDeterminantType;
  InverseDeterminantType determinantCalculator( *variance );
  const unsigned int rank =
    static_cast< unsigned int >( determinantCalculator.rank() );
  this->m_SecretData->m_Rank = rank;

  // see if the matrix is singular -
  // it's possible that the rank is less than the dimensionality
  const bool isSingular = rank < TDimensionality;

  if ( isSingular )
    {

    if ( rank == 0 )
      {
      this->SetDeterminant( 1 );
      return;
      }
    // THIS IS THE CASE FOR PCA & ANY RANK DEFICANCY
    // we calculate the determinant differently
    // using the product of the
    // positive eigenvalues
    Eigen::SelfAdjointEigenSolver< CovarianceMatrixType >
      eigenSolver( *variance, Eigen::EigenvaluesOnly );

    // make sure it converged
    if ( eigenSolver.info() != Eigen::Success )
      {
      this->SetDeterminant( 0 );
      return;
      }

    // start at one (we are taking the product)

    // "The eigenvalues are sorted in increasing order."
    const auto eigenvalues = eigenSolver.eigenvalues();
    this->SetDeterminant(  eigenvalues.tail( rank ).prod() );

    }
  else // !isSingular
    {
      // THIS IS FOR FULL RANK SQUARED COVARIANCE
    // just take the regular determinant
    this->SetDeterminant( determinantCalculator.determinant() );
    }

} // SetDeterminant
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
template< typename TDataType, int TDimensionality >
typename
MultivariateGaussianDistribution< TDataType, TDimensionality >
::ProbabilityType
MultivariateGaussianDistribution< TDataType, TDimensionality >
::GetDeterminant() const
{

  // return the Determinant if set
 if ( this->m_SecretData->m_Determinant )
   return this->m_SecretData->m_Determinant;

 //otherwise 
 cerr << "Determinant has not yet been calculated!" << endl;
 return ProbabilityType();

} // GetDeterminant
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
template< typename TDataType, int TDimensionality >
void
MultivariateGaussianDistribution< TDataType, TDimensionality >
::SetDeterminant( const ProbabilityType determinant )
{
  this->m_SecretData->m_Determinant = determinant;
} // SetDeterminant
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
template< typename TDataType, int TDimensionality >
void
MultivariateGaussianDistribution< TDataType, TDimensionality >
::SetPseudoInverse( const PseudoInversePointer & pseudoInverse )
{
  // Set pseudoInverse and UsePseodoInverse flag;
  this->m_SecretData->m_UsePseudoInverse = true;
  this->m_SecretData->m_CovariancePseudoInverse = pseudoInverse;

} // SetPseudoInverse
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
template< typename TDataType, int TDimensionality >
void
MultivariateGaussianDistribution< TDataType, TDimensionality >
::CalculateInverse()
{
  
  const CovarianceMatrixPointer variance = this->Superclass::GetVariance();

  typedef typename SecretData::InverseDecompositionType InverseDecompositionType;
  InverseDecompositionType & inverse = this->m_SecretData->m_CovarianceInverse;
  inverse = InverseDecompositionType( *variance );

} // CalculateInverse
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
template< typename TDataType, int TDimensionality >
template< int TNumberOfVectors >
std::shared_ptr< Eigen::Matrix<
  TDataType,
  TDimensionality,
  TNumberOfVectors,
  ( TNumberOfVectors == 1 ? Eigen::ColMajor : Eigen::RowMajor ),
  TDimensionality,
  TNumberOfVectors > >
MultivariateGaussianDistribution< TDataType, TDimensionality >
::SecretData::GetRandomVectors( const unsigned int numberOfVectors ) const
{

  // make sure our template parameter
  // matches the number of vectors
  const volatile bool invalidDimensionality = 
    TNumberOfVectors != Eigen::Dynamic && 
    static_cast< unsigned int >( TNumberOfVectors ) != numberOfVectors;

  if ( invalidDimensionality )
    {
    throw std::runtime_error( "Invalid vector dimensionality!" );
    }

  typedef Eigen::Matrix<
    TDataType,
    TDimensionality,
    TNumberOfVectors,
    ( TNumberOfVectors == 1 ? Eigen::ColMajor : Eigen::RowMajor ),
    TDimensionality,
    TNumberOfVectors
  > OutputVectorType;

  typedef std::shared_ptr<
    OutputVectorType
  > OutputVectorPointer;

  // calculate a standard normal random vector X
  typedef UnivariateGaussianDistribution< TDataType > UnivariateGaussianType;
  const typename UnivariateGaussianType::Pointer
    univariateGaussianPointer = UnivariateGaussianType::New();
  UnivariateGaussianType & univariateGaussian = *univariateGaussianPointer;
  univariateGaussian.SetMean    ( 0 );
  univariateGaussian.SetVariance( 1 );

  // for example, get a 12x? vector "z" of a standard normal
  const OutputVectorType standardNormalRandom =
    *univariateGaussian.GetRandomMatrix(
      TDimensionality, numberOfVectors );

  // extract the mean and covariance
  const CovarianceMatrixPointer covariance =
    this->m_EncapsulatingClass->GetVariance();
  const MeanVectorPointer mean =
    this->m_EncapsulatingClass->GetMean();

  // http://bit.ly/NWo5oT
  // calculate Y = mean + A * X
  // where A * A' = covariance
  // so that Y is not a standard normal, but
  // rather has the desired mean and covariance
  const OutputVectorPointer normalRandom(
    new OutputVectorType( TDimensionality, numberOfVectors ) );

  // LLT is unstable, so we use LDLT
  Eigen::LDLT< CovarianceMatrixType > ldlt( *covariance );

  // extract the matrix A = P^T * L * sqrt(D)
  // A^T = sqrt(D) * L^T * P
  // and covariance = A * A^T = P^T * L * D * L^T * P
  const CovarianceMatrixType A =
    ldlt.transpositionsP().transpose() *
    CovarianceMatrixType( ldlt.matrixL() ) *
    ldlt.vectorD().cwiseAbs().cwiseSqrt().asDiagonal();

  // we might have to duplicate the mean across
  // the columns if we have more than one
  *normalRandom = mean->replicate( 1, numberOfVectors ) + A * standardNormalRandom;

  // output
  return normalRandom;

} // GetRandomVectors
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
#ifdef Boost_SERIALIZATION_FOUND

template< typename TDataType, int TDimensionality >
template< class TArchive >
void MultivariateGaussianDistribution< TDataType, TDimensionality >
::serialize(
        TArchive     & archive,
  const unsigned int   version )
{
  
  using boost::serialization::void_cast_register;
  using boost::serialization::make_nvp;
 
  
  // http://bit.ly/WKp6BV
  void_cast_register(
    static_cast< Self       * >( NULL ),
    static_cast< Superclass * >( NULL ) );
  

  if ( version < 1 )
    return;

  archive
    & BOOST_SERIALIZATION_BASE_OBJECT_NVP( Superclass );
	// Rachel's Note: I am asuming currently that the child class will take care of saving the pseudoinverse since it must provide it to this class.
	// However I coded out the inverse type and will keep this in until I am sure it is not necessary.
	/*    & make_nvp( "InverseType", this->m_SecretData->m_UsePseudoInverse );
    
		if( this->m_SecretData->m_UsePseudoInverse )
		  {
		  archive
			& make_nvp( "PseudoInverse", this->m_SecretData->m_CovariancePseudoInverse );
		  }

	  //only reset the inverse if given;
	  if( this->m_SecretData->m_UsePseudoInverse )
		{
		this->SetPseudoInverse( this->m_SecretData->m_CovariancePseudoInverse );
		}
	*/ 

} // serialize

   /*
    * Note: You must do the following:
   
       template
       void MultivariateGaussianDistribution< DataType, Dimensionality >::
       serialize< boost::archive::xml_iarchive >(
         boost::archive::xml_iarchive & archive,
         const unsigned int version );
   
       template
       void MultivariateGaussianDistribution< DataType, Dimensionality >::
       serialize< boost::archive::xml_oarchive >(
         boost::archive::xml_oarchive & archive,
         const unsigned int version );
   
    */

#endif // Boost_SERIALIZATION_FOUND
//////////////////////////////////////////////////////////////////////////////////////////


} // namespace ccipd
#endif // ccipdMultivariateGaussianDistribution_hxx

