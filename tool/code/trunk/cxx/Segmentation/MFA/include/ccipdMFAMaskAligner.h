#ifndef __ccipdMFAMaskAligner_h
#define __ccipdMFAMaskAligner_h



//////////////////////////////////////////////////////////////////////////////////////////

// ccipd includes
#include "ccipdSIMStudiesMaskAligner.h"
#include "ccipdMFABoundingBoxRandomizer.h"
#include "ccipdAffineTypes.h"

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
 * for training an MFA.
 * 
 * \author  Rob Toth
 * \ingroup MFA
 * \date    2012-11-15
 */
class MFAMaskAligner :
  public virtual SIMStudiesMaskAligner,
  public virtual MFABoundingBoxRandomizer,
  public Factory< MFAMaskAligner >
{



//////////////////////////////////////////////////////////////////////////////////////////
  ccipdConstructorDeclarationMacro( MFAMaskAligner )
  ccipdFactorySuperclassTypedefsMacro

public: // typedefs
    typedef MFABoundingBoxRandomizer                SuperclassBoundingBox;
    typedef SIMStudiesMaskAligner                   SuperclassAligner;
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
  
  /// Get a distribution representing a distribution of affine parameters
  ccipdGetDeclareMacro( AffineDistribution, AffineDistributionPointer )

  /// This makes sure all studies are registered and the result is cached.
  virtual void RegisterAllStudies();
//////////////////////////////////////////////////////////////////////////////////////////



protected:
//////////////////////////////////////////////////////////////////////////////////////////  

  /** Class to calculate a defined bounding box. We need to define which superclass
     This function is inherited from */
  virtual BoundingBoxType
    CalculateBoundingBox( const Study & study ) const;


  /**
   * We will extract a bounding
   * box from a study using
   * a given mask.
   */
  virtual BoundingBoxType CalculateNonRandomBoundingBox(
    const Study & study ) const;
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
 * 2013-03-12
 *   Version 2 removed verbose since it's in the base class.
 */
BOOST_CLASS_VERSION( ccipd::MFAMaskAligner, 2 )

BOOST_CLASS_TRACKING(ccipd::MFAMaskAligner,
  boost::serialization::track_always )

  
ccipdBoostVirtualBaseTrue(
  ccipd::MFAMaskAligner::SuperclassBoundingBox,
  ccipd::MFAMaskAligner )
  
ccipdBoostVirtualBaseTrue(
  ccipd::MFAMaskAligner::SuperclassAligner,
  ccipd::MFAMaskAligner )
#endif
//////////////////////////////////////////////////////////////////////////////////////////



#endif // __ccipdMFAMaskAligner_h
