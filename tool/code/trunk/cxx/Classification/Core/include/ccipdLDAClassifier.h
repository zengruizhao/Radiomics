/********************************************************************
	created:	5/20/2013   14:04
	filename: 	..\cxx_trunk_src\Classification\Core\include\ccipdLDAClassifier.h
	author:		Gohary
	purpose:	
*********************************************************************/
#ifndef __ccipdLDAClassifier_h
#define __ccipdLDAClassifier_h

// ccipd includes
#include "ccipdClassifierBase.h"
#include "ccipdDAClassifier.h"


namespace ccipd
{


	class ccipdLDAClassifier : public ccipdDAClassifier
	{
	public:
		ccipdLDAClassifier();
		~ccipdLDAClassifier();

		virtual bool Train(const EigenMatrixTypeD & data, const EigenLabelVector & labels);
		virtual ProbabilisticOutputType ClassifyProbabilistically(const EigenMatrixTypeD & dataToClassify ) const;
		//////////////////////////////////////////////////////////////////

	};



}

#endif // __ccipdLDAClassifier_h