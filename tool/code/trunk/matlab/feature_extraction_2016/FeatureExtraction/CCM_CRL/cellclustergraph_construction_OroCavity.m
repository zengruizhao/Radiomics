%%% classlabel -1 progressor; -0 nonprogressor
% cellclustergraph_construction_OroCavity(4,5)
function cellclustergraph_construction_OroCavity(idxBegin,idxEnd)
%% parameter initialization
addpath(genpath('../Code/George_GraphFeatureExtraction'));
addpath(genpath('/Users/chenglu/Nutstore/PathImAnalysis_Program/Program/Features/George_GraphFeatureExtraction'));


strPath='../Training40_features/';
strIMPath='../Training40/';
% strIMPath='/Users/chenglu/Documents/Dataset/WUSTL_Oral_Histology/Training40/';

dirIM=dir(strIMPath);
dirIM(1:2)=[];
%% begin to work
for i=idxBegin:idxEnd
    
    strCur=sprintf('%s%s',strIMPath,dirIM(i).name);
    fprintf('----------On %dth image named %s -----------\n',i, dirIM(i).name);
    I=imread(strCur);% show(I);
    %% nuclei segmentation, veta watershed method
    strSaveCur=sprintf('%s%s_nuclei.mat',strPath,dirIM(i).name);
    if exist(strSaveCur,'file')~=2
        %%% use adaptive threshold
        [I_norm, ~, ~] = normalizeStaining(I);
        clear I;
        I_normRed=I_norm(:,:,1);
        
        p.scales=[7:2:13];
        try
            disp('begin nuclei segmentation;');
            [nuclei, properties] = nucleiSegmentationV2(I_normRed,p);
        catch ME
            idSegLast = regexp(ME.identifier, '(?<=:)\w+$', 'match');
            %clean up for possible error conditions here. Rethrow if
            %             unknown error.
            switch idSegLast{1}
                case 'nomem'
                    disp('Out of Memory!');
                case 'couldNotReadFile'
                    disp('file not there!');
                otherwise
                    %An unexpected error happened
                    rethrow(ME)
            end
            continue
        end
        
        %%% heuristic to remove non-epithelial regions
        %     dilateRadius = 5;
        %     areaOpenThreshold = 8000;
        %
        %     img = double(curH(:,:,1))/255;
        %
        %     B = img < graythresh(img);
        %
        %     B = imdilate(B, strel('disk', dilateRadius));
        %     B = bwareaopen(B, areaOpenThreshold );
        %
        %     b = false(1, length(properties));
        %
        %     for l = 1:length(properties)
        %
        %         c = round( properties(l).Centroid );
        %
        %         b(l) = B(c(2), c(1));
        %
        %     end
        %
        %     nuclei = nuclei(b);
        %     properties = properties(b);
        
        %%% save results
        save(strSaveCur,'nuclei','properties');
        disp('nuclei result saved; ');
    else
        disp('nuclei file found, loading it');
        load(strSaveCur);
    end
    %% nuclei cluster finding - meanshift
    bandWidth=30;
    str=sprintf('%sMSCellCluster_BW%d_IM%s.mat',strPath,bandWidth,dirIM(i).name);
    if exist(str,'file')~=2
        try
            %% use meanshift to form cell clusters, use location info only
            AlldataPts=[];
            tempC=[properties.Centroid];
            AlldataPts(1:length(properties),1)=tempC(1:2:end);
            AlldataPts(1:length(properties),2)=tempC(2:2:end);
            
            dataPts=AlldataPts(:,1:2);
            
            disp('begin cell cluster finding;');
            
            [clustCent,data2cluster,cluster2dataCell] = MeanShiftCluster(dataPts',bandWidth);
            
            %         bandWidth60=60;
            %         [clustCent60,data2cluster60,cluster2dataCell60] = MeanShiftCluster(dataPts',bandWidth60);
            
        catch ME
            idSegLast = regexp(ME.identifier, '(?<=:)\w+$', 'match');
            %clean up for possible error conditions here. Rethrow if
            %             unknown error.
            switch idSegLast{1}
                case 'nomem'
                    disp('Out of Memory!');
                case 'couldNotReadFile'
                    disp('file not there!');
                otherwise
                    %An unexpected error happened
                    rethrow(ME)
            end
            continue
        end
        %%% save results
        
        save(str,'clustCent','data2cluster','cluster2dataCell','bandWidth');
        %
        %     str=sprintf('%sPro_MSCellCluster_BW%d_%d_IM%d.mat',strPath,bandWidth,bandWidth60,i);
        %     save(str,'clustCent','data2cluster','cluster2dataCell','bandWidth',...
        %         'clustCent60','data2cluster60','cluster2dataCell60','bandWidth60');
        disp('nuclei cluster result saved; ');
    else
        disp('nuclei cluster file found, loading it');
        load(str);
    end
    %% constructing CCG
    info.alpha = 0.35;
    info.radius = 0.2;
    str=sprintf('%s%s_CCGInfo_a=%.2f.mat',strPath,dirIM(i).name,info.alpha);
    
    if exist(str,'file')~=2     
        disp('begin to exract all feature; ');
        %%
        ctemp=[properties.Centroid];
        bounds.centroid_c=ctemp(1:2:end);
        bounds.centroid_r=ctemp(2:2:end);
        
        bounds.nuclei=nuclei;
        
        %%% for the cell clusters, keep the cluster center contains more than 1 cell as the node in the CCG
        list_CCGnode=[];%collection of cell cluster (30 pixels as bandwidth)
        for j=1:length(cluster2dataCell)
            if length(cluster2dataCell{j})>1
                list_CCGnode=[list_CCGnode j];
            end
        end
        
        bounds.CellClusterC_c=clustCent(2,list_CCGnode);
        bounds.CellClusterC_r=clustCent(1,list_CCGnode);
        
        CCbounds.centroid_c=bounds.CellClusterC_c;
        CCbounds.centroid_r=bounds.CellClusterC_r;


        % build graph
        alpha = info.alpha;
        r = info.radius;
        [VX,VY,x,y,edges] = construct_ccgs(CCbounds,alpha, r);

%         [VX,VY,x,y,edges] = Lconstruct_ccgs_optimized(CCbounds,alpha, r);
        
        
        save(str,'VX','VY','x','y','edges');
        disp('all CCG info saved');
    else
        disp('all CCG info file found, loading it');
        load(str);
    end
end

