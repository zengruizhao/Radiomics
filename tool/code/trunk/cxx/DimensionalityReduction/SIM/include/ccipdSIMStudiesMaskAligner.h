#ifndef __ccipdSIMStudiesMaskAligner_h
#define __ccipdSIMStudiesMaskAligner_h



//////////////////////////////////////////////////////////////////////////////////////////

// ccipd includes
#include "ccipdSIMStudiesAligner.h"
#include "ccipdSIMBoundingBoxExtractor.h"
#include "ccipdAffineTypes.h"
#include "ccipdMacros.h"
#include "ccipdFactory.h"

#include "ccipdDisableWarningsMacro.h"
  // itk includes
  #include <itkMacro.h>
#include "ccipdEnableWarningsMacro.h"

// serialization includes
#ifdef Boost_SERIALIZATION_FOUND
  #include <boost/type_traits/is_virtual_base_of.hpp>
#endif // Boost_SERIALIZATION_FOUND
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{




/**
 * This is used to align
 * a collection of masks
 * for training an SIM.
 * 
 * \author  Rachel Sparks
 * \ingroup SIM
 * \date    2013-02-27
 */
class SIMStudiesMaskAligner :
  public SIMStudiesAligner,
  public virtual SIMBoundingBoxExtractor,
  public Factory< SIMStudiesMaskAligner >
{



//////////////////////////////////////////////////////////////////////////////////////////
  ccipdConstructorDeclarationMacro( SIMStudiesMaskAligner )
  ccipdFactorySuperclassTypedefsMacro
//////////////////////////////////////////////////////////////////////////////////////////



public: // typedefs
//////////////////////////////////////////////////////////////////////////////////////////
    typedef SIMBoundingBoxExtractor                 SuperclassBoundingBox;
    typedef SIMStudiesAligner                       SuperclassAligner;
    typedef SuperclassAligner                       Superclass;

    typedef Superclass::TransformType               TransformType;
    typedef Superclass::TransformPointer            TransformPointer;
    typedef Superclass::TransformConstPointer       TransformConstPointer;
    typedef Superclass::LinearTransformType         LinearTransformType;
    typedef Superclass::LinearTransformPointer      LinearTransformPointer;
    typedef Superclass::LinearTransformConstPointer LinearTransformConstPointer;
    typedef Superclass::AffineTransformType         AffineTransformType;
    typedef Superclass::AffineTransformPointer      AffineTransformPointer;
    typedef Superclass::AffineTransformConstPointer AffineTransformConstPointer;

    typedef SuperclassBoundingBox::BoundingBoxType BoundingBoxType;
    typedef SuperclassBoundingBox::BoundingBoxImageConstPointer
      BoundingBoxImageConstPointer;
//////////////////////////////////////////////////////////////////////////////////////////



public: // functions
//////////////////////////////////////////////////////////////////////////////////////////

  /// Which mask (segmentation) object should be used for alignment?
  ccipdSetByrefDeclareMacro( ObjectToAlign, MaskObjectType )
  ccipdGetConstDeclareMacro( ObjectToAlign, MaskObjectType )

  ccipdSetByvalDeclareMacro( Verbose, bool )
  ccipdGetConstDeclareMacro( Verbose, bool )
  itkBooleanMacro          ( Verbose )

  /**
   * \brief Output a volume to be used as a template.
   * 
   * \note
   *   This may change as studies are added or
   *   removed from the collection.
   */
  ccipdGetConstDeclareMacro( TemplateVolume, VolumeImageBaseConstPointer )
  
  /// Set which study all the other should be aligned to.
  ccipdSetByrefDeclareMacro( TemplateStudy, StudyConstPointer )

  /// Add another study to include in the registration.
  virtual void AddStudy( const StudyConstPointer & study );

  virtual void AddStudies( const StudyConstCollectionType & studies );

  virtual void AddStudies( const StudyCollectionType & studies );

  /// Clear all (except the template) studies.
  virtual void ClearStudies();
  
  /// Clear all (except the template) studies.
  virtual void ClearCache();

  /// This makes sure all studies are registered and the result is cached.
  virtual void RegisterAllStudies();

  /**
   * Get the smallest bounding box
   * containing objects
   * of all studies, after
   * registration to
   * the template study.
   */
  ccipdGetConstDeclareMacro( TemplateBoundingBox, BoundingBoxType )
    
  /**
   * Use this to explicitly
   * set which objects to use
   * in creating the bounding
   * box.
   * 
   * By default, the alignment
   * object is used.
   * 
   * Call this before calling
   * GetBoundingBox.
   */
  ccipdSetByrefDeclareMacro( BoundingBoxMaskObjects, MaskObjectCollectionType )

  /// Expand the smallest bounding box by this amount (in mm)
  ccipdSetByvalDeclareMacro( BoundingBoxPadding, float )

//////////////////////////////////////////////////////////////////////////////////////////



protected:
//////////////////////////////////////////////////////////////////////////////////////////
  virtual AffineTransformConstPointer
    RecalculateAffineTransform( const Study & study ) const;

  /** Class to calculate a defined bounding box*/
  virtual BoundingBoxType
    CalculateBoundingBox( const Study & study ) const;

  /**
   * We will use the mask
   * image for the bounding
   * box.
   */
  virtual BoundingBoxImageConstPointer
    CalculateBoundingBoxImage( const Study & study ) const;
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



};
} // namespace



//////////////////////////////////////////////////////////////////////////////////////////
#ifdef Boost_SERIALIZATION_FOUND
BOOST_CLASS_VERSION( ccipd::SIMStudiesMaskAligner, 1 )
BOOST_CLASS_TRACKING(ccipd::SIMStudiesMaskAligner,
  boost::serialization::track_always )

ccipdBoostVirtualBaseTrue(
  ccipd::SIMBoundingBoxExtractor,
  ccipd::SIMStudiesMaskAligner )
#endif
//////////////////////////////////////////////////////////////////////////////////////////



#endif // __ccipdSIMStudiesMaskAligner_h
