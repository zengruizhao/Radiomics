#ifndef __ccipdFEMRenderer_h
#define __ccipdFEMRenderer_h

 

//////////////////////////////////////////////////////////////////////////////////////////
// ccipd includes
#include "ccipdMacros.h"
#include "ccipdFEMTypes.h"
#include "ccipdVTKForwardDeclarations.h"

// std includes
#include <memory> // for smart pointers
#include <array>  // for colors

// itk includes
#include <itkMacro.h>
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{


/**
 * \brief Class to perform VTK rendering of an FEM.
 * 
 * \author  Rob Toth
 * \date    2012-10-30
 * \ingroup FEMRendering
 */
class FEMRenderer
{


public:
//////////////////////////////////////////////////////////////////////////////////////////
  FEMRenderer(); ///< Constructor
  virtual ~FEMRenderer(); ///< Destructor
//////////////////////////////////////////////////////////////////////////////////////////



public: // typedefs
//////////////////////////////////////////////////////////////////////////////////////////

  // things such as opacity, etc.
  typedef double RealPropertiesType;

  /// for 24-bit colors (0-255 per channel)
  typedef unsigned char RGBValueType;

  /// Define a 24-bit RGB color type
  typedef std::array< RGBValueType, 3 > ColorType;

  /// Define the FEM actors we can modify
  enum WhichActor
    {
    LandmarksActor,
    TrianglesActor,
    LoadsSpheresActor,
    LoadsArrowsActor
    };

//////////////////////////////////////////////////////////////////////////////////////////



public: // functions
//////////////////////////////////////////////////////////////////////////////////////////

  /// Define the FEM to render
  ccipdSetByrefDeclareMacro( FEM, FEMObjectConstPointer )
  
  /// Define a VTK renderer on which to display the FEM
  ccipdSetByrefDeclareMacro( Renderer, vtkRendererPointer )

  /// Remove all existing actors from the scene
  void RemoveActors();

  /// Show progress?
  ccipdSetByvalDeclareMacro( Verbose, bool )
  itkBooleanMacro( Verbose )

  /// Update the point locations and loads
  virtual void UpdateFEMRendering( const itk::fem::LinearSystemWrapper & solution );
  
  /// Set a face color
  virtual void SetFaceColor( const WhichActor actor, const ColorType & color );

  /// Set an edge color
  virtual void SetEdgeColor( const WhichActor actor, const ColorType & color );

  /// Get a face color
  virtual ColorType GetFaceColor( const WhichActor actor ) const;

  /// Get an edge color
  virtual ColorType GetEdgeColor( const WhichActor actor ) const;

  /// Set an opacity (1.0 is opaque, 0.0 is transparent)
  virtual void SetOpacity( const WhichActor actor, const RealPropertiesType opacity );

  /// Get an opacity
  virtual RealPropertiesType GetOpacity( const WhichActor actor ) const;

  /// Set the size of the arrows
  ccipdSetByvalDeclareMacro( ArrowScaling, RealPropertiesType )

  /// Get the size of the arrows
  ccipdGetConstDeclareMacro( ArrowScaling, RealPropertiesType )

//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
  /// PIMPL interface
  ccipdPIMPLMacro
//////////////////////////////////////////////////////////////////////////////////////////



}; // class FEMRenderer
} // namespace ccipd
#endif // __ccipdFEMRenderer_h
