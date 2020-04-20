#ifndef __ccipdStudyImageCache_h
#define __ccipdStudyImageCache_h



//////////////////////////////////////////////////////////////////////////////////////////
// ccipd includes
#include "ccipdStudyCache.h"
#include "ccipdVolumeTypes.h"
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



/**
 * This class will be used
 * as a cache of images
 * associated with each study.
 * 
 * \author  Rob Toth
 * \date    2013-02-05
 * \ingroup Patients
 */
class StudyImageCache :
  public StudyCache< VolumeImageBaseConstPointer >,
  public EnableSharedFromThis< StudyImageCache >
{


  
//////////////////////////////////////////////////////////////////////////////////////////
  ccipdConstructorDeclarationMacro( StudyImageCache )
//////////////////////////////////////////////////////////////////////////////////////////



public: // typedefs
//////////////////////////////////////////////////////////////////////////////////////////
  typedef StudyCache< VolumeImageBaseConstPointer > Superclass;
  typedef Superclass::CachedObjectType              CachedObjectType;
  typedef VolumeImageBase                           ImageType;
  typedef CachedObjectType                          ImageConstPointer;
//////////////////////////////////////////////////////////////////////////////////////////



public: // functions
//////////////////////////////////////////////////////////////////////////////////////////

  /**
   * \brief Output the object for a given study.
   * 
   * \note
   *   This version is not const and therefore
   *   will be able to cache the object
   *   so that the next time you call it,
   *   the cached value can be used.
   */
  virtual ImageConstPointer GetImage(
    const StudyConstPointer & study );

  /**
   * \see GetObject
   * 
   * \note
   *   This version is const and therefore
   *   will not be able to cache the object.
   *   Therefore, the if the object is already
   *   cached, it will be used. However, if it
   *   is not cached, it is calculated. Yet
   *   the resulting, calculated object
   *   will not be cached for future uses (since
   *   we are const).
   */
  virtual ImageConstPointer GetImage(
    const StudyConstPointer & study ) const;

//////////////////////////////////////////////////////////////////////////////////////////


  
protected: // functions
//////////////////////////////////////////////////////////////////////////////////////////

  virtual CachedObjectType CalculateObject( const Study & study ) const;

  /**
   * Child classes must provide
   * the ability to calculate
   * an object for a given study.
   */
  virtual ImageConstPointer CalculateImage( const Study & study ) const = 0;

//////////////////////////////////////////////////////////////////////////////////////////


  
private:
//////////////////////////////////////////////////////////////////////////////////////////
  /** Serialization */
#ifdef Boost_SERIALIZATION_FOUND
  /** We are serializable */
  ccipdSerializationDeclarationMacro
#endif // Boost_SERIALIZATION_FOUND
//////////////////////////////////////////////////////////////////////////////////////////



}; // class StudyImageCache
} // namespace ccipd



//////////////////////////////////////////////////////////////////////////////////////////
#ifdef Boost_SERIALIZATION_FOUND
BOOST_CLASS_VERSION( ccipd::StudyImageCache, 1 )
//BOOST_CLASS_TRACKING(ccipd::StudyImageCache, boost::serialization::track_always )
#endif
//////////////////////////////////////////////////////////////////////////////////////////



#endif // __ccipdStudyImageCache_h
