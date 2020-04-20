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