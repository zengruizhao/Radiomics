#ifndef __ccipdRegistrationGUIObserver_h
#define __ccipdRegistrationGUIObserver_h


//////////////////////////////////////////////////////////////////////////////////////////
// ccipd includes
#include "ccipdRegistrationObserver.h"   // our parent class
#include "ccipdMacros.h"                 // for our pimpl interface
#include "ccipdVTKForwardDeclarations.h" // for our props
#include "ccipdVolumeTypes.h"            // we are a 3D observer

// std includes
#include <memory> // for smart pointers
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



/**
 * \brief Update a rendered image at each iteration.
 * 
 * This observer extends the RegistrationObserver
 * class (which displays progress to the console
 * at the end of each, or several, iterations).
 * 
 * This class aims to take a rendered vtk image
 * and update it with the new
 * "moved" result for the current iteration.
 * 
 * \date    2012-06-06
 * \author  Rob Toth
 * \ingroup GUI
 */
class RegistrationGUIObserver : public RegistrationObserver
{



public: // typedef's
//////////////////////////////////////////////////////////////////////////////////////////
  typedef RegistrationGUIObserver   Self;
  typedef RegistrationObserver      Superclass; ///< The base observer class
  typedef itk::SmartPointer< Self > Pointer;

  /// We will use volumetric images to display.
  typedef VolumeImageType         ImageType;
  typedef VolumeImageConstPointer ImageConstPointer;

  typedef VolumeImageBaseConstPointer TemplateImageConstPointer;

  typedef VolumeTransformType    TransformType;
  typedef VolumeTransformPointer TransformPointer;

//////////////////////////////////////////////////////////////////////////////////////////



public: // functions
//////////////////////////////////////////////////////////////////////////////////////////
  itkNewMacro( Self );
  
  /**
   * \brief Define the image to transform.
   * \note You *must* call this prior to anything for the image to update!
   */
  ccipdSetByvalDeclareMacro( MovingImage, ImageConstPointer )
  ccipdGetConstDeclareMacro( MovingImage, ImageConstPointer )

  /**
   * \brief Define the template "moved" image.
   * 
   * This is an optional input.
   * If you don't set this,
   * then the moved image will be used
   * as its own template.
   */
  ccipdSetByvalDeclareMacro( MovedTemplateImage, TemplateImageConstPointer )
  ccipdGetConstDeclareMacro( MovedTemplateImage, TemplateImageConstPointer )

  /**
   * \brief Define the transform to use.
   * 
   * This transform should be able to
   * use the parameters from the optimizer.
   * 
   * \note You *must* call this prior to anything for the image to update!
   */
  ccipdSetByrefDeclareMacro( Transform, TransformPointer )

  /**
   * \brief Define what we should update every iteration.
   * \note You *must* call this prior to anything for the image to update!
   */
  ccipdSetByrefDeclareMacro( ImagePropToUpdate, vtkPropPointer )

  /**
   * \brief Define where we should update every iteration.
   * \note You *must* call this prior to anything for the image to update!
   */
  ccipdSetByrefDeclareMacro( RenderWindow, vtkRenderWindowPointer )

  /**
   * When updating the display,
   * should all "0" pixels
   * be made transparent?
   * 
   * default = false
   */
  ccipdSetByvalDeclareMacro( MakeZeroTransparent, bool )
  ccipdGetConstDeclareMacro( MakeZeroTransparent, bool )
  itkBooleanMacro         ( MakeZeroTransparent )

  /// We are overriding this function to update the image.
  virtual void Execute( const itk::Object * caller, const itk::EventObject & event );

//////////////////////////////////////////////////////////////////////////////////////////



protected: // functions
//////////////////////////////////////////////////////////////////////////////////////////

  /// Constructor
  RegistrationGUIObserver();

  /// Destructor
  virtual ~RegistrationGUIObserver();

  /// Calculate a moved image for this iteration.
  virtual VolumeImageConstPointer GetMovedImage() const;

//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
  ccipdPIMPLMacro
//////////////////////////////////////////////////////////////////////////////////////////



}; // class RegistrationGUIObserver



} // namespace ccipd
#endif // __ccipdRegistrationGUIObserver_h
