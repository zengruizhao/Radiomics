%% visulizaiton of the CCM features
if ispc
    addpath(genpath('C:\Nutstore\Nutstore\PathImAnalysis_Program\Program\LClassifier\ClassificationPackage_CCIPD2009'));
    addpath(genpath('C:\Nutstore\Nutstore\Repository\matlab\matlab\feature_extraction'));
    addpath(genpath('C:\Nutstore\Nutstore\Repository\George-newbranch'));
    addpath(genpath('C:\Nutstore\Nutstore\Repository\George-newbranch\george_code\newbranch\tools\FeatureSelectionTools'));
    addpath(genpath('C:\Nutstore\Nutstore\Repository\George-newbranch\george_code\newbranch\tools\AnalysisTools'));
else
    addpath(genpath('/Users/chenglu/Nutstore/PathImAnalysis_Program/Program/LClassifier/ClassificationPackage_CCIPD2009'));
    addpath(genpath('/Users/chenglu/Nutstore/Repository/matlab/matlab/feature_extraction'));
    addpath(genpath('/Users/chenglu/Nutstore/Repository/George-newbranch'));
    addpath(genpath('/Users/chenglu/Nutstore/Repository/George-newbranch/george_code/newbranch/tools/FeatureSelectionTools'));
    addpath(genpath('/Users/chenglu/Nutstore/Repository/George-newbranch/george_code/newbranch/tools/AnalysisTools'));
end
%% load pre computed feature sets
load OralCavityFullFeatures_epistroma_nonnan_train50.mat
load predictive_label_LDA.mat
%% visulize top TTest picked features
close all;

dataw=data_useful; idx_TTest=set_Feature;
feature_list=setFinalFeature_for_classifer;

for i=1:6
    v=dataw(:,idx_TTest(i));
    group=labels;

    figure(1); subplot(3,2,i);hold on;
    h=boxplot(v, group,'notch','off','whisker',.5,'outliersize',2,'labels',{'Nonprogressor','Progressor'});
    set(h,{'linew'},{2});
    set(findobj(gca,'Type','text'),'FontSize',50,'fontweight','bold');

    strTitle=sprintf('Top%d: %s',i,feature_list{i});
    title(strTitle);
    xlabel('Class type');
    ylabel('Feature value');
    hold off;
end
%% based on the top features, we pick cases to make examplar for feature map displaying
%% top 1 feature
idxtop=4;

idx_top1=idx_TTest(idxtop);

name_top1=feature_list{idxtop};
para_a=str2num(name_top1(end-3:end));
%%% take out all values and check the max/min to determine the extrem cases
% dataw=data_epistroma;
dataw_top1=dataw(:,idx_top1);

dataw_top1_p=dataw_top1(logical(labels));dataw_top1_n=dataw_top1(~logical(labels));
% check
figure(100); plot(find(labels),dataw_top1_p,'p');hold on;
plot(find(~labels),dataw_top1_n,'o');hold off;
% be carefull of the index
[maxv,maxidx]=max(dataw_top1_p);
[minv,minidx]=min(dataw_top1_n);
feature_limits=[minv,maxv];

indx_in_label_p=find(labels);
idx_p=indx_in_label_p(maxidx);

% %%%% test here
% strIMPath='C:\Dataset\WUSTL_Oral_Histology\Training40\';
% dirIM=dir([strIMPath '*.tif']);
% paraC.name=dirIM(idx_p).name;
% extractfullfeature_OroCavity_v3_identify_case(paraC);
% load('C:\Nutstore\Nutstore\Training40_features\CCMFeatures_IMTMA 2_019.tif_epistroma_test.mat');

indx_in_label_n=find(~labels);
idx_n=indx_in_label_n(minidx);

%% check a case here 
% idx_p=idx_n;
strIMPath='C:\Dataset\WUSTL_Oral_Histology\Training50\';
dirIM=dir([strIMPath '*.tif']);
paraC.name=dirIM(idx_p).name;
paraC.alpha=para_a; paraC.r=0.2;
paraC.flag_useotherfeaturelimits=0;
paraC.feature_limits=[];
feature_names={'Area','MajorAxisLength','MinorAxisLength','Eccentricity','Orientation','EquivDiameter','Solidity','Perimeter','Circularity','EllipticalDeviation',...
    'MeanIntensity','IntensityDeviation','IntensityRange','MeanInsideBoundaryIntensity','InsideBoundaryIntensityDeviation','InsideBoundaryIntensityRange',...
    'MeanOutsideBoundaryIntensity','OutsideBoundaryIntensityDeviation','OutsideBoundaryIntensityRange','BoundarySaliency'};
feature_names=feature_names';

% 'OutsideBoundaryIntensityRange' is 19th
paraC.idx_Mor=19;

% haralick_feature_names={'contrast-energy','contrast-inverse-moment','contrast-ave','contrast-var',...
%     'contrast-ent','intensity-ave','intensity-var','intensity-ent','entropy','energy',...
%     'correlation','info-measure1','info-measure2'};

% 'contrast-energy' is 1
paraC.idx_Har=1;
% 'contrast-ent' is 5
paraC.idx_Har=5;

% statistics
% curCCM=[mean(all_feat) median(all_feat) std(all_feat)  range(all_feat) kurtosis(all_feat) skewness(all_feat)];
paraC.idx_Sta=6;

paraC.epi=1; % in epi or stroma

extractfullfeature_OroCavity_v4_identify_case(paraC);