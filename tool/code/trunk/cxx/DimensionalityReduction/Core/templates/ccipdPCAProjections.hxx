#ifndef __ccipdPCAProjections_hxx
#define __ccipdPCAProjections_hxx



//////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdPCAProjections.h"
#include "ccipdMatrixConcatenation.h" // for concatenating matrices

#include <iostream> // for cout, etc.

#include "ccipdDisableWarningsMacro.h"
  #include "Eigen/Core"        // for matrices and vectors
  #include "Eigen/QR"          // for solve()
#include "ccipdEnableWarningsMacro.h"

// this warning was in the Household QR code in eigen
// in visual studio
#ifdef _MSC_VER
#pragma warning( disable: 4244 ) // possible loss of data
#pragma warning( disable: 4714 ) // inlining
#endif // _MSC_VER
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////
template< class TValueType >
std::shared_ptr< Eigen::Matrix< TValueType, -1, -1, Eigen::RowMajor, -1, -1 > >
CalculateProjection(
  const Eigen::Matrix< TValueType, -1, -1, Eigen::RowMajor, -1, -1 > & eigenvectorCols,
  const std::vector<
    std::shared_ptr<
      const Eigen::Matrix<
        TValueType, -1,  1, Eigen::ColMajor, -1,  1 > > >            & values,
  const std::vector< std::size_t >                     & startingRows )
{

  typedef Eigen::Matrix< TValueType, -1, -1, Eigen::RowMajor, -1, -1 > TemplatedMatrixType;
  typedef Eigen::Matrix< TValueType, -1,  1, Eigen::ColMajor, -1,  1 > TemplatedVectorType;
  typedef std::shared_ptr<       TemplatedMatrixType >   TemplatedMatrixPointer;
  typedef std::shared_ptr< const TemplatedMatrixType >   TemplatedMatrixConstPointer;
  typedef std::shared_ptr< const TemplatedVectorType >   TemplatedVectorConstPointer;
  typedef std::vector< TemplatedMatrixConstPointer >     TemplatedMatrixConstCollection;
  typedef typename TemplatedMatrixType::Index            IndexType;

  // check out inptus
  const std::size_t numberOfValues = values.size();
  if ( numberOfValues != startingRows.size() || numberOfValues == 0 )
    return TemplatedMatrixPointer();

  // initial some data
  TemplatedMatrixConstCollection subMatrices;
  const IndexType
    startingCol  = 0,
    numberOfCols = eigenvectorCols.cols(),
    maximumRow   = eigenvectorCols.rows() - 1;
  
  // go through each input
  for ( std::size_t index = 0; index < numberOfValues; ++index )
    {
    
    const IndexType
      startingRow  = startingRows[ index ],
      numberOfRows = values      [ index ]->size();
      
    if ( numberOfRows <= maximumRow )
      subMatrices.push_back( TemplatedMatrixConstPointer(
        new TemplatedMatrixType( eigenvectorCols.block(
          startingRow , startingCol,
          numberOfRows, numberOfCols ) ) ) );
    
    } // starting rows, values
  
  // concatenate
  const TemplatedMatrixPointer
    concatenatedMatrix = ConcatenateMatrixRows( subMatrices );
  const TemplatedVectorConstPointer
    concatenatedVectors = ConcatenateVectors( values );
  
  // calculate the projection
  const TemplatedMatrixPointer projection =
    TemplatedMatrixPointer( new TemplatedMatrixType );

  *projection = concatenatedMatrix->colPivHouseholderQr().solve( *concatenatedVectors );
  
  // output
  return projection;
  
} // CalculateProjection
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace
#endif // __ccipdPCAProjections_hxx
