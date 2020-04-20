#ifndef __ccipdIntensityStandardization_h
#define __ccipdIntensityStandardization_h

#include "ccipdIntensityStandardization.hxx"

//////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdVolumeTypes.h"

#include "ccipdDisableWarningsMacro.h"
  #include <itkImage.h>
#include "ccipdEnableWarningsMacro.h"
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{

////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * This function will standardize the histogram of
 * an image to a template image.
 * 
 * \author  Rob, Ahmad
 * \ingroup Common
 */

template <class TImageType>
typename TImageType::Pointer
StandardizeImage(  typename TImageType::Pointer const inputImage
                 , typename TImageType::Pointer const templateImage
                 , const    unsigned int              histogramBins        = 100
                 , const    unsigned int              histogramMatchPoints = 15
                 );



VolumeImagePointer StandardizeImage(  VolumeImagePointer const inputImage
                                    , VolumeImagePointer const templateImage
                                    , const unsigned int histogramBins        = 100
                                    , const unsigned int histogramMatchPoints = 15 
								   );
////////////////////////////////////////////////////////////////////////////////////////////////////



} // namespace
#endif // __ccipdIntensityStandardization_h
