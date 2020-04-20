#ifndef __ccipdFeatureCollection_h
#define __ccipdFeatureCollection_h



//////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdVolumeTypes.h"
#include "ccipdFeatureCollectionTypes.h"
#include "ccipdHashable.h" // we are hashable
#include "ccipdMacros.h"
#include "ccipdFactory.h"
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



/**
 * \brief Store a collection of features.
 * 
 * This class will store a bunch of features
 * for an image.
 * 
 * It will intelligently cache the features
 * so that if you request a feature that's
 * already been calculated, or already
 * been set, it won't recompute that feature.
 * 
 * It is designed to output a ccipd::VolumeFeaturesType
 * of your requested features.
 * 
 * If you don't want a texture feature to
 * be calculated, you may set it. This will
 * make sure that the set version of the feature
 * is returned.
 * 
 * \author  Rob Toth
 * \ingroup FeatureCollection
 */
class FeatureCollection :
  public Hashable,
  public Factory< FeatureCollection >
{



//////////////////////////////////////////////////////////////////////////////////////////
  ccipdConstructorDeclarationMacro( FeatureCollection )
  ccipdFactorySuperclassTypedefsMacro
//////////////////////////////////////////////////////////////////////////////////////////



public:
//////////////////////////////////////////////////////////////////////////////////////////
  /// Comparison with another FeatureCollection.
  virtual bool operator==( const Hashable & other ) const;
//////////////////////////////////////////////////////////////////////////////////////////



protected:
//////////////////////////////////////////////////////////////////////////////////////////
  /// We want to be hashable.
  virtual HashValueType ReHash() const;
//////////////////////////////////////////////////////////////////////////////////////////



public:
//////////////////////////////////////////////////////////////////////////////////////////
  /** Set/Get functions*/

  /// Which volume will we be extracting features from?
  ccipdSetByvalDeclareMacro( Volume, VolumeImageType *       )
  ccipdGetConstDeclareMacro( Volume, VolumeImageConstPointer )

  /// Do we have an image volume stored?
  bool HasVolume() const;

  ccipdSetByvalDeclareMacro( Verbose, bool ) ///< Show progress?
  ccipdGetConstDeclareMacro( Verbose, bool ) ///< Show progress?

//////////////////////////////////////////////////////////////////////////////////////////



public:
//////////////////////////////////////////////////////////////////////////////////////////

  /// Function to actually extract a feature
  virtual VolumeConstImagesContainer GetFeature(
    const FeatureIDConstPointer & whichFeature ) const;

  /**
   * \brief Constant version of GetFeature().
   * 
   * This function will never recalculate a 
   * feature that isn't already in the cache,
   * so be careful that you have already
   * set the feature, or already have
   * calculated the feature at least once
   * before calling this function.
   * 
   * \returns NULL if the feature isn't in the cache.
   */
  //virtual VolumeConstImagesContainer GetFeature(
  //  const FeatureIDConstPointer & whichFeature ) const;

  /// Do we have the given feature already cached?
  virtual bool HasFeature(
    const FeatureIDConstPointer & whichFeature ) const;

  /// Function to extract multiple features 
  virtual VolumeFeaturesPointer GetFeatures(
    const FeatureIDCollection & whichFeatures );
  
  /// Function to extract multiple features 
  virtual VolumeFeaturesPointer GetFeatures(
    const FeatureIDConstCollection & whichFeatures );

  /**
   * \brief Constant version of GetFeatures().
   * 
   * This function will never recalculate a 
   * feature that isn't already in the cache,
   * so be careful that you have already
   * set each feature, or already have
   * calculated each feature at least once
   * before calling this function.
   */
  virtual VolumeFeaturesConstPointer GetFeatures(
    const FeatureIDCollection & whichFeatures ) const;
 
  /**
   * \see GetFeaturesConst()
   */
  virtual VolumeFeaturesConstPointer GetFeatures(
    const FeatureIDConstCollection & whichFeatures ) const;

  /**
   * \brief Set the feature in the cache.
   * 
   * Function to overwrite a feature with your desired one.
   * After you call this, GetFeature will extract it.
   * 
   * No feature calculation will ever be performed
   * for something you've set.
   */
  virtual void SetFeature(
    const FeatureIDConstPointer & whichFeature,
    const VolumeImageType * const feature );

  /**
   * \brief Remove feature from cache.
   * 
   * This will remove a set feature from the cache, forcing a 
   * calculation of that feature.
   */
  virtual void UnsetFeature(
    const FeatureIDConstPointer & whichFeature );
  
  /// Output the features we are storing.
  ccipdGetConstDeclareMacro( WhichFeatures, FeatureIDConstCollection )

//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
  /// To store our internal data types
  ccipdPIMPLMacro
//////////////////////////////////////////////////////////////////////////////////////////



}; // class TextureFeatureExtractor
} // namespace



#endif // __ccipdFeatureCollection_h
