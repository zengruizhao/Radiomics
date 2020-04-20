


//////////////////////////////////////////////////////////////////////////////////////////

// ccipd include
#include "ccipdMFABoundingBoxRandomizer.h"
#include "ccipdBoundingBoxRandomization.h"
#include "ccipdStudy.h"
#include "ccipdGammaDistribution.h"

// itk includes
#include <itkImageRegion.h>

// serialization includes
#ifdef Boost_SERIALIZATION_FOUND
  #include <boost/archive/xml_iarchive.hpp>
  #include <boost/archive/xml_oarchive.hpp>
#endif // Boost_SERIALIZATION_FOUND

// boost includes
#include <boost/noncopyable.hpp>

//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////
class MFABoundingBoxRandomizer::SecretData : boost::noncopyable
{

public:
  SecretData();

  bool m_Randomize;

  typedef MFABoundingBoxRandomizer::MillimeterType MillimeterType;

  MillimeterType
    m_ExpansionPeak,
    m_ExpansionStandardDeviation;

  typedef GammaDistribution<       MillimeterType   > DistributionType;
  typedef std::shared_ptr  <       DistributionType > DistributionPointer;
  typedef std::shared_ptr  < const DistributionType > DistributionConstPointer;

  const DistributionPointer
    m_ExpansionDistribution;
  
  /// Give the appropriate values to the distribution
  void UpdateDistribution();

}; // class SecretData
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
MFABoundingBoxRandomizer::SecretData::SecretData() :
m_Randomize( true ),
m_ExpansionPeak( 1 ),
m_ExpansionStandardDeviation( 1 ),
m_ExpansionDistribution( DistributionType::New() )
{
  this->UpdateDistribution();
} // constructor
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
MFABoundingBoxRandomizer::MFABoundingBoxRandomizer() :
ccipdNewPIMPLMacro
{
} // constructor
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
MFABoundingBoxRandomizer::~MFABoundingBoxRandomizer()
{
} // destructor
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
MFABoundingBoxRandomizer::BoundingBoxType
MFABoundingBoxRandomizer::GetBoundingBox(
  const StudyConstPointer & study )
{
  const bool randomize = this->GetRandomize();
  if ( randomize )
    return this->GetRandomBoundingBox( study );
  else
    return this->GetNonRandomBoundingBox( study );
}
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
MFABoundingBoxRandomizer::BoundingBoxType
MFABoundingBoxRandomizer::GetBoundingBox(
  const StudyConstPointer & study ) const
{
  const bool randomize = this->GetRandomize();
  if ( randomize )
    return this->GetRandomBoundingBox( study );
  else
    return this->GetNonRandomBoundingBox( study );
}
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
MFABoundingBoxRandomizer::BoundingBoxType
MFABoundingBoxRandomizer::GetRandomBoundingBox(
  const StudyConstPointer & study ) const
{

  // since it's randomizing,
  // we always calculate.
  return this->CalculateRandomBoundingBox( *study );

} // GetRandomBoundingBox
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
MFABoundingBoxRandomizer::BoundingBoxType
MFABoundingBoxRandomizer::GetRandomBoundingBox(
  const StudyConstPointer & study )
{

  // since it's randomizing,
  // we always calculate.
  return this->CalculateRandomBoundingBox( *study );

} // GetRandomBoundingBox
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
MFABoundingBoxRandomizer::BoundingBoxType
MFABoundingBoxRandomizer::GetNonRandomBoundingBox(
  const StudyConstPointer & study ) const
{

  BoundingBoxType boundingBox;
  
  // since we are using const_cast we need
  // to make sure no other omp threads
  // are also using this function
  #pragma omp critical
    {
    
    // remember the value
    const bool oldRandomize = this->GetRandomize();

    // we will be making sure the state doesn't
    // change, we promise!

    Self & nonConstThis = const_cast< Self & >( *this );

    // turn off randomizing
    nonConstThis.RandomizeOff();

    // get the cached, or calculated, bounding box
    boundingBox = this->Superclass::GetBoundingBox( study );

    // turn it back to its old state
    nonConstThis.SetRandomize( oldRandomize );
    
    } // critical
    
  // output
  return boundingBox;

} // GetNonRandomBoundingBox const
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
MFABoundingBoxRandomizer::BoundingBoxType
MFABoundingBoxRandomizer::GetNonRandomBoundingBox(
  const StudyConstPointer & study )
{
  // remember the value
  const bool oldRandomize = this->GetRandomize();

  // turn off randomizing
  this->RandomizeOff();

  // get the cached, or calculated, bounding box
  const BoundingBoxType boundingBox =
    this->Superclass::GetBoundingBox( study );

  // turn it back to its old state
  this->SetRandomize( oldRandomize );

  // output
  return boundingBox;

} // GetNonRandomBoundingBox
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
MFABoundingBoxRandomizer::BoundingBoxType
MFABoundingBoxRandomizer::CalculateBoundingBox(
  const Study & study ) const
{

  // call the appropriate function
  const bool randomize = this->GetRandomize();

  if ( randomize )
    return this->CalculateRandomBoundingBox( study );
  else
    return this->CalculateNonRandomBoundingBox( study );

} // CalculateBoundingBox
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
MFABoundingBoxRandomizer::BoundingBoxType
MFABoundingBoxRandomizer::CalculateRandomBoundingBox(
  const Study & study ) const
{

  // create a study pointer
  const StudyConstPointer studyPointer = study.shared_from_this();

  if ( !studyPointer ) return BoundingBoxType();

  // extract the non-random version
  // (which can use the cache!)
  const BoundingBoxType nonRandomBoundingBox = 
    this->GetNonRandomBoundingBox( studyPointer );

  // ignore 0-sized bounding boxes
  if ( nonRandomBoundingBox.GetNumberOfPixels() == 0 )
    return nonRandomBoundingBox;

  // get the image for this bounding box
  const BoundingBoxImageConstPointer
    image = this->GetBoundingBoxImage( studyPointer );

  if ( !image ) return nonRandomBoundingBox;

  // get the distribution
  const SecretData::DistributionConstPointer
    distribution( this->m_SecretData->m_ExpansionDistribution );

  if ( !distribution ) return nonRandomBoundingBox;

  // randomize
  const BoundingBoxType randomBoundingBox =
    RandomizeBoundingBox( nonRandomBoundingBox, *image, *distribution );
      
  // output
  return randomBoundingBox;

} // CalculateRandomBoundingBox
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void MFABoundingBoxRandomizer::SecretData::UpdateDistribution()
{

  const MillimeterType
    mode              = this->m_ExpansionPeak,
    standardDeviation = this->m_ExpansionStandardDeviation,
    variance          = standardDeviation * standardDeviation;

  this->m_ExpansionDistribution->SetModeAndVariance( mode, variance );
    
} // UpdateDistribution
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void MFABoundingBoxRandomizer::SetExpansionPeak( const MillimeterType variable )
{
  ccipdSetBodyMacro( ExpansionPeak )
  this->m_SecretData->UpdateDistribution();
}
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void MFABoundingBoxRandomizer::SetExpansionStandardDeviation(
  const MillimeterType variable )
{
  ccipdSetBodyMacro( ExpansionStandardDeviation )
  this->m_SecretData->UpdateDistribution();
}
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
ccipdSetByvalImplementMacro( MFABoundingBoxRandomizer, Randomize, bool )
ccipdGetConstImplementMacro( MFABoundingBoxRandomizer, Randomize, bool )
ccipdGetConstImplementMacro( MFABoundingBoxRandomizer,
  ExpansionPeak, MFABoundingBoxRandomizer::MillimeterType )
ccipdGetConstImplementMacro( MFABoundingBoxRandomizer,
  ExpansionStandardDeviation, MFABoundingBoxRandomizer::MillimeterType )
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
#ifdef Boost_SERIALIZATION_FOUND

ccipdSerializationImplementationHeadingMacro( MFABoundingBoxRandomizer )
{

  if ( version < 1 )
    return;

  using boost::serialization::make_nvp;

  // now, serialize
  archive
    & BOOST_SERIALIZATION_BASE_OBJECT_NVP( Superclass )
    & make_nvp( "Randomize"    , this->m_SecretData->m_Randomize     )
    & make_nvp( "ExpansionPeak", this->m_SecretData->m_ExpansionPeak )
    & make_nvp( "ExpansionStandardDeviation",
      this->m_SecretData->m_ExpansionStandardDeviation );
    
} // serialize

ccipdSerializationXMLMacro( MFABoundingBoxRandomizer )

#endif  
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace ccipd
