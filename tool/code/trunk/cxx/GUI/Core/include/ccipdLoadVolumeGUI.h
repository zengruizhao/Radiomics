#ifndef __ccipdLoadVolumeGUI_h
#define __ccipdLoadVolumeGUI_h



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
 * \brief Pop up a dialog letting the user load an volume image.
 * 
 * \param[in] verbose
 *   Whether to give an error if
 *   unable to load the file.
 * 
 * \returns NULL if no file is loaded.
 * 
 * \date    2012-10-21
 * \author  Rachel Sparks
 * \ingroup GUI
 */
VolumeImagePointer LoadVolume(
  QWidget * const parent  = 0,
  const bool      verbose = true );


VolumeImagePointer LoadVolumeFromDirectory(
  QWidget * const parent  = 0,
  const bool      verbose = true );
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace ccipd
#endif // __ccipdLoadVolumeGUI_h
