#ifndef __ccipdUpdateVTKPropColor_h
#define __ccipdUpdateVTKPropColor_h



//////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdVTKForwardDeclarations.h"
//////////////////////////////////////////////////////////////////////////////////////////

#include <vtkProp.h>

namespace ccipd
{
//////////////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Changes a prop's Color.
 * 
 * This will try casting
 * the prop as a bunch of
 * different vtk types
 * and once it finds one that
 * works, it will set its
 * color.
 * 
 * \date    10/02/2013
 * \author  Ahmad
 * \ingroup GUI Core
 */
void UpdateVTKPropColorGUI(const vtkPropPointer & prop);
//////////////////////////////////////////////////////////////////////////////////////////


} // namespace ccipd
#endif // __ccipdUpdateVTKPropTransparency_h