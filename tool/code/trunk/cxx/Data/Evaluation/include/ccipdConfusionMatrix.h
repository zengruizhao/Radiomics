#ifndef __ccipdConfusionMatrix_h
#define __ccipdConfusionMatrix_h

#include "ccipdCore.h"
#include "ccipdDataDLL.h"

#include "ccipdConfusionMatrix.hxx"



//#include "ccipdDisableWarningsMacro.h"
//	#include <itkImageRegionIterator.h>
//#include "ccipdEnableWarningsMacro.h"

namespace ccipd
{

	/*
	A confusion matrix (Kohavi and Provost, 1998) contains information about actual and predicted classifications done by a classification system. Performance of such systems is commonly evaluated using the data in the matrix. The following table shows the confusion matrix for a two class classifier.
	 a || b
	========
	 c || d

The entries in the confusion matrix have the following meaning in the context of our study:

a is the number of correct predictions that an instance is negative (True Negatives),
b is the number of incorrect predictions that an instance is positive,
c is the number of incorrect of predictions that an instance negative, and
d is the number of correct predictions that an instance is positive.
	
	
	*/



	/*
		Calculates True Positive Rate

		Note: Pixels with values other that (default -ve value) 0 or (default +ve value) 1 will be ignored
		This can be used to select a pecific region of comparison
		*/

	Data_EXPORT
	template <class TScalarImageType>
	ConfusionMatrix
	CalculateConfusionMatrix(  typename TScalarImageType::Pointer   pInputImage
	                         , typename TScalarImageType::Pointer   pReferenceImage
	                         , typename TScalarImageType::PixelType NegativeValue   = 0
	                         , typename TScalarImageType::PixelType PositiveValue   = 1
	                        );


} // namespace


#endif // __ccipdStatisticalPerformanceMeasures_h