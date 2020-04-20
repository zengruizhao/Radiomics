#ifndef __ccipdFEMRegistrationGUIObserver_h
#define __ccipdFEMRegistrationGUIObserver_h



//////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdRegistrationGUIObserver.h" // our base class
#include "ccipdFEMTypes.h"
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



/**
 * \brief Update a rendered image at each iteration of an FEM
 * 
 * This observer extends the RegistrationGUIObserver
 * class (which displays an image at the end of each,
 * or several, iterations).
 * 
 * This class does the same thing, but instead of
 * using the current affine result
 * to calculate the current "moved"
 * image, it uses an FEM registration.
 * 
 * \date    2012-06-19
 * \author  Rob Toth
 * \ingroup FEMRendering
 */
class FEMRegistrationGUIObserver : public RegistrationGUIObserver
{



public: // typddefs
//////////////////////////////////////////////////////////////////////////////////////////
  typedef FEMRegistrationGUIObserver Self;
  typedef RegistrationGUIObserver    Superclass;
  typedef itk::SmartPointer< Self >  Pointer;
//////////////////////////////////////////////////////////////////////////////////////////



public: // functions
//////////////////////////////////////////////////////////////////////////////////////////

  itkNewMacro( Self );
  
  /**
   * We are overriding this function to
   * extract the FEM registration
   * that called us.
   */
  virtual void Execute( const itk::Object * caller, const itk::EventObject & event );

  /**
   * We are overriding this function
   * so that the FEM renderer knows
   * where to display the
   * FEM.
   */
  ccipdSetByrefDeclareMacro( RenderWindow, vtkRenderWindowPointer )

  /// Return the FEM renderer we will update every iteration
  ccipdGetConstDeclareMacro( FEMRenderer, FEMRendererPointer )

//////////////////////////////////////////////////////////////////////////////////////////



protected: // functions
//////////////////////////////////////////////////////////////////////////////////////////

  /** Constructor */
  FEMRegistrationGUIObserver();

  /** Destructor */
  virtual ~FEMRegistrationGUIObserver();

  /**
   * We are overriding this function
   * so that the FEM can be used
   * to calculate the moved image.
   */
  virtual VolumeImageConstPointer GetMovedImage() const;

//////////////////////////////////////////////////////////////////////////////////////////


  

// PIMPL interface
//////////////////////////////////////////////////////////////////////////////////////////
  ccipdPIMPLMacro
//////////////////////////////////////////////////////////////////////////////////////////



}; // class FMERegistrationGUIObserver
} // namespace ccipd
#endif // __ccipdFEMRegistrationGUIObserver_h
