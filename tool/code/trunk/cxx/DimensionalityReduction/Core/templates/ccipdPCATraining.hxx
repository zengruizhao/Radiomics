#ifndef __ccipdPCATraining_hxx
#define __ccipdPCATraining_hxx



//////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdPCATraining.h"

#include <iostream> // for cout, etc.

#include "ccipdDisableWarningsMacro.h"
  #include "Eigen/Core"        // for matrices and vectors
  #include "Eigen/Eigenvalues" // for eigen value decomposition
#include "ccipdEnableWarningsMacro.h"
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////
template< class TValueType >
std::shared_ptr< Eigen::Matrix< TValueType, -1, -1, Eigen::RowMajor, -1, -1 > >
CalculateEigenvectorsOfCovariance(
  const Eigen::Matrix< TValueType, -1, -1, Eigen::RowMajor, -1, -1 > & covarianceMatrix,
        Eigen::Matrix< TValueType, -1,  1, Eigen::ColMajor, -1,  1 > & eigenvalues,
  const float                                            varianceToRetain,
  const bool                                             verbose )
{

  using std::cerr;
  using std::cout;
  using std::endl;

  typedef Eigen::Matrix< TValueType, -1, -1, Eigen::RowMajor, -1, -1 > TemplatedMatrixType;
  typedef Eigen::Matrix< TValueType, -1,  1, Eigen::ColMajor, -1,  1 > TemplatedVectorType;
  typedef typename TemplatedVectorType::Index            IndexType;
  typedef std::shared_ptr< TemplatedMatrixType >         TemplatedMatrixPointer;

  // how much variance will we keep?
  const float actualVarianceToRetain = 
    ( varianceToRetain > 0.0f && varianceToRetain <= 1.0f ) ?
    varianceToRetain : // you've input a valid value
    1.0f;              // invalid input; use the default value

  // check to make sure we're square
  if ( covarianceMatrix.rows() != covarianceMatrix.cols() )
    {
    cerr << "Error in line " << __LINE__ << " of file " <<
      __FILE__ << ": covarianceMatrix is not square!" << endl;
    return TemplatedMatrixPointer();
    }

  // solve for the eigenvalues
  typedef Eigen::SelfAdjointEigenSolver< EigenMatrixTypeF > EigenSolverType;

  EigenSolverType eigenSolver( covarianceMatrix, Eigen::ComputeEigenvectors );

  // we want to output them in decreasing order
  const TemplatedMatrixType eigenvectors =
    eigenSolver.eigenvectors().rowwise().reverse();
  eigenvalues = eigenSolver.eigenvalues ().reverse();

  if ( verbose )
    {
    cout << "Calculating eigenvectors for the linked matrix done." << endl;
    cout << "Eigenvectors = " << endl << 
      eigenvectors << endl << endl;
    cout << "Eigenvalues = " << endl << 
      eigenvalues << endl << endl;
    }

  // only retain some of them
  const float
    totalVariance = static_cast< float >( eigenvalues.sum() ),
    varianceToRetainAsPercent = actualVarianceToRetain,
    minimumVariance = totalVariance * varianceToRetainAsPercent;
  
  IndexType eigenvaluesToRetain = 0;
  const IndexType numberOfEigenvalues = eigenvalues.size();
  float currentVariance = 0;
  for ( ; eigenvaluesToRetain < eigenvalues.size(); ++eigenvaluesToRetain )
    {

    // make sure we have a positive eigenvalue
    const TValueType eigenvalue = eigenvalues[ eigenvaluesToRetain ];
    if ( eigenvalue <= 0 ) break;

    // how much variance do we have now?
    currentVariance += eigenvalue;

    // see if we've retained enough variance
    if ( currentVariance >= minimumVariance )
      break;

    } // for eigenvaluesToRetain

  ++eigenvaluesToRetain;
  if ( eigenvaluesToRetain > numberOfEigenvalues )
    eigenvaluesToRetain = numberOfEigenvalues;

  // now actually extract the desired eigenvectors
  const TemplatedMatrixPointer trimmedEigenvectors =
    TemplatedMatrixPointer( new TemplatedMatrixType(
      eigenvectors.leftCols( eigenvaluesToRetain ) ) );

  // gcc makes a mistake if you do:
  // eigenvalues = eigenvalues.head( eigenvaluesToRetain );
  // so we have to do this in 2 parts
  const TemplatedVectorType trimmedEigenvalues =
    eigenvalues.head( eigenvaluesToRetain );
  eigenvalues = trimmedEigenvalues;

  if ( verbose )
    {
    cout << "Total variance = "             << totalVariance       << endl;
    cout << "Minimum variance to retain = " << minimumVariance     << endl;
    cout << "Variance retained = "          << currentVariance     << endl;
    cout << "# of eigenvectors retained = " << eigenvaluesToRetain << endl;

    cout << "Eigenvalues retained = " << endl <<
      trimmedEigenvalues << endl;

    cout << "Eigenvectors retained = " << endl <<
      *trimmedEigenvectors << endl;

    } // verbose

  // output the first few eigenvectors
  return trimmedEigenvectors;

} // CalculateEigenvectorsOfCovariance
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
template< class TValueType >
std::shared_ptr< Eigen::Matrix< TValueType, -1, -1, Eigen::RowMajor, -1, -1 > >
CalculateEigenvectorsOfCovariance(
  const Eigen::Matrix<
    TValueType, -1, -1, Eigen::RowMajor, -1, -1 > & covarianceMatrix,
  const float                         varianceToRetain,
  const bool                          verbose )
{

  // just create a temporary variable
  typedef Eigen::Matrix< TValueType, -1,  1, Eigen::ColMajor, -1,  1 > TemplatedVectorType;
  TemplatedVectorType eigenvalues;

  return CalculateEigenvectorsOfCovariance(
    covarianceMatrix, eigenvalues, varianceToRetain, verbose );

} // CalculateEigenvectorsOfCovariance
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace
#endif // __ccipdPCATraining_hxx
