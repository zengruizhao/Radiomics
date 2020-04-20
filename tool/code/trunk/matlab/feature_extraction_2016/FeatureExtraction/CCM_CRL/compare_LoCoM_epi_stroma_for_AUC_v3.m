%% this used for comparison of LoCoM extracted in epi, stroma, both epistroma-not consider epi/stroma
% please use the LFeatureAnalysis_* to get the feature data
addpath(genpath('/Users/chenglu/Nutstore/PathImAnalysis_Program/Program/LClassifier/ClassificationPackage_CCIPD2009'));
addpath(genpath('/Users/chenglu/Nutstore/Repository/matlab/matlab/feature_extraction'));
addpath(genpath('/Users/chenglu/Nutstore/Repository/George-newbranch'));
addpath(genpath('/Users/chenglu/Nutstore/Repository/George-newbranch/george_code/newbranch/tools/FeatureSelectionTools'));
addpath(genpath('/Users/chenglu/Nutstore/Repository/George-newbranch/george_code/newbranch/tools/AnalysisTools'));
addpath(genpath('C:\Nutstore\Nutstore\Repository\George-newbranch\george_code\newbranch\tools'));
addpath(genpath('/Users/chenglu/Nutstore/PathImAnalysis_Program/Program/OralCavity/'));

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
ccm_data_epistroma=cat(2,ccm_data_epi,ccm_data_stroma);
ccm_fname_epistroma=cat(2,ccm_fname_epi,ccm_fname_stroma);

%% pull features from the whole image
load OralCavityFullFeatures_nonan_train50.mat

%% focus on CCM features only
set_name={'CCM'};
[~,data_ccm,feature_list_ccm]=Lfind_featureidx_by_name(set_name,data_nonan,feature_list_nonan);

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
%% focus on specific statistics
set_usful_a_names={'median','std','skewness','kurtosis'};
[~,ccm_data_whole,ccm_fname_whole]=Lfind_featureidx_by_name(set_usful_a_names,data_ccm_useful_b,feature_list_ccm_useful_b);

%% test for each individual features set for mean AUC
para.intIter=100;%run number
para.intFolds=10;%fold number
para.flag_show=0;
para.flag_classification=1;
para.numTopFeatures=4;

i=1;stdAUC=[];meanAUC=[];
[meanAUC(i),stdAUC(i),setC{i}]=featureselection_randomforest_v2(ccm_data_whole,labels,ccm_fname_whole,para);i=i+1;
[meanAUC(i),stdAUC(i),setC{i}]=featureselection_randomforest_v2(ccm_data_epi,labels,ccm_fname_epi,para);i=i+1;
[meanAUC(i),stdAUC(i),setC{i}]=featureselection_randomforest_v2(ccm_data_stroma,labels,ccm_fname_stroma,para);i=i+1;
[meanAUC(i),stdAUC(i),setC{i}]=featureselection_randomforest_v2(ccm_data_epistroma,labels,ccm_fname_epistroma,para);i=i+1;
% para.numTopFeatures=8;
% [meanAUC(i),stdAUC(i),setC{i}]=featureselection_randomforest(ccm_data_epistroma,labels,ccm_fname_epistroma,para);i=i+1;


% save compare_LoCoM_epi_stroma_for_AUC_v3.mat
% load compare_LoCoM_epi_stroma_for_AUC_v2.mat
% save compare_LoCoM_epi_stroma_for_AUC_v2_train50.mat
% load compare_LoCoM_epi_stroma_for_AUC_v2_train50.mat
%% setC is the set of classifier in the multiple run cross validation
for i=1:length(setC)
    curC=setC{i};
    meanTP(i)=mean([curC.tp]);
    stdTP(i)=std([curC.tp]);
    meanTN(i)=mean([curC.tn]);
    stdTN(i)=std([curC.tn]);
end
%%
cd /Users/chenglu/Nutstore/PathImAnalysis_Program/Program/Miscellaneous

bar_y=meanAUC;
bar_x=1:length(bar_y);
bw_legend={'Whole','Epi','Stroma','Epi&Stroma'};
figure;
handles = barweb(meanAUC, stdAUC, .7, [], [], [], 'Classification AUC', jet, [], bw_legend, 2, 'plot');

% handles = barweb(meanAUC(1:end-1), stdAUC(1:end-1), .7, [], [], [], 'Classification AUC', jet, [], bw_legend, 2, 'plot');
% figure;
% handles = barweb(meanAUC, stdAUC, .7, [], [], [], 'Classification AUC', jet, [], [], 2, 'plot');

%  
% figure;
% bar(bar_x,bar_y);
% ylabel('Classification AUC');
% set(gca,'xticklabel',{'Graph basic','Morph','CCT','CCG-mixed','CCG-pure'});
 
% figure;
% bar(bar_x,bar_y);
% ylabel('Classification AUC');
% set(gca,'xticklabel',{'Graph basic','Morph','CCT','CCG-mixed','CCG-pure'});