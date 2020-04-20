#ifndef __ccipdMatrixConcatenation_hxx
#define __ccipdMatrixConcatenation_hxx



//////////////////////////////////////////////////////////////////////////////////////////
// ccipd includes
#include "ccipdMatrixConcatenation.h"

// std includes
#include <iostream> // for cout, etc.

#include "ccipdDisableWarningsMacro.h"
  // boost includes
  #include <boost/foreach.hpp>

  // eigen includes
  #include "Eigen/Core"        // for matrices and vectors
#include "ccipdEnableWarningsMacro.h"
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////
template< class TValueType >
std::shared_ptr< Eigen::Matrix< TValueType, -1,  1, Eigen::ColMajor, -1,  1 > >
ConcatenateVectors(
  const std::vector<
    std::shared_ptr<
      const Eigen::Matrix<
        TValueType, -1,  1, Eigen::ColMajor, -1,  1 > > > & vectors )
{

  typedef Eigen::Matrix< TValueType, -1,  1, Eigen::ColMajor, -1,  1 > TemplatedVectorType;
  typedef std::shared_ptr<       TemplatedVectorType >   TemplatedVectorPointer;
  typedef std::shared_ptr< const TemplatedVectorType >   TemplatedVectorConstPointer;

  // how big is our output vector going to be?
  size_t outputVectorSize = 0;
  BOOST_FOREACH( const TemplatedVectorConstPointer & currentVector, vectors )
    {
    outputVectorSize += currentVector->size(); 
    }
  
  // initialize our concatenated vector with the correct # of elements
  TemplatedVectorPointer concatenatedVector(
    new TemplatedVectorType( outputVectorSize ) );

  // which element of our concatenated vector are we on?
  typedef typename EigenVectorTypeF::Index IndexType;
  IndexType outputIndex = 0;

  // go through and populate our output
  BOOST_FOREACH( const TemplatedVectorConstPointer & currentVector, vectors )
    {

    const IndexType currentSize = currentVector->size();

    // set the values of our concatenated vector
    concatenatedVector->segment(
      outputIndex, currentSize ) = *currentVector;

    // update our output index
    outputIndex += currentSize;

    } // vectorIter

  // output
  return concatenatedVector;

} // ConcatenateVectors
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
template< class TValueType >
std::shared_ptr< Eigen::Matrix< TValueType, -1, -1, Eigen::RowMajor, -1, -1 > >
ConcatenateMatrixRows(
  const std::vector<
    std::shared_ptr<
      const Eigen::Matrix< TValueType, -1, -1, Eigen::RowMajor, -1, -1 > > > & matrices )
{

  typedef Eigen::Matrix< TValueType, -1, -1, Eigen::RowMajor, -1, -1 > TemplatedMatrixType;
  typedef std::shared_ptr<       TemplatedMatrixType >   TemplatedMatrixPointer;
  typedef std::shared_ptr< const TemplatedMatrixType >   TemplatedMatrixConstPointer;

  // make sure we have at least one matrix
  const TemplatedMatrixPointer badOutput;
  if ( matrices.empty() )
    return badOutput;

  // the number of columns in our output matrix don't change
  // so just use the first matrix's columns
  typedef typename TemplatedMatrixType::Index IndexType;
  const IndexType outputMatrixCols = ( *matrices.begin() )->cols();

  // get the number of rows in the concatenated matrix
  IndexType outputMatrixRows = 0;
  BOOST_FOREACH(  const TemplatedMatrixConstPointer & matrix, matrices )
    {
    
    // get info about the current matrix
    const IndexType
      currentMatrixRows = matrix->rows(),
      currentMatrixCols = matrix->cols();

    // make sure the # of columns doesn't change
    if ( currentMatrixCols != outputMatrixCols )
      return badOutput;

    // add in the size of our current matrix
    outputMatrixRows += currentMatrixRows;
    
    } // for each matrix  

  // initialize our concatenated matrix with the correct # of elements
  TemplatedMatrixPointer concatenatedMatrix(
    new TemplatedMatrixType( outputMatrixRows, outputMatrixCols ) );

  // which element of our concatenated matrix are we on?
  IndexType outputRow = 0;

  // go through and populate our output
  BOOST_FOREACH( const TemplatedMatrixConstPointer & matrix, matrices )
    {

    // set the values of our concatenated matrix
    const IndexType
      outputCol   = 0,
      currentCols = concatenatedMatrix->cols(),
      currentRows = matrix->rows();

    concatenatedMatrix->block(
      outputRow  , outputCol,
      currentRows, currentCols ) = *matrix;

    // update our output index
    outputRow += currentRows;

    } // matrixIter

  // output
  return concatenatedMatrix;

} // ConcatenateMatrixRows
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace
#endif // __ccipdMatrixConcatenation_hxx
