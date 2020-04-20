#ifndef __ccipdSurfaceToDistanceImage_hxx
#define __ccipdSurfaceToDistanceImage_hxx



//////////////////////////////////////////////////////////////////////////////////////////
// ccipd includes
#include "ccipdMaskImage.h"

#include "ccipdDisableWarningsMacro.h"
  // itk includes
  #include <itkImage.h>
#include "ccipdEnableWarningsMacro.h"
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{


  
//////////////////////////////////////////////////////////////////////////////////////////
template< class TImagePixelType, class TMaskPixelType, unsigned int TDimension >
itk::SmartPointer< itk::Image< TImagePixelType, TDimension > >
SurfaceToDistanceImage( 
 const itk::Image< TMaskPixelType, TDimension >  * const surfaceMask,
 const itk::Image< TImagePixelType, TDimension > * const levelSet
 )
{

  // make sure we have everything
  if ( !surfaceMask )
    return ImagePointer();

  if ( !levelSet )
    return ImagePointer();

  return MaskImage< TImagePixelType, TMaskPixelType, TDimension >
    ( levelSet, surfaceMask, 0, true );

} // namespace ccipd

#endif // __ccipdSurfaceMaskToDistanceImage_hxx