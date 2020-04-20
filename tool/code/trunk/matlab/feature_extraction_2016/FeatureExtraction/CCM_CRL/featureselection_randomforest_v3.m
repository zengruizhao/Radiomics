% v3 is the version rank feature on the fly then do classification
function [meanAUC,stdAUC,resultImbalancedC45,feature_scores]=featureselection_randomforest_v3(data,labels,data_name,para)

[resultImbalancedC45,feature_scores] = nFold_AnyClassifier_withFeatureselection_v2(data,labels,data_name,para,1,para.intFolds,para.intIter);

[~,idxMaxAUC_Im]=max([resultImbalancedC45.AUC]);
bestResult_Im=resultImbalancedC45(idxMaxAUC_Im);
meanAUC=mean([resultImbalancedC45.AUC]);
stdAUC=std([resultImbalancedC45.AUC]);
