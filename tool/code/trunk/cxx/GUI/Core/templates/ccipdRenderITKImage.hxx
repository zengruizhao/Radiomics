#ifndef __ccipdRenderITKImage_hxx
#define __ccipdRenderITKImage_hxx

#include "ccipdVolumeTypes.h"
#include "ccipdVTKForwardDeclarations.h" // for vtk types
#include "ccipdRGBSliceTypes.h"
#include "ccipdImageToProp.h"

namespace ccipd
{

template<class TImageType>
void renderITKImage(  typename TImageType::Pointer   image
                    , const vtkRenderWindowPointer & renderWindow
                    ,       vtkPropPointer         & existingImageProp
                    , const unsigned int             slice               //= 0u
                    , const bool                     makeZeroTransparent //= false
                   )
{
	// simple error checking
    //if ( !renderWindow || !image ) return;
    
    // turn our new image into a prop
    const vtkPropPointer newImageProp = CreatePropFromImage<TImageType>( image, slice, makeZeroTransparent );
    
    // update the vtk image
	UpdateVTKImageProp(renderWindow, existingImageProp, newImageProp/*, verbose */);
}




} // namespace


#endif
