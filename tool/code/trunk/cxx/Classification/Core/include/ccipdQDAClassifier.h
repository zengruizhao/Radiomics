/********************************************************************
	created:	5/20/2013   14:14
	filename: 	...\code\cxx_trunk_src\Classification\Core\include\ccipdQDAClassifier.h
	author:		Gohary
	purpose:	
*********************************************************************/
#ifndef __ccipdQDAClassifier_h
#define __ccipdQDAClassifier_h

// ccipd includes
#include "ccipdClassifierBase.h"
#include "ccipdDAClassifier.h"

namespace ccipd
{
	class ccipdQDAClassifier : public ccipdDAClassifier
	{
	public:
		ccipdQDAClassifier();
		~ccipdQDAClassifier();
		//////////////////////////////////////////////////////////////////////////
    /**
		  * brief     : Training Function for QDA (Quadratic Discriminant Analysis) Classification
		  * Created   : Ahmad 5/20/2013 13:38
		  * Updated   : Ahmad 08/29/2013 17:28
      * Params[in]: data  : this is the training data
      *             labels: labels for the training data, tells a data record (sample/observation) belongs to which class
		  * Notes     : 1: numbers in 'labers' are zero-base unsigned integers, this means that classes' numbers starts from zero (first class takes '0', second class takes '1' and so on ...
      *             2: no gaps in numbering the classes, this means that if the maximum number in 'labels' is 7 for example, then all the preceding unsigned int should occur to exist at least once
                       i.e. this means each included class in the training dataset should have at least one sample/observation which belongs to it. 
	    **/ 
		virtual bool Train(const EigenMatrixTypeD & data, const EigenLabelVector & labels);

		virtual ProbabilisticOutputType ClassifyProbabilistically(const EigenMatrixTypeD & dataToClassify ) const;

	};
}

#endif // __ccipdQDAClassifier_h
