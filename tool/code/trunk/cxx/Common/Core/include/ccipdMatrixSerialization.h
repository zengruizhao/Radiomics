
#ifndef __ccipdMatrixSerialization_h
#define __ccipdMatrixSerialization_h



////////////////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdMatrixTypes.h"
////////////////////////////////////////////////////////////////////////////////////////////////////



namespace boost {
namespace serialization {



////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * \brief The function to serialize the matrix.
 */
template< class Archive, class MatrixValueType, int NumberOfRows, int NumberOfCols, int TOptions >
void serialize(
  Archive             & archive,
  Eigen::Matrix<
    MatrixValueType,
    NumberOfRows,
    NumberOfCols,
    TOptions,
    NumberOfRows,
    NumberOfCols >    & matrix,
  const unsigned int    version );
////////////////////////////////////////////////////////////////////////////////////////////////////



} // namespace serialization
} // namespace boost
#endif // __ccipdMatrixSerialization_h
