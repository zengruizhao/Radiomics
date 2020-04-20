#ifndef __ccipdMFALevelsetExtractor_h
#define __ccipdMFALevelsetExtractor_h



//////////////////////////////////////////////////////////////////////////////////////////

// ccipd includes
#include "ccipdMFATypes.h"
#include "ccipdSIMFeatureExtractor.h"
#include "ccipdPatientTypes.h" // for the mask object type
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
 * This class provides levelset
 * features for the MFA.
 * 
 * \ingroup MFA
 * \author  Rob Toth
 * \date    2012-11-17
 */
class MFALevelsetExtractor :
  virtual public MFAFeatureExtractor,
  public Factory< MFALevelsetExtractor >
{



//////////////////////////////////////////////////////////////////////////////////////////
  ccipdConstructorDeclarationMacro( MFALevelsetExtractor )
  ccipdFactorySuperclassTypedefsMacro

public:
  typedef MFAFeatureExtractor Superclass;

//////////////////////////////////////////////////////////////////////////////////////////

  

public:
//////////////////////////////////////////////////////////////////////////////////////////

  /// Say which objects to extract levelsets from
  ccipdSetByrefDeclareMacro( WhichLevelsetObjects, MaskObjectCollectionType )
  ccipdGetConstDeclareMacro( WhichLevelsetObjects, MaskObjectCollectionType )

  /**
   * How "wide" of a sigmoid (curved part) to create from the levelset?
   * 
   * default is 4.0
   * 
   * \note This is ignored if sigmoid levelset is set to false.
   */
  ccipdSetByvalDeclareMacro( SigmoidWidth, float )
  ccipdGetConstDeclareMacro( SigmoidWidth, float )

  /**
   * Smooth the levelset? 
   * 
   * default is 0.0 (no smoothing)
   * 
   * \note This "shrinks" the levelset slightly.
   */
  ccipdSetByvalDeclareMacro( SmoothingVariance, float )
  ccipdGetConstDeclareMacro( SmoothingVariance, float )

  /**
   * Turn on or off the creation
   * of a sigmoid levelset.
   * 
   * \note
   *   If this is on, then
   *   the levelset no longer represents
   *   signed distances.
   * 
   * default = false
   */
  ccipdSetByvalDeclareMacro( SigmoidLevelset, bool )
  ccipdGetConstDeclareMacro( SigmoidLevelset, bool )
  itkBooleanMacro          ( SigmoidLevelset )

  /**
   * Turn on or off normalizing
   * the levelset.
   * 
   * \note
   *   If this is on, then
   *   the levelset no longer represents
   *   signed distances.
   * 
   * default = false
   */
  ccipdSetByvalDeclareMacro( NormalizeLevelset, bool )
  ccipdGetConstDeclareMacro( NormalizeLevelset, bool )
  itkBooleanMacro          ( NormalizeLevelset )

  /// We will say which levelset features we are providing.
  virtual FeatureIDConstCollection GetWhichFeatures() const;
  
  /**
   * Helper function to extract the object name.
   * 
   * Returns an empty string on any issues.
   */
  static MaskObjectType ExtractObjectNameFromFeatureID(
    const FeatureID & whichFeature );

  /// Helper function to create a feature with the given object name.
  static FeatureIDConstPointer CreateFeatureIDWithObject(
    const MaskObjectType & objectName );

//////////////////////////////////////////////////////////////////////////////////////////

  

protected:
//////////////////////////////////////////////////////////////////////////////////////////

  /// Extract a levelset feature from the study.
  virtual VolumeImageConstPointer ExtractFeature(
    const FeatureID & whichFeature ) const;
  
  /**
   * After things such as transformations,
   * the levelset no longer represents
   * distances.
   * 
   * This will recompute the levelset
   * to make sure it represents what
   * it's supposed to represent.
   */
  virtual VolumeImageConstPointer PostprocessFeature(
    const VolumeImageConstPointer & feature ) const;

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
  


}; // class MFALevelsetExtractor
} // namespace ccipd



#ifdef Boost_SERIALIZATION_FOUND
BOOST_CLASS_VERSION( ccipd::MFALevelsetExtractor, 1 )
//BOOST_CLASS_TRACKING(ccipd::MFALevelsetExtractor, boost::serialization::track_always )
#endif



#endif // __ccipdMFALevelsetExtractor_h
