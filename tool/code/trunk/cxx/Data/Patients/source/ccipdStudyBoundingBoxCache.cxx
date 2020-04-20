


//////////////////////////////////////////////////////////////////////////////////////////
// ccipd include
#include "ccipdStudyCache.hxx"
#include "ccipdStudyBoundingBoxCache.h"

#include "ccipdDisableWarningsMacro.h"
  // itk includes
  #include <itkImageRegion.h>
#include "ccipdEnableWarningsMacro.h"
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////
StudyBoundingBoxCache::StudyBoundingBoxCache()
{
} // constructor
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
StudyBoundingBoxCache::~StudyBoundingBoxCache()
{
} // destructor
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
StudyBoundingBoxCache::BoundingBoxType StudyBoundingBoxCache::GetBoundingBox(
  const StudyConstPointer & study )
{
  return this->Superclass::GetCachedObject( study );
}
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
StudyBoundingBoxCache::BoundingBoxType StudyBoundingBoxCache::GetBoundingBox(
  const StudyConstPointer & study ) const
{
  return this->Superclass::GetCachedObject( study );
}
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
StudyBoundingBoxCache::BoundingBoxType StudyBoundingBoxCache::CalculateObject(
  const Study & study ) const
{
  return this->CalculateBoundingBox( study );
}
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
#ifdef Boost_SERIALIZATION_FOUND

ccipdSerializationImplementationHeadingMacro( StudyBoundingBoxCache )
{

  if ( version < 1 )
    return;

  // now, serialize
  archive
    & BOOST_SERIALIZATION_BASE_OBJECT_NVP( Superclass );
    
} // serialize

ccipdSerializationXMLMacro( StudyBoundingBoxCache )

#endif
  
} // namespace ccipd

#ifdef Boost_SERIALIZATION_FOUND
template
struct boost::serialization::version<
  ccipd::StudyCache< ccipd::StudyBoundingBoxCache::CachedObjectType > >;

template
struct boost::serialization::tracking_level<
  ccipd::StudyCache< ccipd::StudyBoundingBoxCache::CachedObjectType > >;

template
void ccipd::StudyCache< ccipd::StudyBoundingBoxCache::CachedObjectType >::
serialize< boost::archive::xml_iarchive >(
  boost::archive::xml_iarchive & archive,
  const unsigned int version );

template
void ccipd::StudyCache< ccipd::StudyBoundingBoxCache::CachedObjectType >::
serialize< boost::archive::xml_oarchive >(
  boost::archive::xml_oarchive & archive,
  const unsigned int version );

#endif // Boost_SERIALIZATION_FOUND
//////////////////////////////////////////////////////////////////////////////////////////