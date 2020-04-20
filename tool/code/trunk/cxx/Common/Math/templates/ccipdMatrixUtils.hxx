/********************************************************************
	Created:	Gohary 08/30/2013 15:05
	Updated:
	filename: 	..\Common\Math\template\ccipdMatrixUtils.hxx
	purpose:	This file just contains templated implementation of some simple Matrix functions which may be useful to other projects.
*********************************************************************/
 

//////////////////////////////////////////////////////////////////////////////////////////
//#include "ccipdMatrixUtils.h"
#include <iostream>
//////////////////////////////////////////////////////////////////////////////////////////

namespace ccipd
{
namespace MatrixUtils
{
  template<class ScalarType, int Rows, int Cols, int Options>
  Eigen::Matrix<ScalarType, Rows, Cols, Options, Rows, Cols> 
  GetCovariance(Eigen::Matrix<ScalarType, Rows, Cols, Options, Rows, Cols> & mat)
	{
		Eigen::Matrix<ScalarType, Rows, Cols, Options, Rows, Cols> centered = mat.rowwise() - mat.colwise().mean();
		return ((centered.transpose() * centered) / ((ScalarType)(mat.rows() - 1)));
	}

  template<class ScalarType, int Rows, int Cols, int Options>
  Eigen::Matrix<ScalarType, Rows, Cols, Options, Rows, Cols>
  GetUniqueValues(Eigen::Matrix<ScalarType, Rows, Cols, Options, Rows, Cols> vec)
  {
    unsigned int rows = (unsigned int) vec.rows();  
    unsigned int cols = (unsigned int) vec.cols(); 

     Eigen::Matrix<ScalarType, Rows, Cols, Options, Rows, Cols> outVec(rows * cols, 1/*cols*/);
    
     // Check the given vector is not empty
    if((rows == 0) || (cols == 0) || ((rows != 1) && (cols !=1)))
    {
      // Error
      //throw(Exception e);
      std::cout << "\n\nError: Input is either corrupted or not a vector ...";
      outVec.resize(0, 0);
      return outVec;
    }

    outVec(0, 0) = vec(0, 0);
    unsigned int uniqueCoeffs = 1;
    for (unsigned int i = 0; i < rows; i++)
    {
      for (unsigned int j = 0; j < cols; j++)
      {
            // Compare the current coefficient with all the previous ones
            // if found: ignore it, if not: add it to the output vector
            bool bFoundNew = true;
            for (unsigned int k = 0; k < uniqueCoeffs; k++)
            {
              if (outVec(k, 0) == vec(i, j)) //contained
              {
                bFoundNew = false;
                break;
              }
            }

            if (bFoundNew)
            {
              outVec(uniqueCoeffs++, 0) = vec(i, j);
            }
      }
    }

    return outVec.topRows(uniqueCoeffs);
  }


  template<class ScalarType, int Rows, int Cols, int Options>
  Eigen::Matrix<ScalarType, Rows, Cols, Options, Rows, Cols>
  GetSorted(Eigen::Matrix<ScalarType, Rows, Cols, Options, Rows, Cols> vec)
  {
    unsigned int rows = (unsigned int) vec.rows();  
    unsigned int cols = (unsigned int) vec.cols(); 

    Eigen::Matrix<ScalarType, Rows, Cols, Options, Rows, Cols> outVec = vec;
    // Check the given vector is not empty
    if((rows == 0) || (cols == 0) || ((rows != 1) && (cols !=1)))
    {
      // Error
      //throw(Exception e);
      std::cout << "\n\nError: Input is either corrupted or not a vector ...";
      outVec.resize(0, 0);
      return outVec;
    }

    if (cols == 1)
    {
      std::sort(outVec.data(), outVec.data() + outVec.rows());
    }
    else if (rows == 1)
    {
      std::sort(outVec.data(), outVec.data() + outVec.cols());
    }

    return outVec;
  }


    template<class ScalarType, int Rows, int Cols, int Options>
    ScalarType GetMedian(Eigen::Matrix<ScalarType, Rows, Cols, Options, Rows, Cols> vecmat)
    {
      // Convert the given matrix to a vector
      unsigned int cols = (unsigned int) vecmat.cols();
      unsigned int rows = (unsigned int) vecmat.rows();
      Eigen::Matrix<ScalarType, Rows, Cols, Options, Rows, Cols> vec(rows * cols, 1);
      for(unsigned int i = 0; i < cols; i++)
      {
        vec.middleRows(i * rows, rows) = vecmat.col(i);
      }

      vec = GetSorted(vec);
      if(0 == vec.size() % 2) // even number of elements, return the mean of the two elements in the middle
      {
        return (vec(vec.size() / 2, 0) + vec(vec.size() / 2 - 1, 0)) / 2;
      }
      else // odd number of elements, return the element in the middle
      {
        return vec(vec.size() / 2, 0);
      }
    }

    template<class ScalarType, int Rows, int Cols, int Options>
    Eigen::Matrix<ScalarType, Rows, Cols, Options, Rows, Cols>
    GetMedianRowwise(Eigen::Matrix<ScalarType, Rows, Cols, Options, Rows, Cols> mat)
    {
      unsigned int rows = (unsigned int) mat.rows();
      Eigen::Matrix<ScalarType, Rows, Cols, Options, Rows, Cols> median_vec(rows, 1);
      for(unsigned int i = 0; i < rows; i++)
      {
        median_vec(i, 0) = GetMedian<ScalarType, Rows, Cols, Options>(mat.row(i));
      }
      return median_vec;
    }

    template<class ScalarType, int Rows, int Cols, int Options>
    Eigen::Matrix<ScalarType, Rows, Cols, Options, Rows, Cols>
    GetMedianColwise(Eigen::Matrix<ScalarType, Rows, Cols, Options, Rows, Cols> mat)
    {
      unsigned int cols = (unsigned int) mat.cols();
      Eigen::Matrix<ScalarType, Rows, Cols, Options, Rows, Cols> median_vec(cols, 1);
      for(unsigned int i = 0; i < cols; i++)
      {
        median_vec(i, 0) = GetMedian<ScalarType, Rows, Cols, Options>(mat.col(i));
      }
      return median_vec;
    }



  template<class ScalarType, int Rows, int Cols, int Options>
  Eigen::Matrix<ScalarType, Rows, Cols, Options, Rows, Cols>
  WhitenMatrix(Eigen::Matrix<ScalarType, Rows, Cols, Options, Rows, Cols> data)
  {
    typedef Eigen::Matrix<ScalarType, Rows, Cols, Options, Rows, Cols> MatrixVectorType;

    // mean_vec = mean(data,1);
    MatrixVectorType mean_vec = data.colwise().mean();
    
    // temp = bsxfun(@minus,data,mean_vec);
    MatrixVectorType whiteneddata = data.rowwise() - mean_vec.row(0);

    ////mad_vec = mad(data, 1); Y is Median(ABS(X-Median(X))
    MatrixVectorType median_vec  = GetMedianColwise<ScalarType, Rows, Cols, Options>(data).transpose();
    MatrixVectorType temp_median = data.rowwise() - median_vec.row(0);
    MatrixVectorType mad_vec     = GetMedianColwise<ScalarType, Rows, Cols, Options>(temp_median.cwiseAbs()); 
   

    //// This aims to make sure that we will not divide by zero in the next step, 
    //// I just leave the number as it is (which means dividing by 1) if it is zero or less
    ////whiteneddata = bsxfun(@rdivide,temp_mean,mad_vec);
    for(int i = 0; i < whiteneddata.cols(); i++)
    {
      if(mad_vec(i, 0) > 0.0f)
      {
        whiteneddata.col(i) /= mad_vec(i, 0);
      }
    }

    return whiteneddata;
  }

  template<class ScalarType, int Rows, int Cols, int Options>
  bool
  WriteMatrixToFile(Eigen::Matrix<ScalarType, Rows, Cols, Options, Rows, Cols> matrix, std::string fileName)
  {
    std::ofstream file(fileName);
    
    if (!file.is_open()) 
	{
		std::cerr << "\nError while writing the matrix to " << fileName << "\n" << __FILE__ << "  " << __LINE__ << "\n";
		return false;
	}

    //Eigen::IOFormat OctaveFmt(Eigen::FullPrecision, 0, ",", ";\n", "", "", "[\n ", "\n]");
    Eigen::IOFormat OctaveFmt(Eigen::FullPrecision, 0, ",", ",\n", "", "", "", ""); // for matlab, loaded by importdata

    file << matrix.format(OctaveFmt) ;
  
    file.close();

    return true;
  }

template<class ScalarType, int Rows, int Cols, int Options>
bool
ReadMatrixFromFile(std::string fileName, Eigen::Matrix<ScalarType, Rows, Cols, Options, Rows, Cols> & matrix)
{
    std::ifstream file(fileName);
    if (!file.is_open()) 
	{
		std::cerr << "\nError while loading the matrix from " << fileName << "\n" << __FILE__ << "  " << __LINE__ << "\n";
		return false;
	}  
    std::vector<ScalarType> vec;
    std::string line;
    unsigned int rows = 0, cols = 0, cols_tmp;
    while (std::getline(file, line))
    {
      if(line.find(',') == std::string::npos ) continue;
      
      std::replace(line.begin(), line.end(), ';', ',');
      std::replace(line.begin(), line.end(), ' ', ',');

      std::string num;
      std::istringstream iss(line);
      cols_tmp = 0;
      while( std::getline(iss, num, ',' ))
      {
        ScalarType numb;
        try
        {
          numb = boost::lexical_cast<ScalarType>(num);
        }
        catch(boost::bad_lexical_cast &)
        {
          continue;
        }

        vec.push_back(numb);

        cols_tmp++;
      }

      if (0 != cols)
      {
        if(cols_tmp != cols) return false;
      }
      else 
      {
        cols = cols_tmp; // (0 == cols )
      }

      rows++;
    }
    
    // If it's a vector, then it's a colums vector, otherwise it will not work with our typedef'ed vectors because they are columnMajor
    if(1 == rows)
    {
      rows = cols;
      cols = 1;
    }

    // map to the matrix
    matrix = Map<Eigen::Matrix<ScalarType, Rows, Cols, Options, Rows, Cols>>(vec.data(), rows, cols);
    
    file.close();
    return true;
}

} // namespace
} // namespace
