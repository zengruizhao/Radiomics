


//////////////////////////////////////////////////////////////////////////////////////////

// ccipd includes
#include "ccipdMFAMaskAligner.h"
#include "ccipdMFATypes.h"
#include "ccipdStudy.h"
#include "ccipdMultivariateGaussianDistribution.h"
#include "ccipdSIMBoundingBoxUnion.h"
#include "ccipdBoundingBox.h"
#include "ccipdCreateBinaryMask.h"
#include "ccipdResizeImage.h"
#include "ccipdSaveImage.h"
#include "ccipdHashable.h"
#include "ccipdFactory.hxx"
#ifndef NDEBUG
#include "ccipdExtractUniqueValues.h"
#endif // NDEBUG

// std includes
#include <unordered_map>

// serialization includes
#include "ccipdSegmentationOptions.h"
#ifdef Boost_SERIALIZATION_FOUND

  #include "ccipdUnorderedMapSerialization.h"    // for serializing our affine cache
  #include "ccipdSharedPointerSerialization.hxx" // for serializing smart pointers
  #include "ccipdMatrixSerialization.h"          // for serializing affine params

  #include <boost/serialization/set.hpp>
  #include <boost/archive/xml_iarchive.hpp>
  #include <boost/archive/xml_oarchive.hpp>

#endif // Boost_SERIALIZATION_FOUND

#include "ccipdDisableWarningsMacro.h"
  // itk includes
  #include <itkIdentityTransform.h> // for alignment with itself
  #include <itkAffineTransform.h>   // for cached affine transformations

  // boost includes
  #include <boost/foreach.hpp>         // for looping
  #ifndef NDEBUG
    #include <boost/lexical_cast.hpp>
  #endif

  // eigen includes
  #include <Eigen/Core>

#include "ccipdEnableWarningsMacro.h"
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
using std::cout;
using std::cerr;
using std::endl;
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////
template
class Factory< MFAMaskAligner >;
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
MFAMaskAligner::MFAMaskAligner()
{
} // constructor
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
MFAMaskAligner::~MFAMaskAligner()
{
} // denstructor
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void MFAMaskAligner::RegisterAllStudies()
{
  BOOST_FOREACH( const StudyConstPointer & study, this->Superclass::GetStudies() )
    {

    // register and cache the transform
    this->Superclass::GetTransform( study );

    // cache the bounding box
    this->CalculateNonRandomBoundingBox( *study );

    // cache the bounding box image
    this->Superclass::GetBoundingBoxImage( study );

    } // for each study

} // RegisterAllStudies
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
AffineDistributionPointer MFAMaskAligner::GetAffineDistribution()
{

  const bool verbose = this->GetVerbose();

  // make sure everything is registered
  if ( verbose ) cout << "Registering all studies:" << endl;
  this->RegisterAllStudies();
  if ( verbose ) cout << "Registering all studies done." << endl;

  typedef MultivariateGaussianDistribution<
    AffineDistributionType::DataType,
    AffineDistributionType::Dimensionality
  > SpecificDistributionType;

  const SpecificDistributionType::Pointer
    affineDistribution( SpecificDistributionType::New() );
  //affineDistribution->EstimateDistributionFromSamples(
  //  allAffineParameters );

  // display info
  if ( verbose )
    {
    const auto randomParameters = affineDistribution->GetRandomVector();
    cout << "Random parameters = " << endl << 
      *randomParameters << endl;
    cout << "Random Value = " <<
      affineDistribution->GetCDF( *randomParameters ) << endl;
    } // verbose

  // output
  return affineDistribution;

} // GetAffineDistribution
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
MFAMaskAligner::BoundingBoxType
MFAMaskAligner::CalculateNonRandomBoundingBox( const Study & study ) const
{

 return this->Superclass::CalculateBoundingBox( study );

} // CalculateNonRandomBoundingBox
//////////////////////////////////////////////////////////////////////////////////////////




//////////////////////////////////////////////////////////////////////////////////////////
MFAMaskAligner::BoundingBoxType
MFAMaskAligner::CalculateBoundingBox( const Study & study ) const
{
 
  return this->SuperclassBoundingBox::CalculateBoundingBox( study );

} // CalculateNonRandomBoundingBox
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
#ifdef Boost_SERIALIZATION_FOUND

ccipdSerializationImplementationHeadingMacro( MFAMaskAligner )
{

  if ( version < 1 )
    return;

  using boost::serialization::void_cast_register;
  using boost::serialization::make_nvp;
 
  // first, populate the cache with
  // all known affine parameters
  // by registering all images
  this->RegisterAllStudies();

  // now, serialize
  archive
    & BOOST_SERIALIZATION_BASE_OBJECT_NVP( SuperclassBoundingBox )
    & BOOST_SERIALIZATION_BASE_OBJECT_NVP( SuperclassAligner     );
   
  if ( version < 2 )
    {
    cout << "Warning: Unused, serialized variable Verbose" << endl;
    bool verbose = true; // dummy variable
    archive & make_nvp( "Verbose", verbose );
    (void)verbose;
    }
    
} // serialize

ccipdSerializationXMLMacro( MFAMaskAligner )

#endif // Boost_SERIALIZATION_FOUND
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace
