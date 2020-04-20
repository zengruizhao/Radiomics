function [meanAUC,stdAUC,resultImbalancedC45]=featureselection_randomforest(data,labels,data_name,para)
%% feature selection using ttest
K=para.numTopFeatures;
dataw=simplewhiten(data);
% K=21;
[TTidx,confidence] = prunefeatures_new(dataw, labels, 'ttestp');
idx_TTest=TTidx(1:K);
set_confidence_TTest=confidence(1:K);
setTopF_TTest=data_name(idx_TTest)';

%% do the classification using random forests
%%% as Sahir paper mention, he did 3-fold randomized cross-validation with 10 runs.
%# split training/testing sets
if para.flag_classification
    intIter=para.intIter;%run number
    intFolds=para.intFolds;%fold number
    % set_Feature=idx_mRMR;
    set_Feature=idx_TTest;
    % set_Feature=idx_Wilcoxon;
    % set_Feature=[1:length(data_name)];
    % set_Feature=[4,12,21];
    % set_Feature=[17,21,6];
    % set_Feature=[12];
    testdata=data; testlabels=labels;
    testdata=simplewhiten(testdata);
    
    % do not consider imbalanced data
    resultImbalancedC45 = nFold_BaggedC45(testdata(:,set_Feature),testlabels,1,intFolds,intIter);
    [~,idxMaxAUC_Im]=max([resultImbalancedC45.AUC]);
    bestResult_Im=resultImbalancedC45(idxMaxAUC_Im);
    meanAUC=mean([resultImbalancedC45.AUC]);
    stdAUC=std([resultImbalancedC45.AUC]);
end
%% visulize top 5 TTest picked features
if para.flag_show
    close all;
    for i=1:5
        v=dataw(:,idx_TTest(i));
        group=labels;
        
        figure;hold on;
        h=boxplot(v, group,'notch','off','whisker',.5,'outliersize',2,'labels',{'Nonprogressor','Progressor'});
        set(h,{'linew'},{2});
        set(findobj(gca,'Type','text'),'FontSize',50,'fontweight','bold');
        
        
        strTitle=sprintf('Top%d: %s',i,data_name{idx_TTest(i)});
        title(strTitle);
        xlabel('Class type');
        ylabel('Feature value');
        hold off;
    end
    
    figure;
    plotroc(bestResult_Im.labels,bestResult_Im.prediction,'BaggedC45');
    
    fprintf('The mean performance: mean AUC=%.4f, best AUC =%.4f\n',...
        mean([resultImbalancedC45.AUC]),bestResult_Im.AUC);
end