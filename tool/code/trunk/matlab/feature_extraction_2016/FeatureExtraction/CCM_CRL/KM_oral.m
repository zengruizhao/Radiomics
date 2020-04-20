%% calculating KM curve for oral cancer
if ismac()
    strIMPath='/Users/chenglu/Documents/Dataset/WUSTL_Oral_Histology/Training40/';
%     strPath='/Users/chenglu/Documents/Dataset/WUSTL_Oral_Histology/Training40_features/';
    strPath='/Users/chenglu/Nutstore/Training40_features/';
    dirIM=dir(strIMPath);
    dirIM(1:3)=[];
else
    strIMPath='C:\Dataset\WUSTL_Oral_Histology\Training40\';
    strPath='C:\Nutstore\Nutstore\Training40_features\';
    dirIM=dir(strIMPath);
    dirIM(1:2)=[];
end

addpath('C:\Nutstore\Nutstore\PathImAnalysis_Program\Program\EvaluationTools\KaplanMeierCurves');
%% The label information
set_trainNon=[9 42 47 65 69 98 101 110 117 118 7 36 40 48 76 83 82 81 92 106];
set_trainPro=[4 35 43 45 56 58 63 75 94 115 19 22 26 38 49 61 70 90 109 114];
set_train=[set_trainNon set_trainPro];
%% below is for the 40 training data
load 40training.mat
load KM_best_classifier.mat
sm=[];
for i=1:length(dirIM)
    strC=dirIM(i).name;
    intIMIdx=str2num(strC(end-6:end-3));

    [lia,locb]=ismember(intIMIdx,code);
    
    sm(i)=suvival_month(locb);
    if ismember(intIMIdx,set_trainPro)
        labels(i)=1;
    else
        labels(i)=-1;
    end
end
predlabels=bestResult_Im.decision;
%%
% TTR - 1xn vector of months from treatment to last follow up
% labelmask - 1xn vector of outcomes (ie. 1 or 2)
% predlabels - 1xn vector of predicted labels (ie. 1 or 2)
labels(labels==-1)=2;
predlabels(predlabels==-1)=2;

labels(labels==1)=3;
predlabels(predlabels==1)=3;

labels(labels==2)=4;
predlabels(predlabels==2)=4;

labels(labels==3)=2;
predlabels(predlabels==3)=2;

labels(labels==4)=1;
predlabels(predlabels==4)=1;

info = plotKaplanMeier(sm,labels,predlabels);


