#ifndef __ccipdMFASegmenterCorrection_h
#define __ccipdMFASegmenterCorrection_h



//////////////////////////////////////////////////////////////////////////////////////////

// ccipd includes
#include "ccipdMFASegmenter.h"
#include "ccipdLandmarkTypes.h"
#include "ccipdLoadableFactory.h"

// serialization includes
#ifdef Boost_SERIALIZATION_FOUND
#include <boost/type_traits/is_virtual_base_of.hpp>
#include <boost/serialization/version.hpp>
#include <boost/serialization/tracking.hpp>
#endif // Boost_SERIALIZATION_FOUND

//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



/**
 * \brief This class will correct MFA results using TPS.
 * 
 * \author  Rob Toth
 * \ingroup MFA
 */
class MFA_EXPORT MFASegmenterCorrection : // class declaration
  public MFASegmenter,
  public Serializable   < MFASegmenterCorrection >,
  public LoadableFactory< MFASegmenterCorrection >
{

  

//////////////////////////////////////////////////////////////////////////////////////////
  ccipdConstructorDeclarationMacro( MFASegmenterCorrection )

public:
  typedef MFASegmenter                    Superclass;
  typedef Serializable       < Self >     SuperclassSerializable;
  typedef LoadableFactory    < Self >     SuperclassFactory;
  ccipdFactoryTypedefsMacro

//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////

public:
  /// Just call SuperclassSerializable's
  virtual bool Load( const std::string & fileName );
  virtual bool Save( const std::string & fileName ) const;

//////////////////////////////////////////////////////////////////////////////////////////



public:
//////////////////////////////////////////////////////////////////////////////////////////

  /**
   * Calculate and output the reconstruction for a feature
   * in the coordinate frame of the feature after
   * adding in the correction transform.
   */
  virtual VolumeImagePointer GetReconstruction(
    const FeatureIDConstPointer & whichFeature ) const;

  /**
   * Output the uncorrected reconstruction.
   */
  virtual VolumeImagePointer GetReconstructionWithoutCorrection(
    const FeatureIDConstPointer & whichFeature ) const;
  
  /// In the original image's coordinate frame.
  virtual VolumeImagePointer GetReconstructedLevelsetWithoutCorrection(
    const MaskObjectType & whichObject ) const;

  virtual VolumeMaskImagePointer GetSegmentationWithoutCorrection(
    const MaskObjectType & whichObject ) const;

  /**
   * Give some points known
   * to be on the surface of
   * an object to create a correction.
   */
  virtual void SetKnownSurfacePoints(
    const MaskObjectType          & whichObject,
    const LandmarkSetConstPointer & points );

  /**
   * Output the known surface points
   * for a given object. If none exist,
   * then output a null pointer.
   */
  virtual LandmarkSetConstPointer GetKnownSurfacePoints(
    const MaskObjectType & whichObject ) const;

  /// Clear all input points but keep any existing correction
  virtual void ClearKnownSurfacePoints();

  /// Clear all input surface points and correction
  virtual void ClearCorrectionAndKnownSurfacePoints();

  /// Keep the surface points, but clear any correction
  virtual void ClearCorrection();

  /**
   * Output the last-generated
   * random fixed points.
   */
  ccipdGetConstDeclareMacro( RandomFixedPoints, LandmarkSetConstPointer )

  /**
   * Use the image corners as fixed points
   * to better constrain the correction.
   * 
   * default = true
   */
  ccipdSetByvalDeclareMacro( FixCorners, bool )
  ccipdGetConstDeclareMacro( FixCorners, bool )
  itkBooleanMacro          ( FixCorners )

  /**
   * Use random points away from
   * the segmentation as fixed.
   * 
   * default = 0
   */
   ccipdSetByvalDeclareMacro( NumberOfRandomFixedPoints, unsigned int )
   ccipdGetConstDeclareMacro( NumberOfRandomFixedPoints, unsigned int )

   /**
    * No anchor points will be
    * allowed this many mm from
    * the manually input
    * known points.
    * 
    * (default = 3 mm)
    */
   ccipdSetByvalDeclareMacro( ForbiddenRadius, TransformValueType )
   ccipdGetConstDeclareMacro( ForbiddenRadius, TransformValueType )

   /**
    * Fix some anchor points
    * on the surface of each object.
    * 
    * This decides how many
    * anchor points per object there are.
    * 
    * (default = 0)
    */
  virtual void SetNumberOfAnchorSurfacePoints(
    const MaskObjectType & whichObject,
    const unsigned int     numberOfAnchorSurfacePoints );

  virtual unsigned int GetNumberOfAnchorSurfacePoints(
    const MaskObjectType & whichObject ) const;

  /**
   * You must call this function
   * when you are done setting
   * all the known landmark points,
   * or the reconstruction won't
   * use the correction.
   */
  virtual void RegenerateCorrection();

//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
  ccipdPIMPLMacro
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
#ifdef Boost_SERIALIZATION_FOUND
  /** We are serializable */
  ccipdSerializationDeclarationMacro
#endif // Boost_SERIALIZATION_FOUND
//////////////////////////////////////////////////////////////////////////////////////////



}; // class MFASegmenterCorrection
} // namespace



//////////////////////////////////////////////////////////////////////////////////////////
#ifdef Boost_SERIALIZATION_FOUND
  
BOOST_CLASS_VERSION( ccipd::MFASegmenterCorrection, 1 )

BOOST_CLASS_TRACKING( ccipd::MFASegmenterCorrection,
  boost::serialization::track_always )
  
ccipdBoostVirtualBaseFalse(
  ccipd::MFASegmenter,
  ccipd::MFASegmenterCorrection )

#endif // Boost_SERIALIZATION_FOUND
//////////////////////////////////////////////////////////////////////////////////////////



#endif // __ccipdMFASegmenterCorrection_h
