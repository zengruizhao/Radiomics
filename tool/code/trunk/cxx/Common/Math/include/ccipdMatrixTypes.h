#ifndef __ccipdMatrixTypes_h
#define __ccipdMatrixTypes_h
//////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdCore.h"
#include "ccipdDisableWarningsMacro.h" 
#include <vector> // for containers
#include <memory> // for smart pointers
#include <Eigen/Dense>
#include <Eigen/SparseCore>
#include "ccipdEnableWarningsMacro.h"

//////////////////////////////////////////////////////////////////////////////////////////
using namespace Eigen;

namespace ccipd
{
//////////////////////////////////////////////////////////////////////////////////////////
// Matrix Types
typedef Matrix<float, Dynamic, Dynamic, RowMajor, Dynamic, Dynamic> EigenMatrixTypeF; // easily define a dynamic Eigen matrix (Float)

typedef Matrix<double, Dynamic, Dynamic, RowMajor, Dynamic, Dynamic> EigenMatrixTypeD; // easily define a dynamic Eigen matrix (Double)

typedef Matrix< int, Dynamic, Dynamic, RowMajor, Dynamic, Dynamic> EigenMatrixTypeI; // easily define a dynamic Eigen matrix (int)

typedef Matrix<unsigned int, Dynamic, Dynamic, RowMajor, Dynamic, Dynamic> EigenMatrixTypeU; // easily define a dynamic Eigen matrix (Unsigned Integer)


// Vector Types

typedef Matrix< float       , Dynamic, 1, ColMajor, Dynamic, 1> EigenVectorTypeF; // easily define a dynamic Eigen vector (Float)

typedef Matrix< double      , Dynamic, 1, ColMajor, Dynamic, 1> EigenVectorTypeD; // easily define a dynamic Eigen vector (double)

typedef Matrix< int         , Dynamic, 1, ColMajor, Dynamic, 1> EigenVectorTypeI; // easily define a dynamic Eigen vector (int)

typedef Matrix< unsigned int, Dynamic, 1, ColMajor, Dynamic, 1> EigenVectorTypeU; // easily define a dynamic Eigen vector (Unsigned integer)

typedef EigenVectorTypeU EigenLabelVector;

//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
// smart pointers
typedef std::shared_ptr<       EigenMatrixTypeF > EigenMatrixPointerF;
typedef std::shared_ptr< const EigenMatrixTypeF > EigenMatrixConstPointerF;
typedef std::shared_ptr<       EigenVectorTypeF > EigenVectorPointerF;
typedef std::shared_ptr< const EigenVectorTypeF > EigenVectorConstPointerF;

typedef std::shared_ptr<       EigenMatrixTypeD > EigenMatrixPointerD;
typedef std::shared_ptr< const EigenMatrixTypeD > EigenMatrixConstPointerD;
typedef std::shared_ptr<       EigenVectorTypeD > EigenVectorPointerD;
typedef std::shared_ptr< const EigenVectorTypeD > EigenVectorConstPointerD;



typedef std::shared_ptr<       EigenMatrixTypeU > EigenMatrixPointerU;
typedef std::shared_ptr< const EigenMatrixTypeU > EigenMatrixConstPointerU;
typedef std::shared_ptr<       EigenVectorTypeU > EigenVectorPointerU;
typedef std::shared_ptr< const EigenVectorTypeU > EigenVectorConstPointerU;
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
// containers
typedef std::vector< EigenMatrixPointerF      > EigenMatrixCollectionF;
typedef std::vector< EigenMatrixConstPointerF > EigenMatrixConstCollectionF;
typedef std::vector< EigenVectorPointerF      > EigenVectorCollectionF;
typedef std::vector< EigenVectorConstPointerF > EigenVectorConstCollectionF;

typedef std::vector< EigenMatrixPointerD      > EigenMatrixCollectionD;
typedef std::vector< EigenMatrixConstPointerD > EigenMatrixConstCollectionD;
typedef std::vector< EigenVectorPointerD      > EigenVectorCollectionD;
typedef std::vector< EigenVectorConstPointerD > EigenVectorConstCollectionD;


typedef std::vector< EigenMatrixPointerU      > EigenMatrixCollectionU;
typedef std::vector< EigenMatrixConstPointerU > EigenMatrixConstCollectionU;
typedef std::vector< EigenVectorPointerU      > EigenVectorCollectionU;
typedef std::vector< EigenVectorConstPointerU > EigenVectorConstCollectionU;
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace
#endif //__ccipdMatrixTypes_h