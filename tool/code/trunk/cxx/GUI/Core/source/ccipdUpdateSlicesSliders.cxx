//////////////////////////////////////////////////////////////////////////////////////////
// ccipd includes
#include "ccipdUpdateSlicesSliders.h"
#include "ccipdDisableWarningsMacro.h"

// itk includes
#include <itkImage.h>
#include <itkVectorImage.h>
#include <itkStatisticsImageFilter.h>

// Qt includes
#include <QSlider>
#include <QSpinBox>
#include <QDoubleSpinBox>
//////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdEnableWarningsMacro.h"


namespace ccipd
{
//////////////////////////////////////////////////////////////////////////////////////////

bool UpdateSliceNumberSlider(  const VolumeImageType * const image
                             ,       QSlider         * const sliceSlider
                             ,       QSpinBox        * const sliceSpinBox
                            )
{
  // error checking
  if ( !image || !sliceSlider) 
    return false; 
    
  const VolumeImageType::SizeValueType sliceNum = image->GetLargestPossibleRegion().GetSize().GetElement(2);

  // update the slider
   sliceSlider->setMinimum ((int)1);
   sliceSlider->setMaximum ((int)sliceNum);
   sliceSlider->setValue   ((int)(1+sliceNum/2));  

   if (sliceSpinBox)
   {
     sliceSpinBox->setMinimum ((int)1);
     sliceSpinBox->setMaximum ((int)sliceNum);
     sliceSpinBox->setValue   ((int)(1+sliceNum/2));  
   }


   return true;
} // UpdateSliceSlider

bool UpdateSliceTimePointSlider(  const VolumeVectorType * const image
							    ,       QSlider          * const sliceSlider
							    ,       QDoubleSpinBox   * const sliceSpinBox
								, const double                   factor       
							   )
{
	// error checking
	if ( !image || !sliceSlider) 
		return false; 

	const auto vectorLength = image->GetVectorLength();

	// update the slider
	sliceSlider->setMinimum   ((int)(1));
	sliceSlider->setMaximum   ((int)vectorLength);
	sliceSlider->setValue     ((int)(1));  
	sliceSlider->setSingleStep((int)(1));

	if (sliceSpinBox)
	{
		sliceSpinBox->setMinimum   (factor);
		sliceSpinBox->setMaximum   (factor * vectorLength);
		sliceSpinBox->setValue     (factor);  
	    sliceSpinBox->setSingleStep(factor);
	}


	return true;
} // UpdateSliceSlider


} // namespace ccipd
