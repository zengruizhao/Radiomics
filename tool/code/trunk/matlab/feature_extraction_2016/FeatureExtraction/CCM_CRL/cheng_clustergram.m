addpath(genpath('/Users/chenglu/Nutstore/PathImAnalysis_Program/Program/LClassifier/ClassificationPackage_CCIPD2009'));
addpath(genpath('/Users/chenglu/Nutstore/Repository/matlab/matlab/feature_extraction'));
addpath(genpath('/Users/chenglu/Nutstore/Repository/George-newbranch'));
addpath(genpath('/Users/chenglu/Nutstore/Repository/George-newbranch/george_code/newbranch/tools/FeatureSelectionTools'));
addpath(genpath('/Users/chenglu/Nutstore/Repository/George-newbranch/george_code/newbranch/tools/AnalysisTools'));
addpath(genpath('C:\Nutstore\Nutstore\Repository\George-newbranch\george_code\newbranch\tools'));
addpath(genpath('/Users/chenglu/Nutstore/PathImAnalysis_Program/Program/OralCavity/'));

%% top 8 features
load OralCavityFullFeatures_epistroma_nonnan.mat
%%% search for all CCM features
set_feature_name='CCM';
set_idx=[];
for i=1:length(feature_list_epistroma)
    if ~isempty(strfind(feature_list_epistroma{i},set_feature_name))
        set_idx=[i set_idx];
    end
end

set_Feature=set_idx;
feat_data=data_epistroma(:,set_Feature);

K=6;

dataw_epistroma=feat_data;
[TTidx,confidence_epistroma] = prunefeatures_new(dataw_epistroma, labels, 'ttestp');
idx_TTest_epistroma=TTidx(1:K);

idx_TTest=idx_TTest_epistroma;
set_Feature=idx_TTest(1:K);

feat_data=data_epistroma(:,set_idx(set_Feature));
feature_list_epistroma{set_idx(set_Feature)}
% setTopFeatureName=feature_list_epistroma{set_idx(set_Feature)};
% labels

m1 = feat_data(find(labels),:);
m2 = feat_data(find(~labels),:);

M_new=[m1;m2];
data=M_new;
groups=[];
groups(1:20)=1;
groups(21:40)=0;



for nc=1:size(data,2)
    temp=data(:,nc);
    maxVec = max(temp);
    minVec = min(temp);
    meanVec = mean(temp);
    varVec = var(temp);
    %# normalize to -1...1
    scaled_data = ((temp-minVec)./(maxVec-minVec) - 0.5 ) *2;

    
    data(:,nc)=scaled_data;
end

dgroup = data;


% Supervised Clustering
% cgo1 = clustergram( dgroup,'Cluster','row','RowLabels',groups );

% Unsupervised Clustering
cgo2 = clustergram( dgroup,'Cluster','all','RowLabels',double(groups) );
% Unsupervised Clustering - linkage complete
% Sometimes this makes for better clustering into 2-3 distinct groups 
% If you're getting one big ugly cluster try this 
% cgo2 = clustergram( dgroup,'Cluster','all','RowLabels',groups,'Linkage', 'complete' );

%% intensity based features (harlick)
set_Feature=711:736;
feat_data=data_epistroma(:,set_Feature);

[TTidx,confidence_stroma] = prunefeatures_new(feat_data, labels, 'ttestp');

feat_data=[];
feat_data=data_epistroma(:,set_Feature(TTidx(1:K)));

m1 = feat_data(find(labels),:);
m2 = feat_data(find(~labels),:);

M_new=[m1;m2];
data=M_new;
groups=[];
groups(1:20)=1;
groups(21:40)=0;



for nc=1:size(data,2)
    temp=data(:,nc);
    maxVec = max(temp);
    minVec = min(temp);
    meanVec = mean(temp);
    varVec = var(temp);
    %# normalize to -1...1
    scaled_data = ((temp-minVec)./(maxVec-minVec) - 0.5 ) *2;

    
    data(:,nc)=scaled_data;
end

dgroup = data;


% Supervised Clustering
% cgo1 = clustergram( dgroup,'Cluster','row','RowLabels',groups );

% Unsupervised Clustering
cgo2 = clustergram( dgroup,'Cluster','all','RowLabels',double(groups) );