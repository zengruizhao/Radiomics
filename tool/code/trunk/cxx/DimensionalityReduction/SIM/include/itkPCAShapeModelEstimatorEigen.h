#ifndef __itkPCAShapeModelEstimatorEigen_h
#define __itkPCAShapeModelEstimatorEigen_h



//////////////////////////////////////////////////////////////////////////////////////////

// ccipd includes
#include "ccipdMacros.h"
#include "ccipdMatrixTypes.h"

#include "ccipdDisableWarningsMacro.h"
  // itk include
  #include <itkImagePCAShapeModelEstimator.h>
#include "ccipdEnableWarningsMacro.h"

// std includes
#include <memory>
//////////////////////////////////////////////////////////////////////////////////////////



namespace itk
{



template< class TInputImage,
          class TOutputImage = Image< double, TInputImage::ImageDimension > >
class ITK_EXPORT PCAShapeModelEstimatorEigen:
  public ImagePCAShapeModelEstimator< TInputImage, TOutputImage >
{



public:
//////////////////////////////////////////////////////////////////////////////////////////
  /** Standard class typedefs. */
  typedef PCAShapeModelEstimatorEigen                              Self;
  typedef ImagePCAShapeModelEstimator< TInputImage, TOutputImage > Superclass;
  typedef SmartPointer<       Self >                               Pointer;
  typedef SmartPointer< const Self >                               ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro( Self );

  /** Run-time type information (and related methods). */
  itkTypeMacro( PCAShapeModelEstimatorEigen, ImagePCAShapeModelEstimator );
//////////////////////////////////////////////////////////////////////////////////////////



public:
////////////////////////////////////////////////////////////////////////////////////////////////////

  typedef typename Superclass::OutputImagePixelType OutputImagePixelType;
  
  typedef Eigen::Matrix<
    OutputImagePixelType,
    -1,  1, Eigen::ColMajor, -1,  1
  > OutputEigenVectorType;
  
  typedef OutputEigenVectorType ProjectionType;
  
  typedef std::shared_ptr<
    ProjectionType
  > ProjectionPointer;
  
  /// After running this filter, you can extract the projection.
  ProjectionPointer GetProjection( const unsigned int trainingImageIndex ) const;

  virtual void SetVerbose( const bool verbose );
  
////////////////////////////////////////////////////////////////////////////////////////////////////



protected:
////////////////////////////////////////////////////////////////////////////////////////////////////
           PCAShapeModelEstimatorEigen(); ///< Constructor
  virtual ~PCAShapeModelEstimatorEigen(); ///< Destructor
  
  /// Calculate the shape models and fill the output images.
  virtual void GenerateData();
//////////////////////////////////////////////////////////////////////////////////////////



private:
//////////////////////////////////////////////////////////////////////////////////////////
  /// We are overriding this function to use the Eigen library
  virtual void EstimateShapeModels();
//////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////
  ccipdPIMPLMacro
////////////////////////////////////////////////////////////////////////////////////////////////////

  
  
}; // class
} // namespace itk
#endif // __itkPCAShapeModelEstimatorEigen_h
