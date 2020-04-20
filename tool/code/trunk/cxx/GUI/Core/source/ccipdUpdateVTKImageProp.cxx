


//////////////////////////////////////////////////////////////////////////////////////////
// ccipd includes
#include "ccipdUpdateVTKImageProp.h"
#include "ccipdImageToProp.h"

// std includes
#include <iostream>

#include "ccipdDisableWarningsMacro.h"

  // itk includes
  #include <itkRGBPixel.h>
#include <itkRGBAPixel.h>


  // vtk includes
  #include <vtkRenderWindow.h>
  #include <vtkRendererCollection.h>
  #include <vtkImageSlice.h>
  #include <vtkRenderWindowInteractor.h>
  #include <vtkSmartPointer.h>
#include "ccipdEnableWarningsMacro.h"

//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
using std::cout;
using std::cerr;
using std::endl;
//////////////////////////////////////////////////////////////////////////////////////////




namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////
void UpdateVTKImageProp(  const vtkRenderWindowPointer &       renderWindow
                        ,       vtkPropPointer         &       existingImageProp
                        , const VolumeImageType        * const image
                        , const unsigned int                   slice
                        , const bool                           makeZeroTransparent
                        , const bool                           verbose 
                       )
{

  // simple error checking
  if ( !renderWindow || !image ) return;

  // turn our new image into a prop
  if ( verbose ) cout << "Creating new prop:" << endl;
  const vtkPropPointer newImageProp = CreatePropFromImage( image, slice, makeZeroTransparent );
  if ( verbose ) cout << "Creating new prop done." << endl;

  // update the vtk image
  if ( verbose ) cout << "Updating VTK image:" << endl;
  UpdateVTKImageProp( renderWindow, existingImageProp, newImageProp, verbose );
  if ( verbose ) cout << "Updating VTK image done." << endl;

} // UpdateVTKImageProp
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
  void UpdateVTKImageProp(  const vtkRenderWindowPointer &       renderWindow
                          ,       vtkPropPointer         &       existingImageProp
                          , const RGBVolumeType          * const image
                          , const unsigned int                   slice
                          , const bool                           makeZeroTransparent
                          , const bool                           verbose 
                         )
  {
        // simple error checking
        if ( !renderWindow || !image ) return;
      
        // turn our new image into a prop
        if ( verbose ) cout << "Creating new prop:" << endl;
        const vtkPropPointer newImageProp = CreatePropFromImage( image, slice, makeZeroTransparent );
        if ( verbose ) cout << "Creating new prop done." << endl;
      
        // update the vtk image
        if ( verbose ) cout << "Updating VTK image:" << endl;
          UpdateVTKImageProp( renderWindow, existingImageProp, newImageProp, verbose );
        if ( verbose ) cout << "Updating VTK image done." << endl;
  
  } // UpdateVTKImageProp
  //////////////////////////////////////////////////////////////////////////////////////////


void UpdateVTKImageProp(  const vtkRenderWindowPointer &       renderWindow
                        ,       vtkPropPointer         &       existingImageProp
                        , const RGBAVolumeType         * const image
                        , const unsigned int                   slice
                        , const bool                           makeZeroTransparent
                        , const bool                           verbose 
                       )
  {
        // simple error checking
        if ( !renderWindow || !image ) 
        {
            cout << "Error(105-3232): Invalid image or render window ..." << endl;
            return;
        }
      
        // turn our new image into a prop
        const vtkPropPointer newImageProp = CreatePropFromImage( image, slice, makeZeroTransparent);

        // update the vtk image
        UpdateVTKImageProp( renderWindow, existingImageProp, newImageProp, verbose);
  
  } // UpdateVTKImageProp

//////////////////////////////////////////////////////////////////////////////////////////
void UpdateVTKImageProp(  const vtkRenderWindowPointer & renderWindow
                        ,       vtkPropPointer         & existingImageProp
                        , const vtkPropPointer         & newImageProp
                        , const bool                     verbose 
                       )
{

  if ( verbose ) cout << "Extracting renderer:" << endl;
  const vtkRendererPointer & renderer = renderWindow->GetRenderers()->GetFirstRenderer();
  if ( verbose ) cout << "Extracting renderer done." << endl;
  
  if ( !existingImageProp )
    {

    if ( verbose ) cout << "Giving prop to renderer:" << endl;

    existingImageProp = newImageProp;

    // give it to the renderer
    renderer->AddViewProp( existingImageProp );

    // reset the camera so all is visible
    renderer->ResetCamera();

    if ( verbose ) cout << "Giving prop to renderer done." << endl;

    } // existingImageProp
  
  // try to turn it into an image slice
  if ( verbose ) cout << "Casting to image slice:" << endl;

  
   vtkImageSlice * const outputImageSlice = dynamic_cast< vtkImageSlice * >( existingImageProp.GetPointer() );
   vtkImageSlice * const newImageSlice    = dynamic_cast< vtkImageSlice * >( newImageProp.GetPointer()      );

  if ( verbose ) cout << "Casting to image slice done." << endl;

  if ( !outputImageSlice || !newImageSlice )
    {
    cerr << "Warning(136-6): Unable to cast to image slices!" << endl;
    return;
    }
   
  // duplicate the mapper
  if ( verbose ) cout << "Updating mapper:" << endl;
  outputImageSlice->ShallowCopy( newImageSlice );
  if ( verbose ) cout << "Updating mapper done." << endl;

  // re-render
  if ( verbose ) cout << "Re-rendering:" << endl;
  
  // okay, render everything
  renderWindow->Render();
  renderWindow->Render(); // once more for good measure

  // see if any props (e.g. images)
  // are visible in the entire viewport
  const bool anyVisibleProps = renderer->PickProp(
    renderer->GetOrigin()[ 0 ],
    renderer->GetOrigin()[ 1 ],
    renderer->GetSize  ()[ 0 ],
    renderer->GetSize  ()[ 1 ] ) != 0;

  // if the camera can't see any
  // props, or for some reason none
  // were rendered, then
  // reset the camera to view all of them
  if ( !anyVisibleProps )
    {
    if ( verbose ) cout << "All props out of range. Resetting camera." << endl;
    renderer->ResetCamera();
    renderWindow->Render();
    renderWindow->Render(); // once more for good measure
    } // anyVisibleProps 

  cout << "Re-rendering done." << endl;

} // UpdateVTKImageProp
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace ccipd
