#ifndef __itkMFARegularizedKnownValues_hxx
#define __itkMFARegularizedKnownValues_hxx



////////////////////////////////////////////////////////////////////////////////////////////////////

// ccipd includes
#include "ccipdFeatureID.h"
#include "ccipdSIMStorage.h"
#include "ccipdSIMSampleImage.h"
#include "ccipdMatrixConcatenation.h"

// itk includes
#include "itkMFARegularizedKnownValues.h"
#include "itkMFAReconstructionMetric.h"

#include "ccipdDisableWarningsMacro.h"

// eigen includes
#include <Eigen/Core>

// boost includes
#include <boost/foreach.hpp>

#include "ccipdEnableWarningsMacro.h"

////////////////////////////////////////////////////////////////////////////////////////////////////



namespace itk
{



////////////////////////////////////////////////////////////////////////////////////////////////////
template< typename TPixelType, unsigned int TDimension >
class MFARegularizedKnownValues< TPixelType, TDimension >::SecretData
{

public:

  typedef MFARegularizedKnownValues<
    TPixelType,
    TDimension
  > EncapsulatingClass;

  typedef typename EncapsulatingClass::MFAMetricType         MFAMetricType;
  typedef typename EncapsulatingClass::MFAMetricConstPointer MFAMetricConstPointer;

  /// store an MFA metric
  MFAMetricConstPointer m_MFAMetric;

  /// typedefs for our points
  typedef typename EncapsulatingClass::PointType PointType;
  typedef typename MFAMetricType::SampleLocationsContainer SampleLocationsContainer;

  /// make sure we are using the same point types
  static_assert( std::is_same<
    PointType, typename SampleLocationsContainer::value_type >::value,
    "The point types differ between the MFA metric and the regularizer class!" );

  typedef ::ccipd::FeatureIDConstPointer FeatureIDConstPointer;
  typedef ::ccipd::HashableFunctor< FeatureIDConstPointer > FunctorType;

  /// For each feature we can have known points
  typedef std::unordered_map<
    FeatureIDConstPointer,
    SampleLocationsContainer,
    FunctorType,
    FunctorType
  > KnownPointLocationsMap;

  /// Store the locations of the known points
  KnownPointLocationsMap
    m_KnownPointLocations;

  /// typedefs for the known values
  typedef typename MFAMetricType::SampledValuesType KnownValuesType;

  typedef std::unordered_map<
    FeatureIDConstPointer,
    KnownValuesType,
    FunctorType,
    FunctorType
  > KnownValuesMap;

  /// Store the known values (each element should be the same size as the known points)
  KnownValuesMap
    m_KnownValues;

}; // class SecretData
////////////////////////////////////////////////////////////////////////////////////////////////////

  

////////////////////////////////////////////////////////////////////////////////////////////////////
template< typename TPixelType, unsigned int TDimension >
MFARegularizedKnownValues< TPixelType, TDimension >
::MFARegularizedKnownValues() 
{
} // Constructor
////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////
template< typename TPixelType, unsigned int TDimension >
MFARegularizedKnownValues< TPixelType, TDimension >
::SecretDataWorkaround::SecretDataWorkaround() :
std::unique_ptr< SecretData >( new SecretData )
{
} // Constructor
////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////
template< typename TPixelType, unsigned int TDimension >
MFARegularizedKnownValues< TPixelType, TDimension >
::SecretDataWorkaround::~SecretDataWorkaround()
{
} // Destructor
////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////
template< typename TPixelType, unsigned int TDimension >
MFARegularizedKnownValues< TPixelType, TDimension >
::~MFARegularizedKnownValues()
{
} // Destructor
////////////////////////////////////////////////////////////////////////////////////////////////////


  
////////////////////////////////////////////////////////////////////////////////////////////////////
template< typename TPixelType, unsigned int TDimension >
void 
MFARegularizedKnownValues< TPixelType, TDimension >
::AddKnownValue(
  const PointType            where,
  const TPixelType           value,
  const ::ccipd::FeatureID & whichFeatureInput )
{

  const typename SecretData::FeatureIDConstPointer
    whichFeature = whichFeatureInput.shared_from_this();

  // add in the point to our known locations
  typename SecretData::SampleLocationsContainer
    & knownPointLocations = this->m_SecretData->m_KnownPointLocations[ whichFeature ];

  knownPointLocations.push_back( where );

  // add in the value to our known values
  typename SecretData::KnownValuesType
    & knownValues = this->m_SecretData->m_KnownValues[ whichFeature ];
  
  // resize to accomodate a new element
  const auto knownValuesIndex = knownValues.size();
  knownValues.conservativeResize( knownValuesIndex + 1 );
  knownValues.coeffRef( knownValuesIndex ) = value;

} // AddKnownValue
////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////
template< typename TPixelType, unsigned int TDimension >
void 
MFARegularizedKnownValues< TPixelType, TDimension >
::ClearKnownValues()
{

  // clear our maps
  this->m_SecretData->m_KnownPointLocations.clear();
  this->m_SecretData->m_KnownValues        .clear();

} // ClearKnownValues
////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////
template< typename TPixelType, unsigned int TDimension >
typename
MFARegularizedKnownValues< TPixelType, TDimension >
::MeasureType
MFARegularizedKnownValues< TPixelType, TDimension >
::GetRegularization( const TransformParametersType & parameters) const
{

  // the bad output is a very high value
  // since the "best" value is a mean
  // absolute distance of 0
  const MeasureType badOutput =
    std::numeric_limits< MeasureType >::max() / 10000;

  // extract our relevant info
  const MFAMetricConstPointer metric = this->m_SecretData->m_MFAMetric;
  if ( !metric ) return badOutput;

  // extract our transform
  typedef typename MFAMetricType::TransformType::ConstPointer TransformConstPointer;
  const TransformConstPointer
    metricTransform = metric->GetTransform();
  if ( !metricTransform ) return badOutput;

  // we will use the inverse to go back
  // to the MFA space from the input space
  const TransformConstPointer
    inverseTransform( metricTransform->GetInverseTransform() );

  // singular matrices will return null inverses
  if ( !inverseTransform ) return badOutput;

  // make sure the parameters are set correctly
  const TransformParametersType metricParameters = metricTransform->GetParameters();
  const unsigned int numberOfParameters = 
    static_cast< unsigned int >( parameters.Size() );
  if ( metricParameters.Size() != numberOfParameters ) return badOutput;

  // make sure each parameter is the same as the input
  for ( unsigned int parameterIndex = 0;
    parameterIndex < numberOfParameters; ++parameterIndex )
    {
      
    if ( metricParameters.GetElement( parameterIndex ) !=
      parameters.GetElement( parameterIndex ) )
      {
      return badOutput;
      }

    } // for parameterIndex

  // if we've gotten this far, then the transform used
  // for the MFA is the same as the input parameters

  // get the linked matrix
  const typename MFAMetricType::VectorConstPointer
    linkedProjection = metric->GetLinkedProjection();
  if ( !linkedProjection ) return badOutput;
  const auto linkedRows = linkedProjection->rows();

  // now we need to sample the mean and eigenvectors
  // for each of the known samples for each feature

  // first extract the trained MFA
  typedef typename MFAMetricType::MFABaseConstPointer MFABaseConstPointer;
  const MFABaseConstPointer trainedMFA = metric->GetTrainedMFA();
  if ( !trainedMFA ) return badOutput;

  typedef typename SecretData::KnownPointLocationsMap KnownPointLocationsMap;
  const KnownPointLocationsMap
    & knownPointLocations = this->m_SecretData->m_KnownPointLocations;
  typedef typename SecretData::KnownValuesMap KnownValuesMap;
  const KnownValuesMap
    & knownValues = this->m_SecretData->m_KnownValues;

  //if ( knownPointLocations.empty() ) return badOutput;

  typedef typename MFAMetricType::VectorType VectorType;
  typedef std::shared_ptr< VectorType >      VectorPointer;

  typedef typename MFAMetricType::ReconstructionConstPointer ReconstructionConstPointer;
  std::vector< ReconstructionConstPointer > reconstructionDifferences;

  // loop through our features
  BOOST_FOREACH( const auto & samplesMapValue, knownPointLocations )
    {

    // get our feature
    typedef typename SecretData::FeatureIDConstPointer FeatureIDConstPointer;
    const FeatureIDConstPointer whichFeature = samplesMapValue.first;

    // get the sample locations
    typedef typename SecretData::SampleLocationsContainer SampleLocationsContainer;
    const SampleLocationsContainer & untransformedLocations = samplesMapValue.second;
    if ( untransformedLocations.empty() ) continue;

    // we need to transform each point to
    // be on the mean / eigenvector space
    SampleLocationsContainer transformedLocations;
    BOOST_FOREACH( const auto & untransformedPoint, untransformedLocations )
      {
      const auto transformedPoint =
        inverseTransform->TransformPoint( untransformedPoint );
      transformedLocations.push_back( transformedPoint );
      } // for each untransformed point

    // see if this feature is in our known values map
    const typename KnownValuesMap::const_iterator
      knownValuesIterator = knownValues.find( whichFeature );
    if ( knownValuesIterator == knownValues.cend() ) continue;

    // get the known values
    typedef typename SecretData::KnownValuesType KnownValuesType;
    const KnownValuesType & values = knownValuesIterator->second;

    // check if they are the same sizes
    if ( transformedLocations.size() !=
      static_cast< std::size_t >( values.size() ) )
      {
      continue;
      }

    // now we will sample the mean and eigenvectors at our locations

    // todo: somehow remove duplicate code with MFAReconstructionBase.hxx?

    // get the mean and eigenvector images
    typedef typename MFAMetricType::MFABaseType MFABaseType;
    typedef typename MFABaseType::VectorImageConstPointer VectorImageConstPointer;
    const VectorImageConstPointer meanAndEigenvectors(
      trainedMFA->GetMeanAndEigenvectors( whichFeature ) );
    if ( !meanAndEigenvectors ) continue;

    // sample
    typedef typename MFABaseType::LinkedMatrixType EigenvectorsMatrixType;

    VectorType             meanVector;
    EigenvectorsMatrixType eigenvectorsMatrix;
    
    // sample it
    ::ccipd::SampleMeanAndEigenvectors< TPixelType, TDimension >(
      meanAndEigenvectors,
      transformedLocations,
      meanVector,
      eigenvectorsMatrix );

    // compute the reconstruction
    // make sure it's the correct size
    const auto
      eigenvectorRows = eigenvectorsMatrix.rows(),
      eigenvectorCols = eigenvectorsMatrix.cols();

    if ( meanVector.size() != eigenvectorRows )
      {
      // something is wrong
      continue;
      }

    if ( eigenvectorCols > linkedRows )
      {
      eigenvectorsMatrix.conservativeResize( eigenvectorRows, linkedRows );
      }
    else if ( eigenvectorCols < linkedRows )
      {
      // something is wrong
      continue;
      }

    // calculate the reconstruction
    const VectorType reconstruction = meanVector + eigenvectorsMatrix * *linkedProjection;

    if ( reconstruction.size() == 0 || reconstruction.size() != values.size() ) continue;

    // calculate the differences
    const VectorPointer differences( new VectorType );
    *differences = reconstruction - values;
    
    // add it to our collection
    reconstructionDifferences.push_back( differences );

    } // for each samples map value

  if ( reconstructionDifferences.empty() ) return badOutput;

  // concatenate
  const VectorPointer concatenatedDifferences = 
    ::ccipd::ConcatenateVectors( reconstructionDifferences );

  if ( !concatenatedDifferences )
    return badOutput;

  if ( concatenatedDifferences->size() == 0 )
    return 0;

  // compute the mean absolute differences
  const MeasureType
    meanAbsoluteDifferences = static_cast< MeasureType >(
      concatenatedDifferences->cwiseAbs().mean() );

  // output (making sure to deal with NaN's)
  if ( meanAbsoluteDifferences != meanAbsoluteDifferences )
    return badOutput;
  else
    return meanAbsoluteDifferences;

} // GetRegularization
////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////
template< typename TPixelType, unsigned int TDimension >
void
MFARegularizedKnownValues< TPixelType, TDimension >
::SetMFAMetric( const MFAMetricConstPointer & variable )
{
  ccipdSetBodyMacro( MFAMetric )
} // SetMFAMetric
////////////////////////////////////////////////////////////////////////////////////////////////////



} // namespace itk
#endif // __itkMFARegularizedKnownValues_hxx
