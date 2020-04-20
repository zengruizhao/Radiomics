#ifndef __ccipdUpdateVTKImageProp_h
#define __ccipdUpdateVTKImageProp_h



//////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdUpdateVTKImageProp.hxx"
#include "ccipdVolumeTypes.h"
#include "ccipdVTKForwardDeclarations.h" // for vtk types
#include "ccipdRGBSliceTypes.h"
#include "ccipdImageToProp.h"
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Change the currently rendered image.
 * 
 * If you already have an existing image
 * in existingImageProp which is displayed
 * on screen, this will update it
 * to display a new slice of a new image.
 * 
 * If !existingImageProp, then it will
 * be added to the renderer.
 * 
 * \see CreatePropFromImage()
 * 
 * \author  Rob Toth
 * \date    2012-06-05
 * \ingroup GUI
 */
//////////////////////////////////////////////////////////////////////////////////////////

void UpdateVTKImageProp(  const vtkRenderWindowPointer &       renderWindow
                        ,       vtkPropPointer         &       existingImageProp
                        , const VolumeImageType        * const image
                        , const unsigned int                   slice
                        , const bool                           makeZeroTransparent = false
                        , const bool                           verbose             = false 
                       );


void UpdateVTKImageProp(  const vtkRenderWindowPointer &       renderWindow
                        ,       vtkPropPointer         &       existingImageProp
                        , const RGBVolumeType          * const image
                        , const unsigned int                   slice
                        , const bool                           makeZeroTransparent = false
                        , const bool                           verbose             = false 
                       );

void UpdateVTKImageProp(  const vtkRenderWindowPointer &       renderWindow
                        ,       vtkPropPointer         &       existingImageProp
                        , const RGBAVolumeType         * const image
                        , const unsigned int                   slice
                        , const bool                           makeZeroTransparent = false
                        , const bool                           verbose             = false 
                       );

//template<class TImage>
//void UpdateVTKImageProperty(  const vtkRenderWindowPointer   &    renderWindow
//                            ,          vtkPropPointer        &    existingImageProp
//                            , typename TImage::Pointer image
//                            , const    unsigned int               slice
//                            , const bool                          makeZeroTransparent = false
//                            , const bool                          verbose             = false 
//                           );

//////////////////////////////////////////////////////////////////////////////////////////
/**
 * This code actually updates the image.
 * 
 * \ingroup GUI
 */
void UpdateVTKImageProp(  const vtkRenderWindowPointer & renderWindow
                        ,       vtkPropPointer         & existingImageProp
                        , const vtkPropPointer         & newImageProp
                        , const bool                     verbose = false 
                       );
//////////////////////////////////////////////////////////////////////////////////////////

} // namespace ccipd
#endif // __ccipdUpdateVTKImageProp_h
