#ifndef __ccipdFeatureTypes_h
#define __ccipdFeatureTypes_h



/**
 * This file contains common types for our
 * texture and non-texture features.
 */



//////////////////////////////////////////////////////////////////////////////////////////
// ccipd includes 
#include "ccipdVolumeTypes.h"

#include "ccipdDisableWarningsMacro.h"
  // itk includes
  #include <itkSize.h> // for window size type
  #include <itkOffset.h>
#include "ccipdEnableWarningsMacro.h"
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////
// radius type 
typedef itk::Size< VolumeDimension >::SizeValueType   VolumeRadiusType;
typedef itk::Size< VolumeDimension >                  VolRadiusType;
typedef itk::Offset<VolumeDimension>                  VolOffsetType;
//////////////////////////////////////////////////////////////////////////////////////////




} // namespace
#endif // __ccipdFeatureTypes_h
