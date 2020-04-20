#ifndef __ccipdStatistics_h
#define __ccipdStatistics_h



////////////////////////////////////////////////////////////////////////////////////////////////////
// ccipd includes
#include "ccipdCore.h"
#include "ccipdVolumeTypes.h"
////////////////////////////////////////////////////////////////////////////////////////////////////

namespace ccipd
{

/**
 * computes the number of pixels below threshold (Templated)
 * \param inImage the input image
 * \param threahold used  
 * \author  Ahmad Algohary
 * \ingroup Filtering
 */
template< class TInputImage>
unsigned int GetPixelCount(  const TInputImage * inImage
	                         , const float         threshold/* = 1.0*/
                          );


unsigned int GetPixelCount(  const VolumeMaskImageType * inImage
                           , const float                 threshold = 1.0
                          );


/**
 * computes the volume of a image by counting the number of pixels below threshold (Templated)
 * \param inImage the input image
 * \param threahold used 
 * 
 * \author  Mirabela Rusu
 * \ingroup Filtering
 */
template< class TInputImage>
EvaluationValueType GetVolume(
	const TInputImage     * inImage,
	const float           threshold = 1.0);

/**
 * computes the volume of a mask by counting the number of pixels below threshold 
 * \param inImage the input mask
 * \param threahold used 
 * 
 * \author  Mirabela Rusu
 * \ingroup Filtering
 */
EvaluationValueType GetVolume(
	const VolumeMaskImageType * inImage,
	const float               threshold = 1.0);

/**
 * computes the min of a image 
 * \param inImage the input image
 * 
 * \author  Mirabela Rusu
 * \ingroup Filtering
 */
template< class TInputImage>
typename TInputImage::PixelType GetMin(const TInputImage * inImage);

/**
 * computes the min of a image 
 * \param inImage the input image
 * 
 * \author  Mirabela Rusu
 * \ingroup Filtering
 */
InternalPixelType GetMin(const VolumeImageType * inImage);

/**
 * computes the min of a mask 
 * \param inImage the input image
 * 
 * \author  Mirabela Rusu
 * \ingroup Filtering
 */
MaskPixelType GetMin(const VolumeMaskImageType * inImage);

/**
 * computes the max of a image 
 * \param inImage the input image
 * 
 * \author  Mirabela Rusu
 * \ingroup Filtering
 */
template< class TInputImage>
typename TInputImage::PixelType GetMax(const TInputImage * inImage);

/**
 * computes the max of a image 
 * \param inImage the input image
 * 
 * \author  Mirabela Rusu
 * \ingroup Filtering
 */
InternalPixelType GetMax(const VolumeImageType * inImage);

/**
 * computes the max of a mask 
 * \param inImage the input image
 * 
 * \author  Mirabela Rusu
 * \ingroup Filtering
 */
MaskPixelType GetMax(const VolumeMaskImageType * inImage);


/**
 * computes the sum of a image 
 * \param inImage the input image
 * 
 * \author  Mirabela Rusu
 * \ingroup Filtering
 */
template< class TInputImage>
typename TInputImage::PixelType GetSum(const TInputImage * inImage);

/**
 * computes the sum of a image 
 * \param inImage the input image
 * 
 * \author  Mirabela Rusu
 * \ingroup Filtering
 */
InternalPixelType GetSum(const VolumeImageType * inImage);

/**
 * computes the standard deviation for the entire image 
 * \param inImage the input image
 * 
 * \author  Mirabela Rusu
 * \ingroup Filtering
 */
template< class TInputImage>
typename TInputImage::PixelType GetSD(const TInputImage * inImage);

/**
 * computes the standard deviation for the entire image 
 * \param inImage the input image
 * 
 * \author  Mirabela Rusu
 * \ingroup Filtering
 */
InternalPixelType GetSD(const VolumeImageType * inImage);


/**
 * computes the quantile  
 * \param inImage the input image
 * \param index whick quantile (should be a value between 0 and 1)
 * \author  Mirabela Rusu
 * \ingroup Filtering
 */
template< class TInputImage>
typename TInputImage::PixelType GetQuantile(const TInputImage * inImage, float percentile);

} //namespace ccipd

#endif //__ccipdStatistics_h
