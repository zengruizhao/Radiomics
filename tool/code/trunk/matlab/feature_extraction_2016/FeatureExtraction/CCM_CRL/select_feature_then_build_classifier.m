%% note in many places we need uncomment/comment to generate diff results
% clear all
addpath(genpath('/Users/chenglu/Nutstore/PathImAnalysis_Program/Program/LClassifier/ClassificationPackage_CCIPD2009'));
addpath(genpath('/Users/chenglu/Nutstore/Repository/matlab/matlab/feature_extraction'));
addpath(genpath('/Users/chenglu/Nutstore/Repository/George-newbranch'));
addpath(genpath('/Users/chenglu/Nutstore/Repository/George-newbranch/george_code/newbranch/tools/FeatureSelectionTools'));
addpath(genpath('/Users/chenglu/Nutstore/Repository/George-newbranch/george_code/newbranch/tools/AnalysisTools'));
addpath(genpath('C:\Nutstore\Nutstore\Repository\George-newbranch\george_code\newbranch\tools'));
addpath(genpath('/Users/chenglu/Nutstore/PathImAnalysis_Program/Program/OralCavity/'));
addpath(genpath('C:\Nutstore\Nutstore\Repository\jon_code\jon_code\Jons_classifier\'));
addpath(genpath('C:\Nutstore\Nutstore\Repository\jon_code\jon_code\'));
addpath(genpath('C:\Nutstore\Nutstore\Repository\matlab\matlab\feature_extraction\mrmr_feature_select\'));
%%
if ismac()
%     strIMPath='/Users/chenglu/Documents/Dataset/WUSTL_Oral_Histology/Training40/';
% %     strPath='/Users/chenglu/Documents/Dataset/WUSTL_Oral_Histology/Training40_features/';
%     strPath='/Users/chenglu/Nutstore/Training40_features/';
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
%% load pre computed feature sets from epi/stroma seperated computations
load OralCavityFullFeatures_epistroma_nonnan_train50.mat
%% pull features from the whole image
load OralCavityFullFeatures_nonan_train50.mat

feature_list_all=cat(2,feature_list_epistroma,feature_list_nonan);
data_all=cat(2,data_epistroma,data_nonan);

data_all_w=simplewhiten(data_all);
%%% remove the {'stroma-Arch:Disorder of distance to 5 Nearest Neighbors'}
%%% and {'stroma-Arch:Disorder of distance to 3 Nearest Neighbors'}; since
%%% they are highly redundent to {'stroma-Arch:Disorder of distance to 7 Nearest Neighbors'}
% set_remove=[10293,10292,10289,10290,10291];
% data_all_w(:,set_remove)=[];
% feature_list_all(set_remove)=[];     
%% step I: do the feature selection and rank the features based on 100 runs 3 fold cross validation 
intIter=100;%run number
intFolds=3;%fold number

[resultImbalancedC45,feature_scores] = nFold_BaggedC45_withFeatureselection(data_all_w,labels,feature_list_all,1,intFolds,intIter);

% save('RandomForest_pickfeatureonthefly_10runs_ranksum.mat','resultImbalancedC45','feature_scores');
[~,idxMaxAUC_Im]=max([resultImbalancedC45.AUC]);
[~,idxMinAUC_Im]=min([resultImbalancedC45.AUC]);
bestResult_Im=resultImbalancedC45(idxMaxAUC_Im);
worstResult_Im=resultImbalancedC45(idxMinAUC_Im);

figure(1);
plotroc(bestResult_Im.labels,bestResult_Im.prediction,'BaggedC45');
fprintf('The mean performance: mean AUC=%.4f, best AUC =%.4f, worst AUC =%.4f\n',...
    mean([resultImbalancedC45.AUC]),bestResult_Im.AUC,worstResult_Im.AUC);

%%% find top features
set_pickedF_idx=find(feature_scores);
[value,idx]=sort(feature_scores(set_pickedF_idx),'descend');
set_feature_onthefly=feature_list_all(set_pickedF_idx(idx))';
% save('featureranking_CCM_new.mat','set_feature_onthefly','value','idx','set_pickedF_idx');

% load('featureranking_CCM_new.mat','set_feature_onthefly','value','idx','set_pickedF_idx');

%%% visulize top TTest picked features
close all;
dataw=data_all_w; idx_TTest=set_pickedF_idx(idx);
for i=1:9
    v=dataw(:,idx_TTest(i));
    group=labels;

    figure(1); subplot(3,3,i);hold on;
    h=boxplot(v, group,'notch','off','whisker',.5,'outliersize',2,'labels',{'Nonprogressor','Progressor'});
    set(h,{'linew'},{2});
    set(findobj(gca,'Type','text'),'FontSize',50,'fontweight','bold');
    strTitle=sprintf('Top%d: %s\n',i,feature_list_all{idx_TTest(i)});
    title(strTitle);
    xlabel('Class type');
    ylabel('Feature value');
    hold off;
end

%% step II: do cross validation on the picked features
%%  do the classification using random forest with data whitening 
resultImbalancedC45 = nFold_BaggedC45(data_all_w(:,set_pickedF_idx(idx(1:5))),labels,1,intFolds,intIter);
[~,idxMaxAUC_Im]=max([resultImbalancedC45.AUC]);
[~,idxMinAUC_Im]=min([resultImbalancedC45.AUC]);
bestResult_Im=resultImbalancedC45(idxMaxAUC_Im);
worstResult_Im=resultImbalancedC45(idxMinAUC_Im);

figure(1);
plotroc(bestResult_Im.labels,bestResult_Im.prediction,'BaggedC45');
fprintf('The mean performance: mean AUC=%.4f, best AUC =%.4f, worst AUC =%.4f\n',...
    mean([resultImbalancedC45.AUC]),bestResult_Im.AUC,worstResult_Im.AUC);
%% %% do the classification using QDA with data whitening
resultQDA = nFold_QDA(data_all_w(:,set_pickedF_idx(idx(1:5))),labels,1,intFolds,intIter);
[~,idxMaxAUC_QDA]=max([resultQDA.AUC]);
[~,idxMinAUC_QDA]=min([resultQDA.AUC]);
bestResult_QDA=resultQDA(idxMaxAUC_QDA);
worstResult_QDA=resultQDA(idxMinAUC_QDA);

figure(1);
plotroc(bestResult_QDA.labels,bestResult_QDA.prediction,'QDA');
fprintf('The mean performance: mean AUC=%.4f, best AUC =%.4f, worst AUC =%.4f\n',...
    mean([resultQDA.AUC]),bestResult_QDA.AUC,worstResult_QDA.AUC);
%% %% do the classification using random forests without data whitening

%%% remove the {'stroma-Arch:Disorder of distance to 5 Nearest Neighbors'}
%%% and {'stroma-Arch:Disorder of distance to 3 Nearest Neighbors'}; since
%%% they are highly redundent to {'stroma-Arch:Disorder of distance to 7 Nearest Neighbors'}
set_remove=[10293,10292,10289,10290,10291];
data_all(:,set_remove)=[];


resultImbalancedC45 = nFold_BaggedC45(data_all(:,set_pickedF_idx(idx(1:5))),labels,1,intFolds,intIter);
[~,idxMaxAUC_Im]=max([resultImbalancedC45.AUC]);
[~,idxMinAUC_Im]=min([resultImbalancedC45.AUC]);
bestResult_Im=resultImbalancedC45(idxMaxAUC_Im);
worstResult_Im=resultImbalancedC45(idxMinAUC_Im);

figure(1);
plotroc(bestResult_Im.labels,bestResult_Im.prediction,'BaggedC45');
fprintf('The mean performance: mean AUC=%.4f, best AUC =%.4f, worst AUC =%.4f\n',...
    mean([resultImbalancedC45.AUC]),bestResult_Im.AUC,worstResult_Im.AUC);
%% %% do the classification using QDA without data whitening
resultQDA = nFold_QDA(data_all(:,set_pickedF_idx(idx(1:5))),labels,1,intFolds,intIter);
[~,idxMaxAUC_QDA]=max([resultQDA.AUC]);
[~,idxMinAUC_QDA]=min([resultQDA.AUC]);
bestResult_QDA=resultQDA(idxMaxAUC_QDA);
worstResult_QDA=resultQDA(idxMinAUC_QDA);

figure(1);
plotroc(bestResult_QDA.labels,bestResult_QDA.prediction,'QDA');
fprintf('The mean performance: mean AUC=%.4f, best AUC =%.4f, worst AUC =%.4f\n',...
    mean([resultQDA.AUC]),bestResult_QDA.AUC,worstResult_QDA.AUC);
%% get the data for validation from validation set, without data whitening

load('OralCavityFullFeatures_epistroma_testing68.mat','data_epistroma','feature_list_epistroma');
load('OralCavityFullFeatures_testing68.mat','data','feature_list');
feature_list_all=cat(2,feature_list_epistroma,feature_list);
data_all=cat(2,data_epistroma,data);

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
% data_validation=simplewhiten(data_validation);
%% build the final classifer and get the predictive label using QDA
data_train=testdata(:,set_Feature);
labels12=labels;
labels12(labels==1)=2;
labels12(labels==0)=1;
try 
    [~,~,probs,~,c] = classify(data_validation,testdata(:,set_Feature),labels12,'quadratic'); 
catch err
    [~,~,probs,~,c] = classify(data_validation,testdata(:,set_Feature),labels12,'diagquadratic'); 
end
label_pre=zeros(68,1);
label_pre(probs(:,2)>0.5)=1;

%%% ID_label_pair is the one I want to report to the doctor

sum(label_pre)/68
load('patientIDs_validationset.mat','patientIDs');
ID_label_pair(:,1)=patientIDs;
ID_label_pair(:,2)=label_pre;

%%% test with prateek predictive label
load validation_labels_prateek.mat
sum(yfit)/68
ID_label_pair_compare=ID_label_pair;
ID_label_pair_compare(:,3)=yfit;
difff=ID_label_pair_compare(:,3)-ID_label_pair_compare(:,2);
sum(abs(difff));


%% %% build the final classifer and get the predictive label using Random Forest

methodstring = 'BaggedC45';
options = statset('UseParallel','never','UseSubstreams','never');
C_rf = TreeBagger(50,testdata(:,set_Feature),labels,'FBoot',0.667,'oobpred','on','Method','classification','NVarToSample','all','NPrint',4,'Options',options);    % create bagged d-tree classifiers from training
[Yfit,Scores] = predict(C_rf,data_validation);   % use to classify testing
% [Yfit,Scores] = predict(C_rf,data_train);   % use to classify testing
label_rf=str2double(Yfit);
% com(:,1)=label_rf;com(:,2)=labels;

%%% ID_label_pair is the one I want to report to the doctor

sum(label_rf)/68
load('patientIDs_validationset.mat','patientIDs');
ID_label_pair(:,1)=patientIDs;
ID_label_pair(:,2)=label_rf;

%%% test with prateek predictive label
load validation_labels_prateek.mat
sum(yfit)/68
ID_label_pair_compare=ID_label_pair;
ID_label_pair_compare(:,3)=yfit;
difff=ID_label_pair_compare(:,3)-ID_label_pair_compare(:,2);
sum(abs(difff));


%% below ar backup codes





%% %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% strategy II: do the feature selection and rank the features based on 100 runs 3 fold cross validation 
intIter=10;%run number
intFolds=3;%fold number

data_all_w=simplewhiten(data_all);
%%% remove the {'stroma-Arch:Disorder of distance to 5 Nearest Neighbors'}
%%% and {'stroma-Arch:Disorder of distance to 3 Nearest Neighbors'}; since
%%% they are highly redundent to {'stroma-Arch:Disorder of distance to 7 Nearest Neighbors'}

set_remove=[10293,10292,10289,10290,10291];
data_all_w(:,set_remove)=[];
feature_list_all(set_remove)=[];     


[resultImbalancedC45,feature_scores] = nFold_BaggedC45_withFeatureselection(data_all_w,labels,feature_list_all,1,intFolds,intIter);

save('RandomForest_pickfeatureonthefly_10runs_ranksum.mat','resultImbalancedC45','feature_scores');
[~,idxMaxAUC_Im]=max([resultImbalancedC45.AUC]);
[~,idxMinAUC_Im]=min([resultImbalancedC45.AUC]);
bestResult_Im=resultImbalancedC45(idxMaxAUC_Im);
worstResult_Im=resultImbalancedC45(idxMinAUC_Im);

figure(1);
plotroc(bestResult_Im.labels,bestResult_Im.prediction,'BaggedC45');
fprintf('The mean performance: mean AUC=%.4f, best AUC =%.4f, worst AUC =%.4f\n',...
    mean([resultImbalancedC45.AUC]),bestResult_Im.AUC,worstResult_Im.AUC);

%%% 
set_pickedF_idx=find(feature_scores);
[value,idx]=sort(feature_scores(set_pickedF_idx),'descend');
set_feature_onthefly=feature_list_all(set_pickedF_idx(idx));


%% build the final classifer using random forest
unq_tra_lab = unique(training_labels);
if numel(unq_tra_lab) ~= 2
    error('Only 2 labels allowed');
else
    idx1 = ismember(training_labels,unq_tra_lab(1));
    idx2 = ismember(training_labels,unq_tra_lab(2));
    training_labels(idx1) = 0;
    training_labels(idx2) = 1;
    idx1 = ismember(testing_labels,unq_tra_lab(1));
    idx2 = ismember(testing_labels,unq_tra_lab(2));
    testing_labels(idx1) = 0;
    testing_labels(idx2) = 1;
end

methodstring = 'BaggedC45';
options = statset('UseParallel','never','UseSubstreams','never');
C_rf = TreeBagger(50,training_set,training_labels,'FBoot',0.667,'oobpred','on','Method','classification','NVarToSample','all','NPrint',4,'Options',options);    % create bagged d-tree classifiers from training
%% get the predictive label for validation set
load('OralCavityFullFeatures_epistroma_testing68.mat','data_epistroma','feature_list_epistroma');
load('OralCavityFullFeatures_testing68.mat','data','feature_list');
feature_list_all=cat(2,feature_list_epistroma,feature_list);
data_all=cat(2,data_epistroma,data);

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
[Yfit,Scores] = predict(C_rf,data_validation);   % use to classify testing
stats.prediction = Scores(:,2); % Select probabilities -- check manual entry for 'predict', look at 'B' to make sure your reqd class is the second column
stats.decision = str2double(Yfit);
%% %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% strategy II: do the feature selection and rank the features based on 100 runs 3 fold cross validation 
intIter=10;%run number
intFolds=3;%fold number

data_all_w=simplewhiten(data_all);

% do not consider imbalanced data
[resultImbalancedC45,feature_scores] = nFold_BaggedC45_withFeatureselection(data_all_w,labels,1,intFolds,intIter);
[~,idxMaxAUC_Im]=max([resultImbalancedC45.AUC]);
[~,idxMinAUC_Im]=min([resultImbalancedC45.AUC]);
bestResult_Im=resultImbalancedC45(idxMaxAUC_Im);
worstResult_Im=resultImbalancedC45(idxMinAUC_Im);
%% below are the jon's method.
bdata=simplewhiten(data_all);
i=1; % random forest for feature selection
classifiers=1;% random forest for as the classifier

runs=50;
% classifiers=1:4;
% numfeatures=min([floor(datalength/10),datawidth]);
numfeatures=10;
% numfeatures=datawidth;
[cout, trainingsets, rankfeatlist, names]=crossfold_classifcation_feb10(bdata,labels, feature_list_all, patientIDs', i, classifiers, runs, numfeatures);
mean([cout.AUC]);

idx_fea=find(rankfeatlist(i).hitnumber);
hitnumber=rankfeatlist(i).hitnumber(idx_fea);

name_topfea=feature_list_all(idx_fea);

for k2=i;
    classifierout(i).output=cout;
    classifierout(i).trainingsets=trainingsets;
    classifierout(i).rankfeatlist=rankfeatlist;
    classifierout(i).names=names;
end
%% analyze the classifier outputs
% for k3=i
    for k2=classifiers
        report(i).name=testname;
        report(i).classifiers(k2).name=classifierout(i).names{k2};
        [FPR,TPR,~,AUC,~,~,~] = perfcurve(classifierout(i).output(k2).groundtruth, classifierout(i).output(k2).prediction,1);
        figure;
        plot(FPR, TPR);
        title([testname, '-', names{k2},'. Avg AUC = ', num2str(AUC), ', STDev AUC = ', num2str(std(classifierout(i).output(k2).AUC))]);
        xlabel('False Positive rate'); ylabel('True Positive rate');
        report(i).classifiers(k2).AUC=AUC;
        report(i).classifiers(k2).bestAUC=max(classifierout(i).output(k2).AUC);
        report(i).classifiers(k2).std_AUC_rf=std(classifierout(i).output(k2).AUC);
    end
    
% end
    %% do feature ranking

    [report(i).sorteddata, report(i).sortedheaders, index]=sort_data_by_vector(classifierout(i).rankfeatlist(i).hitnumber, bdata, inlabels, 'descend');

 
%% set up feature box plots
    m=1;
    for j=1:datalength %This sets up the box plot data
        for k=1:datawidth
    %         boxdata(m)=sorted_data(i,j);
            s=char(report(i).sortedheaders(k));
            charlim=min(length(s),12);
            if binaryout(j)==0
                boxlabels{j,k}=strcat('Low_', s(1:charlim));
            elseif binaryout(j)==1
                boxlabels{j,k}=strcat('High_', s(1:charlim));
            end
            m=m+1;
        end
    end
    [numdata, featurenumber]=size(sorted_data(i).lowODX);

%     for j=1:featurenumber %Gets ranksum comparisons for each feature
%         [ranksumh(j), ranksump(j)]=ranksum(sorted_data(i).lowODX(:,j), sorted_data(i).highODX(:,j));
%     end    
    %% do feature box plots
    for j=1:5 %This does the box plot
%     for j=[9] %This does the box plot
        figure;
        boxplot(report(i).sorteddata(:,j),boxlabels(:,j));
        title(strcat(report(i).name, '-', report(i).sortedheaders{j}));
        xlabel('Imaging Features')
        ylabel('normalized value')
    end
