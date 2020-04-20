#ifndef __ccipdTextureFeatureExtractor_h
#define __ccipdTextureFeatureExtractor_h



//////////////////////////////////////////////////////////////////////////////////////////
// ccipd includes
#include "ccipdVolumeTypes.h"
#include "ccipdFeatureCollectionTypes.h"
#include "ccipdHashable.h"
#include "ccipdMacros.h"
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{

/**
 * \brief Extract texture features from an image.
 * 
 * \author  Rob Toth
 * \ingroup FeatureCollection
 */
class TextureFeatureExtractor : public Hashable
{



public:
//////////////////////////////////////////////////////////////////////////////////////////
  TextureFeatureExtractor();
  ~TextureFeatureExtractor();
//////////////////////////////////////////////////////////////////////////////////////////



public:
//////////////////////////////////////////////////////////////////////////////////////////
  /** Make sure we can compare it to another one. */
  virtual bool operator==( const Hashable & other ) const;
//////////////////////////////////////////////////////////////////////////////////////////



protected:
//////////////////////////////////////////////////////////////////////////////////////////
  /** Make sure we are hashable */
  virtual HashValueType ReHash() const;
//////////////////////////////////////////////////////////////////////////////////////////



public:
//////////////////////////////////////////////////////////////////////////////////////////
  /** Set/Get functions*/

  /// Which volume will we be extracting features from?
  ccipdSetByvalDeclareMacro( Volume, VolumeImageType *       )
  ccipdGetConstDeclareMacro( Volume, VolumeImageConstPointer )
  bool HasVolume() const; ///< Has a volume been set?.

  ccipdSetByvalDeclareMacro( Verbose, bool ) ///< Show progress?
  ccipdGetConstDeclareMacro( Verbose, bool ) ///< Show progress?
//////////////////////////////////////////////////////////////////////////////////////////



public:
//////////////////////////////////////////////////////////////////////////////////////////
  /// Functions to actually extract texture features.
  /*virtual VolumeImageConstPointer GetFeature(  const TextureFeatureEnum            whichFeature
                                             , const AnyTextureFeatureParameters & parameters 
                                            );*/

  virtual VolumeConstImagesContainer GetMultiBandFeature(  const TextureFeatureEnum            whichFeature
                                                    , const AnyTextureFeatureParameters & parameters
                                                   );

   /// You input a collection of features and it outputs a feature image.
  virtual VolumeFeaturesPointer GetFeatures( const TextureFeatureEnumCollection & whichFeatures, const TextureFeatureParametersCollection & whichParameters );

  /// Extract all known texture features. This will only extract using default parameters
  virtual VolumeFeaturesPointer GetAllFeatures();
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
  /// To store our internal data types.
  ccipdPIMPLMacro
//////////////////////////////////////////////////////////////////////////////////////////



}; // class TextureFeatureExtractor



} // namespace
#endif // __ccipdTextureFeatureExtractor_h
