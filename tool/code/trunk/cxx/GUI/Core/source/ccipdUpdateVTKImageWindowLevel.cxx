				


//////////////////////////////////////////////////////////////////////////////////////////
// ccipd includes
#include "ccipdUpdateVTKImageWindowLevel.h"
#include "ccipdDisableWarningsMacro.h"
  // vtk includes
  #include <vtkSmartPointer.h>
  #include <vtkImageSlice.h>    // the actual image prop
  #include <vtkImageProperty.h> // stores the window/level
#include "ccipdEnableWarningsMacro.h"
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{
//////////////////////////////////////////////////////////////////////////////////////////
void UpdateVTKImageWindowLevel(        vtkImageSlice & imageSlice
                               , const double          window
                               , const double          level 
                              )
{
  // set the properties
    imageSlice.GetProperty()->SetColorWindow( window );
    imageSlice.GetProperty()->SetColorLevel ( level  );
} // UpdateVTKImageWindowLevel
//////////////////////////////////////////////////////////////////////////////////////////
void UpdateVTKImageWindow(        vtkImageSlice & imageSlice
                          , const double          window
                         )
{
  imageSlice.GetProperty()->SetColorWindow(window);
} // UpdateVTKImageWindow
//////////////////////////////////////////////////////////////////////////
void UpdateVTKImageLevel(        vtkImageSlice & imageSlice
                         , const double          level
                        )
{
  imageSlice.GetProperty()->SetColorLevel(level);
} // UpdateVTKImageLevel


//////////////////////////////////////////////////////////////////////////////////////////
void UpdateVTKImageWindowLevel(  const vtkPropPointer & imageProp
                               , const double           window
                               , const double           level 
                              )
{
  if ( !imageProp ) return; // error checking

  // try casting it
  vtkImageSlice * const imageSlice = 
    dynamic_cast< vtkImageSlice * >( imageProp.GetPointer() );

  if ( !imageSlice ) return; // error checking

  return UpdateVTKImageWindowLevel( *imageSlice, window, level );

} // UpdateVTKImageWindowLevel
//////////////////////////////////////////////////////////////////////////////////////////
void UpdateVTKImageWindow(  const vtkPropPointer & imageProp
                          , const double           window
                         )
{
  if ( !imageProp ) return; // error checking
  // try casting it
  vtkImageSlice * const imageSlice = dynamic_cast< vtkImageSlice * >( imageProp.GetPointer());

  if ( !imageSlice ) return; // error checking

  return UpdateVTKImageWindow( *imageSlice, window);

} // UpdateVTKImageWindow
//////////////////////////////////////////////////////////////////////////
void UpdateVTKImageLevel(  const vtkPropPointer & imageProp
                         , const double           level
                        )
{
  if ( !imageProp ) return; // error checking

  // try casting it
  vtkImageSlice * const imageSlice = dynamic_cast< vtkImageSlice * >( imageProp.GetPointer() );

  if ( !imageSlice ) return; // error checking

  return UpdateVTKImageLevel( *imageSlice, level );

} // UpdateVTKImageLevel

} // namespace ccipd
