#ifndef __ccipdRegisterMFAStudyOutput_h
#define __ccipdRegisterMFAStudyOutput_h



//////////////////////////////////////////////////////////////////////////////////////////
/// Rachel's note I think I have to set the segmenter here? or see if I can retrieve it from RegisterMFABase
// ccipd includes
#include "ccipdRegisterOutputStudyImage.h"
#include "ccipdRegisterMFABase.h"
#include "ccipdCloneableFactory.h"

#include "ccipdDisableWarningsMacro.h"
  #include <itkMacro.h>
#include "ccipdEnableWarningsMacro.h"
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



/**
 * \brief Class for saving output for the MFA model.
 * 
 * \author  Rob Toth
 * \date    2012-08-03
 * \ingroup Registration
 */
class RegisterMFAStudyOutput :
  virtual public CopyableDerived<
    RegisterMFAStudyOutput,
    RegisterOutputStudyImage >,
  virtual public RegisterMFABase,
  public CloneableFactory< RegisterMFAStudyOutput >
{



//////////////////////////////////////////////////////////////////////////////////////////
  ccipdConstructorDeclarationMacro( RegisterMFAStudyOutput )
        
public:
  typedef CloneableFactory< Self > SuperclassFactory;
  ccipdFactoryTypedefsMacro
//////////////////////////////////////////////////////////////////////////////////////////



public: // types
//////////////////////////////////////////////////////////////////////////////////////////
  typedef RegisterOutputStudyImage     Superclass;  
  typedef Superclass::MaskConstPointer MaskConstPointer;
//////////////////////////////////////////////////////////////////////////////////////////



public: // functions
//////////////////////////////////////////////////////////////////////////////////////////

  /// After registration, crop the moved output to only the visible region? (default true)
  ccipdSetByvalDeclareMacro( CropMovedOutput, bool )

  /// If true, all the features will be reconstructed after registration.
  ccipdSetByvalDeclareMacro( ReconstructAllFeatures, bool )
  itkBooleanMacro          ( ReconstructAllFeatures )

  /// For setting the trained study
  ccipdSetByrefDeclareMacro( TrainedMFA, MFAVolumeBaseConstPointer )
  ccipdGetConstDeclareMacro( TrainedMFA, MFAVolumeBaseConstPointer )
 
  /**
   * Call this when you want to update the moved study
   * with information from the segmenter.
   * 
   * Afterwards, you can call GetMovedStudy
   */
  virtual void UpdateMovedStudy(  const MFASegmenter & segmenter );
//////////////////////////////////////////////////////////////////////////////////////////



protected: // functions
//////////////////////////////////////////////////////////////////////////////////////////
  
  virtual void CopyParametersInternal( const Self & other );

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



}; // class RegisterBase
} // namespace ccipd



//////////////////////////////////////////////////////////////////////////////////////////
#ifdef Boost_SERIALIZATION_FOUND

BOOST_CLASS_VERSION( ccipd::RegisterMFAStudyOutput, 1 )
BOOST_CLASS_TRACKING(ccipd::RegisterMFAStudyOutput,
  boost::serialization::track_always )

ccipdBoostVirtualBaseTrue(
  ccipd::RegisterOutputStudyImage,
  ccipd::RegisterMFAStudyOutput )

ccipdBoostVirtualBaseTrue(
  ccipd::RegisterMFABase,
  ccipd::RegisterMFAStudyOutput )
  
#endif // Boost_SERIALIZATION_FOUND
//////////////////////////////////////////////////////////////////////////////////////////



#endif // __ccipdRegisterMFAStudyOutput_h
