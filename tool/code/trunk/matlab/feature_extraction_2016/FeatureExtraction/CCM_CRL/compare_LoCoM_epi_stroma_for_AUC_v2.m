%% this used for comparison of LoCoM extracted in epi, stroma, both epistroma-not consider epi/stroma
% please use the LFeatureAnalysis_* to get the feature data
%% pull features from epi/stroma
% load OralCavityFullFeatures_epistroma_nonnan.mat
load OralCavityFullFeatures_epistroma_nonnan_train50.mat

set_idx=[];
feature_list=feature_list_epi_nonan;
for i=1:length(feature_list)
    if ~isempty(strfind(feature_list{i},'CCM'))
        set_idx=[set_idx i];
    end
end
ccm_data_epi=data_epi_nonan(:,set_idx);
ccm_fname_epi=feature_list(set_idx);

set_idx=[];
feature_list=feature_list_stroma_nonan;
for i=1:length(feature_list)
    if ~isempty(strfind(feature_list{i},'CCM'))
        set_idx=[set_idx i];
    end
end
ccm_data_stroma=data_stroma_nonan(:,set_idx);
ccm_fname_stroma=feature_list(set_idx);
%%% combine epi stroma features
ccm_data_epistroma=cat(2,ccm_data_epi,ccm_data_stroma);
ccm_fname_epistroma=cat(2,ccm_fname_epi,ccm_fname_stroma);
%% pull features from the whole image
load OralCavityFullFeatures_nonan_train50.mat
set_idx=[];
feature_list=feature_list_nonan;
for i=1:length(feature_list)
    if ~isempty(strfind(feature_list{i},'CCM'))
        set_idx=[set_idx i];
    end
end
ccm_data_whole=data_nonan(:,set_idx); %whole means mixture of epi and stroma
ccm_fname_whole=feature_list(set_idx);
%% test for each individual features set for mean AUC
para.intIter=100;%run number
para.intFolds=3;%fold number
para.flag_show=0;
para.flag_classification=1;
para.numTopFeatures=4;

i=1;stdAUC=[];meanAUC=[];
[meanAUC(i),stdAUC(i),setC{i}]=featureselection_randomforest(ccm_data_whole,labels,ccm_fname_whole,para);i=i+1;
[meanAUC(i),stdAUC(i),setC{i}]=featureselection_randomforest(ccm_data_epi,labels,ccm_fname_epi,para);i=i+1;
[meanAUC(i),stdAUC(i),setC{i}]=featureselection_randomforest(ccm_data_stroma,labels,ccm_fname_stroma,para);i=i+1;
[meanAUC(i),stdAUC(i),setC{i}]=featureselection_randomforest(ccm_data_epistroma,labels,ccm_fname_epistroma,para);i=i+1;
para.numTopFeatures=8;
[meanAUC(i),stdAUC(i),setC{i}]=featureselection_randomforest(ccm_data_epistroma,labels,ccm_fname_epistroma,para);i=i+1;


% save compare_LoCoM_epi_stroma_for_AUC_v2.mat
% load compare_LoCoM_epi_stroma_for_AUC_v2.mat
% save compare_LoCoM_epi_stroma_for_AUC_v2_train50.mat
load compare_LoCoM_epi_stroma_for_AUC_v2_train50.mat
%% setC is the set of classifier in the multiple run cross validation
for i=1:length(setC)
    curC=setC{i};
    meanTP(i)=mean([curC.tp]);
    stdTP(i)=std([curC.tp]);
    meanTN(i)=mean([curC.tn]);
    stdTN(i)=std([curC.tn]);
end
%%
bar_y=meanAUC;
bar_x=1:length(bar_y);
bw_legend={'Mixed','Epi','Stroma','Epi&Stroma'};
figure;
handles = barweb(meanAUC, stdAUC, .7, [], [], [], 'Classification AUC', jet, [], bw_legend, 2, 'plot');

handles = barweb(meanAUC(1:end-1), stdAUC(1:end-1), .7, [], [], [], 'Classification AUC', jet, [], bw_legend, 2, 'plot');
% figure;
% handles = barweb(meanAUC, stdAUC, .7, [], [], [], 'Classification AUC', jet, [], [], 2, 'plot');

%  
% figure;
% bar(bar_x,bar_y);
% ylabel('Classification AUC');
% set(gca,'xticklabel',{'Graph basic','Morph','CCT','CCG-mixed','CCG-pure'});