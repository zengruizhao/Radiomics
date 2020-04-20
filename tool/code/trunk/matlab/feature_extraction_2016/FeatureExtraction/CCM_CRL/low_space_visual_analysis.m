%% visulize the samples in 3-d space using top 3 features
load OralCavityFullFeatures_epistroma_nonnan_train50.mat
load predictive_label_LDA.mat

X=data_useful(:,set_Feature);
 figure;hold on; plot3(X(labels==1,1),X(labels==1,2),X(labels==1,3),'ro','MarkerSize',10);
 plot3(X(labels==0,1),X(labels==0,2),X(labels==0,3),'bx','MarkerSize',10);
 for j=1:length(patientIDs)
     text(X(j,1),X(j,2),X(j,3),num2str(patientIDs(j)));
 end
 xlabel(setFinalFeature_for_classifer(1));
  ylabel(setFinalFeature_for_classifer(2));
   zlabel(setFinalFeature_for_classifer(3));
 legend('Progressor','Non-progressor');
 hold off;
 %% look at the top 2 feature:'stroma-CCM-Eccentricity:skewness(correlation)-a=0.50' first
 setFinalFeature_for_classifer(2)

name_top1= setFinalFeature_for_classifer{2};
para_a=str2num(name_top1(end-3:end));
%%% take out all values and check the max/min to determine the extrem cases
% dataw=data_epistroma;
dataw_top1=X(:,2);

dataw_top1_p=dataw_top1(logical(labels));dataw_top1_n=dataw_top1(~logical(labels));
% check
figure(100); plot(find(labels),dataw_top1_p,'p');hold on;
plot(find(~labels),dataw_top1_n,'o');hold off;
 %% look at the top 1 feature:''epi-CCM-OutsideBoundaryIntensityRange:skewness(contrast-energy)-a=0.48'
 setFinalFeature_for_classifer(1)

name_top1= setFinalFeature_for_classifer{1};
para_a=str2num(name_top1(end-3:end));
%%% take out all values and check the max/min to determine the extrem cases
% dataw=data_epistroma;
dataw_top1=X(:,1);

dataw_top1_p=dataw_top1(logical(labels));dataw_top1_n=dataw_top1(~logical(labels));
% check
figure(100); plot(find(labels),dataw_top1_p,'p');hold on;
plot(find(~labels),dataw_top1_n,'o');hold off;
%% check a case here 
strIMPath='C:\Dataset\WUSTL_Oral_Histology\Training50\';
dirIM=dir([strIMPath '*.tif']);
 %%% look at case 101
temp=find(patientIDs==69);
paraC.name=dirIM(temp).name;
paraC.alpha=para_a; paraC.r=0.2;
paraC.flag_useotherfeaturelimits=0;
paraC.feature_limits=[];
feature_names={'Area','MajorAxisLength','MinorAxisLength','Eccentricity','Orientation','EquivDiameter','Solidity','Perimeter','Circularity','EllipticalDeviation',...
    'MeanIntensity','IntensityDeviation','IntensityRange','MeanInsideBoundaryIntensity','InsideBoundaryIntensityDeviation','InsideBoundaryIntensityRange',...
    'MeanOutsideBoundaryIntensity','OutsideBoundaryIntensityDeviation','OutsideBoundaryIntensityRange','BoundarySaliency'};
feature_names=feature_names';

% 'OutsideBoundaryIntensityRange' is 19th
paraC.idx_Mor=19;
% 'Eccentricity' is 4th
% paraC.idx_Mor=4;

% haralick_feature_names={'contrast-energy','contrast-inverse-moment','contrast-ave','contrast-var',...
%     'contrast-ent','intensity-ave','intensity-var','intensity-ent','entropy','energy',...
%     'correlation','info-measure1','info-measure2'};

% 'contrast-energy' is 1
paraC.idx_Har=1;
% 'contrast-ent' is 5
% paraC.idx_Har=5;
% 'correlation' is 11
% paraC.idx_Har=11;

% statistics
% curCCM=[mean(all_feat) median(all_feat) std(all_feat)  range(all_feat) kurtosis(all_feat) skewness(all_feat)];
paraC.idx_Sta=6;

paraC.epi=1; % in epi or stroma

extractfullfeature_OroCavity_v4_identify_case(paraC);