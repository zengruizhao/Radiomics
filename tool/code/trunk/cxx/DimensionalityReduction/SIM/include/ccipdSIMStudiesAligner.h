#ifndef __ccipdSIMStudiesAligner_h
#define __ccipdSIMStudiesAligner_h



//////////////////////////////////////////////////////////////////////////////////////////
// ccipd includes
#include "ccipdStudyAffineAligner.h"

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
 * This is used to abstract base for all 
 * studies aligners that want to interface 
 * with objects in the SIM class. 
 * 
 * \author  Rachel Sparks
 * \ingroup SIM
 * \date    2013-03-01
 */
class SIMStudiesAligner :
  public StudyAffineAligner,
  public EnableSharedFromThis< SIMStudiesAligner >
{



//////////////////////////////////////////////////////////////////////////////////////////
  ccipdConstructorDeclarationMacro( SIMStudiesAligner )
//////////////////////////////////////////////////////////////////////////////////////////



public: // typedefs
//////////////////////////////////////////////////////////////////////////////////////////
    typedef StudyAffineAligner                      Superclass;
    typedef Superclass::TransformType               TransformType;
    typedef Superclass::TransformPointer            TransformPointer;
    typedef Superclass::TransformConstPointer       TransformConstPointer;
    typedef Superclass::LinearTransformType         LinearTransformType;
    typedef Superclass::LinearTransformPointer      LinearTransformPointer;
    typedef Superclass::LinearTransformConstPointer LinearTransformConstPointer;
    typedef Superclass::AffineTransformType         AffineTransformType;
    typedef Superclass::AffineTransformPointer      AffineTransformPointer;
    typedef Superclass::AffineTransformConstPointer AffineTransformConstPointer;
//////////////////////////////////////////////////////////////////////////////////////////



public: // functions
//////////////////////////////////////////////////////////////////////////////////////////
  ccipdSetByvalDeclareMacro( Verbose, bool )
  itkBooleanMacro          ( Verbose )  
  
  /**
   * If false (default), then
   * this will first try to see
   * if the Study has an existing
   * affine transform stored in it.
   * If so, it will use it. If not,
   * it will recalculate the affine
   * transform.
   * 
   * If true, then this will ignore any
   * existing, stored affine transform
   * in the study, and always calculate
   * the affine (except if it's cached).
   * 
   * \date 2013-03-07
   */
  ccipdSetByvalDeclareMacro( RecalculateTransform, bool )
  ccipdGetConstDeclareMacro( RecalculateTransform, bool )
  itkBooleanMacro          ( RecalculateTransform )  
  
  /// Set which study all the other should be aligned to.
  ccipdSetByrefDeclareMacro( TemplateStudy, StudyConstPointer )
  ccipdGetConstDeclareMacro( TemplateStudy, StudyConstPointer )

  /// Add another study to include in the registration.
  virtual void AddStudy( const StudyConstPointer & study );

  virtual void AddStudies( const StudyConstCollectionType & studies );

  virtual void AddStudies( const StudyCollectionType & studies );

  /// Clear all (except the template) studies.
  virtual void ClearStudies();
  
  /// Clear all (except the template) studies.
  virtual void ClearCache();

  /// Children must implement registration of all the studies
  virtual void RegisterAllStudies() = 0;

////////////////////////////////////////////////////////////////////////////////////////// 
  


protected:
//////////////////////////////////////////////////////////////////////////////////////////

  ccipdGetConstDeclareMacro( Studies,       StudyConstCollectionType )

  virtual AffineTransformConstPointer
    CalculateAffineTransform( const Study & study ) const;

  virtual AffineTransformConstPointer
    RecalculateAffineTransform( const Study & study ) const = 0;

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

/**
 * 2013-03-07:
 *   Version 2 added support for loading in
 *   the study's affine transform and using that.
 *
 */
BOOST_CLASS_VERSION( ccipd::SIMStudiesAligner, 2 )

BOOST_CLASS_TRACKING(ccipd::SIMStudiesAligner,
  boost::serialization::track_always )
#endif
//////////////////////////////////////////////////////////////////////////////////////////



#endif // __ccipdSIMStudiesAligner_h