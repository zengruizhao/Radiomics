#ifndef __ccipdDistribution_hxx
#define __ccipdDistribution_hxx



//////////////////////////////////////////////////////////////////////////////////////////
// ccipd includes
#include "ccipdDistribution.h"


#include "ccipdDisableWarningsMacro.h"
  // eigen includes
  #include <Eigen/Core>

#include <iostream>

#ifdef Boost_SERIALIZATION_FOUND
  #include <boost/archive/xml_iarchive.hpp>
  #include <boost/archive/xml_oarchive.hpp>
  #include "ccipdMatrixSerialization.h"
#endif // Boost_SERIALIZATION_FOUND
//////////////////////////////////////////////////////////////////////////////////////////

#include "ccipdEnableWarningsMacro.h"

//////////////////////////////////////////////////////////////////////////////////////////
using std::cerr;
using std::endl;
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////
template< typename TDataType, int TDimensionality >
Distribution< TDataType, TDimensionality >::Distribution()
{
} // constructor
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
template< typename TDataType, int TDimensionality >
Distribution< TDataType, TDimensionality >::~Distribution()
{
} // destructor
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
template< typename TDataType, int TDimensionality >
typename Distribution< TDataType, TDimensionality >::ProbabilityType
Distribution< TDataType, TDimensionality >
::GetLogPDF( const DistributionVectorType & value ) const
{
  // most simplistic way to compute it
  return std::log( this->GetPDF( value ) );
}
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
template< typename TDataType, int TDimensionality >
typename
Distribution< TDataType, TDimensionality >
::ProbabilityType
Distribution< TDataType, TDimensionality >
::GetCDFComplement( const DistributionVectorType & value ) const
{

  // simplistic approach
  return 1.0 - this->GetCDF( value );

} // GetCDFComplement
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
template< typename TDataType, int TDimensionality >
typename
Distribution< TDataType, TDimensionality >
::ProbabilityType
Distribution< TDataType, TDimensionality >
::GetProbability(
  const DistributionVectorType & value1,
  const DistributionVectorType & value2 ) const
{
  return std::abs( this->GetCDF( value1 ) - this->GetCDF( value2 ) );
} // GetProbability
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
template< typename TDataType, int TDimensionality >
typename
Distribution< TDataType, TDimensionality >
::DistributionVectorsPointer
Distribution< TDataType, TDimensionality >
::GetRandomVectors( const unsigned int vectorSize, const unsigned int numberOfVectors )
{
  #ifdef _MSC_VER
  #pragma warning( disable: 4127 ) // because TDimensionlity != -1 is a constant expression.
  #endif // _MSC_VER

  // simplistic approach
  if ( TDimensionality != -1 && TDimensionality != vectorSize )
    {
    cerr << "Invalid vectorSize given as input." << endl;
    return DistributionVectorsPointer();
    }
  
  // prepare our output
  DistributionVectorsPointer randomVectors(
    new DistributionVectorsType( vectorSize, numberOfVectors ) );

  // fill in each column with a random vector
  for ( unsigned int col = 0; col < numberOfVectors; ++col )
    {

    // calculate a random vector
    const DistributionVectorPointer randomVector =
      this->GetRandomVector();

    if ( !randomVector )
      {
      cerr << "No random vector returned." << endl;
      return DistributionVectorsPointer();
      } 

    if ( randomVector->size() != (int)vectorSize )
      {
      cerr << "Invalid randomVector size." << endl;
      return DistributionVectorsPointer();
      }

    // copy it into the current column
    randomVectors->col( col ) = *randomVector;

    } // for col

  return randomVectors;

} // GetCDFComplement
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
#ifdef Boost_SERIALIZATION_FOUND

template< typename TDataType, int TDimensionality >
template< class TArchive >
void Distribution< TDataType, TDimensionality >
::serialize(
        TArchive     & archive,
  const unsigned int   version )
{
  (void)archive;
  (void)version;
} // serialize

    /*
     * Note: You must do the following:
    
        template
        void Distribution< DataType, Dimensionality >::
        serialize< boost::archive::xml_iarchive >(
          boost::archive::xml_iarchive & archive,
          const unsigned int version );
    
        template
        void Distribution< DataType, Dimensionality >::
        serialize< boost::archive::xml_oarchive >(
          boost::archive::xml_oarchive & archive,
          const unsigned int version );
    
     */

#endif // Boost_SERIALIZATION_FOUND
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace ccipd
#endif // __ccipdDistribution_hxx
