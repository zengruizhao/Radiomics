#ifndef __ccipdPCATraining_h
#define __ccipdPCATraining_h



//////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdMatrixTypes.h"

#include <memory>
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////
/**
 * Each column of the output is an eigenvector.
 * 
 * The first column explains the most variance, and the
 * subsequent columns are decreasing.
 * 
 * Make sure 0 < varianceToRetain <= 1 or "1" will be used.
 * 
 * Make sure covarianceMatrix is square and symmetric.
 *
 * Eigenvalues passes in a pointer to where you want your eigenvalues stored (computed in function). 
 * 
 * \author  Rob Toth
 * \ingroup DimensionalityReduction
 */
template< class TValueType >
std::shared_ptr< Eigen::Matrix< TValueType, -1, -1, Eigen::RowMajor, -1, -1 > >
CalculateEigenvectorsOfCovariance(
  const Eigen::Matrix< TValueType, -1, -1, Eigen::RowMajor, -1, -1 > & covarianceMatrix,
        Eigen::Matrix< TValueType, -1,  1, Eigen::ColMajor, -1,  1 > & eigenvalues,
  const float                                            varianceToRetain = 1.0f,
  const bool                                             verbose = false );

/**
 * Same as above, but do not output the
 * resulting (sorted in decreasing value) eigenvalues.
 * 
 * \author  Rob Toth
 * \ingroup DimensionalityReduction
 */
template< class TValueType >
std::shared_ptr< Eigen::Matrix< TValueType, -1, -1, Eigen::RowMajor, -1, -1 > >
CalculateEigenvectorsOfCovariance(
  const Eigen::Matrix<
    TValueType, -1, -1, Eigen::RowMajor, -1, -1 > & covarianceMatrix, //is this the pseudoinverse? or the covariance estimated from your samples?
  const float                         varianceToRetain = 1.0f,
  const bool                          verbose = false );
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace



#endif // __ccipdPCATraining_h
