%%% classlabel -1 progressor; -0 nonprogressor
% nucleiseg_nucleiclusterfinding_extractfullfeature_OroCavity(4,5)
function nucleiseg_nucleiclusterfinding_extractfullfeature_OroCavity(idxBegin,idxEnd)
%% parameter initialization
addpath(genpath('../Code/George_GraphFeatureExtraction'));
addpath(genpath('/Users/chenglu/Nutstore/PathImAnalysis_Program/Program/Features/George_GraphFeatureExtraction'));

if ismac
    strPath='/Users/chenglu/Documents/Dataset/WUSTL_Oral_Histology/Training40_features/';
    strIMPath='/Users/chenglu/Documents/Dataset/WUSTL_Oral_Histology/Training40/';
else if ispc
    else
        strIMPath='../Training40/';
        strPath='../Training40_features/';
    end
end
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
%         clear I;
        I_normRed=I_norm(:,:,1);
        
        p.scales=[8:2:18];
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
    %% extract all features
    str=sprintf('%sFullFeatures_IM%s.mat',strPath,dirIM(i).name);
    
    %%% the parameter to construct CCG matters, so we want to extract a CCG featuers with different
    %%% parameter.
    
    para.CGalpha_min=0.4; para.CGalpha_max=0.5;
    para.CCGalpha_min=0.36; para.CCGalpha_max=0.4; para.alpha_res=0.01;% larger the alpha, sparse the graph
    para.radius=0.2;
    
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
            if length(cluster2dataCell{j})>0
                list_CCGnode=[list_CCGnode j];
            end
        end
        
        bounds.CellClusterC_c=clustCent(2,list_CCGnode);
        bounds.CellClusterC_r=clustCent(1,list_CCGnode);
        
        [feats,description,CCGinfo,CGinfo] = Lextract_all_features(bounds,para);
        
        save(str,'feats','description','CCGinfo','CGinfo','para','-v7.3');
        disp('all features result saved');
    else
        disp('all features file found, loading it');
        load(str);
    end
    %% extract haralick features
    str=sprintf('%sHaralickFeatures_IM%s.mat',strPath,dirIM(i).name);
    
    if exist(str,'file')~=2
        %%
        disp('begin to exract haralick feature; ');
        [I_norm, ~, ~] = normalizeStaining(I);
        bounds.nuclei=nuclei;
        [feats,description] = Lextract_haralick_features(bounds,I_norm);
        
        save(str,'feats','description','-v7.3');
        disp('haralick features result saved');
    else
        disp('haralick features file found, loading it');
        load(str);
    end
end

