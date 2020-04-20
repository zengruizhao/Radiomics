
//////////////////////////////////////////////////////////////////////////////////////////
// ccipd includes
#include "ccipdFeatureCollection.h"
#include "ccipdFeatureID.h" // our key to the map
#include "ccipdTextureFeatureExtractor.h"
#include "ccipdVolumeContainerToVolumeFeatures.h" // for concatenating features
#include "ccipdExtractFeatureImage.h"
#include "ccipdHashCollection.hxx"
#include "ccipdHashImage.h" // for hashing an image
#include "ccipdResizeImage.h"
#include "ccipdFactory.hxx"
#include "ccipdSaveImage.h"
#include "ccipdDisableWarningsMacro.h"
// std includes
#include <unordered_map> // for our cache


  // itk includes
  #include <itkImage.h> // we need to implement our volume types

  // boost includes
  #include <boost/functional/hash.hpp>
  #include <boost/archive/xml_iarchive.hpp>
  #include <boost/archive/xml_oarchive.hpp> 
#include "ccipdEnableWarningsMacro.h"
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////
template
class Factory< FeatureCollection >;
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
class FeatureCollection::SecretData
{
public: // public to the FeatureCollection class

  SecretData() { }

  /** typedefs */
  typedef std::unordered_map<
    FeatureIDConstPointer,        // our key
    VolumeImageConstPointer, // our value
    HashableFunctor< FeatureIDConstPointer >,
    HashableFunctor< FeatureIDConstPointer >
  > FeaturesCacheType;

  /** Variables */
  TextureFeatureExtractor m_Extractor;
  FeaturesCacheType       m_FeaturesCache;

  /// You provide either a const or non-const feature collection
  template< class TFeatureCollection, class TFeatureIDCollection >
  static VolumeFeaturesPointer GetFeatures(
          TFeatureCollection   & featureCollection,
    const TFeatureIDCollection & whichFeatures );

}; // FeatureCollection::SecretData
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
FeatureCollection::FeatureCollection() :
ccipdNewPIMPLMacro
{
}
FeatureCollection::~FeatureCollection()
{
}
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
bool FeatureCollection::operator==( const Hashable & other ) const
{
  
  if ( this->GetHash() != other.GetHash() )
    return false;

  // cast it to our type
  const FeatureCollection & otherFeatureCollection =
    dynamic_cast< const FeatureCollection & >( other );

  // we've compared to ourself!
  if ( &otherFeatureCollection == this )
    return true;

  // check the variables

  if ( ! ( this->m_SecretData->m_Extractor ==
      otherFeatureCollection.m_SecretData->m_Extractor ) )
    return false;

  typedef SecretData::FeaturesCacheType FeaturesCacheType;
  const FeaturesCacheType
    & ourFeaturesCache   = this->                 m_SecretData->m_FeaturesCache,
    & otherFeaturesCache = otherFeatureCollection.m_SecretData->m_FeaturesCache;

  if ( ourFeaturesCache.size() != otherFeaturesCache.size() )
    return false;

  BOOST_FOREACH( const FeaturesCacheType::value_type ourFeatureCacheElement,
    ourFeaturesCache )
    {

    // does the current cached feature exist in other?
    if ( otherFeaturesCache.find( ourFeatureCacheElement.first ) ==
        otherFeaturesCache.cend() )
      {
      // nope
      return false;
      }

    } // for each study

  // if we've gotten here, we've checked all the variables
  return true;

} // operator==
//////////////////////////////////////////////////////////////////////////////////////////




//////////////////////////////////////////////////////////////////////////////////////////
HashValueType FeatureCollection::ReHash() const
{

  // shortcuts
  using boost::hash_combine;

  // initialize our hasher
  const size_t seed = 0;
  size_t hashValue = seed;

  // hash the extractor
  hash_combine( hashValue, this->m_SecretData->m_Extractor.GetHash() );

  // hash our cache, but sorted
  SortedHashCollectionType sortedCacheHashValues;

  // hash each object and add it to our collection
  sortedCacheHashValues.insert( this->m_SecretData->m_FeaturesCache.size() );
  BOOST_FOREACH( const SecretData::FeaturesCacheType::value_type & cachedValue,
    this->m_SecretData->m_FeaturesCache )
    {

    // hash the feature id
    const FeatureIDConstPointer & featureID = cachedValue.first;
    sortedCacheHashValues.insert( featureID->GetHash() );
    
    // hash the cached feature image (or 0 if NULL)
    const VolumeImageConstPointer & cachedFeature = cachedValue.second;
    sortedCacheHashValues.insert( cachedFeature ? 
      HashImage( cachedFeature.GetPointer() ) : 0 );

    } // for each cache value

  // actually combine all the hash values
  hash_combine( hashValue, GetHashValue( sortedCacheHashValues ) );

  // hash our volume
  const VolumeImageConstPointer volume = this->GetVolume();
  if ( volume )
    {
    hash_combine( hashValue, HashImage( volume.GetPointer() ) );
    }
  else
    {
    hash_combine( hashValue, 0 ); // no volume
    }

  // output
  return hashValue;

} // ReHash
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
VolumeConstImagesContainer FeatureCollection::GetFeature(
  const FeatureIDConstPointer & whichFeature ) const
{
  //SecretData::FeaturesCacheType & cache = this->m_SecretData->m_FeaturesCache;

  // first, check if it's in the map
  // if so, good! return the cached value
  //if ( this->HasFeature( whichFeature ) )
  //  return cache[ whichFeature ];

  if ( whichFeature->HasTextureFeature() )
    {
      // calculate the feature
      const VolumeConstImagesContainer 
        extractedFeature = this->m_SecretData->m_Extractor.GetMultiBandFeature(
          *( whichFeature->GetTextureFeature() ), whichFeature->GetParameters() );
    
      // remember the feature for the future
      //cache[ whichFeature ] = extractedFeature;

      // output
      return extractedFeature;
    } // hastexturefeature
  else
    {
      // not a texture feature, and it hasn't been set
      // so return null
      return VolumeConstImagesContainer();
    }

} // GetFeature
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
//VolumeConstImagesContainer FeatureCollection::GetFeature(
//  const FeatureIDConstPointer & whichFeature ) const
//{
//
//  // only return something if we have it in the cache
//  if ( this->HasFeature( whichFeature ) )
//    return this->m_SecretData->m_FeaturesCache.at( whichFeature );
//  else
//    return NULL;
//  
//} // GetFeatureConst
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
bool FeatureCollection::HasFeature(
  const FeatureIDConstPointer & whichFeature ) const
{

  // extract the cache
  const SecretData::FeaturesCacheType & cache = this->m_SecretData->m_FeaturesCache;

  // check if it exists in the cache
  return cache.find( whichFeature ) != cache.cend();

} // HasFeature
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
VolumeFeaturesConstPointer FeatureCollection::GetFeatures(
  const FeatureIDCollection & whichFeatures ) const
{
  VolumeFeaturesConstPointer features ( SecretData::GetFeatures( *this, whichFeatures ) );
  return features;
} // GetFeatures
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
VolumeFeaturesPointer FeatureCollection::GetFeatures(
  const FeatureIDCollection & whichFeatures )
{
  return SecretData::GetFeatures( *this, whichFeatures );
} // GetFeatures
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
VolumeFeaturesConstPointer FeatureCollection::GetFeatures(
  const FeatureIDConstCollection & whichFeatures ) const
{
  VolumeFeaturesConstPointer features( SecretData::GetFeatures( *this, whichFeatures ) );
  return features;
}
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
VolumeFeaturesPointer FeatureCollection::GetFeatures(
  const FeatureIDConstCollection & whichFeatures )
{
  return SecretData::GetFeatures( *this, whichFeatures );
} // GetFeatures
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
template< class TFeatureCollection, class TFeatureIDCollection >
VolumeFeaturesPointer FeatureCollection::SecretData::GetFeatures(
        TFeatureCollection   & featureCollection,
  const TFeatureIDCollection & whichFeatures )
{

  // prepare the output
  VolumeConstImagesContainer features;

  if ( whichFeatures.empty() )
    return VolumeFeaturesPointer();
  
  // we need to make sure they are the same size - And all of them hold same size as input volume
  const VolumeImageConstPointer templateImage = featureCollection.GetVolume();

  BOOST_FOREACH( const FeatureIDConstPointer whichFeature, whichFeatures )
    {

    // calculate that feature
    // this will call either the const
    // or non-const version based on
    // the template parameter
    VolumeConstImagesContainer featureImage = featureCollection.GetFeature(whichFeature);
    
    
    // input it if one exists
    //if ( featureImage.size() > 0 )
    //  {
      // resize it because they all have to be the same size
        for(unsigned int i = 0; i < featureImage.size(); i++)
          features.push_back(ResizeImage( featureImage.at(i), templateImage ).GetPointer() );
      //}
    } // for featureIndex
  // concatenate and return
  return VolumeContainerToVolumeFeatures( features );
} // GetFeatures
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void FeatureCollection::SetFeature(
  const FeatureIDConstPointer & whichFeature,
  const VolumeImageType       * feature )
{

  // add it to our cache, which is always checked first
  SecretData::FeaturesCacheType & cache = 
    this->m_SecretData->m_FeaturesCache;
  cache[ whichFeature ] = feature;
  this->ClearHash();

} // SetFeature
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void FeatureCollection::UnsetFeature(
  const FeatureIDConstPointer & whichFeature )
{

  // remove it from our cache
  SecretData::FeaturesCacheType & cache = this->m_SecretData->m_FeaturesCache;
  cache.erase( whichFeature );
  this->ClearHash();

} // SetFeature
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void FeatureCollection::SetVolume( const VolumeImageType * const volume )
{

  this->ClearHash();
  this->m_SecretData->m_Extractor.SetVolume( volume );

} // SetVolume
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
VolumeImageConstPointer FeatureCollection::GetVolume() const
{

  return this->m_SecretData->m_Extractor.GetVolume();

} // GetVolume
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void FeatureCollection::SetVerbose( const bool verbose )
{

  this->m_SecretData->m_Extractor.SetVerbose( verbose );

} // SetVolume
//////////////////////////////////////////////////////////////////////////////////////////




//////////////////////////////////////////////////////////////////////////////////////////
bool FeatureCollection::GetVerbose() const
{

  return this->m_SecretData->m_Extractor.GetVerbose();

} // SetVolume
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
FeatureIDConstCollection FeatureCollection::GetWhichFeatures() const
{

  // initialize the collection
  FeatureIDConstCollection whichFeatures;

  // go through the cache
  BOOST_FOREACH( const auto & value, this->m_SecretData->m_FeaturesCache )
    {
    // insert the feature
    whichFeatures.push_back( value.first );
    }

  // output
  return whichFeatures;

} // GetWhichFeatures
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace
