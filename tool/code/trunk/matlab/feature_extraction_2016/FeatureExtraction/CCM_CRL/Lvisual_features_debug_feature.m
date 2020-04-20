%% visulizaiton of the CCM features
%% use this function for debuging and developing new features
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
%% pick a good feature to show from the whole feature set
%% focus on CCM features first
set_name={'CCM'};
[~,data_ccm,feature_list_ccm]=Lfind_featureidx_by_name(set_name,data_epistroma,feature_list_epistroma);

%%% feature selection using ttest, with data whitening

% K=50;
dataw_epistroma=simplewhiten(data_ccm);
feature_list_epistroma=feature_list_ccm;

[TTidx,p] = prunefeatures_new(dataw_epistroma, labels, 'ttestp');

idx_TTest=TTidx(p<0.05);
% set_p=p(1:K);
setTopF_TTest_epistroma=feature_list_epistroma(idx_TTest)';

%% based on the top features, we pick cases to make examplar for feature map displaying
%%% pick 1 feature
idxtop=19;

idx_top1=idx_TTest(idxtop);

name_top1=feature_list_epistroma{idx_top1};
para_a=str2num(name_top1(end-3:end));
%%% take out all values and check the max/min to determine the extrem cases
% dataw=data_epistroma;
dataw_top1=dataw_epistroma(:,idx_top1);

dataw_top1_p=dataw_top1(logical(labels));dataw_top1_n=dataw_top1(~logical(labels));
% check
figure(100); plot(find(labels),dataw_top1_p,'p');hold on;
plot(find(~labels),dataw_top1_n,'x');hold off;
% be carefull of the index
[maxv,maxidx]=max(dataw_top1_p);
[minv,minidx]=min(dataw_top1_n);
feature_limits=[minv,maxv];

%% the case we used for positive and negative
% the name of feature  is epi-CCM-IntensityRange:median(entropy)-a=0.42
idx_p=1;
idx_n=18;
%% show the ccm feature map here
strIMPath='C:\Dataset\WUSTL_Oral_Histology\Training50\';
dirIM=dir([strIMPath '*.tif']);

%%% change positive and negative cases here!!!!!!!!!!!!!!!!!!!!!!!!!!
paraC.name=dirIM(idx_p).name;
paraC.name=dirIM(idx_n).name;

paraC.alpha=para_a; paraC.r=0.2;
paraC.flag_useotherfeaturelimits=0;
paraC.feature_limits=[];
feature_names={'Area','MajorAxisLength','MinorAxisLength','Eccentricity','Orientation','EquivDiameter','Solidity','Perimeter','Circularity','EllipticalDeviation',...
    'MeanIntensity','IntensityDeviation','IntensityRange','MeanInsideBoundaryIntensity','InsideBoundaryIntensityDeviation','InsideBoundaryIntensityRange',...
    'MeanOutsideBoundaryIntensity','OutsideBoundaryIntensityDeviation','OutsideBoundaryIntensityRange','BoundarySaliency'};
feature_names=feature_names';

% 'OutsideBoundaryIntensityRange' is 19th
% EllipticalDeviation is 10
% IntensityRange is 13
paraC.idx_Mor=13;

% haralick_feature_names={'contrast-energy','contrast-inverse-moment','contrast-ave','contrast-var',...
%     'contrast-ent','intensity-ave','intensity-var','intensity-ent','entropy','energy',...
%     'correlation','info-measure1','info-measure2'};

% 'contrast-energy' is 1
paraC.idx_Har=1;
% 'contrast-ent' is 5
paraC.idx_Har=5;
% 'correlation' is 11
paraC.idx_Har=11;
% 'entropy' is 9
paraC.idx_Har=9;

% statistics
% curCCM=[mean(all_feat) median(all_feat) std(all_feat)  range(all_feat) kurtosis(all_feat) skewness(all_feat)];
paraC.idx_Sta=6;
paraC.idx_Sta=4;
paraC.idx_Sta=2;

paraC.epi=1; % in epi or stroma

extractfullfeature_OroCavity_v4_debug_develop_new_feature(paraC);
