#ifndef __itkVectorImageGaussianModelEstimator_hxx
#define __itkVectorImageGaussianModelEstimator_hxx



// implementation similar to itkImageGaussianModelEstimator
////////////////////////////////////////////////////////////////////////////////////////////////////
// our own itk includes
#include "itkVectorImageGaussianModelEstimator.h"

#include "ccipdDisableWarningsMacro.h"
  // itk includes
  #include <itkImageRandomConstIteratorWithIndex.h>
  #include <itkImageRegionConstIterator.h>
#include "ccipdEnableWarningsMacro.h"
////////////////////////////////////////////////////////////////////////////////////////////////////



namespace itk
{
// Constructor
template< class TInputImage,
          class TMembershipFunction,
          class TTrainingImage >
VectorImageGaussianModelEstimator< TInputImage, TMembershipFunction, TTrainingImage >
::VectorImageGaussianModelEstimator(void)
{
	m_UseAllPixels = false;
	m_NumberOfModelSamples = 5000;
	m_Covariance = NULL;
} 

// Desctructor
template< class TInputImage,
          class TMembershipFunction,
          class TTrainingImage >
VectorImageGaussianModelEstimator< TInputImage, TMembershipFunction, TTrainingImage >
::~VectorImageGaussianModelEstimator(void)
{
  if ( m_Covariance ) { delete[] m_Covariance; }
}

// Print Self
template< class TInputImage,
          class TMembershipFunction,
          class TTrainingImage >
void
VectorImageGaussianModelEstimator< TInputImage, TMembershipFunction, TTrainingImage >
::PrintSelf(std::ostream & os, Indent indent) const
{
  os << indent << "                   " << std::endl;
  os << indent << "Gaussian Models generated from the training data." << std::endl;
  os << indent << "TrainingImage: ";
  os << m_TrainingImage.GetPointer() << std::endl;
  os << indent << "Results printed in the superclass " << std::endl;
  os << indent << "                   " << std::endl;

  Superclass::PrintSelf(os, indent);
} // end PrintSelf

/**
 * Generate data (start the model building process)
 */
template< class TInputImage,
          class TMembershipFunction,
          class TTrainingImage >
void
VectorImageGaussianModelEstimator< TInputImage, TMembershipFunction, TTrainingImage >
::GenerateData()
{
  this->EstimateModels();
} // end Generate data

// Takes a set of training images and returns the means
// and variance of the various classes defined in the
// training set.
template< class TInputImage,
          class TMembershipFunction,
          class TTrainingImage >
void
VectorImageGaussianModelEstimator< TInputImage, TMembershipFunction, TTrainingImage >
::EstimateModels()
{
  //Do some error checking
  InputImagePointer inputImage = this->GetInputImage();

  const unsigned int inputDim = InputImageType::ImageDimension;
  
  const unsigned int trainingDim = TTrainingImage::ImageDimension;
  // Check if the training and input image dimensions are the same
  static_assert( inputDim == trainingDim, 
    "Traning and input image dimensions are not the same" );

  InputImageSizeType
  inputImageSize = inputImage->GetBufferedRegion().GetSize();

  typedef InputImageSizeType TrainingImageSizeType;

  TrainingImagePointer trainingImage = this->GetTrainingImage();

  TrainingImageSizeType
  trainingImageSize = trainingImage->GetBufferedRegion().GetSize();

  // Check if size of the two inputs are the same
  for ( unsigned int i = 0; i < TInputImage::ImageDimension; i++ )
    {
    if ( inputImageSize[i] != trainingImageSize[i] ) { throw ExceptionObject(
                                                               __FILE__,
                                                               __LINE__,
                                                               "Input image size is not the same as the training image size.",
                                                               ITK_LOCATION); }
    }



  //-------------------------------------------------------------------
  // Set up the gaussian membership calculators
  //-------------------------------------------------------------------

  unsigned int numberOfModels = this->GetNumberOfModels();

  //-------------------------------------------------------------------
  // Call local function to estimate mean variances of the various
  // class labels in the training set.
  // The statistics class functions have not been used since all the
  // class statistics are calculated simultaneously here.
  //-------------------------------------------------------------------
  this->EstimateGaussianModelParameters();

  //-------------------------------------------------------------------
  // Populate the membership functions for all the classes
  //-------------------------------------------------------------------
  MembershipFunctionPointer membershipFunction;
  typename MembershipFunctionType::MeanVectorType tmean;
  typename MembershipFunctionType::CovarianceMatrixType tcov;

  NumericTraits<typename MembershipFunctionType::MeanVectorType>::SetLength(tmean, m_VectorDimension);
  for ( unsigned int classIndex = 0; classIndex < numberOfModels; classIndex++ )
    {
    membershipFunction = TMembershipFunction::New();

    // Convert to the datatype used for the mean
    for (unsigned int i=0; i < m_VectorDimension; ++i)
      {
      tmean[i] = m_Means.get(classIndex, i);
      }
    membershipFunction->SetMean( tmean );

    tcov = m_Covariance[classIndex]; // convert cov for membership fn
    membershipFunction->SetCovariance(tcov);

    this->AddMembershipFunction(membershipFunction);
    }
} // end train classifier

template< class TInputImage,
          class TMembershipFunction,
          class TTrainingImage >
void
VectorImageGaussianModelEstimator< TInputImage, TMembershipFunction, TTrainingImage >
::EstimateGaussianModelParameters()
{
  //-------------------------------------------------------------------

  unsigned int numberOfModels = ( this->GetNumberOfModels() );

  //-------------------------------------------------------------------
  // Set up the matrices to hold the means and the covariance for the
  // training data
  m_VectorDimension=this->GetInputImage()->GetVectorLength();

  m_Means.set_size(numberOfModels, m_VectorDimension);
  m_Means.fill(0);

  m_NumberOfSamples.set_size(numberOfModels, 1);
  m_NumberOfSamples.fill(0);

  // delete previous allocation first
  if ( m_Covariance ) { delete[] m_Covariance; }
  
  //Number of covariance matrices are equal to the number of classes
  m_Covariance = (MatrixType *)new MatrixType[numberOfModels];

  for ( unsigned int i = 0; i < numberOfModels; i++ )
    {
    m_Covariance[i].set_size(m_VectorDimension, m_VectorDimension);
    m_Covariance[i].fill(0);
    }
  
  //Populate the containers appropraitely
  if (this->GetUseAllPixels())
    {
    this->SampleFullImageRegion();
    }
  else
    {
    this->SampleImageRegion();
    }

  typename InputImagePixelContainer::iterator imageSamples;
  imageSamples = m_InputImageSamples.begin();

  typename InputImagePixelContainer::const_iterator end;
  end = m_InputImageSamples.cend();

  typename TrainingImagePixelContainer::iterator trainingSamples;
  trainingSamples = m_TrainingImageSamples.begin();
  
  for (;imageSamples!=end; ++imageSamples, ++trainingSamples )
    {

    unsigned int classIndex = (unsigned int)* trainingSamples;

    // Training data assumed = 1 band; also the class indices go
    // from 1, 2, ..., n while the corresponding memory goes from
    // 0, 1, ..., n-1.

    //Ensure that the training data is labelled appropriately
    if ( classIndex > numberOfModels )
      {
      throw ExceptionObject(__FILE__, __LINE__);
      }

    if ( classIndex > 0 )
      {
      m_NumberOfSamples[classIndex-1][0] += 1;
      InputImagePixelType inImgVec = * imageSamples;

      for ( unsigned int band_x = 0; band_x < m_VectorDimension; band_x++ )
        {
        m_Means[classIndex-1][band_x] += inImgVec[band_x];
        for ( unsigned int band_y = 0; band_y <= band_x; band_y++ )
          {
          m_Covariance[classIndex-1][band_x][band_y] += inImgVec[band_x] * inImgVec[band_y];
          }
        }
      }
    } // end for

  //Loop through the classes to calculate the means and covariance
  for ( unsigned int classIndex = 0; classIndex < numberOfModels; classIndex++ )
    {
    if ( m_NumberOfSamples[classIndex][0] != 0 )
      {
      for ( unsigned int i = 0; i < m_VectorDimension; i++ )
        {
        m_Means[classIndex][i] /= m_NumberOfSamples[classIndex][0];
        }
      } // end if

    else
      {
      for ( unsigned int i = 0; i < m_VectorDimension; i++ )
        {
        m_Means[classIndex][i] = 0;
        }
      } // end else

    if ( ( m_NumberOfSamples[classIndex][0] - 1 ) != 0 )
      {
      for ( unsigned int band_x = 0; band_x < m_VectorDimension; band_x++ )
        {
        for ( unsigned int band_y = 0; band_y <= band_x; band_y++ )
          {
          m_Covariance[classIndex][band_x][band_y] /=
            ( m_NumberOfSamples[classIndex][0] - 1 );
          } // end for band_y loop
        }   // end for band_x loop
      }     // end if

    else
      {
      for ( unsigned int band_x = 0; band_x < m_VectorDimension; band_x++ )
        {
        for ( unsigned int band_y = 0; band_y <= band_x; band_y++ )
          {
          m_Covariance[classIndex][band_x][band_y] = 0;
          }
        }
      } // end else

    MatrixType tempMeanSq;
    tempMeanSq.set_size(m_VectorDimension, m_VectorDimension);
    tempMeanSq.fill(0);

    for ( unsigned int band_x = 0; band_x < m_VectorDimension; band_x++ )
      {
      for ( unsigned int band_y = 0; band_y <= band_x; band_y++ )
        {
        tempMeanSq[band_x][band_y] =
          m_Means[classIndex][band_x] * m_Means[classIndex][band_y];
        }
      } // end for band_x loop

    if ( ( m_NumberOfSamples[classIndex][0] - 1 ) != 0 )
      {
      tempMeanSq *= ( m_NumberOfSamples[classIndex][0]
                      / ( m_NumberOfSamples[classIndex][0] - 1 ) );
      }
    m_Covariance[classIndex] -= tempMeanSq;

    // Fill the rest of the covairance matrix and make it symmetric
    if ( m_NumberOfSamples[classIndex][0] > 0 )
      {
      unsigned int lastInX = (unsigned int)( m_VectorDimension - 1 );
      unsigned int upperY = (unsigned int) m_VectorDimension;
      for ( unsigned int band_x = 0; band_x < lastInX; band_x++ )
        {
        for ( unsigned int band_y = band_x + 1; band_y < upperY; band_y++ )
          {
          m_Covariance[classIndex][band_x][band_y] =
            m_Covariance[classIndex][band_y][band_x];
          } // end band_y loop
        }   // end band_x loop
      }     // end if loop
    }       // end class index loop
}           // end EstimateGaussianModelParameters
////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////
//Function to populate SampleImageContainers with a set number of pixels
template< class TInputImage,
          class TMembershipFunction,
          class TTrainingImage >
void
VectorImageGaussianModelEstimator< TInputImage, TMembershipFunction, TTrainingImage >
::SampleImageRegion()
{
 
 InputImagePointer inputImage = this->GetInputImage();
 TrainingImagePointer trainingImage = this->GetTrainingImage();

  // Set up a random interator within the user specified input image region.
 // note that this will not retrieve pixels according to index
  typedef ImageRandomConstIteratorWithIndex< InputImageType > InputImageRandomIterator;
  InputImageRandomIterator inIt( inputImage, inputImage->GetBufferedRegion() );
  inIt.SetNumberOfSamples(m_NumberOfModelSamples);
  inIt.GoToBegin();

  m_InputImageSamples.resize( this->GetNumberOfModelSamples() );
  typename InputImagePixelContainer::iterator imSamples;
  imSamples = m_InputImageSamples.begin();
  
  m_TrainingImageSamples.resize( this->GetNumberOfModelSamples() );
  typename TrainingImagePixelContainer::iterator trSamples;
  trSamples = m_TrainingImageSamples.begin();

  while( !inIt.IsAtEnd() )
    {
    
    while ( !this->IsFinite( inIt.Get() ) ) // make sure the pixel is an actual value
      {
      ++inIt;

      if( inIt.IsAtEnd() ) // if we have no more pixels return
        continue;
      } // while ( !this->IsFinite( *imSamples ) ) 

    //get the class value for the pixel
	*(imSamples) = inIt.Get();

	//to get the appropriate trSamples we need to first get the image index
	typename InputImageType::IndexType sampleIndex = inIt.GetIndex();
	typename InputImageType::PointType samplePoint;

	//transform it to a point and then to a training index
	inputImage->TransformIndexToPhysicalPoint(sampleIndex, samplePoint);
		
	typename TrainingImageType::IndexType trainingIndex;
	trainingImage->TransformPhysicalPointToIndex(samplePoint, trainingIndex);

    // get the training sample and add it to the container
	*(trSamples) = m_TrainingImage->GetPixel(trainingIndex);

    ++imSamples;
    ++trSamples;
    ++inIt;
    }// while( imSamples!=imEnd )
}//SampleImageRegion
////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////
//Function to populate SampleImageContainers will all pixel in the image
template< class TInputImage,
          class TMembershipFunction,
          class TTrainingImage >
void
VectorImageGaussianModelEstimator< TInputImage, TMembershipFunction, TTrainingImage >
::SampleFullImageRegion()
{
	
  InputImagePointer inputImage = this->GetInputImage();
  TrainingImagePointer trainingImage = this->GetTrainingImage();

  // Set up a image interators within the user specified region.
  // this will retireve pixel according to index
  typedef ImageRegionConstIterator< InputImageType > InputImageIterator;
  InputImageIterator inIt( inputImage, inputImage->GetBufferedRegion() );

  typedef ImageRegionConstIterator< TrainingImageType > TrainingImageIterator;
  TrainingImageIterator trIt ( trainingImage, trainingImage->GetBufferedRegion() );
  
  m_InputImageSamples.resize( inputImage->GetBufferedRegion().GetNumberOfPixels() );
  typename InputImagePixelContainer::iterator imSamples;
  imSamples = m_InputImageSamples.begin();

  m_TrainingImageSamples.resize( trainingImage->GetBufferedRegion().GetNumberOfPixels() );
  typename TrainingImagePixelContainer::iterator trSamples;
  trSamples = m_TrainingImageSamples.begin();

  while( !inIt.IsAtEnd() )
    {

    while ( !( this->IsFinite( inIt.Get() ) && inIt.IsAtEnd() ) ) // make sure the pixel is an actual value
      {
      ++inIt;

      if( inIt.IsAtEnd() )
        continue;

      } // while ( !this->IsFinite( *imSamples ) ) 

    *(imSamples) = inIt.Get();
    *(trSamples) = trIt.Get();

    ++imSamples;
    ++trSamples; 
    ++inIt;
    ++trIt;
    }//  while( imSamples!=imEnd ) )

// resize to true size of the containers
m_TrainingImageSamples.resize( m_TrainingImageSamples.size() );
m_InputImageSamples.resize   ( m_InputImageSamples.size() );

} //SampleFullImageRegion
////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////
//Function to check if a pixel is finite
template< class TInputImage,
          class TMembershipFunction,
          class TTrainingImage >
bool
VectorImageGaussianModelEstimator< TInputImage, TMembershipFunction, TTrainingImage >
::IsFinite( InputImagePixelType pixel )
{

  const unsigned int pixelDimension = pixel.GetSize();

  for ( unsigned int i = 0; i < pixelDimension; i++ )
  {
  if ( !vnl_math_isfinite( pixel[i] ) )
    return false;
  }

  return true;
} //IsFinite
////////////////////////////////////////////////////////////////////////////////////////////////////



}           // namespace itk
#endif
