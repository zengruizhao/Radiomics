#ifndef __ccipdPCADistribution_hxx
#define __ccipdPCADistribution_hxx



//////////////////////////////////////////////////////////////////////////////////////////
// ccipd includes
#include "ccipdPCADistribution.h"
#include "ccipdPseudoInverse.h"

#include "ccipdDisableWarningsMacro.h"

	// std includes
	#include <iostream>

  // Eigen includes
  #include <Eigen/Eigen>    // for singular values

  #ifdef Boost_SERIALIZATION_FOUND
    #include <boost/archive/xml_iarchive.hpp>
    #include <boost/archive/xml_oarchive.hpp>
    #include "ccipdMatrixSerialization.h"
    #include "ccipdSharedPointerSerialization.hxx"
  #endif // Boost_SERIALIZATION_FOUND
#include "ccipdEnableWarningsMacro.h"

#ifdef _MSC_VER
#pragma warning( disable: 4355 )
//#pragma warning( disable: 4714 )
#endif // _MSC_VER
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
using std::cerr;
using std::endl;
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////
template< typename TDataType >
class PCADistribution< TDataType >::SecretData
{
  
public:

  typedef PCADistribution<
    TDataType
  > EncapsulatingClass;
  
  typedef typename EncapsulatingClass::SecretData Self;
  typename EncapsulatingClass::SecretData & operator=(
    const Self & other ){ return const_cast< Self & >( other ); }

  //SecretData();
  SecretData( const EncapsulatingClass * const encapsulatingClass ); // constructor

  typedef typename EncapsulatingClass::EigenVectorMatrixPointer EigenVectorMatrixType;

  EigenVectorMatrixPointer m_EigenVectors;

  const EncapsulatingClass * const
    m_EncapsulatingClass;

}; // class SecretData
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
template< typename TDataType >
PCADistribution< TDataType >
::SecretData::SecretData(
const EncapsulatingClass * const encapsulatingClass ) :
m_EncapsulatingClass( encapsulatingClass )
{
} // constructor
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
template< typename TDataType >
PCADistribution< TDataType >
::PCADistribution() :
m_SecretData( new SecretData( this ) )
{
} // constructor
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
template< typename TDataType >
PCADistribution< TDataType >
::~PCADistribution()
{
} // destructor
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
template< typename TDataType >
void
PCADistribution< TDataType >
::SetEigenVectorsAndValues(
    const MeanVectorPointer        & mean, 
    const EigenVectorMatrixPointer & eigenVectors,
    const EigenValuePointer        & eigenValues )
{
 
  // normalize the eigenvector by the sqrt(Eigenvalues)
  const EigenVectorMatrixPointer normalizedEigenVectors = eigenVectors;
  *normalizedEigenVectors = (*eigenVectors) * (*eigenValues).cwiseSqrt().asDiagonal();

  // then call second distribution type
  this->SetEigenVectors( mean, normalizedEigenVectors );
  
}
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
template< typename TDataType >
void
PCADistribution< TDataType >
::SetEigenVectors(
    const MeanVectorPointer        & mean, 
    const EigenVectorMatrixPointer & eigenVectors )
{

 this->m_SecretData->m_EigenVectors = eigenVectors;
  this->SetMean( mean );
}
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
template< typename TDataType >
void
PCADistribution< TDataType >
::ClearInverse()
{

  cerr << "PseudoInverse use is required for PCADistribution!" << endl;

} // ClearInverse
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
template< typename TDataType >
void
PCADistribution< TDataType >
::CalculateDeterminant()
{

  this->Superclass::SetDeterminant( 1 );

} // CalculateDeterminant
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
template< typename TDataType >
void
PCADistribution< TDataType >
::CalculatePseudoInverse()
{

  this->Superclass::SetPseudoInverse( CalculatePseudoInverseUsingQR( *this->m_SecretData->m_EigenVectors ) );

} // CalculatePseudoInverse
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
#ifdef Boost_SERIALIZATION_FOUND

template< typename TDataType >
template< class TArchive >
void PCADistribution< TDataType >
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
    & BOOST_SERIALIZATION_BASE_OBJECT_NVP( Superclass )
    & make_nvp( "EigenVectors", this->m_SecretData->m_EigenVectors ); //not sure if serializing the vectors is effecient...

  // recalculate items related to Eigenvectors
  this->SetEigenVectors( this->GetMean(), this->m_SecretData->m_EigenVectors );
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
#endif // __ccipdPCADistribution_hxx

