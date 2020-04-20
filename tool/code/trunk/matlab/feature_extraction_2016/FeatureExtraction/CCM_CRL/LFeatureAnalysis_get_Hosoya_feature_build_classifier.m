% log:
% thi is used for test the CRL features
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
    strIMPath='/Users/chenglu/Documents/Dataset/WUSTL_Oral_Histology/Training50/';
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

idx=1;
data=[];%zeros(length(dirPro_Full),216);
labels=[];
for i=1:length(dirIM)
    strC=dirIM(i).name;
    intIMIdx=str2num(strC(end-6:end-3));
    %     strIM=sprintf('%sPro_%d.mat',strPath_Pro,intIMIdx);
    %     load(strIM);
    %     strNuclei=sprintf('%sPro_%d.mat_nuclei.mat',strPath_Pro,intIMIdx);
    %     load(strNuclei);
    load(sprintf('%sHosoyaFeatures_IM%s.mat',strPath,strC));
    for h=1:length(hosoya_all)
        % HosoyaF store all hosoya features at different packing(idx by h) of CCG in
        % different image(idx by i)
        HosoyaF{h,i}=hosoya_all(h); 
    end
    
    if ismember(intIMIdx,set_trainPro)
        labels(i)=1;
    else
        labels(i)=0;
    end
end
%%% below codes for the hosoya in stroma/epi separately
for i=1:length(dirIM)
    strC=dirIM(i).name;
    intIMIdx=str2num(strC(end-6:end-3));
    %     strIM=sprintf('%sPro_%d.mat',strPath_Pro,intIMIdx);
    %     load(strIM);
    %     strNuclei=sprintf('%sPro_%d.mat_nuclei.mat',strPath_Pro,intIMIdx);
    %     load(strNuclei);
    load(sprintf('%sHosoyaFeatures_IM%s_epistroma.mat',strPath,strC));
    for h=1:length(hosoya_all_epi)
        % HosoyaF store all hosoya features at different packing(idx by h) of CCG in
        % different image(idx by i)
        HosoyaF_epi{h,i}=hosoya_all_epi(h); 
        HosoyaF_stroma{h,i}=hosoya_all_stroma(h); 
    end
end

%% load pre-saved result
% save hosoyaF_train50.mat
load hosoyaF_train50.mat
%% check out the hosoya range for each CCG density
LgetHosoyaStatistics(HosoyaF,labels,set_alpha);
LgetHosoyaStatistics(HosoyaF_epi,labels,set_alpha);
LgetHosoyaStatistics(HosoyaF_stroma,labels,set_alpha);

%% lock down top features 
set_name={'a=0.40'};
[~,data_CRL,feature_list_CRL]=Lfind_featureidx_by_name(set_name,data_all,feature_list_all);

dataw_epistroma=simplewhiten(data_CRL);
feature_list_epistroma=feature_list_CRL;

[TTidx,p] = prunefeatures_new(dataw_epistroma, labels, 'ttestp');
idx_TTest=TTidx(p<0.05);
% set_p=p(1:K);
setTopF_TTest_epistroma=feature_list_epistroma(idx_TTest)';
para.correlation=0.9;
set_candiF=Lpick_top_n_features_with_pvalue_correlation(dataw_epistroma,idx_TTest,18,para.correlation);
set_fff=feature_list_epistroma(set_candiF)';

[TTidx,p_wilcoxon] = prunefeatures_new(dataw_epistroma, labels, 'wilcoxon');
idx_wilcoxon=TTidx(p_wilcoxon<0.1);
setTopF_wilcoxon_epistroma=feature_list_epistroma(idx_wilcoxon)';
set_candiF_wilcoxon=Lpick_top_n_features_with_pvalue_correlation(dataw_epistroma,idx_wilcoxon,18,para.correlation);
set_fff_wilcoxon=feature_list_epistroma(set_candiF_wilcoxon)';
%% feature ranking using data with CRL
intIter=50;%run number
intFolds=10;%fold number

data_all_w=simplewhiten(data_CRL);
feature_list_t=feature_list_CRL;

% para.classifier='QDA';
% para.num_top_feature=5;
% [resultImbalancedC45,feature_scores] = nFold_AnyClassifier_withFeatureselection(data_all_w,labels,feature_list_t,para,1,intFolds,intIter);
% % auc =

para.feature_score_method='weighted';
para.classifier='QDA';
para.num_top_feature=4;
para.featureranking='wilcoxon';
para.correlation_factor=.9;
[resultImbalancedC45,feature_scores] = nFold_AnyClassifier_withFeatureselection_v3(data_all_w,labels,feature_list_t,para,1,intFolds,intIter);
% auc=0.67
% 
% para.feature_score_method='weighted';
% para.classifier='QDA';
% para.num_top_feature=4;
% para.featureranking='ttest';
% para.correlation_factor=.9;
% [resultImbalancedC45,feature_scores] = nFold_AnyClassifier_withFeatureselection_v3(data_all_w,labels,feature_list_t,para,1,intFolds,intIter);
% % auc=0.62
% 
% 
% para.classifier='BaggedC45';
% para.feature_score_method='weighted';
% para.num_top_feature=4;
% para.featureranking='wilcoxon';
% para.correlation_factor=.9;
% % [resultImbalancedC45,feature_scores] = nFold_AnyClassifier_withFeatureselection(data_all_w,labels,feature_list_t,para,1,intFolds,intIter);
% [resultImbalancedC45,feature_scores] = nFold_AnyClassifier_withFeatureselection_v3(data_all_w,labels,feature_list_t,para,1,intFolds,intIter);
% % auc=0.66
% 
% para.classifier='LDA';
% para.feature_score_method='weighted';
% para.num_top_feature=4;
% para.featureranking='wilcoxon';
% para.correlation_factor=.9;
% [resultImbalancedC45,feature_scores] = nFold_AnyClassifier_withFeatureselection_v3(data_all_w,labels,feature_list_t,para,1,intFolds,intIter);
% % auc=0.65


[~,idxMaxAUC_Im]=max([resultImbalancedC45.AUC]);
[~,idxMinAUC_Im]=min([resultImbalancedC45.AUC]);
bestResult_Im=resultImbalancedC45(idxMaxAUC_Im);
worstResult_Im=resultImbalancedC45(idxMinAUC_Im);

figure(1);
plotroc(bestResult_Im.labels,bestResult_Im.prediction,para.classifier);
fprintf('The mean performance: mean AUC=%.4f, best AUC =%.4f, worst AUC =%.4f\n',...
    mean([resultImbalancedC45.AUC]),bestResult_Im.AUC,worstResult_Im.AUC);

[val,idx]=sort(feature_scores,'descend');
set_fff=feature_list_t(idx(1:50))';
figure;bar(val(1:10)/(para.num_top_feature*intIter*intFolds));
set(gca,'FontSize',20);ylabel('rank score');xlabel('feature name');

% set(gca,'XTickLabel',set_Feature_name','XTickLabelRotation',45,'FontSize',2);


set_Feature=idx(1:10);
set_Feature_name=feature_list_t(set_Feature)'
% save CRL_wilcoxon_BaggedC45.mat
%% visulize top features
dataw=data_CRL;
feature_list=feature_list_CRL;
for i=1:6
    v=dataw(:,set_Feature(i));
    group=logical(labels);
    
    figure(1); subplot(2,3,i);hold on;
    
    h=boxplot(v, group,'notch','off','whisker',.5,'outliersize',2,'labels',{'Nonprogressor','Progressor'});
    set(h,{'linew'},{2});
    set(findobj(gca,'Type','text'),'FontSize',50,'fontweight','bold');
    
    strTitle=sprintf('Top%d: %s',i,feature_list{set_Feature(i)});
    title(strTitle);
    xlabel('Class type');
    ylabel('Feature value');
    hold off;
end
%% back up code below
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
% dataw=data_all;
% feature_list=feature_list_all;
% 
% g=1;
% for i=(g-1)*9+1:(g-1)*9+9
%     v=dataw(:,set_candiF(i));
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
%     strTitle=sprintf('Top%d: %s',i,feature_list{set_candiF(i)});
%     title(strTitle);
%     xlabel('Class type');
%     ylabel('Feature value');
%     hold off;
% end
%% focus on CCM features first
set_name={'CCM'};
[~,data_ccm,feature_list_ccm]=Lfind_featureidx_by_name(set_name,data_all,feature_list_all);

%% feature selection using ttest, with data whitening

% K=50;
% dataw_epistroma=simplewhiten(data_ccm);
% feature_list_epistroma=feature_list_ccm;
% 
% [TTidx,p] = prunefeatures_new(dataw_epistroma, labels, 'ttestp');
% 
% idx_TTest=TTidx(p<0.05);
% % set_p=p(1:K);
% setTopF_TTest_epistroma=feature_list_epistroma(idx_TTest)';
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
% set_candiF=Lpick_top_n_features_with_pvalue_correlation(data_ccm,idx_TTest,9);
% set_fff=feature_list_ccm(set_candiF)';

%% visulize top TTest picked features
% dataw=data_ccm;
% feature_list=feature_list_ccm;
% 
% g=1;
% for i=(g-1)*9+1:(g-1)*9+9
%     v=dataw(:,set_candiF(i));
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
%     strTitle=sprintf('Top%d: %s',i,feature_list{set_candiF(i)});
%     title(strTitle);
%     xlabel('Class type');
%     ylabel('Feature value');
%     hold off;
% end

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
%% feature ranking
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
% auc=0.86

% para.classifier='SVM';
% para.num_top_feature=5;
% para.featureranking='ttest';
% % para.params.kernel='rbf';
% % para.params.kernel='linear';
% % para.params.c_range=-10:2:30;
% % para.params.g_range=-10:2:3;
% % para.params.cvfolds=3;
% [resultImbalancedC45,feature_scores] = nFold_AnyClassifier_withFeatureselection_v2(data_all_w,labels,feature_list_t,para,1,intFolds,intIter);

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
figure;bar(val(1:10));

set_Feature=idx(1:6);
set_Feature_name=feature_list_useful(set_Feature)';
%% visulize top features
dataw=data_useful;
feature_list=feature_list_useful;
for i=1:6
    v=dataw(:,set_Feature(i));
    group=logical(labels);
    
    figure(1); subplot(2,3,i);hold on;
    
    h=boxplot(v, group,'notch','off','whisker',.5,'outliersize',2,'labels',{'Nonprogressor','Progressor'});
    set(h,{'linew'},{2});
    set(findobj(gca,'Type','text'),'FontSize',50,'fontweight','bold');
    
    strTitle=sprintf('Top%d: %s',i,feature_list{set_Feature(i)});
    title(strTitle);
    xlabel('Class type');
    ylabel('Feature value');
    hold off;
end
%% test top features by resubmittion 10-fold cv
% intIter=100;%run number
% intFolds=10;%fold number
% set_Feature=idx(1:6);
% set_Feature_name=feature_list_useful(set_Feature)';
% % do not consider imbalanced data
% resultImbalancedC45 = nFold_BaggedC45(data_all_w(:,set_Feature),labels,1,intFolds,intIter);
% [~,idxMaxAUC_Im]=max([resultImbalancedC45.AUC]);
% [~,idxMinAUC_Im]=min([resultImbalancedC45.AUC]);
% bestResult_Im=resultImbalancedC45(idxMaxAUC_Im);
% worstResult_Im=resultImbalancedC45(idxMinAUC_Im);
% 
% figure(1);
% plotroc(bestResult_Im.labels,bestResult_Im.prediction,'BaggedC45');
% fprintf('The mean performance: mean AUC=%.4f, best AUC =%.4f, worst AUC =%.4f\n',...
%     mean([resultImbalancedC45.AUC]),bestResult_Im.AUC,worstResult_Im.AUC);
%% get the data for validation from validation set, without data whitening
load('OralCavityFullFeatures_epistroma_testing68.mat','data_epistroma','feature_list_epistroma');
load('OralCavityFullFeatures_testing68.mat','data','feature_list');
feature_list_all=cat(2,feature_list_epistroma,feature_list);
data_all=cat(2,data_epistroma,data);

load('TopFeatures.mat');
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
%% %% build the final classifer and get the predictive label using Random Forest with top 6 features
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
sum(ID_label(:,2))/68
% save('predictive_label_RF.mat','ID_label','data_validation','data_useful','set_Feature','setFinalFeature_for_classifer');
load predictive_label_RF.mat
%% %% build the final classifer and get the predictive label using Random Forest with top 5 features
methodstring = 'BaggedC45';
options = statset('UseParallel','never','UseSubstreams','never');
C_rf = TreeBagger(50,data_useful(:,set_Feature(1:5)),labels,'FBoot',0.667,'oobpred','on','Method','classification','NVarToSample','all','NPrint',4,'Options',options);    % create bagged d-tree classifiers from training
[Yfit,Scores] = predict(C_rf,data_validation(:,1:5));   % use to classify testing
% [Yfit,Scores] = predict(C_rf,data_train);   % use to classify testing
label_rf_5=str2double(Yfit);
% com(:,1)=label_rf;com(:,2)=labels;

%%% ID_label_pair is the one I want to report to the doctor
sum(label_rf_5)/68
ID_label(:,1)=patientIDs;ID_label(:,2)=label_rf_5;
sum(ID_label(:,2))/68
% save('predictive_label_RF_5features.mat','ID_label','data_validation','data_useful','set_Feature','setFinalFeature_for_classifer');
load predictive_label_RF_5features.mat
%% %% build the final classifer and get the predictive label using SVM
fake_testing_labels=zeros(1,size(data_validation,1));
%%% train SVM
para.params.kernel='linear';
% para.params.c_range=-10:2:30;
% para.params.g_range=-10:2:3;
% para.params.cvfolds=3;

data_training=data_useful(:,set_Feature(1:5));
data_train_and_validation=cat(1,data_training,data_validation(:,1:5));
data_train_and_validation_w=simplewhiten(data_train_and_validation);

label_svm_5 = svm_training_cheng(data_train_and_validation_w(1:50,:), labels, data_train_and_validation_w(51:end,:), fake_testing_labels,para.params);

sum(label_svm_5)/68
ID_label(:,1)=patientIDs;ID_label(:,2)=label_svm_5;
% save('predictive_label_svm_5.mat','ID_label','data_validation','data_useful','set_Feature','setFinalFeature_for_classifer');
sum(ID_label(:,2))/68
load predictive_label_svm_5.mat


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
sum(ID_label(:,2))/68
load predictive_label_LDA.mat
%% get the different between LDA and RF predictive labels
load predictive_label_LDA.mat
ID_label_LDA=ID_label;
load predictive_label_svm_5.mat
ID_label_svm_5=ID_label;

ID_label_diff=logical(abs(ID_label_LDA(:,2)-ID_label_svm_5(:,2)));
ID_label_keep=ones(68,1);ID_label_keep(:)=99;
ID_label_keep(ID_label_diff)=ID_label_LDA(ID_label_diff,2);
sum(ID_label_diff)/length(ID_label_diff)
%% visulize the samples in 3-d space using top 3 features
X=data_useful(:,set_Feature);
 figure;hold on; plot3(X(labels==1,1),X(labels==1,2),X(labels==1,3),'ro','MarkerSize',10);
 plot3(X(labels==0,1),X(labels==0,2),X(labels==0,3),'bx','MarkerSize',10);
 legend('Progressor','Non-progressor');
 hold off;
 %% visulize the training and testing samples in 3-d space using top 3 features
X=data_useful(:,set_Feature);
 figure;hold on; plot3(X(labels==1,1),X(labels==1,2),X(labels==1,3),'ro','MarkerSize',10);
 plot3(X(labels==0,1),X(labels==0,2),X(labels==0,3),'bx','MarkerSize',10);
X=data_validation;
 plot3(X(ID_label(:,2)==1,1),X(ID_label(:,2)==1,2),X(ID_label(:,2)==1,3),'r+','MarkerSize',10);
 plot3(X(ID_label(:,2)==0,1),X(ID_label(:,2)==0,2),X(ID_label(:,2)==0,3),'bs','MarkerSize',10);
 legend('Progressor','Non-progressor','Predicted Progressor','Predicted Non-progressor');
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