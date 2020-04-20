
#include "ccipdStatisticalPerformanceMeasures.h"
#include "ccipdConfusionMatrix.h"


using namespace std;

namespace ccipd
{
	StatisticalPerformanceMeasures calculateStatisticalPerformanceMeasures(ConfusionMatrix & cm)
	{
		StatisticalPerformanceMeasures spm;

		if ((cm.TP + cm.TN + cm.FP + cm.FN) != 0)
			spm.Accuracy = (cm.TP + cm.TN) / (cm.TP + cm.TN + cm.FP + cm.FN);
		else
		{
			cerr << "Error(15-13): TP, TN, FP and FN are all zeros, unable to calculate any of the Performance Measures ..." << endl;
			return spm;
		}

		if ((cm.TP + cm.FP) != 0)
			spm.Precision = cm.TP / (cm.TP + cm.FP);
		else
			cerr << "Error(23-13): TP and FP are all zeros, unable to calculate Precision ..." << endl;

		if ((cm.TP + cm.FN) != 0)
			spm.Sensitivity = cm.TP / (cm.TP + cm.FN);
		else
			cerr << "Error(28-13): TP and FN are all zeros, unable to calculate Sensitivity ..." << endl;

		if ((cm.TN + cm.FP) != 0)
			spm.Specificity = cm.TN / (cm.TN + cm.FP);
		else
			cerr << "Error(33-13): TN and FP are all zeros, unable to calculate Specificity ..." << endl;

		return spm;
	}




} // namespace ccipd
