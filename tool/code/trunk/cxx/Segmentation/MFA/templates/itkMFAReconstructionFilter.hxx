#ifndef __itkMFAReconstructionFilter_hxx
#define __itkMFAReconstructionFilter_hxx



////////////////////////////////////////////////////////////////////////////////////////////////////

// ccipd includes
#include "ccipdSIMStorage.h" // for storing the features

// our own itk includes
#include "itkMFAReconstructionFilter.h"


#include "ccipdDisableWarningsMacro.h"
  // other itk includes
  #include <itkImageRegionConstIteratorWithIndex.h> // for traversing the input image
  #include <itkImageRegionIterator.h>               // for traversing the output image

  // eigen includes
  #include <Eigen/Core> // for our reconstruction Eigen vector
#include "ccipdEnableWarningsMacro.h"
////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////
using std::cout;
using std::endl;
using std::cerr;
////////////////////////////////////////////////////////////////////////////////////////////////////



namespace itk
{

  

//////////////////////////////////////////////////////////////////////////////////////////
/**
 * \brief PIMPL interface to MFAReconstructionFilter class.
 * 
 * \author  Rob Toth
 * \ingroup MFA
 * \date    2012-05-16
 */
template< class TPixelType, unsigned int Dimension >
class MFAReconstructionFilter< TPixelType, Dimension >::SecretData
{
public: // public to the MFAReconstructionFilter class

  /// Constructor
  SecretData();

  bool                     m_UseExistingLocations; ///< Don't use the whole image?
  SampleLocationsContainer m_ExistingLocations;    ///< Only used if m_UseExistingLocations is true

};
////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////
template         < class TPixelType, unsigned int Dimension >
MFAReconstructionFilter< TPixelType,              Dimension >
::SecretData::SecretData() :
m_ExistingLocations( false )
{
} // Constructor
////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////
template         < class TPixelType, unsigned int Dimension >
MFAReconstructionFilter< TPixelType,              Dimension >
::MFAReconstructionFilter() :
ccipdNewPIMPLMacro
{

  // we need at least one feature to reconstruct
  this->SetNumberOfRequiredInputs( 1 );

} // constructor
////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////
template         < class TPixelType, unsigned int Dimension >
MFAReconstructionFilter< TPixelType,              Dimension >
::~MFAReconstructionFilter()
{
} // Destructor
////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////
template         < class TPixelType, unsigned int Dimension >
bool
MFAReconstructionFilter< TPixelType,              Dimension >
::CanReconstructFeature( const ccipd::FeatureIDConstPointer & whichFeature ) const
{

  // extract our data
  const typename MFASuperclass::MFABaseConstPointer
    trainedMFA = this->MFASuperclass::GetTrainedMFA();

  // make sure we have a trained MFA and it has the desired feature
  const bool canReconstruct = trainedMFA && trainedMFA->HasFeature( whichFeature );

  // say why we can't
  if ( !canReconstruct && this->GetVerbose() )
    {
    if ( !trainedMFA )
      cout << "Cannot reconstruct because no trained MFA." << endl;
    else
      cout << "Cannot reconstruct because the MFA wasn't " <<
        "trained with the feature." << endl;

    cout << trainedMFA << endl;

    } // verbose

  // output
  return canReconstruct;

} // CanReconstructFeature
////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////
template         < class TPixelType, unsigned int Dimension >
void
MFAReconstructionFilter< TPixelType,              Dimension >
::ResampleAllFeatures()
{

  // don't smooth when calculating the final reconstructions
  const auto oldSmoothingVariance = this->MFASuperclass::GetSmoothingVariance();
  this->MFASuperclass::SetSmoothingVariance( 0 );

  bool & useExistingLocations = this->m_SecretData->m_UseExistingLocations;

  // now that we have the linked projections
  // and the concatenated projections,
  // we need to use the full mean image and
  // eigenvector image for the actual reconstruction
  const bool oldUseExistingLocations = useExistingLocations;
  useExistingLocations = false;

  // re-initialize our sample locations
  this->MFASuperclass::SampleMeanAndEigenvectors();

  // turn it back on
  useExistingLocations = oldUseExistingLocations;
  this->MFASuperclass::SetSmoothingVariance( oldSmoothingVariance );

} // ResampleAllFeatures
////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////
template         < class TPixelType, unsigned int Dimension >
void
MFAReconstructionFilter< TPixelType,              Dimension >
::GenerateData()
{

  const bool verbose = this->MFASuperclass::GetVerbose();
    
  // resample and recompute all our inputs
  if ( verbose ) cout << "Initializing reconstruction filter:" << endl;
  this->MFASuperclass::Initialize();
  if ( verbose ) cout << "Initializing reconstruction filter done." << endl;
  
  // we must calculate the projections
  if ( verbose ) cout << "Calculating linked projection:" << endl;
  this->MFASuperclass::CalculateLinkedProjection();
  if ( verbose ) cout << "Calculating linked projection done." << endl;

  // now we must resample all pixels from all features
  // in case we had used a subset previously
  if ( verbose ) cout << "Resampling all features:" << endl;
  this->ResampleAllFeatures();
  if ( verbose ) cout << "Resampling all features done." << endl;
  
  // at this point, the user is supposed to call GetReconstruction()
  // which will finish the computation

} // GenerateData
////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////
template         < class TPixelType, unsigned int Dimension >
void
MFAReconstructionFilter< TPixelType,              Dimension >
::SetExistingLocations( const SampleLocationsContainer & existingLocations )
{

  this->m_SecretData->m_ExistingLocations = existingLocations;
  this->Modified();

} // SetExistingLocations
////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////
template         < class TPixelType, unsigned int Dimension > typename
MFAReconstructionFilter< TPixelType,              Dimension >::OutputImagePointer
MFAReconstructionFilter< TPixelType,              Dimension >
::GetReconstructedImage( const ccipd::FeatureIDConstPointer & whichFeature ) const
{

  const bool verbose = this->MFASuperclass::GetVerbose();

  // first use the linked projection to calculate the projection
  if ( verbose ) cout << "Calculating reconstruction:" << endl;
  typedef typename MFASuperclass::ReconstructionConstPointer ReconstructionConstPointer;
  const ReconstructionConstPointer reconstruction(
    this->MFASuperclass::GetReconstruction( whichFeature ) );
  if ( verbose ) cout << "Calculating reconstruction done." << endl;

  // get the images
  const typename InputImageType::ConstPointer
    input = this->GetInput();
  const typename OutputImageType::ConstPointer
    requestedOutput = this->GetOutput();

  // check to make sure we have the correct number of pixels in our region
  typedef typename OutputImageType::RegionType OutputImageRegionType;
  OutputImageRegionType requestedRegion = requestedOutput->GetRequestedRegion();
  if ( static_cast< int >( requestedRegion.GetNumberOfPixels() )
      != reconstruction->size() )
    {
    itkExceptionMacro( << "Requested region has a a size of " <<
      requestedRegion.GetNumberOfPixels() << " while the reconstruction " <<
      "has a size of " << reconstruction->size() );
    }

  // prepare the reconstruction output
  if ( verbose ) cout << "Preparing reconstruction image:" << endl;
  const OutputImagePointer reconstructedImage = OutputImageType::New();
  reconstructedImage->SetBufferedRegion( requestedRegion );
  reconstructedImage->SetOrigin   ( requestedOutput->GetOrigin   () );
  reconstructedImage->SetDirection( requestedOutput->GetDirection() );
  reconstructedImage->SetSpacing  ( requestedOutput->GetSpacing  () );
  reconstructedImage->Allocate();
  if ( verbose ) cout << "Preparing reconstruction image done." << endl;
  
  // prepare the iterators
  typedef ImageRegionIterator< OutputImageType > OutputIterType;
  OutputIterType outputIterator( reconstructedImage, requestedRegion );
  const PixelType * reconstructionIterator = reconstruction->data();

  // go through the pixels in the output image
  if ( verbose ) cout << "Copying reconstruction to the reconstruction image:" << endl;
  for ( outputIterator.GoToBegin(); !outputIterator.IsAtEnd();
      ++outputIterator, ++reconstructionIterator )
    {

    // get our pixel value
    typedef typename OutputImageType::PixelType OutputImagePixelType;
    const OutputImagePixelType value =
      static_cast< OutputImagePixelType > (
        *reconstructionIterator );

    // set the output pixel to the current vector's value
    outputIterator.Set( value );
      
    } // outputIterator
  if ( verbose ) cout << "Copying reconstruction to the reconstruction image done." << endl;

  // return the output
  return reconstructedImage;

} // GetReconstructedImage
////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////
template         < class TPixelType, unsigned int Dimension > typename
MFAReconstructionFilter< TPixelType,              Dimension >::SampleLocationsContainer
MFAReconstructionFilter< TPixelType,              Dimension >
::CalculateSampleLocations()
{

  // we can optionally use an existing reconstruction's samples
  if ( this->GetUseExistingLocations() )
    {

    const SampleLocationsContainer & existingLocations =
      this->m_SecretData->m_ExistingLocations;

    // have we actually input an existing set of samples?
    if ( existingLocations.empty() )
      {
      itkWarningMacro( << "Warning: " <<
        "m_UseExistingLocations was set to true " <<
        "but the samples haven't been set!  " <<
        "Please call SetExistingLocations first.  " <<
        "Sampling all locations by default." );
      }
    else // we forgot to set another base!
      {
      return existingLocations;
      }

    } // UseExistingLocations
  
  // we are going to sample all the pxiels
  return CalculateAllSampleLocations();

} // CalculateSampleLocations
////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////
template         < class TPixelType, unsigned int Dimension > typename
MFAReconstructionFilter< TPixelType,              Dimension >::SampleLocationsContainer
MFAReconstructionFilter< TPixelType,              Dimension >
::CalculateAllSampleLocations()
{

  // just pick the first one and sample all the pixels
  typedef typename InputImageType::ConstPointer InputImageConstPointer;
  const InputImageConstPointer inputImage = this->GetInput();

  // get the full buffered region of the texture image
  typedef typename InputImageType::RegionType InputImageRegionType;
  InputImageRegionType region = inputImage->GetBufferedRegion();
  const auto size = region.GetNumberOfPixels();

  // prepare our output
  SampleLocationsContainer samples;
  samples.resize( size );

  // Set up a region interator
  ImageRegionConstIteratorWithIndex< InputImageType >
    regionIter( inputImage, region );

  // sample points
  for ( typename SampleLocationsContainer::iterator
    samplesIter = samples.begin();
    samplesIter != samples.cend();
    ++samplesIter, ++regionIter )
    {

    // get the index
    typedef typename InputImageType::IndexType InputImageIndexType;
    const InputImageIndexType index = regionIter.GetIndex();

    // get the physical location
    inputImage->TransformIndexToPhysicalPoint( index, *samplesIter );
    
    } // samplesIter

  // return our selected point locations
  return samples;

} // CalculateAllSampleLocations
////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////
template         < class TPixelType, unsigned int Dimension >
void
MFAReconstructionFilter< TPixelType,              Dimension >
::SetUseExistingLocations( const bool useExistingLocations )
{
  this->m_SecretData->m_UseExistingLocations = useExistingLocations;
  this->Modified();
} // SetUseExistingLocations
////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////
template         < class TPixelType, unsigned int Dimension >
bool
MFAReconstructionFilter< TPixelType,              Dimension >
::GetUseExistingLocations() const
{
  return this->m_SecretData->m_UseExistingLocations;
} // GetUseExistingLocations
////////////////////////////////////////////////////////////////////////////////////////////////////



} // namespace itk
#endif // __itkMFAReconstructionFilter_hxx
