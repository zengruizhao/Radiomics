#ifndef __ccipdGaussianDistribution_hxx
#define __ccipdGaussianDistribution_hxx



//////////////////////////////////////////////////////////////////////////////////////////
// ccipd includes
#include "ccipdGaussianDistribution.h"

#include "ccipdDisableWarningsMacro.h"
  // Eigen includes
  #include <Eigen/Core>
#include "ccipdEnableWarningsMacro.h"

#ifdef Boost_SERIALIZATION_FOUND
  #include <boost/archive/xml_iarchive.hpp>
  #include <boost/archive/xml_oarchive.hpp>
  #include "ccipdMatrixSerialization.h"
  #include "ccipdSharedPointerSerialization.hxx"
#endif // Boost_SERIALIZATION_FOUND

//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////
template< typename TDataType, int TDimensionality >
class GaussianDistribution< TDataType, TDimensionality >::SecretData
{
  
public:
  SecretData(); // constructor

  typedef GaussianDistribution<
    TDataType,
    TDimensionality
  > EncapsulatingClass;

  typedef typename EncapsulatingClass::MeanVectorType          MeanVectorType;
  typedef typename EncapsulatingClass::MeanVectorPointer       MeanVectorPointer;
  typedef typename EncapsulatingClass::CovarianceMatrixType    CovarianceMatrixType;
  typedef typename EncapsulatingClass::CovarianceMatrixPointer CovarianceMatrixPointer;
 

  MeanVectorPointer       m_Mean;
  CovarianceMatrixPointer m_Variance;

}; // class SecretData
//////////////////////////////////////////////////////////////////////////////////////////




//////////////////////////////////////////////////////////////////////////////////////////
template< typename TDataType, int TDimensionality >
GaussianDistribution< TDataType, TDimensionality >
::SecretData::SecretData()
{

  // initialize our variables
  MeanVectorPointer mean( new MeanVectorType ); 
  mean->setZero();
  this->m_Mean = mean;

  CovarianceMatrixPointer variance( new CovarianceMatrixType );
  variance->setIdentity();

  this->m_Variance = variance;

} // constructor
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
template< typename TDataType, int TDimensionality >
GaussianDistribution< TDataType, TDimensionality >
::GaussianDistribution() :
ccipdNewPIMPLMacro
{
} // constructor
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
template< typename TDataType, int TDimensionality >
GaussianDistribution< TDataType, TDimensionality >
::~GaussianDistribution()
{
} // destructor
//////////////////////////////////////////////////////////////////////////////////////////




//////////////////////////////////////////////////////////////////////////////////////////
template< typename TDataType, int TDimensionality >
void
GaussianDistribution< TDataType, TDimensionality >
::SetMean( const MeanVectorPointer & variable )
{
  ccipdSetBodyMacro( Mean )
}
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
template< typename TDataType, int TDimensionality >
void
GaussianDistribution< TDataType, TDimensionality >
::SetVariance( const CovarianceMatrixPointer & variable )
{
  ccipdSetBodyMacro( Variance )

} // SetVariance
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
template< typename TDataType, int TDimensionality >
typename
GaussianDistribution< TDataType, TDimensionality >
::MeanVectorPointer
GaussianDistribution< TDataType, TDimensionality >
::GetMean() const
{
  ccipdGetBodyMacro( Mean )
}
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
template< typename TDataType, int TDimensionality >
typename
GaussianDistribution< TDataType, TDimensionality >
::CovarianceMatrixPointer
GaussianDistribution< TDataType, TDimensionality >
::GetVariance() const
{
  ccipdGetBodyMacro( Variance )
} // GetCovariance
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
template< typename TDataType, int TDimensionality >
typename
GaussianDistribution< TDataType, TDimensionality >
::DistributionVectorPointer
GaussianDistribution< TDataType, TDimensionality >
::GetMode() const
{

  // the mode is the mean
  return this->GetMean();

} // GetCovariance
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
template< typename TDataType, int TDimensionality >
void
GaussianDistribution< TDataType, TDimensionality >
::EstimateDistributionFromSamples( const DistributionVectorsType & inputSamples )
{

  // estimate the mean
  const MeanVectorPointer mean ( new MeanVectorType );
  *mean = inputSamples.rowwise().mean();
  this->SetMean( mean );

  // estimate the covariance
  const DistributionVectorsType meanSubtracted =
    inputSamples.colwise() - *mean;

  // how many samples do we have?
  const auto numberOfSamples = inputSamples.cols();

  const TDataType denominator =
    static_cast< TDataType >( numberOfSamples - 1 );

  
  const CovarianceMatrixPointer covariance ( new CovarianceMatrixType );
   *covariance = 
     ( meanSubtracted * meanSubtracted.transpose() ).array() /
    denominator;

  // set the covariance
  this->SetVariance( covariance );

} // EstimateDistributionFromSamples
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
#ifdef Boost_SERIALIZATION_FOUND

template< typename TDataType, int TDimensionality >
template< class TArchive >
void GaussianDistribution< TDataType, TDimensionality >
::serialize(
        TArchive     & archive,
  const unsigned int   version )
{
  using boost::serialization::make_nvp;

  if ( version < 1 )
    return;

  archive
    & BOOST_SERIALIZATION_BASE_OBJECT_NVP( Superclass )
    & make_nvp( "Mean"    , this->m_SecretData->m_Mean     )
    & make_nvp( "Variance", this->m_SecretData->m_Variance );

  // re-set and recalculate things
  this->SetMean    ( this->m_SecretData->m_Mean     );
  this->SetVariance( this->m_SecretData->m_Variance );

} // serialize

   /*
    * Note: You must do the following:
   
       template
       void GaussianDistribution< DataType, Dimensionality >::
       serialize< boost::archive::xml_iarchive >(
         boost::archive::xml_iarchive & archive,
         const unsigned int version );
   
       template
       void GaussianDistribution< DataType, Dimensionality >::
       serialize< boost::archive::xml_oarchive >(
         boost::archive::xml_oarchive & archive,
         const unsigned int version );
   
    */

#endif // Boost_SERIALIZATION_FOUND
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace ccipd
#endif // ccipdGaussianDistribution_hxx

