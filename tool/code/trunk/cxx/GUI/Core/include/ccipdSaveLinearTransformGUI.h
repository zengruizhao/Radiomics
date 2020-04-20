#ifndef __ccipdSaveLinearTransformGUI_h
#define __ccipdSaveLinearTransformGUI_h



//////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdVolumeTypes.h"
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
class QWidget;
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////
/**
 * \brief Pop up a dialog letting the user save a linear transform.
 * 
 * \param[in] verbose
 *   Whether to give an error if
 *   unable to save the file.
 * 
 * \returns true if the save was successful.
 * 
 * \date    2012-06-18
 * \author  Rob Toth
 * \ingroup GUI
 */
bool SaveLinearTransform(
  const VolumeLinearTransformType &       transform,
        QWidget                   * const parent  = 0,
  const bool                              verbose = true );
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace ccipd
#endif // __ccipdSaveLinearTransformGUI_h
