



//////////////////////////////////////////////////////////////////////////////////////////

// ccipd includes
#include "ccipdUpdateWindowLevelSliders.h"
#include "ccipdDisableWarningsMacro.h"

// itk includes
#include <itkImage.h>
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
bool UpdateWindowLevelSliders(  const VolumeImageType * const image
                              ,       QSlider         * const windowSlider
                              ,       QSlider         * const levelSlider
                              , const float                   sliderFactor  /*= 100.0f*/ 
                              ,       QSpinBox        * const windowSpinBox /*= NULL*/
                              ,       QSpinBox        * const levelSpinBox  /*= NULL*/
                             )
{
	// error checking
  if (!image || !windowSlider || !levelSlider) 
    return false; 

  // compute statistics
  typedef itk::StatisticsImageFilter< VolumeImageType > StatisticsType;
  const StatisticsType::Pointer statistics = StatisticsType::New();

  statistics->SetInput( image );

  statistics->Update();

  // get the output
  StatisticsType::RealType
    min         = statistics->GetMinimum(),
    max         = statistics->GetMaximum(),
    mean        = statistics->GetMean   ();

  if ( mean!=mean ) // if we have nan pixels
    mean = ( max - min )/2;

  const StatisticsType::RealType
    range       = max - min,
    steps       = 10.0,
    windowMin   = 0,
    windowMax   = range * sliderFactor,// * 2.0,
    windowStep  = ( windowMax - windowMin ) / steps,
    windowValue = windowStep * steps / 2.0,
    levelMin    = min   * sliderFactor,
    levelMax    = max   * sliderFactor,
    levelStep   = ( levelMax - levelMin ) / 10.0,
    levelValue  = mean * sliderFactor;


  // update the sliders
  windowSlider->setMinimum ((int)windowMin  );
  windowSlider->setMaximum ((int)windowMax  );
  windowSlider->setValue   ((int)windowValue);  
  windowSlider->setPageStep((int)windowStep );

  levelSlider->setMinimum ((int)levelMin  );
  levelSlider->setMaximum ((int)levelMax  );
  levelSlider->setPageStep((int)levelStep );
  levelSlider->setValue   ((int)levelValue);


  if (windowSpinBox)
  {
    windowSpinBox->setMinimum   ((int)windowMin  );
    windowSpinBox->setMaximum   ((int)windowMax  );
    windowSpinBox->setValue     ((int)windowValue);  
    //windowSpinBox->setSingleStep((int)windowStep );  
  }

  if (levelSpinBox)
  {
    levelSpinBox->setMinimum   ((int)levelMin   );
    levelSpinBox->setMaximum   ((int)levelMax   );
    levelSpinBox->setValue     ((int)levelValue );
    //levelSpinBox->setSingleStep((int)levelStep  );
  }

  return true;

} // UpdateWindowLevelSliders
//////////////////////////////////////////////////////////////////////////////////////////

bool UpdateWindowLevelSliders(  const VolumeImageType * const image
                              ,       QSlider         * const windowSlider
                              ,       QSlider         * const levelSlider 
                              ,       QDoubleSpinBox  * const windowSpinBox /*= NULL*/
                              ,       QDoubleSpinBox  * const levelSpinBox  /*= NULL*/
                             )
{
	// error checking
  if (!image || !windowSlider || !levelSlider) 
    return false; 

  // compute statistics
  typedef itk::StatisticsImageFilter< VolumeImageType > StatisticsType;
  const StatisticsType::Pointer statistics = StatisticsType::New();
  statistics->SetInput( image );
  statistics->Update();

  // get the output
  const StatisticsType::RealType
    min         = statistics->GetMinimum(),
    max         = statistics->GetMaximum(),
    mean        = statistics->GetMean   (),
    
	range       = max - min,
    steps       = 100.0,
    
	windowMin   = 0.00,
    windowMax   = range,
    windowStep  = range / steps,
    windowValue = windowMax / 2.0,
    
	levelMin    = min,
    levelMax    = max,
    levelStep   = range / steps,
    levelValue  = mean;
    
  



  // update the double spin boxes
  if (windowSpinBox)
	{
		windowSpinBox->setMinimum   ((double)windowMin  );
		windowSpinBox->setMaximum   ((double)windowMax  );
		windowSpinBox->setSingleStep((double)windowStep ); 
		windowSpinBox->setValue     ((double)windowValue);  
	}

	if (levelSpinBox)
	{
		levelSpinBox->setMinimum   ((double)levelMin  ); 
		levelSpinBox->setMaximum   ((double)levelMax  );
		levelSpinBox->setSingleStep((double)levelStep );
		levelSpinBox->setValue     ((double)levelValue);
	}

	// update the sliders
	if (windowSlider)
	{
		windowSlider->setMinimum   ((int)0  );
		windowSlider->setMaximum   ((int)100);
		windowSlider->setSingleStep((int)1  );
		windowSlider->setValue     ((int) (100.0 * (windowValue - min) / range));  
	}
	
	if (levelSlider)
	{
		levelSlider->setMinimum   ((int)0  );
		levelSlider->setMaximum   ((int)100);
		levelSlider->setSingleStep((int)1  );
		levelSlider->setValue     ((int) (100.0 * (levelValue - min) / range));
	}

  return true;

} // UpdateWindowLevelSliders






bool UpdateSliceSlider(  const VolumeImageType * const image
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

bool UpdateWindowLevelSliders2(  const VolumeImageType * const image
                               ,       QSlider         * const windowSlider
                               ,       QSlider         * const levelSlider
                               ,       QSpinBox        * const windowSpinBox /*= NULL*/
                               ,       QSpinBox        * const levelSpinBox  /*= NULL*/
                              )
{

  if (!image || !windowSlider || !levelSlider) 
    return false; // error checking

  // compute statistics
  typedef itk::StatisticsImageFilter< VolumeImageType > StatisticsType;
  const StatisticsType::Pointer statistics = StatisticsType::New();

  statistics->SetInput( image );
  statistics->Update();

  // get the output
  const StatisticsType::RealType
    min         = statistics->GetMinimum(),
    max         = statistics->GetMaximum(),
    
	range = max - min,

	step = range / 100.0,
	// scale to 0-100

	
	
	windowMin   = min + step,
    windowMax   = max - min,
    windowStep  = step,
    windowValue = windowMax / 2.0,
    
    levelMin    = min   ,
    levelMax    = max   ,

    levelStep   = step,
    levelValue  = min + windowValue/ 2.0;


  // update the sliders
  windowSlider->setMinimum ((int)windowMin  );
  windowSlider->setMaximum ((int)windowMax  );
  windowSlider->setValue   ((int)windowValue);  
  windowSlider->setSingleStep((int)windowStep );

  levelSlider->setMinimum ((int)levelMin  );
  levelSlider->setMaximum ((int)levelMax  );
  levelSlider->setSingleStep((int)levelStep );
  levelSlider->setValue   ((int)levelValue);


  if (windowSpinBox)
  {
    windowSpinBox->setMinimum   ((int)windowMin  );
    windowSpinBox->setMaximum   ((int)windowMax  );
    windowSpinBox->setValue     ((int)windowValue);  
    windowSpinBox->setSingleStep((int)windowStep );  
  }

  if (levelSpinBox)
  {
    levelSpinBox->setMinimum   ((int)levelMin   );
    levelSpinBox->setMaximum   ((int)levelMax   );
    levelSpinBox->setValue     ((int)levelValue );
    levelSpinBox->setSingleStep((int)levelStep  );
  }

  return true;

} // UpdateWindowLevelSliders2

bool UpdateWindowLevelSliders2(  const VolumeImageType * const image
							   ,       QSlider         * const windowSlider
							   ,       QSlider         * const levelSlider
							   ,       QDoubleSpinBox  * const windowSpinBox /*= NULL*/
							   ,       QDoubleSpinBox  * const levelSpinBox  /*= NULL*/
							  )
{

	if (!image || !windowSlider || !levelSlider) 
		return false; // error checking

	// The horizontal slider bar will always have the range of 100;

	// compute statistics
	typedef itk::StatisticsImageFilter< VolumeImageType > StatisticsType;
	const StatisticsType::Pointer statistics = StatisticsType::New();

	statistics->SetInput( image );
	statistics->Update();

	// get the output
	const StatisticsType::RealType
		min         = statistics->GetMinimum(),
		max         = statistics->GetMaximum(),
		range       = max - min,
		
		windowMin   = 0.00,
		windowMax   = range,
		windowStep  = range / 100.0f, //100 steps for the doublespin box
		windowValue = range / 2.0,
		
		levelMin    = min   ,
		levelMax    = max   ,
		levelStep   = range / 100.0f,
		levelValue  = min + range / 2.0;

	if (windowSpinBox)
	{
		windowSpinBox->setMinimum   ((double)windowMin  );
		windowSpinBox->setMaximum   ((double)windowMax  );
		windowSpinBox->setValue     ((double)windowValue);  
		windowSpinBox->setSingleStep((double)windowStep );  
	}

	if (levelSpinBox)
	{
		levelSpinBox->setMinimum   ((double)levelMin  );
		levelSpinBox->setMaximum   ((double)levelMax  );
		levelSpinBox->setValue     ((double)levelValue);
		levelSpinBox->setSingleStep((double)levelStep);
	}

	// update the sliders
	if (windowSlider)
	{
		windowSlider->setMinimum   ((int)0  );
		windowSlider->setMaximum   ((int)100);
		windowSlider->setSingleStep((int)1  );
		windowSlider->setValue     ((int)50 );  
	}
	
	if (levelSlider)
	{
		levelSlider->setMinimum   ((int)0  );
		levelSlider->setMaximum   ((int)100);
		levelSlider->setSingleStep((int)1  );
		levelSlider->setValue     ((int)50 );
	}

	return true;

} // UpdateWindowLevelSliders2


bool UpdateTransparencySlider(  const VolumeImageType * const pImage
                              ,       QSlider         * const pTransparencySlider
                              ,       QSpinBox        * const pTransparencySpinBox
                             )
{
  if (!pImage) return false;

  pTransparencySlider->setMinimum(0);
  pTransparencySlider->setMaximum(100);
  pTransparencySlider->setSingleStep(1);
  pTransparencySlider->setValue(100);

  if (pTransparencySpinBox)
  {
    pTransparencySpinBox->setMinimum(0);
    pTransparencySpinBox->setMaximum(100);
    pTransparencySpinBox->setSingleStep(1);
    pTransparencySpinBox->setValue(100);
  }

  return true;
}


} // namespace ccipd
