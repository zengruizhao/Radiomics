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
%     strIMPath='C:\Dataset\WUSTL_Oral_Histology\Training40\';
%     strPath='C:\Nutstore\Nutstore\Training40_features\';
    
    strIMPath='C:\Dataset\WUSTL_Oral_Histology\Training50\';
    strPath='C:\Dataset\WUSTL_Oral_Histology\Training50_features\';
    
    dirIM=dir(strIMPath);
    dirIM(1:2)=[];
end

% dirIM=dir(strIMPath);
% dirIM(1:2)=[];
bandWidth=30;
%% The label information
% for the 40 training
% set_trainNon=[9 42 47 65 69 98 101 110 117 118 7 36 40 48 76 83 82 81 92 106];
% set_trainPro=[4 35 43 45 56 58 63 75 94 115 19 22 26 38 49 61 70 90 109 114];

% for the 50 training
set_trainNon=[9 42 47 65 69 98 101 110 117 118 7 36 40 48 76 83 82 81 92 106 50 24 30 79 67];
set_trainPro=[4 35 43 45 56 58 63 75 94 115 19 22 26 38 49 61 70 90 109 114 32 107 80 15 72];

set_train=[set_trainNon set_trainPro];
%% gather features on Pro/non-Por cases, one row for one case, one column for one feature
% idx=1;
% data_epi=[];%zeros(length(dirPro_Full),216);
% data_stroma=[];%zeros(length(dirPro_Full),216);
% 
% labels=[];
% patientIDs=[];
% %%% gather 'full features'
% for i=1:length(dirIM)
%     strC=dirIM(i).name;
%     intIMIdx=str2num(strC(end-6:end-3));
%     %     strIM=sprintf('%sPro_%d.mat',strPath_Pro,intIMIdx);
%     %     load(strIM);
%     %     strNuclei=sprintf('%sPro_%d.mat_nuclei.mat',strPath_Pro,intIMIdx);
%     %     load(strNuclei);
%     load(sprintf('%sFullFeatures_IM%s_epistroma.mat',strPath,strC));
%     
%     data_epi(idx,:)=cell2mat(feats_epi);
%     data_stroma(idx,:)=cell2mat(feats_stroma);idx=idx+1;
%     
%     patientIDs(i)=intIMIdx;
%     if ismember(intIMIdx,set_trainPro)
%         labels(i)=1;% sum(labels);
%     else
%         labels(i)=0;
%     end
% end
% 
% idx=1;feature_list_epi=[];feature_list_stroma=[];
% for i=1:length(description_epi)
%     curC=description_epi{i};
%     for j=1:length(curC)
%         feature_list_epi{idx}=['epi-' curC{j}]; 
%         feature_list_stroma{idx}=['stroma-' curC{j}];     
%         idx=idx+1;
%     end
% end
% 
% %%% gather haralick features
% idxh=1;
% data_haralick_epi=[];data_haralick_stroma=[];
% 
% for i=1:length(dirIM)
%     strC=dirIM(i).name;
%     str=sprintf('%sHaralickFeatures_IM%s_epistroma.mat',strPath,dirIM(i).name);
%     load(str);
%     data_haralick_epi(idxh,:)=cell2mat(feats_epi);
%     data_haralick_stroma(idxh,:)=cell2mat(feats_stroma);
%     idxh=idxh+1;
% end
% data_epi=cat(2,data_epi,data_haralick_epi);
% data_stroma=cat(2,data_stroma,data_haralick_stroma);
% 
% curC=description_epi{1};
% for j=1:length(curC)
%     feature_list_epi{idx}=['epi-' curC{j}];
%     feature_list_stroma{idx}=['stroma-' curC{j}];  
%     idx=idx+1;
% end
% 
% %%% gather CCM features
% 
% idxc=1;
% data_ccm_epi=[];data_ccm_stroma=[];
% 
% for i=1:length(dirIM)
%     strC=dirIM(i).name;
%     str=sprintf('%sCCMFeatures_IM%s_epistroma.mat',strPath,dirIM(i).name);
%     load(str);
%     data_ccm_epi(idxc,:)=feats_epi;
%     data_ccm_stroma(idxc,:)=feats_stroma;
%     idxc=idxc+1;
% end
% data_epi=cat(2,data_epi,data_ccm_epi);
% data_stroma=cat(2,data_stroma,data_ccm_stroma);
% 
% curC=description_epi;
% for j=1:length(curC)
%     feature_list_epi{idx}=['epi-' curC{j}];
%     feature_list_stroma{idx}=['stroma-' curC{j}];  
%     idx=idx+1;
% end
% 
% %%% combine epi and stroma features into one
% feature_list_epistroma=cat(2,feature_list_epi,feature_list_stroma);
% data_epistroma=cat(2,data_epi,data_stroma);
% 
% % save OralCavityFullFeatures_epistroma_train50.mat
% % load OralCavityFullFeatures_epistroma_train50.mat
% %%  need to remove the feature with NaN
% idx_nan=[];
% for i=1:size(data_epi,2)
%     curfv=data_epi(:,i);
%     if sum(isnan(curfv))>0 |  sum(isinf(curfv))>0 
%         idx_nan=[idx_nan i];
%     end 
% end
% data_epi_nonan=data_epi;
% data_epi_nonan(:,idx_nan)=[];
% feature_list_epi_nonan=feature_list_epi;
% feature_list_epi_nonan(idx_nan)=[];
% 
% idx_nan=[];
% for i=1:size(data_stroma,2)
%     curfv=data_stroma(:,i);
%     if sum(isnan(curfv))>0|  sum(isinf(curfv))>0 
%         idx_nan=[idx_nan i];
%     end 
% end
% data_stroma_nonan=data_stroma;
% data_stroma_nonan(:,idx_nan)=[];
% feature_list_stroma_nonan=feature_list_stroma;
% feature_list_stroma_nonan(idx_nan)=[];
% 
% feature_list_epi=feature_list_epi_nonan;
% feature_list_stroma=feature_list_stroma_nonan;
% 
% data_epi=data_epi_nonan;
% data_stroma=data_stroma_nonan;
% %%% combine epi and stroma features into one
% feature_list_epistroma=cat(2,feature_list_epi,feature_list_stroma);
% data_epistroma=cat(2,data_epi,data_stroma);
% %% load pre computed feature sets
% save('OralCavityFullFeatures_epistroma_nonnan_train50.mat','feature_list_epistroma','data_epistroma','labels','patientIDs');
% % load OralCavityFullFeatures_epistroma_nonnan_train50.mat
% % load OralCavityFullFeatures_ws_8_18_CGa=0.4_0.5_CCGa=0.36_0.4.mat
% % save OralCavityFullFeatures_ws_8_18a=0.35_0.45_mixedtype.mat
% % save OralCavityFullFeatures_ws_8_18a=0.36_0.4.mat
% % load OralCavityFullFeatures_a=0.38.mat
% % load OralCavityFullFeatures_ws_8_18a=0.36_0.4.mat
% 
% %% below are for the non epi/stroma separated features
% %% gather features on Pro/non-Por cases, one row for one case, one column for one feature
% idx=1;
% data=[];%zeros(length(dirPro_Full),216);
% labels=[];
% %%% gather 'full features'
% for i=1:length(dirIM)
%     strC=dirIM(i).name;
%     intIMIdx=str2num(strC(end-6:end-3));
%     %     strIM=sprintf('%sPro_%d.mat',strPath_Pro,intIMIdx);
%     %     load(strIM);
%     %     strNuclei=sprintf('%sPro_%d.mat_nuclei.mat',strPath_Pro,intIMIdx);
%     %     load(strNuclei);
%     load(sprintf('%sFullFeatures_IM%s.mat',strPath,strC));
%     
%     data(idx,:)=cell2mat(feats);idx=idx+1;
%     if ismember(intIMIdx,set_trainPro)
%         labels(i)=1;
%     else
%         labels(i)=0;
%     end
% end
% 
% idx=1;feature_list=[];
% for i=1:length(description)
%     curC=description{i};
%     for j=1:length(curC)
%         feature_list{idx}=curC{j}; idx=idx+1;
%     end
% end
% 
% %%% gather haralick features
% idxh=1;
% data_haralick=[];
% for i=1:length(dirIM)
%     strC=dirIM(i).name;
%     intIMIdx=str2num(strC(end-6:end-3));
%     %     strIM=sprintf('%sPro_%d.mat',strPath_Pro,intIMIdx);
%     %     load(strIM);
%     %     strNuclei=sprintf('%sPro_%d.mat_nuclei.mat',strPath_Pro,intIMIdx);
%     %     load(strNuclei);
%     str=sprintf('%sHaralickFeatures_IM%s.mat',strPath,dirIM(i).name);
%     load(str);
%     data_haralick(idxh,:)=cell2mat(feats);idxh=idxh+1;
% end
% data=cat(2,data,data_haralick);
% 
% curC=description{1};
% for j=1:length(curC)
%     feature_list{idx}=curC{j}; idx=idx+1;
% end
% 
% %%% gather CCM features
% 
% idxc=1;
% data_ccm=[];
% 
% for i=1:length(dirIM)
%     strC=dirIM(i).name;
%     str=sprintf('%sCCMFeatures_IM%s.mat',strPath,dirIM(i).name);
%     load(str);
%     data_ccm(idxc,:)=feats;
%     idxc=idxc+1;
% end
% data=cat(2,data,data_ccm);
% 
% curC=description;
% for j=1:length(curC)
%     feature_list{idx}=curC{j};
%     idx=idx+1;
% end
% %%  need to remove the feature with NaN
% idx_nan=[];
% for i=1:size(data,2)
%     curfv=data(:,i);
%     if sum(isnan(curfv))>0|  sum(isinf(curfv))>0 
%         idx_nan=[idx_nan i];
%     end 
% end
% data_nonan=data;
% data_nonan(:,idx_nan)=[];
% feature_list_nonan=feature_list;
% feature_list_nonan(idx_nan)=[];
% %% load pre computed feature sets
% save('OralCavityFullFeatures_nonan_train50.mat','feature_list_nonan','data_nonan');
% % load OralCavityFullFeatures_nonan_train50.mat
%% pre select the feature below
% clear all
%% load pre computed feature sets from epi/stroma seperated computations
load OralCavityFullFeatures_epistroma_nonnan_train50.mat
%% pull features from the whole image
load OralCavityFullFeatures_nonan_train50.mat

feature_list_all=cat(2,feature_list_epistroma,feature_list_nonan);
data_all=cat(2,data_epistroma,data_nonan);
%% lock down top features with low correlation for whole data
dataw_epistroma=simplewhiten(data_all);
feature_list_epistroma=feature_list_all;

[TTidx,p] = prunefeatures_new(dataw_epistroma, labels, 'ttestp');

idx_TTest=TTidx(p<0.05);
% set_p=p(1:K);
setTopF_TTest_epistroma=feature_list_epistroma(idx_TTest)';

set_candiF=Lpick_top_n_features_with_pvalue_correlation(data_all,idx_TTest,18);
set_fff=feature_list_all(set_candiF)';
%% visulize top TTest picked features
dataw=data_all;
feature_list=feature_list_all;

g=1;
for i=(g-1)*9+1:(g-1)*9+9
    v=dataw(:,set_candiF(i));
    group=logical(labels);
    if mod(i,9)==0
        figure(g); subplot(3,3,9);hold on;
    else
        figure(g); subplot(3,3,mod(i,9));hold on;
    end
    
    h=boxplot(v, group,'notch','off','whisker',.5,'outliersize',2,'labels',{'Nonprogressor','Progressor'});
    set(h,{'linew'},{2});
    set(findobj(gca,'Type','text'),'FontSize',50,'fontweight','bold');
    
    strTitle=sprintf('Top%d: %s',i,feature_list{set_candiF(i)});
    title(strTitle);
    xlabel('Class type');
    ylabel('Feature value');
    hold off;
end
%% focus on CCM features first
set_name={'CCM'};
[~,data_ccm,feature_list_ccm]=Lfind_featureidx_by_name(set_name,data_all,feature_list_all);

%% feature selection using ttest, with data whitening

% K=50;
dataw_epistroma=simplewhiten(data_ccm);
feature_list_epistroma=feature_list_ccm;

[TTidx,p] = prunefeatures_new(dataw_epistroma, labels, 'ttestp');

idx_TTest=TTidx(p<0.05);
% set_p=p(1:K);
setTopF_TTest_epistroma=feature_list_epistroma(idx_TTest)';
%% visulize top TTest picked features
% close all;
% dataw=data_ccm;
% feature_list=feature_list_ccm;
% 
% g=1;
% for i=(g-1)*9+1:(g-1)*9+9
%     v=dataw(:,idx_TTest(i));
%     group=logical(labels);
%     if mod(i,9)==0
%         figure(g); subplot(3,3,9);hold on;
%     else
%         figure(g); subplot(3,3,mod(i,9));hold on;
%     end
%     
%     h=boxplot(v, group,'notch','off','whisker',.5,'outliersize',2,'labels',{'Nonprogressor','Progressor'});
%     set(h,{'linew'},{2});
%     set(findobj(gca,'Type','text'),'FontSize',50,'fontweight','bold');
%     
%     strTitle=sprintf('Top%d: %s',i,feature_list{idx_TTest(i)});
%     title(strTitle);
%     xlabel('Class type');
%     ylabel('Feature value');
%     hold off;
% end
%% check a feature
% idx_top1=idx_TTest(1);
% dataw_top1=dataw(:,idx_top1); %dataw_top1=simplewhiten(dataw_top1);
% 
% dataw_top1_p=dataw_top1(logical(labels));dataw_top1_n=dataw_top1(~logical(labels));
% 
% figure; plot(find(labels),dataw_top1_p,'p');hold on;
% plot(find(~labels),dataw_top1_n,'o');
% title(feature_list{ idx_top1});
% hold off;
%% compute the correlation between two features
% corr(dataw(:,idx_TTest(1)),dataw(:,idx_TTest(16)))
% X=data_ccm(:,idx_TTest);
% [RHO]=corr(X);
%% lock down top features with low correlation
set_candiF=Lpick_top_n_features_with_pvalue_correlation(data_ccm,idx_TTest,9);
set_fff=feature_list_ccm(set_candiF)';

%% visulize top TTest picked features
dataw=data_ccm;
feature_list=feature_list_ccm;

g=1;
for i=(g-1)*9+1:(g-1)*9+9
    v=dataw(:,set_candiF(i));
    group=logical(labels);
    if mod(i,9)==0
        figure(g); subplot(3,3,9);hold on;
    else
        figure(g); subplot(3,3,mod(i,9));hold on;
    end
    
    h=boxplot(v, group,'notch','off','whisker',.5,'outliersize',2,'labels',{'Nonprogressor','Progressor'});
    set(h,{'linew'},{2});
    set(findobj(gca,'Type','text'),'FontSize',50,'fontweight','bold');
    
    strTitle=sprintf('Top%d: %s',i,feature_list{set_candiF(i)});
    title(strTitle);
    xlabel('Class type');
    ylabel('Feature value');
    hold off;
end

%% %% do the classification using random forests
% testdata=data_ccm;
% 
% intIter=20;%run number
% intFolds=3;%fold number
% % set_Feature=idx_mRMR;
% % set_Feature=idx_TTest([1,4,5,9,10]);
% % set_Feature=idx_TTest(1:5);
% % set_Feature=idx_Wilcoxon;
% % set_Feature=[1:length(feature_list)];
% % set_Feature=[4,12,21];
% % set_Feature=[17,21,6];
% 
% set_Feature=idx_TTest([3,8,14,17,19,23,24,26,27]);
% set_Feature=idx_TTest([3,8,14,17,19]);%,23,24,26,27]);
% set_Feature=set_candiF(1:5);
% 
% % set_Feature=idx_TTest([1,2,3,4]);%,23,24,26,27]);
% testlabels=labels;
% testdata=simplewhiten(testdata);
% 
% % do not consider imbalanced data
% resultImbalancedC45 = nFold_BaggedC45(testdata(:,set_Feature),testlabels,1,intFolds,intIter);
% [~,idxMaxAUC_Im]=max([resultImbalancedC45.AUC]);
% [~,idxMinAUC_Im]=min([resultImbalancedC45.AUC]);
% bestResult_Im=resultImbalancedC45(idxMaxAUC_Im);
% worstResult_Im=resultImbalancedC45(idxMinAUC_Im);
% 
% figure(1);
% plotroc(bestResult_Im.labels,bestResult_Im.prediction,'BaggedC45');
% fprintf('The mean performance: mean AUC=%.4f, best AUC =%.4f, worst AUC =%.4f\n',...
%     mean([resultImbalancedC45.AUC]),bestResult_Im.AUC,worstResult_Im.AUC);

%% strategy II: do the feature selection and rank the features based on 100 runs 3 fold cross validation 
%% focus on useful CCM features 
set_usful_ccm_names={'OutsideBoundaryIntensityRange','Eccentricity','MeanIntensity'};
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
[~,data_ccm_useful_c,feature_list_ccm_useful_c]=Lfind_featureidx_by_name(set_usful_a_names,data_ccm_useful_b,feature_list_ccm_useful_b);
% auc=0.69
%% focus on specific statistics
set_usful_a_names={'median','std','skewness','kurtosis'};
[~,data_ccm_useful_d,feature_list_ccm_useful_d]=Lfind_featureidx_by_name(set_usful_a_names,data_ccm_useful_c,feature_list_ccm_useful_c);
% auc=0.76 in 10 fold cv

%% add features from other classes
% {'stroma-Arch:Disorder of distance to 7 Nearest Neighbors';'stroma-Delaunay:Side Length Disorder'};
% actually the architectual features
%% grab architectual features 
load('GraphFeatureDescription.mat');
set_usful_arch_names=GraphFeatureDescription;
[~,data_arch,feature_list_arch]=Lfind_featureidx_by_name(set_usful_arch_names,data_all,feature_list_all);
%% epi and stroma only
set_usful_a_names={'epi','stroma'};
[~,data_arch_a,feature_list_arch_a]=Lfind_featureidx_by_name(set_usful_a_names,data_arch,feature_list_arch);

%% cat features from diff feature classes
data_useful=cat(2,data_ccm_useful_d,data_arch_a);
feature_list_useful=cat(2,feature_list_ccm_useful_d,feature_list_arch_a);
%% focus on specific statistics
% set_usful_a_names={'0.5','0.40'};
% % set_usful_a_names={'0.5'};
% [~,data_ccm_useful_e,feature_list_ccm_useful_e]=Lfind_featureidx_by_name(set_usful_a_names,data_ccm_useful_d,feature_list_ccm_useful_d);
% auc=0.72
%%
intIter=100;%run number
intFolds=10;%fold number

% set_Feature=set_candiF(1:5);
% data_all_w=simplewhiten(data_ccm(:,set_Feature));
% feature_list_t=feature_list_ccm(set_Feature);
% 
% data_all_w=simplewhiten(data_ccm_useful);
% feature_list_t=feature_list_ccm_useful;
% 
% data_all_w=simplewhiten(data_ccm_useful_a);
% feature_list_t=feature_list_ccm_useful_a;
% 
% data_all_w=simplewhiten(data_ccm_useful_b);
% feature_list_t=feature_list_ccm_useful_b;
% 
% data_all_w=simplewhiten(data_ccm_useful_c);
% feature_list_t=feature_list_ccm_useful_c;
% 
% data_all_w=simplewhiten(data_ccm_useful_d);
% feature_list_t=feature_list_ccm_useful_d;

% data_all_w=simplewhiten(data_ccm_useful_e);
% feature_list_t=feature_list_ccm_useful_e;

data_all_w=simplewhiten(data_useful);
feature_list_t=feature_list_useful;

% para.classifier='BaggedC45';
% para.num_top_feature=5;
% [resultImbalancedC45,feature_scores] = nFold_AnyClassifier_withFeatureselection(data_all_w,labels,feature_list_t,para,1,intFolds,intIter);
% % auc=0.85
% 
% para.classifier='QDA';
% para.num_top_feature=5;
% [resultImbalancedC45,feature_scores] = nFold_AnyClassifier_withFeatureselection(data_all_w,labels,feature_list_t,para,1,intFolds,intIter);
% % auc =0.84
% 
% para.classifier='SVM';
% para.num_top_feature=5;
% [resultImbalancedC45,feature_scores] = nFold_AnyClassifier_withFeatureselection(data_all_w,labels,feature_list_t,para,1,intFolds,intIter);
% % auc=0.86

para.classifier='LDA';
para.num_top_feature=5;
[resultImbalancedC45,feature_scores] = nFold_AnyClassifier_withFeatureselection(data_all_w,labels,feature_list_t,para,1,intFolds,intIter);
% auc=0.88


[~,idxMaxAUC_Im]=max([resultImbalancedC45.AUC]);
[~,idxMinAUC_Im]=min([resultImbalancedC45.AUC]);
bestResult_Im=resultImbalancedC45(idxMaxAUC_Im);
worstResult_Im=resultImbalancedC45(idxMinAUC_Im);

figure(1);
plotroc(bestResult_Im.labels,bestResult_Im.prediction,'BaggedC45');
fprintf('The mean performance: mean AUC=%.4f, best AUC =%.4f, worst AUC =%.4f\n',...
    mean([resultImbalancedC45.AUC]),bestResult_Im.AUC,worstResult_Im.AUC);

[val,idx]=sort(feature_scores,'descend');
set_fff=feature_list_t(idx(1:50))';

set_Feature=idx(1:6);
set_Feature_name=feature_list_useful(set_Feature)';
%% test top features by resubmittion 10-fold cv
intIter=100;%run number
intFolds=10;%fold number
set_Feature=idx(1:6);
set_Feature_name=feature_list_useful(set_Feature)';
% do not consider imbalanced data
resultImbalancedC45 = nFold_BaggedC45(data_all_w(:,set_Feature),labels,1,intFolds,intIter);
[~,idxMaxAUC_Im]=max([resultImbalancedC45.AUC]);
[~,idxMinAUC_Im]=min([resultImbalancedC45.AUC]);
bestResult_Im=resultImbalancedC45(idxMaxAUC_Im);
worstResult_Im=resultImbalancedC45(idxMinAUC_Im);

figure(1);
plotroc(bestResult_Im.labels,bestResult_Im.prediction,'BaggedC45');
fprintf('The mean performance: mean AUC=%.4f, best AUC =%.4f, worst AUC =%.4f\n',...
    mean([resultImbalancedC45.AUC]),bestResult_Im.AUC,worstResult_Im.AUC);
%% get the data for validation from validation set, without data whitening

load('OralCavityFullFeatures_epistroma_testing68.mat','data_epistroma','feature_list_epistroma');
load('OralCavityFullFeatures_testing68.mat','data','feature_list');
feature_list_all=cat(2,feature_list_epistroma,feature_list);
data_all=cat(2,data_epistroma,data);

setFinalFeature_for_classifer=set_Feature_name;
%%% get the same feature data that we used in the training
set_feature_idx=[];
for i=1:length(setFinalFeature_for_classifer)
    curF=setFinalFeature_for_classifer(i);
    for j=1:length(feature_list_all)
        if strcmp(curF,feature_list_all(j));
            set_feature_idx=[set_feature_idx j];
        end
    end
end
% feature_list_all(set_feature_idx);
data_validation=data_all(:,set_feature_idx);
load patientIDs_validationset.mat
%% %% build the final classifer and get the predictive label using Random Forest
methodstring = 'BaggedC45';
options = statset('UseParallel','never','UseSubstreams','never');
C_rf = TreeBagger(50,data_useful(:,set_Feature),labels,'FBoot',0.667,'oobpred','on','Method','classification','NVarToSample','all','NPrint',4,'Options',options);    % create bagged d-tree classifiers from training
[Yfit,Scores] = predict(C_rf,data_validation);   % use to classify testing
% [Yfit,Scores] = predict(C_rf,data_train);   % use to classify testing
label_rf=str2double(Yfit);
% com(:,1)=label_rf;com(:,2)=labels;

%%% ID_label_pair is the one I want to report to the doctor
sum(label_rf)/68
ID_label(:,1)=patientIDs;ID_label(:,2)=label_rf;
% save('predictive_label_RF.mat','ID_label','data_validation','data_useful','set_Feature','setFinalFeature_for_classifer');
load predictive_label_RF.mat
%% %% build the final classifer and get the predictive label using SVM
%% %% build the final classifer and get the predictive label using LDA
try 
    [~,~,probs,~,c] = classify(data_validation,data_useful(:,set_Feature),labels,'linear'); 
catch err
    [~,~,probs,~,c] = classify(data_validation,data_useful(:,set_Feature),labels,'diaglinear'); 
end
label_lda=zeros(68,1);
label_lda(probs(:,2)>0.5)=1;

%%% ID_label is the one I want to report to the doctor
sum(label_lda)/68
ID_label(:,1)=patientIDs;ID_label(:,2)=label_lda;
% save('predictive_label_LDA.mat','ID_label','data_validation','data_useful','set_Feature','setFinalFeature_for_classifer');
load predictive_label_LDA.mat
%% visulize the samples in 3-d space using top 3 features
X=data_useful(:,set_Feature);
 figure;hold on; plot3(X(labels==1,1),X(labels==1,2),X(labels==1,3),'ro','MarkerSize',10);
 plot3(X(labels==0,1),X(labels==0,2),X(labels==0,3),'bx','MarkerSize',10);
 legend('Progressor','Non-progressor');
 hold off;
 %% visulize the samples in 3-d space using top 3 ccm features
X=data_useful(:,set_Feature);
 figure;hold on; plot3(X(labels==1,1),X(labels==1,2),X(labels==1,4),'ro','MarkerSize',10);
 plot3(X(labels==0,1),X(labels==0,2),X(labels==0,4),'bx','MarkerSize',10);
 X=data_validation;
 
 plot3(X(ID_label(:,2)==1,1),X(ID_label(:,2)==1,2),X(ID_label(:,2)==1,4),'r+','MarkerSize',10);
 plot3(X(ID_label(:,2)==0,1),X(ID_label(:,2)==0,2),X(ID_label(:,2)==0,4),'bs','MarkerSize',10);
 legend('Progressor','Non-progressor','Predicted Progressor','Predicted Non-progressor');
 hold off;