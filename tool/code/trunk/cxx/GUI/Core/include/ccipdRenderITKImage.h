#ifndef __ccipdRenderITKImage_h
#define __ccipdRenderITKImage_h

#include "ccipdRenderITKImage.hxx"

namespace ccipd
{
template<class TImageType>
void renderITKImage(  typename TImageType::Pointer   image
                    , const vtkRenderWindowPointer & renderWindow
                    ,       vtkPropPointer         & existingImageProp
                    , const unsigned int             slice               = 0u
                    , const bool                     makeZeroTransparent = false
                   );




} // namespace


#endif
