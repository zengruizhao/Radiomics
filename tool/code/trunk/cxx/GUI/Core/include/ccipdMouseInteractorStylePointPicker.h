#ifndef __ccipdMouseInteractorStylePointerPicker_h
#define __ccipdMouseInteractorStylePointerPicker_h



////////////////////////////////////////////////////////////////////////////////////////////////////
// ccipd includes
#include "ccipdMacros.h"
#include "ccipdLandmarkTypes.h" // our output
#include "ccipdVTKForwardDeclarations.h"

// std includes
#include <memory> // for smart pointers

#include "ccipdDisableWarningsMacro.h"
  // itk includes
  #include <itkMacro.h>

  // vtk includes
  #include <vtkInteractorStyleImage.h> // our base class
#include "ccipdEnableWarningsMacro.h"
////////////////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



/**
 * \brief A VTK interactor style used to also pixel landmarks.
 * 
 * \note Based off http://bit.ly/JC05ht
 * 
 * \author  Rob Toth
 * \date    2012-05-05
 * \ingroup GUI
 */
class MouseInteractorStylePointPicker : public vtkInteractorStyleImage
{



public:
////////////////////////////////////////////////////////////////////////////////////////////////

  MouseInteractorStylePointPicker(); ///< Constructor

  /**
   * \brief Constructor for when you already have some landmarks.
   * 
   * \note
   *   This will copy the landmarks so as to leave
   *   the input untouched.
   * 
   * \note
   *   You need to pass in a render window
   *   to display the initial crosshairs to
   *   because one might not have been assigned yet.
   * 
   * \warning
   *   Make sure you have a renderer in the renderWindow!
   * 
   */
  MouseInteractorStylePointPicker(
    const LandmarkSetConstPointer &       initialLandmarks,
          vtkRenderWindow         * const renderWindow );

  virtual ~MouseInteractorStylePointPicker(); ///< Destructor

  /// For creating a pointer to ourself
  static MouseInteractorStylePointPicker * New(); ///<

  /// What type are we?
  vtkTypeMacro( MouseInteractorStylePointPicker, vtkInteractorStyleImage);
 
  /// If true, then every time a point is picked, it is output to std::cout.
  ccipdSetByvalDeclareMacro( Verbose, bool )
  ccipdGetConstDeclareMacro( Verbose, bool )
  itkBooleanMacro          ( Verbose       )

  /// We are going to pick points using this button.
  virtual void OnRightButtonDown();
  virtual void OnLeftButtonDown( ) ;


  /// Output the picked points.
  ccipdGetConstDeclareMacro( Landmarks, LandmarkSetPointer )

////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////
  ccipdPIMPLMacro
////////////////////////////////////////////////////////////////////////////////////////////////



}; // class MouseInteractorStylePointPicker





class vtkInteractorStyleCustomized : public vtkInteractorStyleImage
{
public:
  vtkInteractorStyleCustomized(){SetInteractionModeToImage3D();};

  virtual ~vtkInteractorStyleCustomized(){};

  virtual void OnMouseWheelForward()
  {
      
  };
  virtual void OnMouseWheelBackward()
  {
      
  };
    /// 
  static vtkInteractorStyleCustomized* New(){return new vtkInteractorStyleCustomized;}; ///< For creating a pointer to self


  vtkTypeMacro( vtkInteractorStyleCustomized, vtkInteractorStyleImage);



};

















}  // namespace ccipd
#endif // __ccipdMouseInteractorStylePointerPicker_h
