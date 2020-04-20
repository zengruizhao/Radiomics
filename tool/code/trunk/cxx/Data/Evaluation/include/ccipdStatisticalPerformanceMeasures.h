#ifndef __StatisticalPerformanceMeasures_h
#define __StatisticalPerformanceMeasures_h

#include "ccipdStatisticalPerformanceMeasures.hxx"

class ConfusionMatrix;

namespace ccipd
{
	StatisticalPerformanceMeasures calculateStatisticalPerformanceMeasures(ConfusionMatrix & cm);
	


	template <class TScalarImageType>
	StatisticalPerformanceMeasures
	calculateStatisticalPerformanceMeasures(  typename TScalarImageType::Pointer   pInputImage
	                                        , typename TScalarImageType::Pointer   pReferenceImage 
	                                        , typename TScalarImageType::PixelType NegativeValue   = 0
	                                        , typename TScalarImageType::PixelType PositiveValue   = 1
	                                       );
	
	


} // namespace ccipd


#endif // __TestStatisticalPerformanceMeasures_h