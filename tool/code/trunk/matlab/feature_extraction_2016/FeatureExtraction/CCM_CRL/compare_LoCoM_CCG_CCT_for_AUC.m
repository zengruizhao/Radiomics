%% this used for comparison of LoCoM with CCG, CCT
% please use the LFeatureAnalysis_* to get the feature data
%% load pre computed feature sets
load OralCavityFullFeatures.mat
%% spliting the features based on the name in epi or stroma
set_feature_name={'Morph','CGT','CCG','Haralick','CCM'};
for n=1:length(set_feature_name)
    for i=1:length(feature_list)
        if ~isempty(strfind(feature_list{i},set_feature_name{n}))
            idx(n)=i;
        end
    end
end

for n=1:1
    for i=1:length(feature_list)
        if ~isempty(strfind(feature_list{i},set_feature_name{n}))
            idxMorph_begin=i;
            break;
        end
    end
end
data_epi=data;feature_list_epi=feature_list;
%% CGT cell cluster tensor features
data_CGT_epi=data_epi(:,idx(1)+1:idx(2)); description_CGT_epi=feature_list_epi(:,idx(1)+1:idx(2));
%% CCG mixed type features
data_CCG_epi=data_epi(:,idx(2)+1:idx(3)); description_CCG_epi=feature_list_epi(:,idx(2)+1:idx(3));
%% CCM
data_ccm_epi=data_ccm;
description_CCM_epi=feature_list_epi(:,idx(4)+1:idx(5));
%% test for each individual features set for mean AUC
para.intIter=100;%run number
para.intFolds=3;%fold number
para.flag_show=0;
para.flag_classification=1;
para.numTopFeatures=4;

i=1;stdAUC=[];meanAUC=[];
[meanAUC(i),stdAUC(i)]=featureselection_randomforest(data_CGT_epi,labels,description_CGT_epi,para);i=i+1;
[meanAUC(i),stdAUC(i)]=featureselection_randomforest(data_CCG_epi,labels,description_CCG_epi,para);i=i+1;
[meanAUC(i),stdAUC(i)]=featureselection_randomforest(data_ccm_epi,labels,description_CCM_epi,para);i=i+1;

bar_y=meanAUC;
bar_x=1:length(bar_y);
bw_legend={'CCT','CCG','LoCoM'};
figure;
handles = barweb(meanAUC, stdAUC, .7, [], [], [], 'Classification AUC', jet, [], bw_legend, 2, 'plot');
% figure;
% handles = barweb(meanAUC, stdAUC, .7, [], [], [], 'Classification AUC', jet, [], [], 2, 'plot');

%  
% figure;
% bar(bar_x,bar_y);
% ylabel('Classification AUC');
% set(gca,'xticklabel',{'Graph basic','Morph','CCT','CCG-mixed','CCG-pure'});