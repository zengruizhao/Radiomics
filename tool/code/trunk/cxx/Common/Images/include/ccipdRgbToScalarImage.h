#ifndef __ccipdBoundingBoxCoordinates_h
#define __ccipdBoundingBoxCoordinates_h


//////////////////////////////////////////////////////////////////////////////////////////
// ccipd includes
#include "ccipdITKForwardDeclarations.h"
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{

	/**
	* Converts RGB Scalar Image to Scalar Image
	*
	* \author  Ahmad Algohary
	* \ingroup Common Images
	* \date    10/06/2013
	*/
	template<  class TRGBImage, class TScalarImage>
	typename TRGBImage::Pointer
		RGBToScalarImage(typename TScalarImage::Pointer pInputImage
		, typename TScalarImage::PixelType ThresholdMin
		, typename TScalarImage::PixelType ThresholdMax
		);


} // namespace ccipd
#endif // __ccipdBoundingBoxCoordinates_h
