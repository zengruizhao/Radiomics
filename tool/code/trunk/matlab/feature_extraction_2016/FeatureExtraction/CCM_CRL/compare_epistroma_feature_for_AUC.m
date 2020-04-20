%% this used for comparison of different feature set
% please use the LFeatureAnalysis_* to get the feature data
%% load pre computed feature sets
load OralCavityFullFeatures_epistroma.mat
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
%% basic cell graph features
data_BCG_epi=data_epi(:,1:idxMorph_begin-1); description_BCG_epi=feature_list_epi(:,1:idxMorph_begin-1);
data_BCG_stroma=data_stroma(:,1:idxMorph_begin-1); description_BCG_stroma=feature_list_stroma(:,1:idxMorph_begin-1);
%% morphlogical features
data_Morph_epi=data_epi(:,idxMorph_begin:idx(1)); description_Morph_epi=feature_list_epi(:,1:idx(1));
data_Morph_stroma=data_stroma(:,1:idx(1)); description_Morph_stroma=feature_list_stroma(:,1:idx(1));
%% CGT cell cluster tensor features
data_CGT_epi=data_epi(:,idx(1)+1:idx(2)); description_CGT_epi=feature_list_epi(:,idx(1)+1:idx(2));
data_CGT_stroma=data_stroma(:,idx(1)+1:idx(2)); description_CGT_stroma=feature_list_stroma(:,idx(1)+1:idx(2));
%% CCG mixed type features
data_CCG_epi=data_epi(:,idx(2)+1:idx(3)); description_CCG_epi=feature_list_epi(:,idx(2)+1:idx(3));
data_CCG_stroma=data_stroma(:,idx(2)+1:idx(3)); description_CCG_stroma=feature_list_stroma(:,idx(2)+1:idx(3));
%% haralick
description_haralick_epi=feature_list_epi(:,idx(3)+1:idx(4));
description_haralick_stroma=feature_list_stroma(:,idx(3)+1:idx(4));
%% CCM
description_CCM_epi=feature_list_epi(:,idx(4)+1:idx(5));
description_CCM_stroma=feature_list_stroma(:,idx(4)+1:idx(5));
%% test for each individual features set for mean AUC
para.intIter=100;%run number
para.intFolds=3;%fold number
para.flag_show=0;
para.flag_classification=1;
para.numTopFeatures=4;

load meanAUC.mat

i=1;stdAUC=[];meanAUC=[];
% meanAUC_CG_basic=featureselection_randomforest(feat_CG_basic,labels,name_CG_basic,para);
[meanAUC(i),stdAUC(i)]=featureselection_randomforest(data_Morph_epi,labels,description_Morph_epi,para);i=i+1;
[meanAUC(i),stdAUC(i)]=featureselection_randomforest(data_CGT_epi,labels,description_CGT_epi,para);i=i+1;
[meanAUC(i),stdAUC(i)]=featureselection_randomforest(data_CCG_epi,labels,description_CCG_epi,para);i=i+1;
[meanAUC(i),stdAUC(i)]=featureselection_randomforest(data_haralick_epi,labels,description_haralick_epi,para);i=i+1;
[meanAUC(i),stdAUC(i)]=featureselection_randomforest(data_ccm_epi,labels,description_CCM_epi,para);i=i+1;

[meanAUC(i),stdAUC(i)]=featureselection_randomforest(data_Morph_stroma,labels,description_Morph_stroma,para);i=i+1;
[meanAUC(i),stdAUC(i)]=featureselection_randomforest(data_CGT_stroma,labels,description_CGT_stroma,para);i=i+1;
[meanAUC(i),stdAUC(i)]=featureselection_randomforest(data_CCG_stroma,labels,description_CCG_stroma,para);i=i+1;
[meanAUC(i),stdAUC(i)]=featureselection_randomforest(data_haralick_stroma,labels,description_haralick_stroma,para);i=i+1;
[meanAUC(i),stdAUC(i)]=featureselection_randomforest(data_ccm_stroma,labels,description_CCM_stroma,para);i=i+1;

bar_y=meanAUC;
bar_x=1:length(bar_y);
bw_legend={'Epi:Morph','Epi:CCT','Epi:CCG','Epi:Texture','Epi:LoCoM','Stroma:Morph','Stroma:CCT','Stroma:CCG','Stroma:Texture','Stroma:LoCoM'};
figure;
handles = barweb(meanAUC, stdAUC, .7, [], [], [], 'Classification AUC', jet, [], bw_legend, 2, 'plot');
figure;
handles = barweb(meanAUC, stdAUC, .7, [], [], [], 'Classification AUC', jet, [], [], 2, 'plot');

%  
% figure;
% bar(bar_x,bar_y);
% ylabel('Classification AUC');
% set(gca,'xticklabel',{'Graph basic','Morph','CCT','CCG-mixed','CCG-pure'});