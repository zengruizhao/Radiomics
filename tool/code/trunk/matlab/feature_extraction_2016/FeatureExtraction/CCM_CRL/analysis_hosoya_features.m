% clear all
addpath(genpath('/Users/chenglu/Nutstore/PathImAnalysis_Program/Program/LClassifier/ClassificationPackage_CCIPD2009'));
addpath(genpath('/Users/chenglu/Nutstore/Repository/matlab/matlab/feature_extraction'));
% addpath(genpath('/Users/chenglu/Nutstore/Repository/George-newbranch'));
addpath(genpath('/Users/chenglu/Nutstore/Repository/George-newbranch/george_code/newbranch/tools/FeatureSelectionTools'));
addpath(genpath('/Users/chenglu/Nutstore/Repository/George-newbranch/george_code/newbranch/tools/AnalysisTools'));
%%
strIMPath='/Users/chenglu/Documents/Dataset/WUSTL_Oral_Histology/Training40/';
% strIMPath='C:\Dataset\WUSTL_Oral_Histology\Training40\';

strPath='/Users/chenglu/Documents/Dataset/WUSTL_Oral_Histology/Training40_features/';
% strPath='/Users/chenglu/Documents/Dataset/WUSTL_Oral_Histology/Training40_features_ws=8_18_a=0.36_0.4/';
strPath='/Users/chenglu/Documents/Dataset/WUSTL_Oral_Histology/Training40_features_ws=8_18_a=3.5_4.5_MixedType/';
% strPath='C:/Dataset/WUSTL_Oral_Histology/Training40_features_ws=8_18_a=3.5_4.5_MixedType/';

dirIM=dir(strIMPath);
dirIM(1:3)=[];
bandWidth=30;
%% The label information
set_trainNon=[9 42 47 65 69 98 101 110 117 118 7 36 40 48 76 83 82 81 92 106];
set_trainPro=[4 35 43 45 56 58 63 75 94 115 19 22 26 38 49 61 70 90 109 114];
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
%% read pre computed hosoya 

% HosoyaF store all hosoya features at different packing(idx by h) of CCG in
% different image(idx by i)

load hosoyaF.mat

%% check out the hosoya range for each CCG density
Hosoya_cat=[]; Hosoya_range=[];
Hosoya_Non=[];Hosoya_Pro=[];
for i=1:size(HosoyaF,1)
    Hosoya_cur=[];Pro=[];Non=[];
    for j=1:size(HosoyaF,2)
        temp=cell2mat(HosoyaF{i,j});
        Hosoya_cur=[Hosoya_cur temp];
        if labels(j)
           Pro=[Pro temp];
        else
            Non=[Non temp];
        end
    end
    tmp=unique(Hosoya_cur);
    [stmp]=sort(tmp,'descend');
    
    Hosoya_cur(Hosoya_cur==Inf)=stmp(2);
    Non(Non==Inf)=stmp(2);
    Pro(Pro==Inf)=stmp(2);
    
    Hosoya_range(i,1)=min(Hosoya_cur);
    Hosoya_range(i,2)=max(Hosoya_cur);    
    Hosoya_All{i}=Hosoya_cur; 
    Hosoya_Non{i}=Non;
    Hosoya_Pro{i}=Pro;    
end
%% get the statistics for each patient
HF_stat=[];
for i=1:size(HosoyaF,1)
    for j=1:size(HosoyaF,2)       
        temp=cell2mat(HosoyaF{i,j});
        tmp=unique(temp);
        [stmp]=sort(tmp,'descend');
        temp(temp==Inf)=stmp(2);
    
        HF_stat{i,j}=[mean(temp),...
            median(temp),...
            std(temp),...
            range(temp),skewness(temp),...
            kurtosis(temp)];
    end
end
%% seperate the class and compare in terms of statistics
Pro=[];Non=[];
for i=1:size(HosoyaF,1)
    for j=1:size(HosoyaF,2)       
        temp=HF_stat{i,j};
        if labels(j)
            Pro(i,j,:)=temp;
        else
            Non(i,j,:)=temp;
        end
    end
end

%% plot out the boxplot for each CCG dense level and each statistics
for i=1:size(Pro,1)
    figure(i);
    for j=1:size(Pro,3)
        group(1:20)=1;group(21:40)=0;
        data=[Pro(i,labels==1,j) Non(i,labels==0,j)];
        subplot(3,2,j);boxplot(data,group);
    end
end

%% cat all data at different dense level together as feature matrix
Alldata=[];
for i=1:size(Pro,1)
    data=squeeze([Pro(i,find(labels==1),:) Non(i,find(labels==0),:)]);
    Alldata=cat(2,Alldata,data);
end
%% get the pdf, of three bins, for each patients
for i=1:length(Hosoya_All)
    
%     figure;
%     cur=Hosoya_Non{i};
%     cur(cur>1000)=1000;
%     cur(cur==2)=[];
%     [x,bin]=hist([cur],length(cur));
%     
%     bins=get(h,'Values');
%     figure;
%     cur=Hosoya_Non{i};
%     cur(cur>2000)=2000;
%     histogram([cur],30);   
end
%% feature selection using mRMR, pick the top 1
% dataw=simplewhiten(data);
% % map the data in [0,100] and all integers
% dataw_discrete=makeDataDiscrete(dataw);
% setAll=1:size(dataw,2);
% K=20;
% idx_mRMR=[];
% for i=1:K
%     [idx_temp] = mrmr_mid_d(dataw_discrete(:,setAll), labels, 1);
%     setAll=setdiff(setAll,idx_temp);
%     idx_mRMR=[idx_mRMR idx_temp];
% end
% setTopF_mRMR=feature_list(idx_mRMR)';
%% visulize top mRMR picked features
% close all;
% for i=1:length(idx_mRMR)
%     v=dataw_discrete(:,idx_mRMR(i));
%     group=labels;
% 
%     figure;hold on;
%     h=boxplot(v, group,'notch','off','whisker',.5,'outliersize',2,'labels',{'Nonprogressor','Progressor'});
%     set(h,{'linew'},{2});
%     set(findobj(gca,'Type','text'),'FontSize',50,'fontweight','bold');
% 
% 
%     strTitle=sprintf('Top%d: %s',i,feature_list{idx_mRMR(i)});
%     title(strTitle);
%     xlabel('Class type');
%     ylabel('Feature value');
%     hold off;
% end
%% feature selection using ttest
data=Alldata;
K=10;
dataw=simplewhiten(data);
% K=21;
[TTidx,confidence] = prunefeatures_new(dataw, labels, 'ttestp');
idx_TTest=TTidx(1:K);
set_confidence_TTest=confidence(1:K);
% setTopF_TTest=feature_list(idx_TTest)';
%% feature selection using wilcoxon rank sum test
% K=16;
% [Wilcoxonidx,confidence] = prunefeatures_new(dataw, labels, 'wilcoxon');
% idx_Wilcoxon=Wilcoxonidx(1:K);
% set_confidence_Wilconxon=confidence(1:K);
% setTopF_Wilcoxon=feature_list(idx_Wilcoxon)';
%% project the top features statistics using PCA into 1D space
data_r=dataw(:,idx_Wilcoxon(1:3));
data_r=dataw(:,idx_TTest(1:5));

coeff = pca(data_r);
mainCoeff=coeff(:,1);
dataPCA=data_r*mainCoeff;

setPro=find(labels==1);
setNonPro=find(labels==0);

%%% show in figure
figure;hold on;
plot(dataPCA(setPro),ones(1,length(setPro)),'bs','MarkerSize',15);
plot(dataPCA(setNonPro),ones(1,length(setNonPro)),'rp','MarkerSize',15);

xlabel('PCA value');
str=sprintf('Top %d features - PCA',size(data_r,2));
legend('Progressor','Non-progressor');
title(str);
set(gca,'FontSize',20);
hold off;

%% project the top features using PCA into 2D space
coeff = pca(data_r);
mainCoeff=coeff(:,1:2);
dataPCA=data_r*mainCoeff;

%%% show in figure
figure;hold on;
plot(dataPCA(setPro,1),dataPCA(setPro,2),'bs','MarkerSize',15);
plot(dataPCA(setNonPro,1),dataPCA(setNonPro,2),'rp','MarkerSize',15);

% xlabel('PCA value');
str=sprintf('Top %d features - PCA',size(data_r,2));
legend('Progressor','Non-progressor');
title(str);
set(gca,'FontSize',20);
hold off;

%% project the top features using PCA into 3D space
coeff = pca(data_r);
mainCoeff=coeff(:,1:3);
dataPCA=data_r*mainCoeff;

%%% show in figure
figure;hold on;t = 0.001;

plot3(dataPCA(setPro,1),dataPCA(setPro,2),dataPCA(setPro,3),'ks','MarkerSize',20,'MarkerFace','k');
% [A , C] = MinVolEllipse([dataPCA(1:5,1),dataPCA(1:5,2),dataPCA(1:5,3)]', t);
%  Ellipse_plot(A,C);
 
plot3(dataPCA(setNonPro,1),dataPCA(setNonPro,2),dataPCA(setNonPro,3),'rp','MarkerSize',20,'MarkerFace','r');
% [A , C] = MinVolEllipse([dataPCA(6:11,1),dataPCA(6:11,2),dataPCA(6:11,3)]', t);
%  Ellipse_plot(A,C);
 
% xlabel('PCA value');
str=sprintf('Top %d features - PCA',size(data_r,2));
legend('Progressor','Non-progressor');
title(str);
set(gca,'FontSize',25);
az = 114;
el = 70;
view(az, el);
hold off;

%% visulize top 5 TTest picked features
close all;
for i=1:5
    v=dataw(:,idx_TTest(i));
    group=labels;

    figure;hold on;
    h=boxplot(v, group,'notch','off','whisker',.5,'outliersize',2,'labels',{'Nonprogressor','Progressor'});
    set(h,{'linew'},{2});
    set(findobj(gca,'Type','text'),'FontSize',50,'fontweight','bold');


    strTitle=sprintf('Top%d: %s',i,feature_list{idx_TTest(i)});
    title(strTitle);
    xlabel('Class type');
    ylabel('Feature value');
    hold off;
end
%% incremental feature inclusion analysis, add n feature at a time, then check the AUC 
%%% it is good to determine the top k feature are usefull for
%%% classification.

setFeature=TTidx;
% setFeature=Wilcoxonidx;
% setFeature=idx_mRMR;

n=1;% number of incremental feature added
intIter=20;%run number
intFolds=3;%fold number
testdata=data; testlabels=labels;
testdata=simplewhiten(testdata);
setMeanAUC=[];

for i=1:n:21%length(setFeature)
    % do not consider imbalanced data
    resultImbalancedC45 = nFold_BaggedC45(testdata(:,setFeature(1:i)),testlabels,1,intFolds,intIter);
    setMeanAUC(i)=mean([resultImbalancedC45.AUC]);
end
setMeanAUC(setMeanAUC==0)=[];
figure;plot(1:n:21,setMeanAUC);hold on;
% figure;plot(1:n:length(setFeature),setMeanAUC);hold on;
xlabel('Feature numbers'); ylabel('Mean AUC in 20 run 3-fold cross validation');
title('Feature incremental inclusion based on Wilcoxon test'); hold off;
% title('Feature incremental inclusion based on ttest'); hold off;

%% do the classification using random forests
%%% as Sahir paper mention, he did 3-fold randomized cross-validation with 10 runs.
%# split training/testing sets
intIter=30;%run number
intFolds=3;%fold number
% set_Feature=idx_mRMR;
set_Feature=idx_TTest;
% set_Feature=idx_Wilcoxon;
% set_Feature=[1:length(feature_list)];
% set_Feature=[4,12,21];
% set_Feature=[17,21,6];
% set_Feature=[12];
testdata=data; testlabels=labels;
testdata=simplewhiten(testdata);

% do not consider imbalanced data
resultImbalancedC45 = nFold_BaggedC45(testdata(:,set_Feature),testlabels,1,intFolds,intIter);
[~,idxMaxAUC_Im]=max([resultImbalancedC45.AUC]);
[~,idxMinAUC_Im]=min([resultImbalancedC45.AUC]);
bestResult_Im=resultImbalancedC45(idxMaxAUC_Im);
worstResult_Im=resultImbalancedC45(idxMinAUC_Im);


figure(1);
plotroc(bestResult_Im.labels,bestResult_Im.prediction,'BaggedC45');

fprintf('The mean performance: mean AUC=%.4f, best AUC =%.4f, worst AUC =%.4f\n',...
    mean([resultImbalancedC45.AUC]),bestResult_Im.AUC,worstResult_Im.AUC);