#ifndef __ccipdUpdateVTKImageWindowLevel_h
#define __ccipdUpdateVTKImageWindowLevel_h



//////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdVTKForwardDeclarations.h"
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{
//////////////////////////////////////////////////////////////////////////////////////////


/** \brief  : Update window/leveling of a vtk image.
  * \Created: Rob Toth 2012-06-12
  * \Updated: Ahmad Algohary 7/25/2013 
  * \ingroup: GUI
  * \param  : imageSlice: The slice to be updated
  * \param  : window: The new wnidow width
  * \param  : level: The new color level
  * \return : void
  * \notes  :
  */
void UpdateVTKImageWindowLevel(  vtkImageSlice & imageSlice
                               , const double          window
                               , const double          level 
                              );
//////////////////////////////////////////////////////////////////////////////////////////
void UpdateVTKImageWindow(  vtkImageSlice & imageSlice
                          , const double          window
                         );

void UpdateVTKImageLevel(  vtkImageSlice & imageSlice
                         , const double          level
                        );
//////////////////////////////////////////////////////////////////////////////////////////
/** 
 * \brief Helper function to use a prop.
 * 
 * If the prop can't be converted to
 * a reasonable type,
 * then it will be ignored and
 * nothing will happen.
 * 
 * \date    2012-06-12
 * \author  Rob Toth
 * \ingroup GUI
 */
void UpdateVTKImageWindowLevel(  const vtkPropPointer & imageProp
                               , const double           window
                               , const double           level 
                              );
//////////////////////////////////////////////////////////////////////////////////////////
void UpdateVTKImageWindow(  const vtkPropPointer & imageProp
                          , const double           window
                         );
//////////////////////////////////////////////////////////////////////////
void UpdateVTKImageLevel(  const vtkPropPointer & imageProp
                         , const double           level 
                        );

} // namespace ccipd
#endif // __ccipdUpdateVTKImageWindowLevel_h
