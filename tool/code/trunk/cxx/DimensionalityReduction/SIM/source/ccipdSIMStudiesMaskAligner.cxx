


//////////////////////////////////////////////////////////////////////////////////////////
// ccipd includes
#include "ccipdSIMStudiesMaskAligner.h"
#include "ccipdStudy.h"
#include "ccipdRegisterAffineMasks.h"
#include "ccipdApplyTransformation.h"
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
#include "ccipdDimensionalityReductionOptions.h"
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
  #include <boost/lexical_cast.hpp>
  
  // eigen includes
  #include <Eigen/Core>
#include "ccipdEnableWarningsMacro.h"
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
using std::cout;
using std::cerr;
using std::endl;
using std::string;
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{


//////////////////////////////////////////////////////////////////////////////////////////
template
class Factory< SIMStudiesMaskAligner >;
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
class SIMStudiesMaskAligner::SecretData
{

public:
  SecretData(); ///< Constructor

  bool
    m_HaveCachedBoundingBox, ///< Do we need to recompute the bounding box?
    m_Verbose;
  MaskObjectType
    m_ObjectToAlign;
  float
    m_BoundingBoxPadding;
  VolumeRegionType
    m_CachedBoundingBox;
  MaskObjectCollectionType
    m_BoundingBoxMaskObjects;

  /// Make sure every study is registered.
  void RegisterAllStudies();

}; // class SecretData
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
SIMStudiesMaskAligner::SecretData::SecretData() :
m_HaveCachedBoundingBox( false ),
m_Verbose              ( true  ),
m_BoundingBoxPadding( std::numeric_limits< float >::max() ) // don't crop the images
{
} // Constructor
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
SIMStudiesMaskAligner::SIMStudiesMaskAligner() :
ccipdNewPIMPLMacro
{
} // constructor
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
SIMStudiesMaskAligner::~SIMStudiesMaskAligner()
{
} // denstructor
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
VolumeImageBaseConstPointer SIMStudiesMaskAligner::GetTemplateVolume() const
{

  // extract the template study
  const StudyConstPointer & templateStudy = 
    this->GetTemplateStudy() ?     // If we have set one, use it.
    this->GetTemplateStudy() :
    ! this->GetStudies().empty() ? // Do we have any studies at all?
    *this->GetStudies().cbegin() : // If so, just pick one.
    StudyConstPointer();                      // If not, come up empty.

  // if we couldn't come up with a template
  // study, just return empty
  if ( !templateStudy )
    return VolumeImageBaseConstPointer();

  // try to get the given mask
  const MaskObjectType & alignmentObject =
    this->m_SecretData->m_ObjectToAlign;

  const bool hasAlignmentMask =
    templateStudy->HasMask( alignmentObject );

  if ( hasAlignmentMask )
    {
    //SaveImage( templateStudy->GetMask( alignmentObject ).GetPointer(), "template_volume.mha", true );
    // If we have the mask, then use it.
    return templateStudy->GetMask( alignmentObject ).GetPointer();
    }
  else if ( templateStudy->GetNumberOfMasks() > 0 )
    {
    //SaveImage( templateStudy->GetOneMask().GetPointer(), "template_volume.mha", true );
    // If we have any mask, use it
    return templateStudy->GetOneMask().GetPointer();
    }
  else if ( templateStudy->HasVolume() )
    {
    //SaveImage( templateStudy->GetVolume().GetPointer(), "template_volume.mha", true );
    // If we have a volume, settle for that.
    return templateStudy->GetVolume().GetPointer();
    }
  else
    {
    // No luck.
    return VolumeImageBaseConstPointer();
    }

} // GetTemplateVolume
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
SIMStudiesMaskAligner::AffineTransformConstPointer
SIMStudiesMaskAligner::RecalculateAffineTransform(
  const Study & study ) const
{

  const bool verbose = this->m_SecretData->m_Verbose;

  // extract our template volume
  const VolumeImageBaseConstPointer templateVolume(
    this->GetTemplateVolume() );

  const StudyConstPointer templateStudy( this->GetTemplateStudy() );
  if ( !templateStudy )
    return AffineTransformConstPointer();

  const MaskObjectType alignmentObject = 
    this->m_SecretData->m_ObjectToAlign;

  if ( verbose ) cout << "Object to align = " << alignmentObject << endl;

  // make sure it just has 1's and 0's
  if ( verbose ) cout << "Creating binary masks:" << endl;

  const VolumeMaskImageConstPointer
    fixedMaskOriginal( templateStudy->GetMask( alignmentObject ) );
  if ( !fixedMaskOriginal )
    {
    cerr << "Error: No fixed template mask!" << endl;
    return AffineTransformConstPointer();
    }
    
  const VolumeMaskImageConstPointer
    movingMask( study.GetMask( alignmentObject ) ),
    fixedMask( ResizeImage(
      fixedMaskOriginal.GetPointer(), templateVolume.GetPointer() ) ),
#ifdef NDEBUG
    binaryFixedMask ( CreateBinaryMask( fixedMask .GetPointer() ) ),
    binaryMovingMask( CreateBinaryMask( movingMask.GetPointer() ) );
#else
    binaryFixedMask ( fixedMask  ),
    binaryMovingMask( movingMask );
#endif

  if ( verbose ) cout << "Creating binary masks done." << endl;

  // if our current mask is also the template volume...
  if ( movingMask.GetPointer() == fixedMaskOriginal.GetPointer() )
    {

    if ( verbose )
      cout << "Fixed mask is the template volume. Using identity." << endl;

    // just set it to an identity
    const AffineTransformPointer transform = AffineTransformType::New();
    transform->SetIdentity();
    return transform.GetPointer();
    }

  // none exist; we must register
  const double
    tX = 10,
    tY = 10,
    tZ = 10,
    sX = .10,
    sY = .10,
    sZ = .10,
    rX = 0,
    rY = 0,
    rZ = 0,
    padding = 10;

#ifndef NDEBUG
  const auto
    uniqueFixedValues  = ExtractUniqueValues(
      fixedMask .GetPointer() ),
    uniqueFixedBinaryValues = ExtractUniqueValues(
      binaryFixedMask.GetPointer() ),
    uniqueMovingValues = ExtractUniqueValues(
      movingMask.GetPointer() ),
    uniqueMovingBinaryValues = ExtractUniqueValues(
      binaryMovingMask.GetPointer() );
#endif // NDEBUG

  if ( verbose ) cout << "Registering two masks:" << endl;

  const AffineTransformConstPointer
    transform = dynamic_cast< const AffineTransformType * >(
      RegisterTwoMasks(
        binaryFixedMask, binaryMovingMask, verbose,
        tX, tY, tZ, sX, sY, sZ, rX, rY, rZ, padding ).GetPointer() );

  if ( verbose ) cout << "Registering two masks done." << endl;

#ifndef NDEBUG
  // save them
  const VolumeMaskImageConstPointer movedMask(
    ApplyTransform( transform, binaryMovingMask, binaryFixedMask ) );
    
  SaveImage( movedMask.GetPointer(), std::string(
    boost::lexical_cast< std::string >( study.GetHash() ) ) + std::string( ".mha" ), true );
#endif

  return transform;

} // GetTransform
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void SIMStudiesMaskAligner::RegisterAllStudies()
{

  // "uncache" the bounding box
  this->m_SecretData->m_HaveCachedBoundingBox = false;

  BOOST_FOREACH( const StudyConstPointer & study, this->GetStudies() )
    {

    // register and cache the transform
    this->GetTransform( study );

    // cache the bounding box
    this->GetBoundingBox( study );

    // cache the bounding box image
    this->GetBoundingBoxImage( study );
    
    } // for each study

} // RegisterAllStudies
//////////////////////////////////////////////////////////////////////////////////////////




//////////////////////////////////////////////////////////////////////////////////////////
SIMStudiesMaskAligner::BoundingBoxImageConstPointer
SIMStudiesMaskAligner::CalculateBoundingBoxImage( const Study & study ) const
{
  //SaveImage( study.GetOneMask().GetPointer(),
  //  std::string( "bounding_box_image_" ) + study.GetDescription() + std::string( ".mha" ), true );
    
  // just pick a random mask from the study
  return study.GetOneMask().GetPointer();
}
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
SIMStudiesMaskAligner::BoundingBoxType
SIMStudiesMaskAligner::CalculateBoundingBox( const Study & study ) const
{

  const bool verbose = this->m_SecretData->m_Verbose;

  // first get a "template" mask
  if ( verbose ) cout << "Getting template image:" << endl;
  const auto templateImage = this->GetBoundingBoxImage( study.SuperclassShared::shared_from_this() );
  if ( verbose ) cout << "Getting template image done." << endl;

  // extract our mask objects to
  // use to create the bounding box
  const MaskObjectCollectionType & maskObjects =
    this->m_SecretData->m_BoundingBoxMaskObjects;

  // prepare our masks
  VolumeConstMaskImagesContainer masks;

  using boost::lexical_cast;
  
  // loop through each mask object
  BOOST_FOREACH( const MaskObjectType & maskObject, maskObjects )
    {

    if ( verbose ) cout << "Including object " << maskObject << endl;

    // get the mask for this object
    // and resize it to the template
    if ( verbose ) cout << "Resizing mask:" << endl;

    const VolumeMaskImageConstPointer
      mask( study.GetMask( maskObject ) ),
      resizedMask( ResizeMask( mask, templateImage ) );

      
    if ( verbose ) cout << "Resizing mask done." << endl;

    if ( resizedMask )
      {
      /*
      SaveImage( mask.GetPointer(),
        string( "mask_" ) +
        //lexical_cast< string >( study.GetHash() ) +
        study.GetDescription() +
        string( ".mha" ),
        true );
      SaveImage( resizedMask.GetPointer(),
        string( "resized_mask_" ) +
        //lexical_cast< string >( study.GetHash() ) +
        study.GetDescription() +
        string( ".mha" ),
        true );
      */
      // add it to our list
      masks.push_back( resizedMask );
      }

    } // for each mask object

  // calculate our bounding box
  if ( verbose )
    cout << "Getting bounding box of the union of all objects:" << endl;

  // note that the padding is used for the template bounding box,
  // not the individual bounding boxes
  const BoundingBoxType boundingBox = GetBoundingBoxUnion( masks );
  
  if ( verbose )
    cout << "Getting bounding box of the union of all objects done." << endl;

  /*
  if ( boundingBox.GetNumberOfPixels() > 0 && masks.size() > 0 )
    {
    const VolumeMaskImageConstPointer boundingBoxMask(
      CreateMaskFromBoundingBox(
        boundingBox, *masks.cbegin() ) );
    const string fileName =
      string( "bounding_box_" ) +
      //lexical_cast< string >( study.GetHash() ) +
      study.GetDescription() +
      string( ".mha" );
    SaveImage( boundingBoxMask.GetPointer(), fileName, true );
    }
  */
  
  return boundingBox;

} // CalculateBoundingBox
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
SIMStudiesMaskAligner::BoundingBoxType
SIMStudiesMaskAligner::GetTemplateBoundingBox() const
{

  const bool verbose = this->m_SecretData->m_Verbose;

  // see if we have it cached
  if ( this->m_SecretData->m_HaveCachedBoundingBox )
    {
    cout << "Using cached bounding box." << endl;
    return this->m_SecretData->m_CachedBoundingBox;
    }
    
  // first get a "template" mask
  if ( verbose ) cout << "Getting template image:" << endl;
  const BoundingBoxImageConstPointer templateImage(
    this->GetTemplateVolume() );
  if ( verbose ) cout << "Getting template image done." << endl;

  // extract our mask objects to
  // use to create the bounding box
  const MaskObjectCollectionType & maskObjects =
    this->m_SecretData->m_BoundingBoxMaskObjects;

  // prepare our masks
  VolumeConstMaskImagesContainer masks;

  // loop through each study
  BOOST_FOREACH( const StudyConstPointer & study,
    this->GetStudies() )
    {

    if ( !study ) continue;

    // loop through each mask object
    BOOST_FOREACH( const MaskObjectType & maskObject, maskObjects )
      {

      if ( verbose ) cout << "Including object " << maskObject << endl;

      // get the mask for this object
      // and transform it to the template
      if ( verbose ) cout << "Transforming mask:" << endl;

      const bool extrapolate = false;
      const VolumeMaskImageConstPointer
        mask( study->GetMask( maskObject ) ),
        transformedMask( ApplyTransform(
          this->GetTransform( study ), mask, templateImage, extrapolate ) );

      /*
      const string suffix = maskObject + "_" + study->GetDescription() + ".mha";
      SaveImage( transformedMask.GetPointer(),
        string( "transformed_mask_" ) + suffix, true );
      SaveImage( mask.GetPointer(),
        string( "mask_" ) + suffix, true );
      */
        
      if ( verbose ) cout << "Transforming mask done." << endl;

      // add it to our list
      masks.push_back( transformedMask );

      } // for each mask object
    } // for each study
  
  // calculate our bounding box
  if ( verbose )
    cout << "Getting bounding box of the union of all objects:" << endl;

  // note that the padding is used for the template bounding box,
  // not the individual bounding boxes
  const BoundingBoxType boundingBox =
    GetBoundingBoxUnion( masks, this->m_SecretData->m_BoundingBoxPadding );
  
  if ( verbose )
    cout << "Getting bounding box of the union of all objects done." << endl;

  // cache it
  this->m_SecretData->m_CachedBoundingBox = boundingBox;
  this->m_SecretData->m_HaveCachedBoundingBox = true;
  
  return boundingBox;

} // GetTemplateBoundingBox
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void SIMStudiesMaskAligner::AddStudy( const StudyConstPointer & study )
{

  // see if it already exists
  StudyConstCollectionType studies( this->GetStudies() );

  if ( studies.find( study ) == studies.cend() )
    {
    // nope; insert it
    this->Superclass::AddStudy( study );
    this->m_SecretData->m_HaveCachedBoundingBox = false;
    }

} // AddStudy
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void SIMStudiesMaskAligner::AddStudies( const StudyConstCollectionType & studies )
{
  this->Superclass::AddStudies( studies );
  this->m_SecretData->m_HaveCachedBoundingBox = false;
}
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void SIMStudiesMaskAligner::AddStudies( const StudyCollectionType & studies )
{
  
  this->Superclass::AddStudies( studies );
  this->m_SecretData->m_HaveCachedBoundingBox = false;
}
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void SIMStudiesMaskAligner::ClearStudies()
{
  this->Superclass::ClearStudies();
  this->m_SecretData->m_HaveCachedBoundingBox = false;
}
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void SIMStudiesMaskAligner::SetTemplateStudy( const StudyConstPointer & study )
{
  this->Superclass::SetTemplateStudy( study );
  this->m_SecretData->m_HaveCachedBoundingBox = false;
  this->ClearCache(); // clear the cached transforms
} // SetTemplateStudy
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void SIMStudiesMaskAligner::SetBoundingBoxPadding( const float boundingBoxPadding )
{
  this->m_SecretData->m_BoundingBoxPadding = boundingBoxPadding;
  this->m_SecretData->m_HaveCachedBoundingBox = false;
}
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void SIMStudiesMaskAligner::ClearCache()
{
  this->SuperclassImageCache      ::ClearCache();
  this->SuperclassBoundingBoxCache::ClearCache();
}
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
ccipdSetByvalImplementMacro( SIMStudiesMaskAligner, Verbose      , bool           )
ccipdGetConstImplementMacro( SIMStudiesMaskAligner, Verbose      , bool           )
ccipdSetByrefImplementMacro( SIMStudiesMaskAligner, ObjectToAlign, MaskObjectType )
ccipdGetConstImplementMacro( SIMStudiesMaskAligner, ObjectToAlign, MaskObjectType )
ccipdSetByrefImplementMacro( SIMStudiesMaskAligner,
  BoundingBoxMaskObjects, MaskObjectCollectionType )
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
#ifdef Boost_SERIALIZATION_FOUND

ccipdSerializationImplementationHeadingMacro( SIMStudiesMaskAligner )
{

  if ( version < 1 )
    return;

  using boost::serialization::void_cast_register;
  using boost::serialization::make_nvp;
 
  // http://bit.ly/WKp6BV

  // first, populate the cache with
  // all known affine parameters
  // by registering all images
  this->RegisterAllStudies();

  // now, serialize
  archive
    & BOOST_SERIALIZATION_BASE_OBJECT_NVP( SuperclassBoundingBox )
    & BOOST_SERIALIZATION_BASE_OBJECT_NVP( SuperclassAligner     )
    & make_nvp( "Verbose"               , this->m_SecretData->m_Verbose                )
    & make_nvp( "ObjectToAlign"         , this->m_SecretData->m_ObjectToAlign          )
    & make_nvp( "BoundingBoxMaskObjects", this->m_SecretData->m_BoundingBoxMaskObjects )
    & make_nvp( "BoundingBoxPadding"    , this->m_SecretData->m_BoundingBoxPadding     );
    
} // serialize

ccipdSerializationXMLMacro( SIMStudiesMaskAligner )

#endif // Boost_SERIALIZATION_FOUND
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace
