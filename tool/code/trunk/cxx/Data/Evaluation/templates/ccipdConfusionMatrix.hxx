#ifndef __ccipdConfusionMatrix_hxx
#define __ccipdConfusionMatrix_hxx

#include "ccipdDisableWarningsMacro.h"
	#include <itkImageRegionConstIterator.h>
	#include <iostream>
#include "ccipdEnableWarningsMacro.h"


namespace ccipd
{

	// Should be moved to types.h
	struct ConfusionMatrix
	{
		unsigned int TP; // True  Positive
		unsigned int FP; // False Positives
		unsigned int TN; // True  Negatives
		unsigned int FN; // False Negatives
		ConfusionMatrix() : TP(0), FP(0), TN(0), FN(0){}
	};



template <class TScalarImageType>
ConfusionMatrix 
CalculateConfusionMatrix(typename TScalarImageType::Pointer   pInputImage
, typename TScalarImageType::Pointer   pReferenceImage
, typename TScalarImageType::PixelType NegativeValue //= 0
, typename TScalarImageType::PixelType PositiveValue //= 1
)
{
	// Check that the size of the input image is the same as the size of the 
	if (pInputImage->GetLargestPossibleRegion().GetSize() != pReferenceImage->GetLargestPossibleRegion().GetSize())
	{
		std::cerr << "Error: dimension mismatch ...";
	}

	typedef itk::ImageRegionConstIterator<TScalarImageType> ConstIteratorType;

	ConstIteratorType in(pInputImage, pInputImage->GetRequestedRegion());
	ConstIteratorType ref(pReferenceImage, pInputImage->GetRequestedRegion());



	ConfusionMatrix cm;

	for (in.GoToBegin(), ref.GoToBegin(); !in.IsAtEnd(); ++in, ++ref)
	{
		if (ref.Get() == PositiveValue)
		{
			if (in.Get() == PositiveValue) cm.TP++;
			else if (in.Get() == NegativeValue) cm.FN++;
		}
		else if (ref.Get() == NegativeValue)
		{
			if (in.Get() == NegativeValue) cm.TN++;
			else if (in.Get() == PositiveValue) cm.FP++;
		}
	}

	return cm;
}



} // namespace


#endif
