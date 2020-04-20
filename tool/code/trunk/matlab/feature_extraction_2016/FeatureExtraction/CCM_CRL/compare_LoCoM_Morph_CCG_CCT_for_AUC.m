%% this used for comparison of LoCoM with CCG, CCT
% please use the LFeatureAnalysis_* to get the feature data
%% load pre computed feature sets from epi/stroma seperated computations
load OralCavityFullFeatures_epistroma_nonnan_train50.mat
%% focus on CCM features only
set_name={'CCM'};
[~,data_ccm,feature_list_ccm]=Lfind_featureidx_by_name(set_name,data_epistroma,feature_list_epistroma);

%% focus on useful CCM features 
% set_usful_ccm_names={'OutsideBoundaryIntensityRange','Eccentricity','MeanIntensity'};
set_usful_ccm_names={'OutsideBoundaryIntensityRange','Eccentricity','MeanIntensity','Area','IntensityRange','MeanOutsideBoundaryIntensity'};
[~,data_ccm_useful,feature_list_ccm_useful]=Lfind_featureidx_by_name(set_usful_ccm_names,data_ccm,feature_list_ccm);
% auc=0.58
%% focus on specific CCG resolution
set_usful_a_names={'0.38','0.48','0.50','0.40'};
[~,data_ccm_useful_a,feature_list_ccm_useful_a]=Lfind_featureidx_by_name(set_usful_a_names,data_ccm_useful,feature_list_ccm_useful);
% auc=0.62
%% focus on specific harralick features
set_usful_a_names={'correlation','contrast-energy','contrast-ent','info-measure2'};
[~,data_ccm_useful_b,feature_list_ccm_useful_b]=Lfind_featureidx_by_name(set_usful_a_names,data_ccm_useful_a,feature_list_ccm_useful_a);
% auc=0.69
%% focus on stroma/epi
set_usful_a_names={'epi','stroma'};
% [~,data_ccm_useful_c,feature_list_ccm_useful_c]=Lfind_featureidx_by_name(set_usful_a_names,data_ccm_useful_a,feature_list_ccm_useful_a);

[~,data_ccm_useful_c,feature_list_ccm_useful_c]=Lfind_featureidx_by_name(set_usful_a_names,data_ccm_useful_b,feature_list_ccm_useful_b);
% auc=0.69
%% focus on specific statistics
set_usful_a_names={'median','std','skewness','kurtosis'};
[~,data_ccm_useful_d,feature_list_ccm_useful_d]=Lfind_featureidx_by_name(set_usful_a_names,data_ccm_useful_c,feature_list_ccm_useful_c);
% auc=0.76 in 10 fold cv

%%% pull feature for epi
set_name={'epi'};
[~,ccm_data_epi,ccm_fname_epi]=Lfind_featureidx_by_name(set_name,data_ccm_useful_d,feature_list_ccm_useful_d);

%%% pull feature for stroma
set_name={'stroma'};
[~,ccm_data_stroma,ccm_fname_stroma]=Lfind_featureidx_by_name(set_name,data_ccm_useful_d,feature_list_ccm_useful_d);

%%% combine epi stroma features
data_ccm_epi=cat(2,ccm_data_epi,ccm_data_stroma);
description_CCM_epi=cat(2,ccm_fname_epi,ccm_fname_stroma);
%% CGT cell cluster tensor features
set_name={'CGT'};
[~,data_CGT_epi,description_CGT_epi]=Lfind_featureidx_by_name(set_name,data_epistroma,feature_list_epistroma);
%% CCG features
set_name={'CCG'};
[~,data_CCG_epi,description_CCG_epi]=Lfind_featureidx_by_name(set_name,data_epistroma,feature_list_epistroma);
%% Morphology features
set_name={'Morph'};
[~,data_Morph_epi,description_Morph_epi]=Lfind_featureidx_by_name(set_name,data_epistroma,feature_list_epistroma);
%% Basic Graph features
set_name={'Voronoi','Delaunay','MST','Arch'};
[~,data_BG_epi,description_BG_epi]=Lfind_featureidx_by_name(set_name,data_epistroma,feature_list_epistroma);

%% test for each individual features set for mean AUC
para.intIter=100;%run number
para.intFolds=10;%fold number
para.flag_show=0;
para.flag_classification=1;
para.numTopFeatures=4;

i=1;stdAUC=[];meanAUC=[];
[meanAUC(i),stdAUC(i)]=featureselection_randomforest_v2(data_CGT_epi,labels,description_CGT_epi,para);i=i+1;
[meanAUC(i),stdAUC(i)]=featureselection_randomforest_v2(data_CCG_epi,labels,description_CCG_epi,para);i=i+1;
[meanAUC(i),stdAUC(i)]=featureselection_randomforest_v2(data_ccm_epi,labels,description_CCM_epi,para);i=i+1;
[meanAUC(i),stdAUC(i)]=featureselection_randomforest_v2(data_BG_epi,labels,description_BG_epi,para);i=i+1;
[meanAUC(i),stdAUC(i)]=featureselection_randomforest_v2(data_Morph_epi,labels,description_Morph_epi,para);i=i+1;

%%
% save compare_LoCoM_Morph_CCG_CCT_for_AUC.mat
load compare_LoCoM_Morph_CCG_CCT_for_AUC.mat

%%
bar_y=meanAUC;
bar_x=1:length(bar_y);
if ~ispc()
cd /Users/chenglu/Nutstore/PathImAnalysis_Program/Program/Miscellaneous
else
cd C:\Nutstore\Nutstore\PathImAnalysis_Program\Program\Miscellaneous
end
meanAUC_show(1:2)=meanAUC(1:2);
stdAUC_show(1:2)=stdAUC(1:2);
meanAUC_show(3)=meanAUC(5);
stdAUC_show(3)=stdAUC(5);
meanAUC_show(4)=meanAUC(3);
stdAUC_show(4)=stdAUC(3);

bw_legend={'CCT','CCG','Morph','LoCoM'};

figure;
handles = barweb(meanAUC_show, stdAUC_show, .7, [], [], [], 'Classification AUC', jet, [], bw_legend, 2, 'plot');
% figure;
% handles = barweb(meanAUC, stdAUC, .7, [], [], [], 'Classification AUC', jet, [], [], 2, 'plot');

%  
% figure;
% bar(bar_x,bar_y);
% ylabel('Classification AUC');
% set(gca,'xticklabel',{'Graph basic','Morph','CCT','CCG-mixed','CCG-pure'});