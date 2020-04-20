#ifndef __ccipdMaskToLevelset_h
#define __ccipdMaskToLevelset_h


#include "ccipdMaskToLevelset.hxx"
//////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdITKForwardDeclarations.h"
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{

/**
 * Calculate the levelset from a single mask.
 *
 * If smoothVariance is <= 0, no smoothing is performed.
 *
 * \ingroup Segmentation
 */
//////////////////////////////////////////////////////////////////////////////////////////
template< class TImagePixelType, class TMaskPixelType, unsigned int TDimension >
itk::SmartPointer< itk::Image< TImagePixelType, TDimension > >
MaskToLevelset(   const itk::Image< TMaskPixelType, TDimension >  * const mask
				, const bool  normalize				= false
				, const bool  sigmoid				= false
				, const float sigmoidWidth			= 4.0
				, const float smoothingVariance		= 0.0
				, const bool  resizeIsotropically	= true
			   );
//////////////////////////////////////////////////////////////////////////////////////////

} // namespace
#endif // __ccipdMaskToLevelset_h