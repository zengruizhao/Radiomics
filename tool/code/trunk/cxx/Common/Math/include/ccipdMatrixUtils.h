/********************************************************************
	Created:	Gohary 5/20/2013 12:54
	Updated:
	filename: 	..\Common\Math\include\ccipdMatrixUtils.h
	purpose:	This file just contains headers of some simple Matrix functions which may be useful to other projects.
*********************************************************************/

#ifndef __ccipdMatrixUtils_h
#define __ccipdMatrixUtils_h


#include "ccipdDisableWarningsMacro.h"
#include <fstream>
#include <string>
#include <vector>
//#include <deque>

#include <boost/lexical_cast.hpp>

#include "ccipdEnableWarningsMacro.h"

#include "ccipdMatrixTypes.h"
#include "ccipdMatrixUtils.hxx"


//////////////////////////////////////////////////////////////////////////////////////////

using namespace Eigen;
using namespace std;


namespace ccipd
{

namespace MatrixUtils
{
	/*
		brief     : Calculates the covariance matrix of a given matrix.
		Created   : Gohary 5/20/2013 13:00
		Updated   : Gohary 8/30/2013 20:10
		Param[in] : mat: the given data matrix.
		Param[out]:
		Return    : MatrixXd: The covariance matrix
		Notes     : Find further details on the corresponding source file
	 */
  template<class ScalarType, int Rows, int Cols, int Options>
  Matrix<ScalarType, Rows, Cols, Options, Rows, Cols> 
  GetCovariance(Matrix<ScalarType, Rows, Cols, Options, Rows, Cols> & mat);

    /** \brief  : get the unique coefficient values in a given vector 
      * \Created: axa667 6/26/2013
      * \Updated:  
      * \ingroup: MatrixUtils
      * \param  : vec: The given vector
      * \param  : 
      * \return : EigenLabelVector: vector of unique coefficients contained in the given one 
      * \notes  : The length of the returned vector is always less than or equal to that of the given one 
      * 
      * 
      * 
      */
     template<class ScalarType, int Rows, int Cols, int Options>
     Eigen::Matrix<ScalarType, Rows, Cols, Options, Rows, Cols>
     GetUniqueValues(Eigen::Matrix<ScalarType, Rows, Cols, Options, Rows, Cols> vec);


      template<class ScalarType, int Rows, int Cols, int Options>
      Eigen::Matrix<ScalarType, Rows, Cols, Options, Rows, Cols>
      GetSorted(Eigen::Matrix<ScalarType, Rows, Cols, Options, Rows, Cols> vec);


    /** \brief  : Whitening the data: Mean = 0, MAD = 1 on a per-feature basis 
      * \Created: axa667 8/28/2013
      * \Updated:  
      * \ingroup: MatrixUtils
      * \param  : data: The given N x D matrix
      * \param  : 
      * \return : EigenMatrixTypeF: The whitened matrix 
      * \notes  : Accounts for cases where MAD = 0 too 
      *           MAD: Mean or median absolute deviation
      */
    template<class ScalarType, int Rows, int Cols, int Options>
    Eigen::Matrix<ScalarType, Rows, Cols, Options, Rows, Cols>
    WhitenMatrix(Eigen::Matrix<ScalarType, Rows, Cols, Options, Rows, Cols> data);


    /** \brief  : Calculates and returns the median of a givenvector 
      * \Created: Ahmad 8/28/2013
      * \Updated:  
      * \ingroup: MatrixUtils
      * \param  : vec: The given N x 1 vector
      * \param  : 
      * \return :  
      * \notes  : In case of even number of coefficients in the given vector, the mean of the two elements in the middle is returned
      *           example: double MatrixUtils::GetMedian(EigenVectorTypeD vec);
      */
    template<class ScalarType, int Rows, int Cols, int Options>
    ScalarType GetMedian(Eigen::Matrix<ScalarType, Rows, Cols, Options, Rows, Cols> vecmat);
   
    template<class ScalarType, int Rows, int Cols, int Options>
    Eigen::Matrix<ScalarType, Rows, Cols, Options, Rows, Cols>
    GetMedianRowwise(Eigen::Matrix<ScalarType, Rows, Cols, Options, Rows, Cols> mat);
   
    template<class ScalarType, int Rows, int Cols, int Options>
    Eigen::Matrix<ScalarType, Rows, Cols, Options, Rows, Cols>
    GetMedianColwise(Eigen::Matrix<ScalarType, Rows, Cols, Options, Rows, Cols> mat);

    template<class ScalarType, int Rows, int Cols, int Options>
    bool
    WriteMatrixToFile(Eigen::Matrix<ScalarType, Rows, Cols, Options, Rows, Cols> matrix, std::string fileName);

    template<class ScalarType, int Rows, int Cols, int Options>
    bool
    ReadMatrixFromFile(std::string fileName, Eigen::Matrix<ScalarType, Rows, Cols, Options, Rows, Cols> & matrix);
    



} // namespace

} // namespace
#endif // __ccipdMatrixUtils_h