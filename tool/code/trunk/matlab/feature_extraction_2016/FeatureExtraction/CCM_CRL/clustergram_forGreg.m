% clear all
cd '\\csehomes\csehome$\nmb60\My Documents\MATLAB\Experiments\BrUOG\All_Phases\Peak Features\Statistics'
load('featurestats_fullCOLLAGE.mat')

omit = {'P053A','P017B','P030B','P058B','P027A','P009B'};
LUM_ind = find(strcmp(featurestats.type,'Luminal'));
BAS_ind = find(strcmp(featurestats.type,'HER2_Basal'));
ENR_ind = find(strcmp(featurestats.type,'HER2_Enriched'));


for i = 1:length(omit)
    LUM_ind = LUM_ind(LUM_ind~=find(strcmp(featurestats.studyID,omit{i})));
    BAS_ind = BAS_ind(BAS_ind~=find(strcmp(featurestats.studyID,omit{i})));
    ENR_ind = ENR_ind(ENR_ind~=find(strcmp(featurestats.studyID,omit{i})));
end 

LUM_ftrs = featurestats.collageftrs(LUM_ind,:);
BAS_ftrs = featurestats.collageftrs(BAS_ind,:);
ENR_ftrs = featurestats.collageftrs(ENR_ind,:);

m1 = ENR_ftrs;
m2 = BAS_ftrs;
m3 = LUM_ftrs;

M_new=[ENR_ftrs;BAS_ftrs;LUM_ftrs];
data=M_new;
groups=zeros(size(data,1),1); % Enriched
groups(size(m1,1)+1:size(m1,1)+1+size(m2,1),1)=1; % Basal
groups(end-size(m3,1)+1:end,1)=2; %Luminal


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

d1=data(1:size(m1,1),:);
d2=data(size(m1,1)+1:size(m1,1)+size(m2,1),:);
d3=data(end-size(m3,1)+1:end,:);
dgroup = [d1;d2;d3];


% Supervised Clustering
cgo1 = clustergram( dgroup,'Cluster','row','RowLabels',groups );

% Unsupervised Clustering
cgo2 = clustergram( dgroup,'Cluster','all','RowLabels',groups );
% Unsupervised Clustering - linkage complete
% Sometimes this makes for better clustering into 2-3 distinct groups 
% If you're getting one big ugly cluster try this 
% cgo2 = clustergram( dgroup,'Cluster','all','RowLabels',groups,'Linkage', 'complete' );
