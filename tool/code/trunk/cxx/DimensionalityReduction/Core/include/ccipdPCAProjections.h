#ifndef __ccipdPCAProjections_h
#define __ccipdPCAProjections_h



//////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdMatrixTypes.h"

#include <vector>
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////
/**
 * This will take a subset of the eigenvectorCols matrix
 * and use that to calculate the z-scores.  The output
 * will be the same size as the number of columns in
 * the input matrix.
 * startingRows says which rows each "values" starts on.
 * 
 * \author  Rob Toth
 * \ingroup DimensionalityReduction
 */
template< class TValueType >
std::shared_ptr< Eigen::Matrix< TValueType, -1, -1, Eigen::RowMajor, -1, -1 > >
CalculateProjection(
  const Eigen::Matrix< TValueType, -1, -1, Eigen::RowMajor, -1, -1 > & eigenvectorCols,
  const std::vector<
    std::shared_ptr<
      const Eigen::Matrix<
        TValueType, -1,  1, Eigen::ColMajor, -1,  1 > > >            & values,
  const std::vector< std::size_t >                     & startingRows );
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace



#endif // __ccipdPCAProjections_h
