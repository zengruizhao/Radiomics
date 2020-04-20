#ifndef __ccipdPseudoInverse_hxx
#define __ccipdPseudoInverse_hxx



//////////////////////////////////////////////////////////////////////////////////////////
// ccipd includes
#include "ccipdPseudoInverse.h"

#include "ccipdDisableWarningsMacro.h"
  // eigen includes
  #include <Eigen/Core>       // for matrices and vectors
  #include <Eigen/SparseCore> // for sparse D in SVD
  #include <Eigen/SVD>        // for pseudo-inverse
  #include <Eigen/LU>         // for .inverse()
#include "ccipdEnableWarningsMacro.h"

// std includes
#include <vector>
#include <iostream>
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{




//////////////////////////////////////////////////////////////////////////////////////////
template< class TValueType >
std::shared_ptr< Eigen::Matrix< TValueType, -1, -1, Eigen::RowMajor, -1, -1 > >
CalculatePseudoInverseUsingSVD(
  const Eigen::Matrix< TValueType, -1, -1, Eigen::RowMajor, -1, -1 > & eigenvectorCols )
{


  // Adapted from http://eigen.tuxfamily.org/index.php?title=FAQ

  typedef Eigen::Matrix< TValueType, -1, -1, Eigen::RowMajor, -1, -1 > TemplatedMatrixType;

  // calculate SVD
  typedef Eigen::JacobiSVD< TemplatedMatrixType > SVDType;
  SVDType svd( eigenvectorCols, Eigen::ComputeFullU | Eigen::ComputeFullV );

  const double tolerance = 1.e-6; // choose your tolerance wisely!

  // extract the singular values
  typedef typename SVDType::SingularValuesType SingularValuesType;
  const SingularValuesType & singularValues = svd.singularValues();

  // prepare to store triplets of the diagonal elements
  // of the D inverse
  typedef Eigen::Triplet< TValueType > TripletType;
  std::vector< TripletType > triplets;

  typedef typename SingularValuesType::Index IndexType;
  for ( IndexType singularValueIndex = 0;
    singularValueIndex < singularValues.size();
    ++singularValueIndex)
    {

    // extract the current singular value ( in double precision )
    const double singularValue = static_cast< double >(
      singularValues.coeff( singularValueIndex ) );

    if ( singularValue > tolerance )
      triplets.push_back( TripletType(
        static_cast< unsigned int >( singularValueIndex ), // row
        static_cast< unsigned int >( singularValueIndex ), // col
        static_cast< TValueType >(
          1.0 / singularValue ) ) ); // inverse

    } // for singularValueIndex

  // prepare the sparse diagonal matrix D in SVD
  typedef Eigen::SparseMatrix<
    TValueType,
    0,
    IndexType
  > SparseMatrixType;
  SparseMatrixType matrixDInverse(
    eigenvectorCols.cols(),
    eigenvectorCols.rows() );

  matrixDInverse.setFromTriplets( triplets.cbegin(), triplets.cend() );
  
  // extract the u, and v
  typedef typename SVDType::MatrixUType MatrixUType;
  typedef typename SVDType::MatrixVType MatrixVType;
  const MatrixUType & matrixU = svd.matrixU();
  const MatrixVType & matrixV = svd.matrixV();

  // actually calculate the pseudo-inverse
  typedef TemplatedMatrixType PseudoInverseType;
  typedef std::shared_ptr< PseudoInverseType > PseudoInversePointer;
  const PseudoInversePointer pseudoInverse( new PseudoInverseType );

  // http://bit.ly/IfgX1I
  *pseudoInverse =
    matrixV *
    matrixDInverse *
    matrixU.transpose();

  return pseudoInverse;

} // CalculatePseudoInverseUsingSVD
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
template< class TValueType >
std::shared_ptr< Eigen::Matrix< TValueType, -1, -1, Eigen::RowMajor, -1, -1 > >
CalculatePseudoInverseUsingInversion(
  const Eigen::Matrix< TValueType, -1, -1, Eigen::RowMajor, -1, -1 > & eigenvectorCols )
{

  // create a new smart pointer to store the inverse 
  typedef Eigen::Matrix< TValueType, -1, -1, Eigen::RowMajor, -1, -1 > PseudoInverseType;
  typedef std::shared_ptr< PseudoInverseType > PseudoInversePointer;
  const PseudoInversePointer pseudoInverse( new PseudoInverseType );

  std::cout << std::endl <<
    ( eigenvectorCols.transpose() * eigenvectorCols ).inverse() << std::endl;

  // calculate
  *pseudoInverse =
    ( eigenvectorCols.transpose() * eigenvectorCols ).inverse()
    * eigenvectorCols.transpose();

  return pseudoInverse;

} // CalculatePseudoInverseUsingInversion
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
template< class TValueType >
std::shared_ptr< Eigen::Matrix< TValueType, -1, -1, Eigen::RowMajor, -1, -1 > >
CalculatePseudoInverseUsingQR(
  const Eigen::Matrix< TValueType, -1, -1, Eigen::RowMajor, -1, -1 > & eigenvectorCols )
{

  // create a new smart pointer to store the inverse 
  typedef Eigen::Matrix< TValueType, -1, -1, Eigen::RowMajor, -1, -1 > PseudoInverseType;
  typedef std::shared_ptr< PseudoInverseType > PseudoInversePointer;
  const PseudoInversePointer pseudoInverse( new PseudoInverseType );

  // We want the pseudo-inverse of eigenvectorCols

  // Let A = eigenvectorCols' * eigenvectorCols

  const PseudoInverseType innerProduct =
    eigenvectorCols.transpose() * eigenvectorCols;

  // Traditionally we do x = inverse(A) * eigenvectorCols'

  // Now we have A * x = B
  // where B is eigenvectorCols',
  // and x is the pseudo-inverse.

  // so we solve a linear system

  // colPivHouseholderQr may be slightly faster
  // but the inner product matrix should be small
  // enough that the extra accuracy
  // is worth it
  *pseudoInverse = innerProduct.fullPivHouseholderQr().solve(
    eigenvectorCols.transpose() );

  return pseudoInverse;

} // CalculatePseudoInverseUsingQR
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
template< class TValueType >
std::shared_ptr< Eigen::Matrix< TValueType, -1, -1, Eigen::RowMajor, -1, -1 > >
CalculatePseudoInverseUsingQR(
  const Eigen::Matrix< TValueType, -1, -1, Eigen::RowMajor, -1, -1 > & eigenvectorCols,
  const Eigen::Matrix< TValueType, -1,  1, Eigen::ColMajor, -1,  1 > & weights )
{

  // create a new smart pointer to store the inverse 
  typedef Eigen::Matrix< TValueType, -1, -1, Eigen::RowMajor, -1, -1 > PseudoInverseType;
  typedef std::shared_ptr< PseudoInverseType > PseudoInversePointer;
  const PseudoInversePointer pseudoInverse( new PseudoInverseType );

  // We want the pseudo-inverse of eigenvectorCols

  // Let A = eigenvectorCols' * eigenvectorCols

  const PseudoInverseType innerProduct =
    eigenvectorCols.transpose() * weights.asDiagonal() * eigenvectorCols;

  // Traditionally we do x = inverse(A) * eigenvectorCols'

  // Now we have A * x = B
  // where B is eigenvectorCols',
  // and x is the pseudo-inverse.

  // so we solve a linear system

  // colPivHouseholderQr may be slightly faster
  // but the inner product matrix should be small
  // enough that the extra accuracy
  // is worth it
  *pseudoInverse = innerProduct.fullPivHouseholderQr().solve(
    eigenvectorCols.transpose() * weights.asDiagonal() );

  return pseudoInverse;

} // CalculatePseudoInverseUsingQR (weighted)
//////////////////////////////////////////////////////////////////////////////////////////




//////////////////////////////////////////////////////////////////////////////////////////
template< class TValueType >
std::shared_ptr< Eigen::Matrix< TValueType, -1, -1, Eigen::RowMajor, -1, -1 > >
CalculatePseudoInverse(
  const Eigen::Matrix< TValueType, -1, -1, Eigen::RowMajor, -1, -1 > & eigenvectorCols )
{
  
  // svd is the preferred choice
  try
    {
    return CalculatePseudoInverseUsingSVD( eigenvectorCols );
    }
  catch( ... )
    {
    }

  // then try to QR decomposition
  try
    {
    return CalculatePseudoInverseUsingQR( eigenvectorCols );
    }
  catch( ... )
    {
    }

  // finally use the least-accurate inversion
  return CalculatePseudoInverseUsingInversion( eigenvectorCols ); 

} // CalculatePseudoInverse
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
template< class TValueType >
std::shared_ptr< Eigen::Matrix< TValueType, -1, -1, Eigen::RowMajor, -1, -1 > >
CalculatePseudoInverseUsingInversion(
  const Eigen::Matrix< TValueType, -1, -1, Eigen::RowMajor, -1, -1 > & eigenvectorCols,
  const Eigen::Matrix< TValueType, -1,  1, Eigen::ColMajor, -1,  1 > & weights )
{

  typedef Eigen::Matrix< TValueType, -1, -1, Eigen::RowMajor, -1, -1 > TemplatedMatrixType;
  typedef std::shared_ptr< TemplatedMatrixType >         TemplatedMatrixPointer;

  const TemplatedMatrixPointer pseudoInverse( new TemplatedMatrixType );
  *pseudoInverse = 
    ( eigenvectorCols.transpose() * weights.asDiagonal() * eigenvectorCols ).inverse()
    * eigenvectorCols.transpose() * weights.asDiagonal();

  return pseudoInverse;

} // CalculatePseudoInverse (weighted)
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
template< class TValueType >
std::shared_ptr< Eigen::Matrix< TValueType, -1, -1, Eigen::RowMajor, -1, -1 > >
CalculatePseudoInverse(
  const Eigen::Matrix< TValueType, -1, -1, Eigen::RowMajor, -1, -1 > & eigenvectorCols,
  const Eigen::Matrix< TValueType, -1,  1, Eigen::ColMajor, -1,  1 > & weights )
{
  // prefer the QR
  try
    {
    return CalculatePseudoInverseUsingQR( eigenvectorCols, weights );
    }
  catch( ... )
    {
    }

  // finally use the least-accurate inversion
  return CalculatePseudoInverseUsingInversion( eigenvectorCols, weights ); 

} // CalculatePseudoInverse (weighted)
//////////////////////////////////////////////////////////////////////////////////////////




} // namespace
#endif // __ccipdPseudoInverse_hxx
