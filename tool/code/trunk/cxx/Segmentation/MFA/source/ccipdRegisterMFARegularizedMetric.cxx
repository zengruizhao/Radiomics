


//////////////////////////////////////////////////////////////////////////////////////////

// ccipd includes
#include "ccipdRegisterMFARegularizedMetric.h"
#include "ccipdMFALevelsetExtractor.h" // for creating features
#include "ccipdCloneableFactory.hxx"

// std includes
#include <unordered_map>

// itk includes
#include "itkMFAReconstructionMetric.h"
#include "itkIntermediateRegularizedMetric.h"
#include "itkMFARegularizedKnownValues.h"

#include "ccipdDisableWarningsMacro.h"

// itk includes
#include <itkPointSet.h>

// boost includes
#include <boost/foreach.hpp>

#include "ccipdEnableWarningsMacro.h"

// serialization includes
#ifdef Boost_SERIALIZATION_FOUND
  #include <boost/archive/xml_iarchive.hpp>
  #include <boost/archive/xml_oarchive.hpp>
  #include <boost/serialization/export.hpp>
#endif // Boost_SERIALIZATION_FOUND

//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
using std::cerr;
using std::endl;
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////
template
class Copyable< RegisterMFARegularizedMetric >;

template
class Cloneable< RegisterMFARegularizedMetric >;

template
class CloneableFactory< RegisterMFARegularizedMetric >;

template
class Factory< RegisterMFARegularizedMetric >;
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
class RegisterMFARegularizedMetric::SecretData
{

public:
  SecretData();

  typedef RegisterMFARegularizedMetric::RegularizationWeightType RegularizationWeightType;

  RegularizationWeightType
    m_BoundingBoxWeight,
    m_KnownPointsWeight;

  AffineDistributionConstPointer
    m_Distribution;

  VolumeAffineTransformConstPointer
    m_IntermediateTransform;

  RegisterMFARegularizedMetric::LikelihoodType
    m_LikelihoodType;

  typedef std::unordered_map<
    MaskObjectType,
    LandmarkSetConstPointer
  > KnownPointsMap;

  KnownPointsMap
    m_KnownPoints;

}; // class SecretData
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
RegisterMFARegularizedMetric::RegisterMFARegularizedMetric() :
ccipdNewPIMPLMacro
{
} // constructor
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
RegisterMFARegularizedMetric::SecretData::SecretData() :
m_BoundingBoxWeight( 0 ),
m_KnownPointsWeight( 0 ),
m_LikelihoodType( RegisterMFARegularizedMetric::LikelihoodRelativePDF )
{
} // constructor
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
RegisterMFARegularizedMetric::~RegisterMFARegularizedMetric()
{
} // destructor
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
RegisterMFARegularizedMetric::MetricPointer
RegisterMFARegularizedMetric::GetMetric() const
{
  
  const bool verbose = this->GetVerbose();

  // extract the underlying metric
  const MFAMetricPointer underlyingMetric( this->GetMFAMetric() );
  
  // extract the regularization distribution
  const AffineDistributionConstPointer
    distribution( this->m_SecretData->m_Distribution );

  if ( !distribution )
    {
    if ( verbose ) cerr << "Warning: No distribution set." << endl;
    return MetricPointer( underlyingMetric );
    }

  // extract the amount to regularize
  const RegularizationWeightType
    boundingBoxWeight = this->GetBoundingBoxWeight(),
    knownPointsWeight = this->GetKnownPointsWeight();

  // get the intermediate transform.
  // this is okay if it's null because
  // the regularizer will just ignore it
  const VolumeAffineTransformConstPointer
    intermediateTransform( this->m_SecretData->m_IntermediateTransform );

  // create the regularized metric
  typedef itk::IntermediateRegularizedMetric<
    MetricType::FixedImagePixelType, MetricType::FixedImageDimension
  > BoundingBoxRegularizedMetricType;

  const BoundingBoxRegularizedMetricType::Pointer
    boundingBoxRegularizedMetric = BoundingBoxRegularizedMetricType::New();

  // give everything to the metric that we have
  boundingBoxRegularizedMetric->SetUnderlyingMetric     ( underlyingMetric      );
  boundingBoxRegularizedMetric->SetDistribution         ( distribution          );
  boundingBoxRegularizedMetric->SetRegularizationWeight ( boundingBoxWeight     );
  boundingBoxRegularizedMetric->SetIntermediateTransform( intermediateTransform );

  // We train the distribution of the transform
  // which is applied first, followed by the
  // bounding box transform.
  // Therefore, the intermediate transform
  // (the bounding box transform)
  // is second.
  boundingBoxRegularizedMetric->IntermediateFirstOff();
  
  // set how to calculate the likelihood
  switch ( this->GetLikelihoodType() )
    {

    case LikelihoodPDF :
      boundingBoxRegularizedMetric->SetLikelihoodType(
        BoundingBoxRegularizedMetricType::LikelihoodPDF );
      break;

    case LikelihoodLogPDF :
      boundingBoxRegularizedMetric->SetLikelihoodType(
        BoundingBoxRegularizedMetricType::LikelihoodLogPDF );
      break;

    case LikelihoodRelativePDF :
      boundingBoxRegularizedMetric->SetLikelihoodType(
        BoundingBoxRegularizedMetricType::LikelihoodRelativePDF );
      break;

    } // switch

  // we need to set the fixed region in here
  // because ITK didn't make it virtual... annoying
  const VolumeRegionType fixedRegion = this->GetFixedImageRegion();
  boundingBoxRegularizedMetric->SetFixedImageRegion( fixedRegion );

  // now add in the known points to the metric
  // making like a regularization chain
  // KnownPoints -- Bounding Box -- MFA Metric

  typedef ::itk::MFARegularizedKnownValues<
    MetricType::FixedImagePixelType, MetricType::FixedImageDimension
  > KnownPointsRegularizedMetricType;

  // create the regularizer
  const KnownPointsRegularizedMetricType::Pointer
    knownPointsRegularizedMetric = KnownPointsRegularizedMetricType::New();

  // set all the properties
  knownPointsRegularizedMetric->SetUnderlyingMetric( boundingBoxRegularizedMetric  );
  knownPointsRegularizedMetric->SetMFAMetric       ( underlyingMetric.GetPointer() );
  knownPointsRegularizedMetric->SetRegularizationWeight( knownPointsWeight );
  
  // we need to set the fixed region in here
  // because ITK didn't make it virtual... annoying
  knownPointsRegularizedMetric->SetFixedImageRegion( fixedRegion );

  // add in all the points
  BOOST_FOREACH( const auto & knownPointsValue, this->m_SecretData->m_KnownPoints )
    {

    // get the object
    const MaskObjectType & whichObject = knownPointsValue.first;
    
    // create a feature from this object
    const FeatureIDConstPointer whichFeature =
      MFALevelsetExtractor::CreateFeatureIDWithObject( whichObject );
    if ( !whichFeature ) continue;

    // get the landmarks
    const LandmarkSetConstPointer pointsForThisFeature( knownPointsValue.second );
    if ( !pointsForThisFeature ) continue;

    // loop through each point
    const LandmarkSetType::PointIdentifier
      numberOfPoints = pointsForThisFeature->GetNumberOfPoints();
    for ( LandmarkSetType::PointIdentifier pointIndex = 0;
      pointIndex < numberOfPoints; ++pointIndex )
      {

      // extract the point
      const LandmarkPointType
        knownPoint = pointsForThisFeature->GetPoint( pointIndex );

       // we are on the surface
      const InternalPixelType
        levelsetValue = 0;

      // set the value of this point for our regularizer
      knownPointsRegularizedMetric->AddKnownValue(
        knownPoint, levelsetValue, *whichFeature );

      } // for pointIndex

    } // known points

  // output the regularized metric
  return MetricPointer( knownPointsRegularizedMetric );

} // GetMetric
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void RegisterMFARegularizedMetric::ClearKnownPoints()
{
  this->m_SecretData->m_KnownPoints.clear();
}
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void RegisterMFARegularizedMetric::SetKnownPoints( 
  const LandmarkSetConstPointer & points,
  const MaskObjectType          & whichLevelsetObject )
{
  this->m_SecretData->m_KnownPoints[ whichLevelsetObject ] = points;
} // SetKnownPoints
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void RegisterMFARegularizedMetric::CopyParametersInternal( const Self & other )
{
  this->SetDistribution         ( other.m_SecretData->m_Distribution          );
  this->SetIntermediateTransform( other.m_SecretData->m_IntermediateTransform );
  this->SetBoundingBoxWeight    ( other.GetBoundingBoxWeight() );
  this->SetKnownPointsWeight    ( other.GetKnownPointsWeight() );
  this->SetLikelihoodType       ( other.GetLikelihoodType   () );
} // CopyParametersInternal
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////

ccipdSetByvalImplementMacro( RegisterMFARegularizedMetric,
  LikelihoodType, LikelihoodType )
ccipdGetConstImplementMacro( RegisterMFARegularizedMetric,
  LikelihoodType, RegisterMFARegularizedMetric::LikelihoodType )
ccipdSetByvalImplementMacro( RegisterMFARegularizedMetric,
  BoundingBoxWeight, RegularizationWeightType )
ccipdGetConstImplementMacro( RegisterMFARegularizedMetric,
  BoundingBoxWeight, RegisterMFARegularizedMetric::RegularizationWeightType )
ccipdSetByvalImplementMacro( RegisterMFARegularizedMetric,
  KnownPointsWeight, RegularizationWeightType )
ccipdGetConstImplementMacro( RegisterMFARegularizedMetric,
  KnownPointsWeight, RegisterMFARegularizedMetric::RegularizationWeightType )
ccipdSetByrefImplementMacro( RegisterMFARegularizedMetric,
  Distribution, AffineDistributionConstPointer )
ccipdSetByrefImplementMacro( RegisterMFARegularizedMetric,
  IntermediateTransform, VolumeAffineTransformConstPointer )

//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
#ifdef Boost_SERIALIZATION_FOUND

ccipdSerializationImplementationHeadingMacro( RegisterMFARegularizedMetric )
{

  using boost::serialization::make_nvp;

  boost::serialization::void_cast_register(
    static_cast< ConcreteFactorySelf< Self > * >( NULL ),
    static_cast< Self * >( NULL ) );

  if ( version < 1 )
    return;

  archive
    & BOOST_SERIALIZATION_BASE_OBJECT_NVP( RegisterMFAMetric )
    & make_nvp( "LikelihoodType"      , this->m_SecretData->m_LikelihoodType );

  if ( version >= 2 )
    {
    archive
      & make_nvp( "BoundingBoxWeight", this->m_SecretData->m_BoundingBoxWeight )
      & make_nvp( "KnownPointsWeight", this->m_SecretData->m_KnownPointsWeight );
    } // verison 2
  else
    {
    archive & make_nvp( "RegularizationWeight", this->m_SecretData->m_BoundingBoxWeight );
    } // version < 2

} // serialize

ccipdSerializationXMLMacro( RegisterMFARegularizedMetric )

typedef ConcreteFactorySelf< RegisterMFARegularizedMetric > 
  RegisterMFARegularizedMetricConcrete;

#endif // Boost_SERIALIZATION_FOUND
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace ccipd
 


#ifdef Boost_SERIALIZATION_FOUND

  BOOST_CLASS_VERSION( ccipd::RegisterMFARegularizedMetricConcrete, 2 )
  BOOST_CLASS_EXPORT_GUID( ccipd::RegisterMFARegularizedMetric , "RegisterMFARegularizedMetric" )
  BOOST_CLASS_EXPORT( ccipd::RegisterMFARegularizedMetricConcrete )
      ccipdBoostVirtualBaseFalse(
    ccipd::RegisterMFARegularizedMetric,
    ccipd::RegisterMFARegularizedMetricConcrete )

#endif // Boost_SERIALIZATION_FOUND
