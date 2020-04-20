#ifndef __StatisticalPerformanceMeasures_hxx
#define __StatisticalPerformanceMeasures_hxx


#include "ccipdStatisticalPerformanceMeasures.h"

#include "ccipdConfusionMatrix.h"


namespace ccipd
{

	// AA TODO: Should be moved to types.h
	struct StatisticalPerformanceMeasures
	{
		double Accuracy, Precision, Sensitivity, Specificity;

		StatisticalPerformanceMeasures() : Accuracy(0.0), Precision(0.0), Sensitivity(0.0), Specificity(0.0){}
		
	};

	StatisticalPerformanceMeasures calculateStatisticalPerformanceMeasures(ConfusionMatrix & cm);

	
	
	template <class TScalarImageType>
	StatisticalPerformanceMeasures
	calculateStatisticalPerformanceMeasures(  typename TScalarImageType::Pointer   pInputImage
	                                        , typename TScalarImageType::Pointer   pReferenceImage
	                                        , typename TScalarImageType::PixelType NegativeValue   //= 0
	                                        , typename TScalarImageType::PixelType PositiveValue   //= 1
	                                       )
	{
		ConfusionMatrix cm = CalculateConfusionMatrix<TScalarImageType>(pInputImage, pReferenceImage, NegativeValue, PositiveValue);
        StatisticalPerformanceMeasures spm = calculateStatisticalPerformanceMeasures(cm);
		return spm;
	}

	



} // namespace ccipd


#endif // __TestStatisticalPerformanceMeasures_hxx