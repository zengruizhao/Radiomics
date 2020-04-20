#ifndef __ccipdUpdateWindowLevelSliders_h
#define __ccipdUpdateWindowLevelSliders_h



//////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdVolumeTypes.h"
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
class QSlider;
class QSpinBox;
class QDoubleSpinBox;
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////
/**
 * Use the statistics
 * of an image to update
 * the min and max
 * of sliders.
 * 
 * \param[in] sliderFactor
 *   Will multiply the min and
 *   max by this value for
 *   more granularity.
 * 
 * \author  Rob Toth 2013-01-29
 * \Updated Ahmad Algohary 07/25/2013
 * \ingroup GUI
 * \date   2013-01-29
 */
  bool UpdateWindowLevelSliders(  const VolumeImageType * const image
                                ,       QSlider         * const windowSlider
                                ,       QSlider         * const levelSlider
                                , const float                   sliderFactor  = 100.0f 
                                ,       QSpinBox        * const windowSpinBox = NULL
                                ,       QSpinBox        * const levelSpinBox  = NULL
                               );

  bool UpdateWindowLevelSliders(  const VolumeImageType * const image
                                ,       QSlider         * const windowSlider
                                ,       QSlider         * const levelSlider
                                ,       QDoubleSpinBox        * const windowSpinBox = NULL
                                ,       QDoubleSpinBox        * const levelSpinBox  = NULL
                               );
//////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
/** \brief  : Uses the number of slices in the passed volume to 
 **           update (set min, max and range) of the slice slider
  * \Created: Ahmad 7/25/2013
  * \Updated:  
  * \ingroup: GUI
  * \param  : image: the volume image
  * \param  : sliceSlider: the slider to be updated
  * \return : void
  */
bool UpdateSliceSlider( const VolumeImageType * const image
                       ,      QSlider         * const sliceSlider
                       ,      QSpinBox        * const sliceSpinBox = NULL
                      );
  
bool UpdateWindowLevelSliders2(  const VolumeImageType * const image
                               ,       QSlider         * const windowSlider
                               ,       QSlider         * const levelSlider
                               ,       QSpinBox        * const windowSpinBox = NULL
                               ,       QSpinBox        * const levelSpinBox  = NULL
                              );

bool UpdateWindowLevelSliders2(  const VolumeImageType * const image
							   ,       QSlider         * const windowSlider
							   ,       QSlider         * const levelSlider
							   ,       QDoubleSpinBox  * const windowSpinBox
							   ,       QDoubleSpinBox  * const levelSpinBox
							   );

bool UpdateTransparencySlider(  const VolumeImageType * const pImage
                              ,       QSlider         * const pTransparencySlider
                              ,       QSpinBox        * const pTransparencySpinBox = NULL
                             );


} // namespace ccipd
#endif // __ccipdUpdateWindowLevelSliders_h
