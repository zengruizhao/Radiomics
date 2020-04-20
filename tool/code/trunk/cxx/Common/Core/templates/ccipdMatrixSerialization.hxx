#ifndef __ccipdMatrixSerialization_hxx
#define __ccipdMatrixSerialization_hxx



////////////////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdMatrixSerialization.h"

#include "ccipdDisableWarningsMacro.h"
 // boost includes
  #include <boost/serialization/version.hpp>
  #include <boost/serialization/nvp.hpp>
  #include <boost/serialization/array.hpp>

  // Eigen includes
  #include <Eigen/Core>
#include "ccipdEnableWarningsMacro.h"
////////////////////////////////////////////////////////////////////////////////////////////////////



namespace boost {
namespace serialization {



////////////////////////////////////////////////////////////////////////////////////////////////////
template< class Archive, class MatrixValueType, int NumberOfRows, int NumberOfCols, int TOptions >
void serialize(
  Archive & archive,
  Eigen::Matrix<
    MatrixValueType,
    NumberOfRows,
    NumberOfCols,
    TOptions,
    NumberOfRows,
    NumberOfCols
  > & matrix,
  const unsigned int version )
{

  (void)version; // don't warn about unused parameter

  // serialize the matrix size
  typename Eigen::Matrix< MatrixValueType, -1, -1, TOptions, -1, -1>::Index
//  std::size_t
    rows = matrix.rows(),
    cols = matrix.cols();
  archive
    & BOOST_SERIALIZATION_NVP(rows)
    & BOOST_SERIALIZATION_NVP(cols);

  // make sure the matrix is the correct size
  if ( matrix.rows() != rows || matrix.cols() != cols )
    matrix.resize(rows, cols);

  // serialize the matrix data as an array
  using boost::serialization::make_array;
  typedef boost::serialization::array< MatrixValueType > DataArray;
  DataArray matrixData = make_array(matrix.data(), rows * cols); // extract the data pointer

  // save the matrix data
  archive & boost::serialization::make_nvp( "MatrixData", matrixData );

} // serialize
////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////
template < class MatrixValueType, int NumberOfRows, int NumberOfCols, int TOptions >
struct version< Eigen::Matrix<
  MatrixValueType,
  NumberOfRows,
  NumberOfCols,
  TOptions,
  NumberOfRows,
  NumberOfCols > >
{

  // version 1 of matrix serialization
  static const unsigned int value = 1;

}; // version
////////////////////////////////////////////////////////////////////////////////////////////////////



} // namespace serialization
} // namespace boost
#endif // __ccipdMatrixSerialization_hxx
