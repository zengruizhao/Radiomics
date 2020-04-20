#ifndef __itkReconstructionMetric_hxx
#define __itkReconstructionMetric_hxx



////////////////////////////////////////////////////////////////////////////////////////////////////
// ccipd includes
#include "ccipdEvaluateMetrics.h"
#include "ccipdExtractFeatureImage.h"
#include "ccipdSIMStorage.h"
#include "ccipdApplyTransformation.h"
#include "ccipdSaveImage.h"

// std includes
#include <algorithm>
#include <cmath>
#include <limits> // for epsilon
#include <fstream>
#include <omp.h>

// boost includes
#include <boost/foreach.hpp>

// our own itk includes
#include "itkMFAReconstructionMetric.h"

#include "ccipdDisableWarningsMacro.h"

  // other itk includes
  #include <itkImageRandomConstIteratorWithIndex.h>
  #include <itkImageRegionIteratorWithIndex.h>
  #include <itkImageDuplicator.h> // for copying an image
  #include <itkNumericTraits.h>
  #include <itkPointsLocator.h> // for searching for nearest points in GetContributions
  #include <itkPointSet.h>

  // eigen includes
  #include <Eigen/Core> // for matrix computations
#include "ccipdEnableWarningsMacro.h"

#ifdef _MSC_VER
#pragma warning( disable: 4127 )
#pragma warning( disable: 4714 )
#endif // _MSC_VER
////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////
using namespace std;


namespace itk
{

  

//////////////////////////////////////////////////////////////////////////////////////////
/**
 * \brief PIMPL interface to MFAReconstructionMetric class.
 * 
 * \author  Rob Toth
 * \ingroup MFA
 * \date    2012-05-16
 */
template< class TPixelType, unsigned int Dimension >
class MFAReconstructionMetric< TPixelType, Dimension >::SecretData
{
public: // public to the MFAReconstructionMetric class

  /// Constructor
  SecretData();

  bool
    m_UseNCC,      ///< NCC if true, or SSD it false
    m_Initialized; ///< So you don't initialize twice.

  typedef MFAReconstructionMetric<
    TPixelType,
    Dimension
  > EncapsulatingClass;

  typedef typename
    EncapsulatingClass::MFASuperclass::VectorType VectorType;
  typedef typename
    EncapsulatingClass::MFASuperclass::VectorConstPointer VectorConstPointer;
  typedef std::shared_ptr< VectorType > VectorPointer;
  typedef typename
    EncapsulatingClass::SampleLocationsContainer SampleLocationsContainer;
  typedef typename
    EncapsulatingClass::FixedImageSampleContainer FixedImageSampleContainer;
  typedef typename
    EncapsulatingClass::TransformType TransformType;

  /// Transform a bunch of points.
  static SampleLocationsContainer GetTransformedPoints(
    const FixedImageSampleContainer & fixedSamples,
    const TransformType             & transformer );

  /// Transform a bunch of points multi-threaded.
  static SampleLocationsContainer GetTransformedPointsMultiThreaded(
    const FixedImageSampleContainer & fixedSamples,
    const TransformType             & transformer );

}; // clas SecretData
////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////
template         < class TPixelType, unsigned int Dimension >
MFAReconstructionMetric< TPixelType,              Dimension >
::SecretDataWorkaround::SecretDataWorkaround() :
std::unique_ptr< SecretData >( new SecretData )
{
} // Constructor
////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////
template         < class TPixelType, unsigned int Dimension >
MFAReconstructionMetric< TPixelType,              Dimension >
::SecretDataWorkaround::~SecretDataWorkaround()
{
} // Destructor
////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////
template         < class TPixelType, unsigned int Dimension >
MFAReconstructionMetric< TPixelType,              Dimension >
::SecretData::SecretData() :
m_UseNCC     ( true  ),
m_Initialized( false )
{
} // Constructor
////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////
template         < class TPixelType, unsigned int Dimension >
MFAReconstructionMetric< TPixelType,              Dimension >
::MFAReconstructionMetric()
{
  this->Superclass::ComputeGradientOff(); // no idea why this is on by default
} // Constructor
////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////
template              < class TPixelType, unsigned int Dimension >
void MFAReconstructionMetric< TPixelType,              Dimension >
::Initialize() throw( ExceptionObject )
{

  // are we already initialized?
  if ( this->m_SecretData->m_Initialized )
    return;

  const bool verbose = this->MFASuperclass::GetVerbose();

  // first, call the base class's Initialize function
  if ( verbose ) std::cout << "Initializing metric:" << std::endl; 
  this->Superclass   ::Initialize();
  if ( verbose ) std::cout << "Initializing metric done." << std::endl; 

  if ( verbose ) std::cout << "Initializing MFA:" << std::endl;
  this->MFASuperclass::Initialize();
  if ( verbose ) std::cout << "Initializing MFA done." << std::endl;

  // say we're initialized
  this->m_SecretData->m_Initialized = true;

} // Initialize
////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////
template              < class TPixelType, unsigned int Dimension >
void MFAReconstructionMetric< TPixelType,              Dimension >
::PrintSelf(std::ostream & os, Indent indent) const
{
  Superclass::PrintSelf( os, indent );
}
////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////
template         < class TPixelType, unsigned int Dimension > typename
MFAReconstructionMetric< TPixelType,              Dimension >::SampleLocationsContainer
MFAReconstructionMetric< TPixelType,              Dimension >
::CalculateSampleLocations()
{

  const bool debug = false;
  
  // the MultiThreadingInitialize function actually
  // pre-samples our fixed image, which is exactly what
  // we want to do.  The result is in m_FixedImageSamples.
  this->Superclass::MultiThreadingInitialize();

  // prepare the output
  SampleLocationsContainer samples;
  samples.resize( this->Superclass::m_FixedImageSamples.size() );

  // prepare some iterators to actually populate a list of only points (not values)
  typename FixedImageSampleContainer::const_iterator
    fixedIter = this->Superclass::m_FixedImageSamples.begin();
  const typename FixedImageSampleContainer::const_iterator
    fixedEnd = this->Superclass::m_FixedImageSamples.end();
  typename SampleLocationsContainer::iterator
    samplesIter = samples.begin();

  // populate our list of samples
  typedef ccipd::LandmarkSetType PointSetType;
  const typename PointSetType::Pointer pointSet = PointSetType::New();
  pointSet->Initialize();
  typedef typename PointSetType::PointsContainer PointsContainer;
  
  const typename PointsContainer::Pointer container = PointsContainer::New();
  
  int pointIndex = 0;
  const int maxPoints = 1000;
  
  for ( ; fixedIter != fixedEnd; ++fixedIter, ++samplesIter )
    {
    
    if ( debug && pointIndex < maxPoints )
      container->InsertElement( pointIndex++, fixedIter->point );
    
    *samplesIter = fixedIter->point;
    }
    
  if ( debug )
    {
    pointSet->SetPoints( container );
    ccipd::SaveLandmarks( pointSet.GetPointer(), "fixedLandmarks.vtk", true );
    } // debug

  // return our set of points
  return samples;

}
////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////
/** This is the main function which must calculate the metric value. */
template         < class TPixelType, unsigned int Dimension > typename
MFAReconstructionMetric< TPixelType,              Dimension >::MeasureType
MFAReconstructionMetric< TPixelType,              Dimension >
::GetValue( const TransformParametersType & parameters ) const
{

  // show progress?
  const volatile bool
    //verbose = this->MFASuperclass::GetVerbose(),
    debug   = false;
  
  // since GetValue must be const, this little trick
  // will allow us to call our non-const functions
  // from GetValue
  Self * unconstThis = const_cast< Self * >( this );

  // make sure we're initialized
  if ( !this->m_SecretData->m_Initialized )
    unconstThis->Initialize();

  if ( debug ) cout << "Parameters = " << parameters << endl;

  // set up the parameters in the transform
  this->m_Transform->SetParameters( parameters );

  // transform each point
  typedef SampleLocationsContainer MappedPointsContainer;
  const MappedPointsContainer mappedPoints =
    SecretData::GetTransformedPoints(//MultiThreaded(
       this->Superclass::m_FixedImageSamples,
      *this->Superclass::m_Transform );

  if (debug)
	  cout << "Point #100 = " << cout << mappedPoints[100] << endl;

  // check to see if every point is inside our moving mask
  if ( this->m_MovingImageMask )
    {

    const MeasureType worstMeasureValue = 
      this->GetUseNCC() ?
      NumericTraits< MeasureType >::min() :
      NumericTraits< MeasureType >::max() ;

    BOOST_FOREACH( const auto & mappedPoint, mappedPoints )
      {
      if ( !this->m_MovingImageMask->IsInside( mappedPoint ) )
        return worstMeasureValue;
      } // mappedPointsIter

    } // hasMovingMask
 
  if ( debug ) std::cout << "Calculating linked projection:" << std::endl;
  const bool onlyUseMeanImageForMetric = false;
  unconstThis->MFASuperclass::CalculateLinkedProjection(
    mappedPoints, onlyUseMeanImageForMetric );
  const auto linkedProjection = this->GetLinkedProjection();
  if ( onlyUseMeanImageForMetric && linkedProjection )
    {
    std::cerr << "Error: Projection exists! Projection = " << std::endl <<
      linkedProjection->transpose() << std::endl;
    }
  if ( debug ) std::cout << "Calculating linked projection done." << std::endl;

  // find out which features we've input
  const ccipd::FeatureIDConstCollection whichFeatures =
    this->MFASuperclass::GetInputFeatureIDs();

  // prepare our output (to be averaged)
  MeasureType metricSum = 0;
  
  // to do: clean this code up
  int featureIndex = 0;
  
  // go through each of the input features and calculate the metric
  BOOST_FOREACH( const ccipd::FeatureIDConstPointer & whichFeature, whichFeatures )
    {

    // add it into our sum
    metricSum += this->GetValue( whichFeature ); 

    // extract the feature
    if ( debug )
      {
      
      // todo: put the actual pointer types here
      
      const auto mfa = this->GetTrainedMFA();
      if ( mfa )
        {
        const auto meanAndEigenvectors =
          mfa->GetMeanAndEigenvectors( whichFeature );
          
        if ( meanAndEigenvectors.IsNotNull() )
          {
          
          // get the mean (fixed image)
          const auto meanImage = ccipd::ExtractFeatureImage(
            meanAndEigenvectors.GetPointer(), 0 );
            
          // now we need the transformed, moved image
          const auto movingImage = ccipd::ExtractFeatureImage(
            this->GetInputFeatures(), featureIndex++ );
            
          const bool extrapolate = false;
          const TPixelType defaultValue = std::numeric_limits< TPixelType >::quiet_NaN();
          
          const auto movedImage = ccipd::ApplyTransform(
            this->Superclass::m_Transform,
            movingImage,
            meanImage,
            extrapolate, defaultValue );
            
          // calculate the built-in NCC value
          const auto NCC = ccipd::CalculateNCC(
            movedImage.GetPointer(),
            meanImage .GetPointer(),
            static_cast< float >( mappedPoints.size() ) /
            static_cast< float >( meanImage->GetBufferedRegion().GetNumberOfPixels() ) );
            
          std::cout << "Transform = " << this->Superclass::m_Transform << std::endl;
          
          ccipd::SaveImage( movingImage.GetPointer(), "moving_builtin.mha", true );
          ccipd::SaveImage( movedImage.GetPointer(), "moved_builtin.mha", true );
          ccipd::SaveImage( meanImage .GetPointer(), "mean_builtin.mha" , true );
          
          std::cout << "Built-in NCC = " << NCC << std::endl;
            
          } // meanAndEigenvectors
        } // mfa
      } // debug
       
    } //whichFeatures

  // return the metric averaged over all features
  const MeasureType
    numberOfInputFeatures = static_cast< MeasureType >( whichFeatures.size() ),
    averageMetricValue = metricSum / numberOfInputFeatures;

  if ( debug ) cout << "Metric = " << averageMetricValue << endl;

  return averageMetricValue;
  
} // GetValue
////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////
template         < class TPixelType, unsigned int Dimension > typename
MFAReconstructionMetric< TPixelType,              Dimension >::
SecretData::SampleLocationsContainer
MFAReconstructionMetric< TPixelType,              Dimension >
::SecretData::GetTransformedPointsMultiThreaded(
  const FixedImageSampleContainer & fixedSamples,
  const TransformType             & transformer )
{


  // how many sample locations do we have?
  const auto numberOfSamples = fixedSamples.size();

  // prepare our transformed locations
  typedef SampleLocationsContainer MappedPointsContainer;
  MappedPointsContainer mappedPoints( numberOfSamples );

  // how many threads will the sampler be using?
  int numberOfThreads;
  #pragma omp parallel
    {
    numberOfThreads = omp_get_num_threads();
    }

  // Be even more granular in case a thread ends early.
  // While the above value is actually how many threads
  // the program will use, the below value is actually
  // how many splits the data will have.
  const int numberOfChunks = numberOfThreads * 2;

  #pragma omp parallel for // parallel for loops
  for ( int chunk = 0; chunk < numberOfChunks; ++chunk )
    {

    // figure out how many samples to process in this thread
    int chunkSize = static_cast< int >( numberOfSamples / numberOfChunks ); // floor
    const unsigned int firstSampleIndex = chunk * chunkSize;
    if ( chunk == numberOfChunks - 1 ) // last chunk
      {
      // the last chunk has a different chunk size
      chunkSize = static_cast< int >(
        numberOfSamples - ( numberOfChunks - 1 ) * chunkSize );
      }

    // prepare our iterators
    // get an iterator for our fixed samples
    typename FixedImageSampleContainer::const_iterator
      fixedSamplesBegin = fixedSamples.begin() + firstSampleIndex;
    const typename FixedImageSampleContainer::const_iterator
      fixedSamplesEnd = fixedSamplesBegin + chunkSize;
    typename MappedPointsContainer::iterator mappedPointsIter =
      mappedPoints.begin() + firstSampleIndex;

    // go through each sample
    for ( typename FixedImageSampleContainer::const_iterator
      fixedSamplesIter = fixedSamplesBegin;
      fixedSamplesIter != fixedSamplesEnd;
      ++fixedSamplesIter, ++mappedPointsIter )
      {

      // get the physical point location
      const FixedImagePointType fixedPoint = fixedSamplesIter->point;

      // map the physical location onto the moving image
      const MovingImagePointType mappedPoint =
        transformer.TransformPoint( fixedPoint );

      // assign it to our mapped container
      *mappedPointsIter = mappedPoint;

      } // fixedSamplesIter
    } // chunk

  // output
  return mappedPoints;

} // GetTransformedPoints
////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////
template         < class TPixelType, unsigned int Dimension > typename
MFAReconstructionMetric< TPixelType,              Dimension >::
SecretData::SampleLocationsContainer
MFAReconstructionMetric< TPixelType,              Dimension >
::SecretData::GetTransformedPoints(
  const FixedImageSampleContainer & fixedSamples,
  const TransformType             & transformer )
{

  const bool debug = false;
  
  // get an iterator for our fixed samples
  typename FixedImageSampleContainer::const_iterator
    fixedSamplesBegin = fixedSamples.begin();
  const typename FixedImageSampleContainer::const_iterator
    fixedSamplesEnd = fixedSamples.end();

  // how many sample locations do we have?
  const auto numberOfSamples = fixedSamples.size();

  // prepare our transformed locations
  typedef SampleLocationsContainer MappedPointsContainer;
  MappedPointsContainer mappedPoints( numberOfSamples );
  typename MappedPointsContainer::iterator mappedPointsIter =
    mappedPoints.begin();

  typedef ccipd::LandmarkSetType PointSetType;
  const typename PointSetType::Pointer pointSet = PointSetType::New();
  pointSet->Initialize();
  typedef typename PointSetType::PointsContainer PointsContainer;
  
  const typename PointsContainer::Pointer container = PointsContainer::New();
  
  int pointIndex = 0;
  const int maxPoints = 1000;
  
  // transform each point
  for ( typename FixedImageSampleContainer::const_iterator
    fixedSamplesIter = fixedSamplesBegin;
    fixedSamplesIter != fixedSamplesEnd;
    ++fixedSamplesIter, ++mappedPointsIter )
    {

    // get the physical point location
    const FixedImagePointType fixedPoint = fixedSamplesIter->point;

    // map the physical location onto the moving image
    const MovingImagePointType mappedPoint =
      transformer.TransformPoint( fixedPoint );

    if ( debug && pointIndex < maxPoints )
      container->InsertElement( pointIndex++, mappedPoint );
    
    // assign it to our mapped container
    *mappedPointsIter = mappedPoint;

    } // fixedSamplesIter

  if ( debug )
    {
    pointSet->SetPoints( container );
    ccipd::SaveLandmarks( pointSet.GetPointer(), "movedLandmarks.vtk", true );
    } // debug

  // output
  return mappedPoints;

} // GetTransformedPoints
////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////
template         < class TPixelType, unsigned int Dimension > typename
MFAReconstructionMetric< TPixelType,              Dimension >::MeasureType
MFAReconstructionMetric< TPixelType,              Dimension >
::GetValue( const ccipd::FeatureIDConstPointer & whichFeature ) const
{

  const volatile bool debug =
    //true;
    false;
    //rand() < ( RAND_MAX * .002 );

  if ( debug ) std::cout << "Calculating reconstruction:" << std::endl;
 
  // first, calculate the reconstruction
  typedef typename MFASuperclass::ReconstructionConstPointer ReconstructionConstPointer;
  const ReconstructionConstPointer reconstruction =
    this->MFASuperclass::GetReconstruction( whichFeature );

  // get the original sampled values (from which the reconstruction was calculated)
  typedef typename MFASuperclass::SampledValuesConstPointer SampledValuesConstPointer;
  SampledValuesConstPointer sampledValues =
    this->MFASuperclass::GetSampledValues( whichFeature );
 
  if ( debug ) std::cout << "Calculating reconstruction done." << std::endl;

  // extract the values we will be comparing

  typedef typename MFASuperclass::ReconstructionType ReconstructionType;
  const ReconstructionType
    & reconstructed = *reconstruction, //ReconstructionType( 5 ), // /*&newProjection, //*/ this->MFASuperclass::m_Reconstructions  [ featureIndex ],
    & movedValues   = *sampledValues; //ReconstructionType( 5 ); // /*oldProjection; //*/ this->MFASuperclass::m_InputSampleValues[ featureIndex ];

  // will we weight each pixel differently?
  const bool useNCC = this->GetUseNCC();

  // if everything is 0, don't bother calculating
  const bool allZeros = ( movedValues.array() == 0 ).all();

  const MeasureType worstMeasureValue = useNCC ?
    0 : std::numeric_limits< MeasureType >::max() / 10;

  if ( allZeros )
    {
    return worstMeasureValue;
    }

  // normalize ("N"CC)
  const ReconstructionType
    normalizedMoved =
      //movedValues,
      //movedValues.normalized(),
      ( movedValues.array() - movedValues.mean() ).matrix().normalized(),
    normalizedReconstructed =
      //reconstructed;
      //reconstructed.normalized();
      ( reconstructed.array() - reconstructed.mean() ).matrix().normalized();

  if ( useNCC )
    {

    // dot product (N"CC")
    const MeasureType NCC = normalizedMoved.dot( normalizedReconstructed );
   
    if ( debug )
      {
      std::cout << "moved mean = " << movedValues.mean() << std::endl;
      std::cout << "moved norm = " << movedValues.norm() << std::endl;
      std::cout << "reconstructed mean = " << reconstructed.mean() << std::endl;
      std::cout << "reconstructed norm = " << reconstructed.norm() << std::endl;
      std::cout << "moved = " << 
        movedValues.head( 10 ).transpose() << std::endl;
      std::cout << "reconstructed = " << 
        reconstructed.head( 10 ).transpose() << std::endl;
      std::cout << "normalizedMoved = " << 
        normalizedMoved.head( 10 ).transpose() << std::endl;
      std::cout << "normalizedReconstructed = " << 
        normalizedReconstructed.head( 10 ).transpose() << std::endl;
      std::cout << "NCC = " << NCC << std::endl;

      std::cout << "Saving csv:" << std::endl;
      
      try
        {

        std::ofstream outputFile;
        outputFile.open( "NCC.csv" );
    
        for ( int row = 0; row < movedValues.size(); ++row )
          {
          // write it to the file followed by a space
          outputFile << movedValues[ row ] << "," <<
            reconstructed[ row ] << std::endl;
          } // row

        outputFile.close();

        } // try
      catch( ... )
        {
        }

      std::cout << "Saving csv done." << std::endl;

      } // debug

    // make sure we don't return NaN
    // we are always minimizing!!!!
    return ( NCC != NCC ) ? worstMeasureValue : -NCC;

    } // useNCC

  else // weighted SSD
    {

    // calculate the differences squared
    typedef typename MFASuperclass::VectorType VectorType;
    const VectorType
      differencesSquared = ( movedValues - reconstructed ).array().abs2();

//#define MFA_USE_WEIGHTED_SSD 1
#ifdef MFA_USE_WEIGHTED_SSD
    const typename SecretData::VectorConstPointer
      weightsPointer = this->GetPseudoInverseWeights( whichFeature );
    const VectorType
      & weights                    = *weightsPointer,
        weightedDifferencesSquared = differencesSquared.array() * weights.array();
    const MeasureType
      numerator   = weightedDifferencesSquared.sum(),
      denominator = weights.sum(),
      SSD         = numerator / denominator;
#else // unweighted
    const MeasureType
      SSD = differencesSquared.mean();
#endif // MFA_USE_WEIGHTED_SSD
    
    if ( debug )
      {

      std::cout << "differencesSquared = " <<
        differencesSquared.head( 10 ).transpose() << std::endl;
#ifdef MFA_USE_WEIGHTED_SSD
      std::cout << "weights = " <<
        weights.head( 10 ).transpose() << std::endl;
      std::cout << "numerator = "   << numerator   << std::endl;
      std::cout << "denominator = " << denominator << std::endl;
#endif // MFA_USE_WEIGHTED_SSD
      std::cout << "SSD = " << SSD << endl;

      } // debug
    
    // if it's NaN return the worst value
    return ( SSD != SSD ) ? worstMeasureValue : SSD;

    } // UseNCC
    
} // GetValue ( one feature )
////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////
template              < class TPixelType, unsigned int Dimension >
void MFAReconstructionMetric< TPixelType,              Dimension >
::GetDerivative(
  const TransformParametersType & itkNotUsed( parameters ),
        DerivativeType          & itkNotUsed( derivative ) ) const
{
} // GetDerivative
////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////
template              < class TPixelType, unsigned int Dimension >
bool MFAReconstructionMetric< TPixelType,              Dimension >
::GetUseNCC() const
{
  return this->m_SecretData->m_UseNCC;
} // GetUseNCC
////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////
template              < class TPixelType, unsigned int Dimension >
void MFAReconstructionMetric< TPixelType,              Dimension >
::SetUseNCC( const bool useNCC )
{

  this->m_SecretData->m_UseNCC = useNCC;
  this->Modified();

} // SetUseNCC
////////////////////////////////////////////////////////////////////////////////////////////////////



} // namespace itk
#endif // __itkReconstructionMetric_hxx
