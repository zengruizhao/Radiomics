addpath('/Users/chenglu/Nutstore/PathImAnalysis_Program/Program/Miscellaneous/exportToPPTX');

% show image with CCG on it
% clear all
strIMPath='/Users/chenglu/Documents/Dataset/WUSTL_Oral_Histology/Training40/';
strPath='/Users/chenglu/Documents/Dataset/WUSTL_Oral_Histology/Training40_features/';
strPath='/Users/chenglu/Documents/Dataset/WUSTL_Oral_Histology/Training40_features_ws=8_18_a=3.5_4.5_MixedType/';

dirIM=dir(strIMPath);
dirIM(1:3)=[];

bandWidth=30;
% info.alpha = 0.38;
% info.radius = 0.2;

para.alpha_min=0.37; para.alpha_max=0.42; para.alpha_res=0.01;% larger the alpha, sparse the graph
para.radius=0.2;
set_alpha=[para.alpha_min:para.alpha_res:para.alpha_max];
%% The label information
set_trainNon=[9 42 47 65 69 98 101 110 117 118 7 36 40 48 76 83 82 81 92 106];
set_trainPro=[4 35 43 45 56 58 63 75 94 115 19 22 26 38 49 61 70 90 109 114];
set_train=[set_trainNon set_trainPro];

for p=1:length(set_alpha)
    exportToPPTX('new');
    for i=1:length(dirIM)
        strC=dirIM(i).name;
        
        strCur=sprintf('%s%s',strIMPath,dirIM(i).name);
        I=imread(strCur);% show(I);
        
        strSaveCur=sprintf('%s%s_nuclei.mat',strPath,dirIM(i).name);
        load(strSaveCur);
        
        str=sprintf('%sMSCellCluster_BW%d_IM%s.mat',strPath,bandWidth,dirIM(i).name);
        load(str);
        
        load(sprintf('%sHosoyaFeatures_IM%s.mat',strPath,strC));

%         str=sprintf('%s%s_CCGInfo_a=%.2f.mat',strPath,dirIM(i).name,info.alpha);
%         load(str);
        
        intIMIdx=str2num(strC(end-6:end-3));
        
        if ismember(intIMIdx,set_trainPro)
            labels(i)=1;
        else
            labels(i)=0;
        end
        
        %     fprintf('On the %dth TMA...\n',intIMIdx);
        
        %% display nuclei and nuclei area
        %         show(I,1);hold on;
        %         for k = 1:length(nuclei)
        %             plot(nuclei{k}(:,2), nuclei{k}(:,1), 'g-', 'LineWidth', 2);
        %         end
        %         hold off;
        
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
        
        curCCGinfo=CCGinfo{p};
        
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
        
        bounds.CellClusterC_c=clustCent(2,list_CCGnode);
        bounds.CellClusterC_r=clustCent(1,list_CCGnode);
        
        nodes.centroid_c=bounds.CellClusterC_c;
        nodes.centroid_r=bounds.CellClusterC_r;
        
        show(I,1);hold on;
        for x = 1: numel(nodes)
            plot(nodes(x).centroid_r, nodes(x).centroid_c,'go', 'MarkerSize',2, 'MarkerFaceColor', 'g');
        end
        plot(curCCGinfo.VX', curCCGinfo.VY', 'y-', 'LineWidth', 1);
        hold off;
        
        
        if labels(i)==1
            strlab='Progressor';
        else
            strlab='Nonprogressor';
        end
        
        exportToPPTX('addslide');
        exportToPPTX('addpicture',gcf);
        
        exportToPPTX('addtext',sprintf('Nuclei Cluster Graph on %s Image. filename: %s \n Image size: %dx%d',...
            strlab,dirIM(i).name,size(I,1),size(I,2)),'Position',[0 0 5 5]);
        
    end
    % newFile = exportToPPTX('saveandclose','CCGonIM_nonPro');
    newFile = exportToPPTX('saveandclose',sprintf('TMA_OralCavity_CCG_mixed_Maps_a=%.2f',set_alpha(p)));
end