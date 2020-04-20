#ifndef __ccipdMatrixConcatenation_h
#define __ccipdMatrixConcatenation_h



//////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdMatrixTypes.h"

#include <vector>
#include <memory> // for smart pointers
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Concatenate vectors.
 * 
 * \date    2012-04-26
 * \author  Rob Toth
 * \ingroup DimensionalityReduction
 */
template< class TValueType >
std::shared_ptr< Eigen::Matrix< TValueType, -1,  1, Eigen::ColMajor, -1,  1 > >
ConcatenateVectors(
  const std::vector<
    std::shared_ptr<
      const Eigen::Matrix<
        TValueType, -1,  1, Eigen::ColMajor, -1,  1 > > > & vectors );
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
/**
 * Helper function to concatenate a set of matrices, rowwise.
 * For example, a 2x4 and a 3x4 will result in a 5x4 matrix.
 * 
 * \author  Rob Toth
 * \ingroup DimensionalityReduction
 */
template< class TValueType >
std::shared_ptr< Eigen::Matrix< TValueType, -1, -1, Eigen::RowMajor, -1, -1 > >
ConcatenateMatrixRows(
  const std::vector<
    std::shared_ptr<
      const Eigen::Matrix< TValueType, -1, -1, Eigen::RowMajor, -1, -1 > > > & matrices );
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace



#endif // __ccipdMatrixConcatenation_h
