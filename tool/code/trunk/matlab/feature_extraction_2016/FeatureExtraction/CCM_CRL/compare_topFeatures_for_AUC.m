% this is used to evaluating the top features in oral cavity
%% note in many places we need uncomment/comment to generate diff results
% clear all
addpath(genpath('/Users/chenglu/Nutstore/PathImAnalysis_Program/Program/LClassifier/ClassificationPackage_CCIPD2009'));
addpath(genpath('/Users/chenglu/Nutstore/Repository/matlab/matlab/feature_extraction'));
addpath(genpath('/Users/chenglu/Nutstore/Repository/George-newbranch'));
addpath(genpath('/Users/chenglu/Nutstore/Repository/George-newbranch/george_code/newbranch/tools/FeatureSelectionTools'));
addpath(genpath('/Users/chenglu/Nutstore/Repository/George-newbranch/george_code/newbranch/tools/AnalysisTools'));
addpath(genpath('C:\Nutstore\Nutstore\Repository\George-newbranch\george_code\newbranch\tools'));
addpath(genpath('/Users/chenglu/Nutstore/PathImAnalysis_Program/Program/OralCavity/'));

%%
if ismac()
    strIMPath='/Users/chenglu/Documents/Dataset/WUSTL_Oral_Histology/Training40/';
%     strPath='/Users/chenglu/Documents/Dataset/WUSTL_Oral_Histology/Training40_features/';
    strPath='/Users/chenglu/Nutstore/Training40_features/';
    dirIM=dir(strIMPath);
    dirIM(1:3)=[];
else
    strIMPath='C:\Dataset\WUSTL_Oral_Histology\Training40\';
    strPath='C:\Nutstore\Nutstore\Training40_features\';
    dirIM=dir(strIMPath);
    dirIM(1:2)=[];
end

% dirIM=dir(strIMPath);
% dirIM(1:2)=[];
bandWidth=30;
%% The label information
set_trainNon=[9 42 47 65 69 98 101 110 117 118 7 36 40 48 76 83 82 81 92 106];
set_trainPro=[4 35 43 45 56 58 63 75 94 115 19 22 26 38 49 61 70 90 109 114];
set_train=[set_trainNon set_trainPro];
%% pull features from epi/stroma
load OralCavityFullFeatures_epistroma_nonnan.mat
%% pull features from the whole image
load OralCavityFullFeatures_nonan.mat
%% combine features
data_combined=cat(2,data_epi_nonan,data_stroma_nonan,data_nonan);
feature_list_combined=cat(2,feature_list_epi_nonan,feature_list_stroma_nonan,feature_list_nonan);
%% feature selection using ttest, without data whitening
K=50;

dataw_epistroma=data_combined;
feature_list_epistroma=feature_list_combined;

[TTidx,confidence_epistroma] = prunefeatures_new(dataw_epistroma, labels, 'ttestp');
idx_TTest_epistroma=TTidx(1:K);
set_confidence_TTest_epistroma=confidence_epistroma(1:K);
setTopF_TTest_epistroma=feature_list_epistroma(idx_TTest_epistroma)';

%% visulize top TTest picked features
close all;
% dataw=dataw_epi; idx_TTest=idx_TTest_epi;
% feature_list=feature_list_epi;

% dataw=dataw_stroma; idx_TTest=idx_TTest_stroma;
% feature_list=feature_list_stroma;

dataw=dataw_epistroma; idx_TTest=idx_TTest_epistroma;
feature_list=feature_list_epistroma;

for i=1:9
    v=dataw(:,idx_TTest(i));
    group=labels;

    figure(1); subplot(3,3,i);hold on;
    h=boxplot(v, group,'notch','off','whisker',.5,'outliersize',2,'labels',{'Nonprogressor','Progressor'});
    set(h,{'linew'},{2});
    set(findobj(gca,'Type','text'),'FontSize',50,'fontweight','bold');

    strTitle=sprintf('Top%d: %s',i,feature_list{idx_TTest(i)});
    title(strTitle);
    xlabel('Class type');
    ylabel('Feature value');
    hold off;
end

%% do the classification using random forests
idx_TTest=idx_TTest_epistroma;
testdata=data_epistroma;

intIter=30;%run number
intFolds=3;%fold number
% set_Feature=idx_mRMR;
set_Feature=idx_TTest(1:1);

testlabels=labels;
testdata=simplewhiten(testdata);

para.intIter=100;%run number
para.intFolds=3;%fold number
para.flag_show=0;
para.flag_classification=1;
para.numTopFeatures=4;

i=1;stdAUC=[];meanAUC=[];
[meanAUC(i),stdAUC(i)]=featureselection_randomforest(data_ccm,labels,description,para);i=i+1;
[meanAUC(i),stdAUC(i)]=featureselection_randomforest(data_ccm_epi,labels,description_epi,para);i=i+1;
[meanAUC(i),stdAUC(i)]=featureselection_randomforest(data_ccm_stroma,labels,description_stroma,para);i=i+1;
para.numTopFeatures=8;
[meanAUC(i),stdAUC(i)]=featureselection_randomforest(data_ccm_epistroma,labels,description_epistroma,para);i=i+1;


figure(1);
plotroc(bestResult_Im.labels,bestResult_Im.prediction,'BaggedC45');

fprintf('The mean performance: mean AUC=%.4f, best AUC =%.4f, worst AUC =%.4f\n',...
    mean([resultImbalancedC45.AUC]),bestResult_Im.AUC,worstResult_Im.AUC);