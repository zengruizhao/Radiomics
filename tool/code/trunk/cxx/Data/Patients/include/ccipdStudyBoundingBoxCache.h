#ifndef __ccipdStudyBoundingBoxCache_h
#define __ccipdStudyBoundingBoxCache_h



//////////////////////////////////////////////////////////////////////////////////////////
// ccipd includes
#include "ccipdStudyCache.h"
#include "ccipdVolumeTypes.h"
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



/**
 * This class will be used
 * as a cache of bounding boxes
 * associated with each study.
 * 
 * \author  Rob Toth
 * \date    2013-02-05
 * \ingroup Patients
 */
class StudyBoundingBoxCache :
  public StudyCache< VolumeRegionType >,
  public EnableSharedFromThis< StudyBoundingBoxCache >
{



//////////////////////////////////////////////////////////////////////////////////////////
  ccipdConstructorDeclarationMacro( StudyBoundingBoxCache )
//////////////////////////////////////////////////////////////////////////////////////////



public: // typedefs
//////////////////////////////////////////////////////////////////////////////////////////
  typedef StudyCache< VolumeRegionType > Superclass;
  typedef Superclass::CachedObjectType   CachedObjectType;
  typedef CachedObjectType               BoundingBoxType;
//////////////////////////////////////////////////////////////////////////////////////////



public: // functions
//////////////////////////////////////////////////////////////////////////////////////////

  /**
   * \brief Output the bounding box for a given study.
   * 
   * \note
   *   This version is not const and therefore
   *   will be able to cache the bounding box
   *   so that the next time you call it,
   *   the cached value can be used.
   */
  virtual BoundingBoxType GetBoundingBox(
    const StudyConstPointer & study );

  /**
   * \see GetBoundingBox
   * 
   * \note
   *   This version is const and therefore
   *   will not be able to cache the bounding box.
   *   Therefore, the if the object is already
   *   cached, it will be used. However, if it
   *   is not cached, it is calculated. Yet
   *   the resulting, calculated bounding box
   *   will not be cached for future uses (since
   *   we are const).
   */
  virtual BoundingBoxType GetBoundingBox(
    const StudyConstPointer & study ) const;

//////////////////////////////////////////////////////////////////////////////////////////


  
protected: // functions
//////////////////////////////////////////////////////////////////////////////////////////

  virtual CachedObjectType CalculateObject( const Study & study ) const;

  /**
   * Child classes must provide
   * the ability to calculate
   * a bounding box for a given study.
   */
  virtual CachedObjectType CalculateBoundingBox( const Study & study ) const = 0;

//////////////////////////////////////////////////////////////////////////////////////////

  

private:
//////////////////////////////////////////////////////////////////////////////////////////
  /** Serialization */
#ifdef Boost_SERIALIZATION_FOUND
  /** We are serializable */
  ccipdSerializationDeclarationMacro
#endif // Boost_SERIALIZATION_FOUND
//////////////////////////////////////////////////////////////////////////////////////////



}; // class StudyBoundingBoxCache
} // namespace ccipd



//////////////////////////////////////////////////////////////////////////////////////////
#ifdef Boost_SERIALIZATION_FOUND
BOOST_CLASS_VERSION( ccipd::StudyBoundingBoxCache, 1 )
//BOOST_CLASS_TRACKING(ccipd::StudyBoundingBoxCache, boost::serialization::track_always )
#endif
//////////////////////////////////////////////////////////////////////////////////////////



#endif // __ccipdStudyBoundingBoxCache_h
