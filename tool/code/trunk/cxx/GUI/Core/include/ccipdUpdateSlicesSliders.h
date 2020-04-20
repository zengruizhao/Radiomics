#ifndef __ccipdUpdateSlicesSliders_h
#define __ccipdUpdateSlicesSliders_h



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
bool UpdateSliceNumberSlider(  const VolumeImageType * const image
                             ,       QSlider         * const sliceSlider
                             ,       QSpinBox        * const sliceSpinBox = NULL
                            );
  

/* brief: for time series (4D) images
Ahmad Jan 6, 2014
*/

bool UpdateSliceTimePointSlider(  const VolumeVectorType * const image
                                ,       QSlider          * const sliceSlider
                                ,       QDoubleSpinBox   * const sliceSpinBox = NULL
								, const double                   factor       = 1.0
                               );






} // namespace ccipd
#endif // __ccipdUpdateWindowLevelSliders_h
