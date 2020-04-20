#ifndef __ccipdSIMBoundingBoxExtractor_h
#define __ccipdSIMBoundingBoxExtractor_h



//////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdSIMTypes.h"
#include "ccipdStudyImageCache.h"
#include "ccipdStudyBoundingBoxCache.h"
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



/**
 * \brief Base class for extracting bounding boxes.
 * 
 * \todo Caching.
 * 
 * \ingroup MFA
 * \author  Rob Toth
 * \date    2013-02-03
 */
class SIMBoundingBoxExtractor :
  public StudyImageCache,
  public StudyBoundingBoxCache,
  public EnableSharedFromThis< SIMBoundingBoxExtractor >
{



//////////////////////////////////////////////////////////////////////////////////////////
  ccipdConstructorDeclarationMacro( SIMBoundingBoxExtractor )

public:
  typedef StudyImageCache          SuperclassImageCache;
  typedef StudyBoundingBoxCache    SuperclassBoundingBoxCache;

  typedef SuperclassBoundingBoxCache::BoundingBoxType BoundingBoxType;
  typedef SuperclassImageCache::ImageType             BoundingBoxImage;
  typedef SuperclassImageCache::ImageConstPointer     BoundingBoxImageConstPointer;

//////////////////////////////////////////////////////////////////////////////////////////



public:
//////////////////////////////////////////////////////////////////////////////////////////

  BoundingBoxImageConstPointer GetBoundingBoxImage(
    const StudyConstPointer & study );
    
  BoundingBoxImageConstPointer GetBoundingBoxImage(
    const StudyConstPointer & study ) const;
  
  /**
   * \brief Output a volume to be used as a template.
   */
  virtual VolumeImageBaseConstPointer GetTemplateVolume() const = 0;

  /**
   * Get a bounding box.
   */
  virtual VolumeRegionType GetTemplateBoundingBox() const = 0;

//////////////////////////////////////////////////////////////////////////////////////////


protected:
//////////////////////////////////////////////////////////////////////////////////////////

  /**
   * Child classes must provide
   * the ability to get
   * the image for a given study
   * from which the bounding box
   * will be computed.
   */
  virtual BoundingBoxImageConstPointer
    CalculateBoundingBoxImage( const Study & study ) const = 0;

  virtual BoundingBoxImageConstPointer
    CalculateImage( const Study & study ) const;

//////////////////////////////////////////////////////////////////////////////////////////
  

  
private:
//////////////////////////////////////////////////////////////////////////////////////////
  /** Serialization */
#ifdef Boost_SERIALIZATION_FOUND
  /** We are serializable */
  ccipdSerializationDeclarationMacro
#endif // Boost_SERIALIZATION_FOUND
//////////////////////////////////////////////////////////////////////////////////////////



}; // class SIMBoundingBoxExtractor
} // namespace ccipd



//////////////////////////////////////////////////////////////////////////////////////////
#ifdef Boost_SERIALIZATION_FOUND
BOOST_CLASS_VERSION( ccipd::SIMBoundingBoxExtractor, 1 )
BOOST_CLASS_TRACKING(ccipd::SIMBoundingBoxExtractor, boost::serialization::track_always )
#endif
//////////////////////////////////////////////////////////////////////////////////////////



#endif // __ccipdSIMBoundingBoxExtractor_h
