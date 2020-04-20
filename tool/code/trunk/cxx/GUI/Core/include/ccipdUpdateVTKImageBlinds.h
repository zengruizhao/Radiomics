#ifndef __ccipdUpdateVTKImageBlinds_h
#define __ccipdUpdateVTKImageBlinds_h



//////////////////////////////////////////////////////////////////////////////////////////
// ccipd includes
#include "ccipdITKForwardDeclarations.h"
#include "ccipdVTKForwardDeclarations.h"
//////////////////////////////////////////////////////////////////////////////////////////


namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Change the blinds on an image.
 * 
 * This will take an existing image prop,
 * and create (or modify) some blinds
 * to that a checkerboard (either
 * horizontally or vertically) appears.
 * 
 * \param[in] *Blinds
 *   If you set this to 0 or 1,
 *   that direction is ignored.
 * 
 * \param[in] image
 *   The image from which the imageSlice
 *   was created.
 * 
 * \date    2012-06-05
 * \author  Rob Toth
 * \ingroup GUI
 */
template< unsigned int TDimension >
void UpdateVTKImageBlinds(
  const itk::ImageBase< TDimension > & image,
        vtkImageSlice                & imageSlice,
  const unsigned int                   horizontalBlinds,
  const unsigned int                   verticalBlinds );
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Cast to the correct image prop and update the blinds.
 * 
 * \date    2012-06-11
 * \author  Rob Toth
 * \ingroup GUI
 */
template< unsigned int TDimension >
void UpdateVTKImageBlinds(
  const itk::ImageBase< TDimension > & image,
        vtkProp                      & imageProp,
  const unsigned int                   horizontalBlinds,
  const unsigned int                   verticalBlinds );
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace ccipd
#endif // __ccipdUpdateVTKImageBlinds_h
