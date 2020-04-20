addpath('/Users/chenglu/Nutstore/PathImAnalysis_Program/Program/Miscellaneous/exportToPPTX');

% show image with CCG on it
% clear all
strIMPath='/Users/chenglu/Documents/Dataset/WUSTL_Oral_Histology/Training40/';
strPath='/Users/chenglu/Documents/Dataset/WUSTL_Oral_Histology/Training40_features/';

dirIM=dir(strIMPath);
dirIM(1:3)=[];

bandWidth=30;

alpha = 0.4;
r = 0.2;
%% The label information
set_trainNon=[9 42 47 65 69 98 101 110 117 118 7 36 40 48 76 83 82 81 92 106];
set_trainPro=[4 35 43 45 56 58 63 75 94 115 19 22 26 38 49 61 70 90 109 114];
set_train=[set_trainNon set_trainPro];

%% gather features on Pro/non-Por cases, one row for one case, one column for one feature

exportToPPTX('new');
for i=1:length(dirIM)
    strC=dirIM(i).name;
    
    strCur=sprintf('%s%s',strIMPath,dirIM(i).name);
    I=imread(strCur);% show(I);
    
    strSaveCur=sprintf('%s%s_nuclei.mat',strPath,dirIM(i).name);
    load(strSaveCur);
    
    str=sprintf('%sMSCellCluster_BW%d_IM%s.mat',strPath,bandWidth,dirIM(i).name);
    load(str);
    
%     str=sprintf('%s%s_CCGInfo_a=%.2f.mat',strPath,dirIM(i).name,info.alpha);
%     load(str);
    
    intIMIdx=str2num(strC(end-6:end-3));
    
    if ismember(intIMIdx,set_trainPro)
        labels(i)=1;
    else
        labels(i)=0;
    end
    
    %     fprintf('On the %dth TMA...\n',intIMIdx);
    
    %% display nuclei and nuclei area
    show(I,1);hold on;
    for k = 1:length(nuclei)
        plot(nuclei{k}(:,2), nuclei{k}(:,1), 'g-', 'LineWidth', 2);
    end
    hold off;
    
    %     exportToPPTX('addslide');
    %     exportToPPTX('addpicture',gcf);
    %
    %     exportToPPTX('addtext',sprintf('Nuclei on Image. filename: %s \n Image size: %dx%d\n',...
    %         dirIM(i).name,size(I,1),size(I,2)),'Position',[0 0 5 5]);
    
    %     displayNucleiProperties(curIM, nuclei, properties, 'Area');
    
    %% show MS nuclei cluster
    %
    %%% show the cluster nodes only
    %     show(I,1);
    %     hold on
    %     numObj=size(clustCent,2);
    %     for k = 1 : numObj
    %         plot(clustCent(1,k),clustCent(2,k), 'rp');
    %     end
    %     hold off;
    %
    %     exportToPPTX('addslide');
    %     exportToPPTX('addpicture',gcf);
    %
    %     exportToPPTX('addtext',sprintf('Nuclei Clusters on Image. filename: %s \n Image size: %dx%d\n',...
    %         dirIM(i).name,size(I,1),size(I,2)),'Position',[0 0 5 5]);
    
    %         %%% for the cell clusters, keep the cluster center contains more than 1 cell as the node in the CCG
    %         list_CCGnode=[];%collection of cell cluster (30 pixels as bandwidth)
    %         for j=1:length(cluster2dataCell)
    %             if length(cluster2dataCell{j})>1
    %                 list_CCGnode=[list_CCGnode j];
    %             end
    %         end
    %% display CCG on image
    
    %% construct CCG and visualize CCG
    
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
    
    show(I,1);hold on;
    for x = 1: numel(nodes)
        plot(nodes(x).centroid_r, nodes(x).centroid_c,'go', 'MarkerSize',1, 'MarkerFaceColor', 'g');
    end
    plot(VX', VY', 'y-', 'LineWidth', 1);
    text(100,220,sprintf('alpha=%.2f, r=%.2f\n%s',alpha,r,strlabel));
    hold off;
    
    if labels(i)==1
        strlab='Progressor';
    else
        strlab='Nonprogressor';
    end
    
    exportToPPTX('addslide');
    exportToPPTX('addpicture',gcf);
    
    exportToPPTX('addtext',sprintf('Nuclei Cluster Graph-tpye mixed on %s Image. filename: %s \n Image size: %dx%d',...
        strlab,dirIM(i).name,size(I,1),size(I,2)),'Position',[0 0 5 5]);
    
end
% newFile = exportToPPTX('saveandclose','CCGonIM_nonPro');
newFile = exportToPPTX('saveandclose',sprintf('TMA_OralCavity_CCG_mixed_Maps_a=%.2f',alpha));