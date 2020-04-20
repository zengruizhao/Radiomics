%% this used for comparison of LoCoM extracted in epi, stroma, both epistroma, not consider epi/stroma
% please use the LFeatureAnalysis_* to get the feature data
%% load pre computed feature sets
% load OralCavityFullFeatures.mat
% load OralCavityFullFeatures_epistroma.mat
load OralCavityFullFeatures_ccm.mat

%%% combine epi and stroma features into one
description_epistroma=cat(2,description_epi,description_stroma);
data_ccm_epistroma=cat(2,data_ccm_epi,data_ccm_stroma);
%% test for each individual features set for mean AUC
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

% save compare_LoCoM_epi_stroma_for_AUC.mat
load compare_LoCoM_epi_stroma_for_AUC.mat

bar_y=meanAUC;
bar_x=1:length(bar_y);
bw_legend={'Mixed','Epi','Stroma','Epi&Stroma'};
figure;
handles = barweb(meanAUC, stdAUC, .7, [], [], [], 'Classification AUC', jet, [], bw_legend, 2, 'plot');
% figure;
% handles = barweb(meanAUC, stdAUC, .7, [], [], [], 'Classification AUC', jet, [], [], 2, 'plot');

%  
% figure;
% bar(bar_x,bar_y);
% ylabel('Classification AUC');
% set(gca,'xticklabel',{'Graph basic','Morph','CCT','CCG-mixed','CCG-pure'});