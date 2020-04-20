#ifndef __ccipdPseudoInverse_h
#define __ccipdPseudoInverse_h



//////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdMatrixTypes.h"

#include <memory>
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{




//////////////////////////////////////////////////////////////////////////////////////////
/**
 * http://ccipd.rutgers.edu/short/d
 * The return type is of size eigenvectorCols.transpose()
 * 
 * \author  Rob Toth
 * \ingroup DimensionalityReduction
 */
template< class TValueType >
std::shared_ptr< Eigen::Matrix< TValueType, -1, -1, Eigen::RowMajor, -1, -1 > >
CalculatePseudoInverseUsingSVD(
  const Eigen::Matrix< TValueType, -1, -1, Eigen::RowMajor, -1, -1 > & eigenvectorCols );

/**
 * This will use a traditional
 * matrix inversion technique
 * which isn't as numerically
 * stable as SVD for calculating
 * the pseudo-inverse but may
 * be the only possible route
 * if the SVD can't create a
 * matrix as large as U is required
 * to be.
 * 
 * \author  Rob Toth
 * \ingroup DimensionalityReduction
 */
template< class TValueType >
std::shared_ptr< Eigen::Matrix< TValueType, -1, -1, Eigen::RowMajor, -1, -1 > >
CalculatePseudoInverseUsingInversion(
  const Eigen::Matrix< TValueType, -1, -1, Eigen::RowMajor, -1, -1 > & eigenvectorCols );

/**
 * This will use a traditional
 * matrix inversion technique
 * which isn't as numerically
 * stable as SVD for calculating
 * the weighted pseudo-inverse but may
 * be the only possible route
 * if the SVD can't create a
 * matrix as large as U is required
 * to be.
 * 
 * \author  Rob Toth
 * \ingroup DimensionalityReduction
 */
template< class TValueType >
std::shared_ptr< Eigen::Matrix< TValueType, -1, -1, Eigen::RowMajor, -1, -1 > >
CalculatePseudoInverseUsingInversion(
  const Eigen::Matrix< TValueType, -1, -1, Eigen::RowMajor, -1, -1 > & eigenvectorCols,
  const Eigen::Matrix< TValueType, -1,  1, Eigen::ColMajor, -1,  1 > & weights );

/**
 * This will use a QR decomposition
 * and actually solve a least squares
 * system to find the pseudo-inverse.
 * 
 * \author  Rob Toth
 * \ingroup DimensionalityReduction
 */
template< class TValueType >
std::shared_ptr< Eigen::Matrix< TValueType, -1, -1, Eigen::RowMajor, -1, -1 > >
CalculatePseudoInverseUsingQR(
  const Eigen::Matrix< TValueType, -1, -1, Eigen::RowMajor, -1, -1 > & eigenvectorCols );

/**
 * This will use a QR decomposition
 * and actually solve a least squares
 * system to find the weighted pseudo-inverse.
 * 
 * \author  Rob Toth
 * \ingroup DimensionalityReduction
 */
template< class TValueType >
std::shared_ptr< Eigen::Matrix< TValueType, -1, -1, Eigen::RowMajor, -1, -1 > >
CalculatePseudoInverseUsingQR(
  const Eigen::Matrix< TValueType, -1, -1, Eigen::RowMajor, -1, -1 > & eigenvectorCols,
  const Eigen::Matrix< TValueType, -1,  1, Eigen::ColMajor, -1,  1 > & weights );

/**
 * First, try the
 * CalculatePseudoInverseUsingSVD() 
 * method, but if that fails
 * then try the traditional
 * CalculatePseudoINverseUsingInversion()
 * method.
 * 
 * \author  Rob Toth
 * \ingroup DimensionalityReduction
 */
template< class TValueType >
std::shared_ptr< Eigen::Matrix< TValueType, -1, -1, Eigen::RowMajor, -1, -1 > >
CalculatePseudoInverse(
  const Eigen::Matrix< TValueType, -1, -1, Eigen::RowMajor, -1, -1 > & eigenvectorCols );

/**
 * http://ccipd.rutgers.edu/short/e
 * The return type is of size eigenvectorCols.transpose()
 * 
 * \author  Rob Toth
 * \ingroup DimensionalityReduction
 */
template< class TValueType >
std::shared_ptr< Eigen::Matrix< TValueType, -1, -1, Eigen::RowMajor, -1, -1 > >
CalculatePseudoInverse(
  const Eigen::Matrix< TValueType, -1, -1, Eigen::RowMajor, -1, -1 > & eigenvectorCols,
  const Eigen::Matrix< TValueType, -1,  1, Eigen::ColMajor, -1,  1 > & weights );
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace



#endif // __ccipdPseudoInverse_h
