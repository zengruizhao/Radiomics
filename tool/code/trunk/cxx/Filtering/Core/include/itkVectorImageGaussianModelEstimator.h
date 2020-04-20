#ifndef __itkVectorImageGaussianModelEstimator_h
#define __itkVectorImageGaussianModelEstimator_h



//////////////////////////////////////////////////////////////////////////////////////////
#include <math.h>
#include <float.h>

#include <vnl/vnl_vector.h>
#include <vnl/vnl_matrix.h>
#include <vnl/vnl_matrix_fixed.h>
#include <vnl/vnl_math.h>
#include <vnl/algo/vnl_matrix_inverse.h>

#include "ccipdDisableWarningsMacro.h"
  #include <itkMacro.h>
  #include <itkImageModelEstimatorBase.h>
#include "ccipdEnableWarningsMacro.h"
//////////////////////////////////////////////////////////////////////////////////////////



namespace itk
{
/** \class ImageGaussianModelEstimator
 *  \brief Base class for ImageGaussianModelEstimator object.
 *
 * itkVectorImageGaussianModelEstimator generates the Gaussian model for given
 * tissue types (or class types) in an input training data set for
 * segmentation. The training data set is typically provided as a set of
 * labelled/classified data set by the user. A Gaussian model is generated
 * for each label present in the training data set.
 *
 * The user should ensure that both the input and training images
 * are of the same size. The input data consists of the raw data and the
 * training data has class labels associated with each pixel.
 *
 * A zero label is used to identify the background. A model is not
 * calculated for the background (its mean and covariance will be
 * zero). Positive labels are classes for which models will be
 * estimated. Negative labels indicate unlabeled data where no models
 * will be estimated.
 *
 * This object supports data handling of multiband images. The object
 * accepts the input image in vector format only, where each pixel is a
 * vector and each element of the vector corresponds to an entry from
 * 1 particular band of a multiband dataset. A single band image is treated
 * as a vector image with a single element for every vector. The classified
 * image is treated as a single band scalar image.
 *
 * This function is templated over the type of input and output images. In
 * addition, a third parameter for the MembershipFunction needs to be
 * specified. In this case a Membership function that stores Gaussian models
 * needs to be specified.
 *
 * The function EstimateModels() calculates the various models, creates the
 * membership function objects and populates them.
 *
 * \author  Rachel Sparks
 * \ingroup Filtering
 */
template< class TInputImage,
          class TMembershipFunction,
          class TTrainingImage >
class ITK_EXPORT VectorImageGaussianModelEstimator:
  public ImageModelEstimatorBase< TInputImage, TMembershipFunction >
{



public:
//////////////////////////////////////////////////////////////////////////////////////////
  /** Standard class typedefs. */
  typedef VectorImageGaussianModelEstimator                           Self;
  typedef ImageModelEstimatorBase< TInputImage, TMembershipFunction > Superclass;
  typedef SmartPointer< Self >                                        Pointer;
  typedef SmartPointer< const Self >                                  ConstPointer;

  /** Type definition for the input image. */
  typedef          TInputImage                           InputImageType;
  typedef typename InputImageType::SizeType              InputImageSizeType;
  typedef typename InputImageType::Pointer               InputImagePointer;
  typedef typename InputImageType::PixelType             InputImagePixelType;
  typedef typename std::vector< InputImagePixelType >    InputImagePixelContainer;
  
  /** Type definitions for the training image. */
  typedef          TTrainingImage                        TrainingImageType;
  typedef typename TrainingImageType::Pointer            TrainingImagePointer;
  typedef typename TrainingImageType::PixelType          TrainingImagePixelType;
  typedef typename std::vector< TrainingImagePixelType > TrainingImagePixelContainer;


  /** Type definitions for the membership function . */
  typedef TMembershipFunction                   MembershipFunctionType;
  typedef typename TMembershipFunction::Pointer MembershipFunctionPointer;
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(VectorImageGaussianModelEstimator, ImageModelEstimatorBase);

  /** Set the training image. */
  itkSetMacro(TrainingImage, TrainingImagePointer);

  /** Get the training image. */
  itkGetConstMacro(TrainingImage, TrainingImagePointer);
  
  /**Set the number of samples used to generate the model*/
  itkSetMacro      ( NumberOfModelSamples, SizeValueType );

  /**Get the number of samples used to generate the model*/
  itkGetConstMacro ( NumberOfModelSamples, SizeValueType );

  /**Boolean to contruct model using all samples*/
  itkSetMacro(UseAllPixels, bool );
  itkGetConstReferenceMacro(UseAllPixels, bool);
//////////////////////////////////////////////////////////////////////////////////////////



protected:
//////////////////////////////////////////////////////////////////////////////////////////
  VectorImageGaussianModelEstimator(); //Constructor
  ~VectorImageGaussianModelEstimator(); //Destructor
  virtual void PrintSelf(std::ostream & os, Indent indent) const;

  /** Starts the image modeling process */
  void GenerateData();

private:
  VectorImageGaussianModelEstimator(const Self &); //purposely not implemented
  void operator=(const Self &);              //purposely not implemented
  
  /** Check to make sure the vector contains no NaN or Inf vaules */
  bool IsFinite( InputImagePixelType pixel ); 

  TrainingImagePointer m_TrainingImage;

  //for subsampling the images
  bool                        m_UseAllPixels;
  SizeValueType               m_NumberOfModelSamples;
  InputImagePixelContainer    m_InputImageSamples;
  TrainingImagePixelContainer m_TrainingImageSamples;
  
  //for creating the model
  typedef vnl_matrix< double > MatrixType;
  unsigned int                m_VectorDimension;  
  MatrixType                  m_NumberOfSamples;
  MatrixType                  m_Means;
  MatrixType                  * m_Covariance;
  
  // Generates the model based on the training input data.
  // Achieves the goal of training the classifier.
  virtual void EstimateModels();

  // Helper function to estimate the model parameters
  virtual void EstimateGaussianModelParameters();

  // Helper function to subsample the training and input image
  // with a random iterator
  virtual void SampleImageRegion();
  
  // Helper function to sample the full training and input image
  virtual void SampleFullImageRegion();

}; // class VectorImageGaussianModelEstimator
} // namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkVectorImageGaussianModelEstimator.hxx"
#endif

#endif
