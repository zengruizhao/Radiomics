#ifndef __ccipdMFABoundingBoxRandomizer_h
#define __ccipdMFABoundingBoxRandomizer_h



//////////////////////////////////////////////////////////////////////////////////////////
// ccipd includes
#include "ccipdSIMBoundingBoxExtractor.h"

// itk includes
#include <itkMacro.h>

// serialization includes
#ifdef Boost_SERIALIZATION_FOUND
  #include <boost/type_traits/is_virtual_base_of.hpp>
#endif // Boost_SERIALIZATION_FOUND
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



/**
 * \brief Base class for extracting random bounding boxes.
 * 
 * \ingroup MFA
 * \author  Rob Toth
 * \date    2013-02-03
 */
class MFABoundingBoxRandomizer :
  public virtual SIMBoundingBoxExtractor,
  public EnableSharedFromThis< MFABoundingBoxRandomizer >
{



//////////////////////////////////////////////////////////////////////////////////////////
  ccipdConstructorDeclarationMacro( MFABoundingBoxRandomizer )

public:
  typedef SIMBoundingBoxExtractor       Superclass;
  typedef Superclass::BoundingBoxType   BoundingBoxType;
  typedef Superclass::ImageType         BoundingBoxImage;
  typedef Superclass::ImageConstPointer BoundingBoxImageConstPointer;

//////////////////////////////////////////////////////////////////////////////////////////



public:
//////////////////////////////////////////////////////////////////////////////////////////

  /**
   * We are overriding this (non-const) function
   * so that the bounding boxes are never
   * cached.
   * 
   * This is because you may be randomizing
   * the bounding boxes, and if so,
   * you don't want to cache anything
   * for a given study.
   */
  BoundingBoxType GetBoundingBox( const StudyConstPointer & study );

  BoundingBoxType GetBoundingBox( const StudyConstPointer & study ) const;

  BoundingBoxType GetNonRandomBoundingBox(
    const StudyConstPointer & study );
  BoundingBoxType GetNonRandomBoundingBox(
    const StudyConstPointer & study ) const;
  
  BoundingBoxType GetRandomBoundingBox(
    const StudyConstPointer & study );
  BoundingBoxType GetRandomBoundingBox(
    const StudyConstPointer & study ) const;

  /**
   * If true (default),
   * then the bounding box will be
   * randomly expanded.
   */
  ccipdSetByvalDeclareMacro( Randomize, bool )

  /// \see SetRandomize
  ccipdGetConstDeclareMacro( Randomize, bool )

  /// \see SetRandomize
  itkBooleanMacro( Randomize )

  /// This is hard-coded in itkImageBase.h ...
  typedef double MillimeterType;

  /**
   * Set the peak number of mm
   * to expand the bounding box.
   * 
   * default = 1
   * 
   * \note
   *   This corresponds to the
   *   MODE not the mean.
   */
   ccipdSetByvalDeclareMacro( ExpansionPeak, MillimeterType )
   ccipdGetConstDeclareMacro( ExpansionPeak, MillimeterType )

  /**
   * Set the standard deviation (in mm)
   * to expand the bounding box.
   * 
   * default = 1
   */
   ccipdSetByvalDeclareMacro( ExpansionStandardDeviation, MillimeterType )
   ccipdGetConstDeclareMacro( ExpansionStandardDeviation, MillimeterType )

//////////////////////////////////////////////////////////////////////////////////////////


protected:
//////////////////////////////////////////////////////////////////////////////////////////

  /**
   * We will choose whether
   * or not to get a random bounding
   * box.
   */
  virtual BoundingBoxType
    CalculateBoundingBox( const Study & study ) const;

  /**
   * We will randomize the bounding
   * box corners.
   */
  virtual BoundingBoxType
    CalculateRandomBoundingBox( const Study & study ) const;

  /**
   * Child classes must provide
   * the ability to get
   * the original, non-random
   * bounding box for a study.
   */
  virtual BoundingBoxType
    CalculateNonRandomBoundingBox( const Study & study ) const = 0;

//////////////////////////////////////////////////////////////////////////////////////////
  


//////////////////////////////////////////////////////////////////////////////////////////
  ccipdPIMPLMacro
//////////////////////////////////////////////////////////////////////////////////////////


  
private:
//////////////////////////////////////////////////////////////////////////////////////////
  /** Serialization */
#ifdef Boost_SERIALIZATION_FOUND
  /** We are serializable */
  ccipdSerializationDeclarationMacro
#endif // Boost_SERIALIZATION_FOUND
//////////////////////////////////////////////////////////////////////////////////////////



}; // class MFABoundingBoxRandomizer
} // namespace ccipd



//////////////////////////////////////////////////////////////////////////////////////////
#ifdef Boost_SERIALIZATION_FOUND
BOOST_CLASS_VERSION( ccipd::MFABoundingBoxRandomizer, 1 )
BOOST_CLASS_TRACKING(ccipd::MFABoundingBoxRandomizer,
  boost::serialization::track_always )

  
ccipdBoostVirtualBaseTrue(
  ccipd::SIMBoundingBoxExtractor,
  ccipd::MFABoundingBoxRandomizer )
#endif
//////////////////////////////////////////////////////////////////////////////////////////



#endif // __ccipdMFABoundingBoxRandomizer_h
