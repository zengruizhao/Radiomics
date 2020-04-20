


////////////////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdMatrixSerialization.hxx"
#include "ccipdCommonCoreOptions.h"
////////////////////////////////////////////////////////////////////////////////////////////////////



#ifdef Boost_SERIALIZATION_FOUND



/**
 * This file compiles the Eigen matrix serialization code
 * for our defined matrix types, for xml archives.
 */



////////////////////////////////////////////////////////////////////////////////////////////////////
// the whole code depends on having the serialization library
#include "boost/archive/xml_iarchive.hpp"
#include "boost/archive/xml_oarchive.hpp"
////////////////////////////////////////////////////////////////////////////////////////////////////



namespace boost
{
namespace serialization
{



////////////////////////////////////////////////////////////////////////////////////////////////////
using boost::archive::xml_iarchive;
using boost::archive::xml_oarchive;
////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////
// Serialize dynamically sized matrices

// xml input
template void serialize< xml_iarchive, ccipd::InternalPixelType, -1, -1, 1 >(
  xml_iarchive & ar, ccipd::EigenMatrixTypeF & matrix, const unsigned int version);

// xml output
template void serialize< xml_oarchive, ccipd::InternalPixelType, -1, -1, 1 >(
  xml_oarchive & ar, ccipd::EigenMatrixTypeF & matrix, const unsigned int version);
////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////
// Serialize 12-row vectors for affine transformations 

// xml input
template void serialize< xml_iarchive, double, 12, 1, 0 >(
  xml_iarchive & ar,
  Eigen::Matrix< double, 12, 1, 0, 12, 1 > & matrix,
  const unsigned int version);

// xml output
template void serialize< xml_oarchive, double, 12, 1 >(
  xml_oarchive & ar,
  Eigen::Matrix< double, 12, 1, 0, 12, 1 > & matrix,
  const unsigned int version);
////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////
// Serialize 12-row vectors for affine transformations 

// xml input
template void serialize< xml_iarchive, double, 12, 1, 1 >(
  xml_iarchive & ar,
  Eigen::Matrix< double, 12, 1, 1, 12, 1 > & matrix,
  const unsigned int version);

// xml output
template void serialize< xml_oarchive, double, 12, 1, 1 >(
  xml_oarchive & ar,
  Eigen::Matrix< double, 12, 1, 1, 12, 1 > & matrix,
  const unsigned int version);
////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////
// Serialize 12x12 matrices for affine transformations 

// xml input
template void serialize< xml_iarchive, double, 12, 12, 0 >(
  xml_iarchive & ar,
  Eigen::Matrix< double, 12, 12, 0, 12, 12 > & matrix,
  const unsigned int version);

// xml output
template void serialize< xml_oarchive, double, 12, 12, 0 >(
  xml_oarchive & ar,
  Eigen::Matrix< double, 12, 12, 0, 12, 12 > & matrix,
  const unsigned int version);
////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////
// Serialize 12x12 matrices for affine transformations 

// xml input
template void serialize< xml_iarchive, double, 12, 12, 1 >(
  xml_iarchive & ar,
  Eigen::Matrix< double, 12, 12, 1, 12, 12 > & matrix,
  const unsigned int version);

// xml output
template void serialize< xml_oarchive, double, 12, 12, 1 >(
  xml_oarchive & ar,
  Eigen::Matrix< double, 12, 12, 1, 12, 12 > & matrix,
  const unsigned int version);
////////////////////////////////////////////////////////////////////////////////////////////////////



} // namespace serialization
} // namespace boost
#endif // Boost_SERIALIZATION_FOUND
