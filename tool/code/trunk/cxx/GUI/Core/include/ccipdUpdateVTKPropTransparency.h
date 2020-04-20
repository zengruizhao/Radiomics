#ifndef __ccipdUpdateVTKPropTransparency_h
#define __ccipdUpdateVTKPropTransparency_h



//////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdVTKForwardDeclarations.h"
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{


//////////////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Changes a prop's transparency.
 * 
 * This will try casting
 * the prop as a bunch of
 * different vtk types
 * and once it finds one that
 * works, it will set the
 * transparency.
 * 
 * \param[in] opacity
 *   0.0 is fully transparent.
 *   1.0 is fully opaque.
 *   Any value now within that
 *   range (inclusive) is ignored.
 * 
 * \date    2012-06-07
 * \author  Rob Toth
 * \ingroup GUI
 */
void UpdateVTKPropTransparency(
  const vtkPropPointer & prop,
  const double           opacity,
  const bool             verbose = true );
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
/**
 * Same as UpdateVTKPropTransparency( const vtkPropPointer &, const double )
 * but allows you to input an integer between 0 and 100 (inclusive)
 * to represent the opacity.
 * 
 * \date    2012-06-07
 * \author  Rob Toth
 * \ingroup GUI
 */
void UpdateVTKPropTransparency(
  const vtkPropPointer & prop,
  const unsigned int     opacity,
  const bool             verbose = true );
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace ccipd
#endif // __ccipdUpdateVTKPropTransparency_h
