
//////////////////////////////////////////////////////////////////////////////////////////
// ccipd includes
#include "ccipdCore.h"
#include "ccipdMatrixUtils.h"
#include "ccipdDisableWarningsMacro.h"

// std includes
#include <iostream> // for cout
#include "ccipdEnableWarningsMacro.h"

#ifdef _MSC_VER
#pragma warning(disable: 4714) // AA Should be fixed
#endif // _MSC_VER
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
// using shortcuts
using namespace std;
using namespace ccipd;
//////////////////////////////////////////////////////////////////////////////////////////

int main()
{
  // Test Covariance
  EigenMatrixTypeF mat1(2, 2);
  mat1 << 1, 9, 2, 55;
  EigenMatrixTypeF mat2 = MatrixUtils::GetCovariance(mat1);
  cout << "\n" << mat2<< "\n";

  // Test GetUniqueValues
  EigenMatrixTypeF vec1(6, 1);
  vec1 << 1, 2, 2, 3.5, 3.5, 7;
  EigenMatrixTypeF vec2 = MatrixUtils::GetUniqueValues(vec1);
  cout << "\n" << vec2<< "\n";

  // Test Sort 1
  EigenVectorTypeU vec3(5);
  //Eigen::VectorXf vec3(5);
  vec3 << 1, 9, 2, 55, 0;
  EigenVectorTypeU vec4 = MatrixUtils::GetSorted(vec3);
  cout << "\n" << vec4 << "\n";

  // Test GetMedian 1
  EigenVectorTypeF vec5(7);
  vec5 << 1, 9, 2, 55, 0, 22, 45;
  float medianValf = MatrixUtils::GetMedian(vec5);
  cout << "\n" << medianValf << "\n";

    // Test GetMedian 2
  EigenMatrixTypeD mat6(2, 3);
  mat6 << 1, 9, 2, 55, 0, -2;
  double medianVald = MatrixUtils::GetMedian(mat6);
  cout << "\n" << medianVald<< "\n";

  //// Test GetMedianRowwise 1
  EigenMatrixTypeD mat7(3, 5);
  mat7 << 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15;
  EigenVectorTypeD median_vec8 = MatrixUtils::GetMedianRowwise(mat7);
  cout << "\n" << median_vec8 << "\n"; 

  //// Test GetMedianColwise
  //EigenMatrixTypeD mat7(3, 5);
  //mat7 << 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15;
  EigenVectorTypeD median_vec9 = MatrixUtils::GetMedianColwise(mat7);
  cout << "\n" << median_vec9 << "\n"; 

  // Test Whitening Data 1
    EigenMatrixTypeD data1(3, 4);
    data1 << 1, 1.5, 2.77, 7, 9, 1, 2, 0, 23, 45, 33, 0.001;
    cout << "\n" << data1<< "\n";
    EigenMatrixTypeD whiteneddata1 = MatrixUtils::WhitenMatrix(data1);
    cout << "\n" << whiteneddata1<< "\n";

      // Test Whitening Data 2
    EigenMatrixTypeF data2(3, 4);
    data2 << 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12;
    cout << "\n" << data2<< "\n";
    EigenMatrixTypeF whiteneddata2 = MatrixUtils::WhitenMatrix(data2);
    cout << "\n" << whiteneddata2<< "\n";


    // Test Writing a Matrix to File
    MatrixUtils::WriteMatrixToFile(whiteneddata2, "Matrix.txt");


    // Test Reading a Matrix from Text File
    EigenMatrixTypeF mat4(3, 7); // Input dimensions are ignored, and new dimensions are acquired from the file
    MatrixUtils::ReadMatrixFromFile("Matrix.txt", mat4);

    cout << "\n" << mat4<< "\n";


    return EXIT_SUCCESS;

} // main