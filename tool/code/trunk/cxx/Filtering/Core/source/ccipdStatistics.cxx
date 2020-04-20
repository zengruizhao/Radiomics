
////////////////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdStatistics.hxx"
#include "ccipdVolumeTypes.h"
////////////////////////////////////////////////////////////////////////////////////////////////////

using std::cout;
using std::endl;


namespace ccipd
{
unsigned int GetPixelCount(  const VolumeMaskImageType * inImage
                           , const float                 threshold /*= 1.0*/
                          )
{
  return GetPixelCount<VolumeMaskImageType>(inImage, threshold);
}

EvaluationValueType GetVolume(
	const VolumeMaskImageType * inImage,
	const float               threshold)
{
  return GetVolume<VolumeMaskImageType>(inImage, threshold);
}

InternalPixelType GetMin(const VolumeImageType * inImage)
{
	return GetMin<VolumeImageType>(inImage);
};

MaskPixelType GetMin(const VolumeMaskImageType * inImage)
{
	return GetMin<VolumeMaskImageType>(inImage);
};

InternalPixelType GetMax(const VolumeImageType * inImage)
{
	return GetMax<VolumeImageType>(inImage);
};


MaskPixelType GetMax(const VolumeMaskImageType * inImage)
{
	return GetMax<VolumeMaskImageType>(inImage);
};

InternalPixelType GetSum(const VolumeImageType * inImage)
{
	return GetSum<VolumeImageType>(inImage);
};

InternalPixelType GetSD(const VolumeImageType * inImage)
{
	return GetSD<VolumeImageType>(inImage);
};

InternalPixelType GetQuantile(const VolumeImageType * inImage, float percentile)
{
	return GetQuantile<VolumeImageType>(inImage, percentile);
};


} //namespace ccipd} //namespace ccipd
