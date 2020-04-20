% this script is used to test CCG parameter

addpath(genpath('/Users/chenglu/Nutstore/PathImAnalysis_Program/Program/Features/George_GraphFeatureExtraction'));

strIMPath='/Users/chenglu/Documents/Dataset/WUSTL_Oral_Histology/Training40/';
strPath='/Users/chenglu/Documents/Dataset/WUSTL_Oral_Histology/Training40_features/';
strPath='/Users/chenglu/Documents/Dataset/WUSTL_Oral_Histology/Training40_features_ws=8_18_a=0.36_0.4/';

dirIM=dir(strIMPath);
dirIM(1:3)=[];

bandWidth=30;
%% The label information
set_trainNon=[9 42 47 65 69 98 101 110 117 118 7 36 40 48 76 83 82 81 92 106];
set_trainPro=[4 35 43 45 56 58 63 75 94 115 19 22 26 38 49 61 70 90 109 114];

%% load image
i=3;
strC=dirIM(i).name;

strCur=sprintf('%s%s',strIMPath,dirIM(i).name);
I=imread(strCur);% show(I);

intIMIdx=str2num(strC(end-6:end-3));
if ismember(intIMIdx,set_trainPro)
    labels(i)=1;
    strlabel='Progressor';
else
    labels(i)=0;
    strlabel='NonProgressor';
end
%% display nuclei and nuclei area
strSaveCur=sprintf('%s%s_nuclei.mat',strPath,dirIM(i).name);
load(strSaveCur);

show(I);hold on;
for k = 1:length(nuclei)
    plot(nuclei{k}(:,2), nuclei{k}(:,1), 'g-', 'LineWidth', 2);
end
hold off;
%% load nuclei cluster result based on MS clustering
str=sprintf('%sMSCellCluster_BW%d_IM%s.mat',strPath,bandWidth,dirIM(i).name);
load(str);
%% construct CCG and visualize CCG

alpha=0.4; r=0.2;

ctemp=[properties.Centroid];
bounds.centroid_c=ctemp(1:2:end);
bounds.centroid_r=ctemp(2:2:end);

bounds.nuclei=nuclei;

%%% for the cell clusters, keep the cluster center contains more than 1 cell as the node in the CCG
list_CCGnode=[];%collection of cell cluster (30 pixels as bandwidth)
for j=1:length(cluster2dataCell)
    if length(cluster2dataCell{j})>0
        list_CCGnode=[list_CCGnode j];
    end
end

CCGbounds.centroid_c=clustCent(2,list_CCGnode);
CCGbounds.centroid_r=clustCent(1,list_CCGnode);

[VX,VY,x,y,edges] = construct_ccgs(CCGbounds,alpha, r);
        
nodes.centroid_c=CCGbounds.centroid_c;
nodes.centroid_r=CCGbounds.centroid_r;

show(I);hold on;
for x = 1: numel(nodes)
    plot(nodes(x).centroid_r, nodes(x).centroid_c,'go', 'MarkerSize',2, 'MarkerFaceColor', 'g');
end
plot(VX', VY', 'y-', 'LineWidth', 2);
text(100,220,sprintf('alpha=%.2f, r=%.2f\n%s',alpha,r,strlabel));
hold off;

