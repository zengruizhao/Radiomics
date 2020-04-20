%%  need to remove the feature with NaN
addpath(genpath('/Users/chenglu/Nutstore/PathImAnalysis_Program/Program/LClassifier/ClassificationPackage_CCIPD2009'));
addpath(genpath('/Users/chenglu/Nutstore/Repository/matlab/matlab/feature_extraction'));
% addpath(genpath('/Users/chenglu/Nutstore/Repository/George-newbranch'));
addpath(genpath('/Users/chenglu/Nutstore/Repository/George-newbranch/george_code/newbranch/tools/FeatureSelectionTools'));
addpath(genpath('/Users/chenglu/Nutstore/Repository/George-newbranch/george_code/newbranch/tools/AnalysisTools'));
addpath(genpath('/Users/chenglu/Nutstore/PathImAnalysis_Program/Program/LClassifier/ClassificationPackage_CCIPD2009'));
addpath(genpath('/Users/chenglu/Nutstore/Repository/matlab/matlab/feature_extraction'));
addpath(genpath('/Users/chenglu/Nutstore/Repository/George-newbranch'));
addpath(genpath('/Users/chenglu/Nutstore/Repository/George-newbranch/george_code/newbranch/tools/FeatureSelectionTools'));
addpath(genpath('/Users/chenglu/Nutstore/Repository/George-newbranch/george_code/newbranch/tools/AnalysisTools'));
addpath(genpath('C:\Nutstore\Nutstore\Repository\George-newbranch\george_code\newbranch\tools'));
addpath(genpath('C:\Nutstore\Nutstore\PathImAnalysis_Program\Program\LClassifier\ClassificationPackage_CCIPD2009'));
addpath(genpath('/Users/chenglu/Nutstore/PathImAnalysis_Program/Program/OralCavity'));
addpath(genpath('C:\Nutstore\Nutstore\PathImAnalysis_Program\Program\Features\tSNE_matlab'));



bad=[8 15 17 26 24 29];
data=FLAIR_CELesion_CoLlAGe_stat_data;
data(bad,:)=[];
label=start_binaryout;
label(bad)=[];

%% MES
sum(label)/length(label)
para.featureranking='wilcoxon';
para.classifier='BaggedC45';
n=3;
nIter=10;
shuffle=1;
para.num_top_feature=3;

labels=label;
labels(labels==0)=-1;
labels(labels==1)=0;
labels(labels==-1)=1;

 [stats, feature_scores] = LnFold_BaggedC45_MES_withFeatureRanking(data,labels,para,shuffle,n,nIter);
 mean([stats.AUC]);
 
 figure;bar(feature_scores);
 
 % intIter=20;%run number
% intFolds=3;%fold number
% para.classifier='BaggedC45';
% para.featureranking='wilcoxon';
% para.num_top_feature=15;
% [resultImbalancedC45, feature_scores] = LnFold_BaggedC45_MES_withFeatureRanking(data_all_w,labels,para,1,intFolds,intIter);
% % auc=0.46
%% 
intIter=10;%run number
intFolds=3;%fold number
para.classifier='SVM';
para.featureranking='wilcoxon';
para.num_top_feature=3;
para.correlation_factor=0.9;

feature_list_t=num2cell(1:length(data));
[resultImbalancedC45,feature_scores] = nFold_AnyClassifier_withFeatureselection_v2(data,labels,feature_list_t,para,1,n,nIter);
mean([resultImbalancedC45.AUC]);