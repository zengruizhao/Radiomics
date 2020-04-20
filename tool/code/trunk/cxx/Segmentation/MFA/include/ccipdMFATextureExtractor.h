#ifndef __ccipdMFATextureExtractor_h
#define __ccipdMFATextureExtractor_h



//////////////////////////////////////////////////////////////////////////////////////////

// ccipd includes
#include "ccipdSIMFeatureExtractor.h"
#include "ccipdFactory.h"

// serialization includes
#include "ccipdSegmentationOptions.h"
#ifdef Boost_SERIALIZATION_FOUND
  ccipdSerializationForwardDeclarationMacro
  #include <boost/serialization/version.hpp>
#endif // Boost_SERIALIZATION_FOUND

//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



/**
 * This class provides texture
 * features for the MFA.
 * 
 * By default it starts out
 * with just the intensity feature.
 * 
 * But this can be removed
 * and/or replaced by other features.
 * 
 * \ingroup MFA
 * \author  Rob Toth
 * \date    2012-11-17
 */
class MFATextureExtractor :
  virtual public SIMFeatureExtractor,
  public Factory< MFATextureExtractor >
{



//////////////////////////////////////////////////////////////////////////////////////////
  ccipdConstructorDeclarationMacro( MFATextureExtractor )
  ccipdFactorySuperclassTypedefsMacro

public:
  typedef SIMFeatureExtractor Superclass;

//////////////////////////////////////////////////////////////////////////////////////////

  

public:
//////////////////////////////////////////////////////////////////////////////////////////

  /// Clear our remembered list of texture features to extract.
  virtual void ClearWhichFeatures();

  /// Return the texture features give to this class.
  ccipdGetConstDeclareMacro( WhichFeatureObjects, TextureFeatureEnumCollection )

  /// Override any existing texture features with this collection
  ccipdSetByrefDeclareMacro( WhichFeatureObjects, TextureFeatureEnumCollection )

  /// Add in a new texture feature for extraction.
  virtual void AddTextureFeature( const TextureFeatureEnum whichTextureFeature );

  /// We will say which texture features we are providing.
  virtual FeatureIDConstCollection GetWhichFeatures() const;

//////////////////////////////////////////////////////////////////////////////////////////

  

protected:
//////////////////////////////////////////////////////////////////////////////////////////

  /// We will first preprocess our volume image before extracting textures.
  virtual void PreprocessFeature(
    const FeatureID & whichFeature ) const;

  /// Extract a texture feature from the study.
  virtual VolumeImageConstPointer ExtractFeature(
    const FeatureID & whichFeature ) const;

//////////////////////////////////////////////////////////////////////////////////////////


  
private:
//////////////////////////////////////////////////////////////////////////////////////////
  /** Serialization */
#ifdef Boost_SERIALIZATION_FOUND
  /** We are serializable */
  ccipdSerializationDeclarationMacro
#endif // Boost_SERIALIZATION_FOUND
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
  ccipdPIMPLMacro
//////////////////////////////////////////////////////////////////////////////////////////
  


}; // class MFATextureExtractor
} // namespace ccipd



#ifdef Boost_SERIALIZATION_FOUND
/** 
 * History:
 * 
 * Version 2 added better
 * texture feature serialization.
 */
BOOST_CLASS_VERSION( ccipd::MFATextureExtractor, 2 )
  
//BOOST_CLASS_TRACKING(ccipd::MFATextureExtractor, boost::serialization::track_always )

#endif


#endif // __ccipdMFATextureExtractor_h
