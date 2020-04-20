%%% classlabel -1 progressor; -0 nonprogressor
% nucleiseg_nucleiclusterfinding_extractfullfeature_OroCavity_v2(4,5)

% V2 extract features from epi/stroma/epistroma seperately
% V3 extract CCM features from epi/stroma/epistroma seperately
% V4 extract CCM features using correct CCM calculation
% V5 extract CRLM features 
function nucleiseg_nucleiclusterfinding_extractfullfeature_OroCavity_v5(idxBegin,idxEnd)
%% parameter initialization
addpath(genpath('../Code/George_GraphFeatureExtraction'));
addpath(genpath('/Users/chenglu/Nutstore/PathImAnalysis_Program/Program/Features/George_GraphFeatureExtraction'));
addpath(genpath('/Users/chenglu/Nutstore/PathImAnalysis_Program/Program/OropharyngealSCC_TMA'));
addpath(genpath('C:\Nutstore\Nutstore\PathImAnalysis_Program\Program\Features\George_GraphFeatureExtraction'));
if ismac
    strPath='/Users/chenglu/Documents/Dataset/WUSTL_Oral_Histology/Training50_features/';
%     strPath='/Users/chenglu/Nutstore/Training40_features/';
    strIMPath='/Users/chenglu/Documents/Dataset/WUSTL_Oral_Histology/Training50/';
    strESMask='/Users/chenglu/Documents/Dataset/WUSTL_Oral_Histology/training50_downby8_png/';
    dirIM=dir(strIMPath);
    dirIM(1:3)=[];
else if ispc
        strPath='C:\Nutstore\Nutstore\Training50_features\';
        strIMPath='C:\Dataset\WUSTL_Oral_Histology\Training50\';
        strESMask='C:\Nutstore\Nutstore\training50_downby8_png\';
        
        strPath='C:\Dataset\WUSTL_Oral_Histology\Training50_features\';
        strIMPath='C:\Dataset\WUSTL_Oral_Histology\Training50\';
        strESMask='C:\Dataset\WUSTL_Oral_Histology\training50_downby8_png\';

        dirIM=dir(strIMPath);
        dirIM(1:2)=[];
    else
        strIMPath='../Training50/';
        strPath='../Training50_features/';
        strESMask='../Training50_downby8_png/';
        
%         strIMPath='../testing68/';
%         strPath='../testing68_features/';
%         strESMask='../testing68_downby8_png/';        
        dirIM=dir([strIMPath '*.tif']);
%         dirIM(1:2)=[];
    end
end

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
               
        %%% save results
        save(strSaveCur,'nuclei','properties');
        disp('nuclei result saved; ');
    else
        disp('nuclei file found, loading it');
        load(strSaveCur);
    end
    
    %%  !!!!!!!! below code are for the feature extractions with epi/stroma seperation !!!!!!!!
    %% grouping nuclei based on epi/stroma area, denoting epi/stroma as ES henceforth
    strSave=sprintf('%snuclei_label_in_epistroma_IM%s.mat',strPath,dirIM(i).name);
    if exist(strSave,'file')~=2
        
        %% read ES mask first
        str_cur=dirIM(i).name;
        str=sprintf('%s%s_mask.png',strESMask,str_cur(1:end-4));
        bw=imread(str); %show(mask); show(~mask);
        %%%% fill in some small holes due to inaccurate annotations.
        cc= bwconncomp(~bw);
        stats = regionprops(cc, 'Area');
        idx = find([stats.Area] > 20);
        bw_remove = ismember(labelmatrix(cc), idx); %show( bw_remove);
        mask= ~bw_remove;
        %%%% project the mask to original dimension
        mask=imresize(mask,[size(I,1) size(I,2)]);%  LshowBWonIM(mask,I);
        
        %% find nuclei in epi/stroma, use 'nuclei_label' to indicate the nuclei
        %%% in epi-1, or stroma-0
        all_xy=[properties.Centroid];
        all_x=all_xy(1:2:end);all_y=all_xy(2:2:end);
        all_idx=sub2ind([size(I,1) size(I,2)],round(all_y),round(all_x));
        mask_list=mask(:);
        nuclei_label=mask_list(all_idx);
        %%%% check
        %     LshowBWonIM(mask,I,1);hold on;
        %     for k = 1:length(nuclei)
        %         if nuclei_label(k)
        %             plot(nuclei{k}(:,2), nuclei{k}(:,1), 'g-', 'LineWidth', 1);
        %         else
        %             plot(nuclei{k}(:,2), nuclei{k}(:,1), 'b-', 'LineWidth', 1);
        %         end
        %     end
        %     hold off;
        save(strSave,'nuclei_label');
        disp('nuclei label in epi/stroma file saved');
    else
        disp('nuclei label in epi/stroma file found, loading it');
        load(strSave);
    end
    %% nuclei cluster finding - meanshift with epi/stroma seperation
    bandWidth=30;
    str=sprintf('%sMSCellCluster_BW%d_IM%s_epistroma.mat',strPath,bandWidth,dirIM(i).name);
    if exist(str,'file')~=2
        try
            %% use meanshift to form cell clusters, use location info only
            %%% in epi
            AlldataPts=[];
            tempC=[properties(nuclei_label).Centroid];
            AlldataPts(1:sum(nuclei_label),1)=tempC(1:2:end);
            AlldataPts(1:sum(nuclei_label),2)=tempC(2:2:end);
            
            dataPts_epi=AlldataPts(:,1:2);
            
            disp('begin cell cluster finding in epi;');
            
            [clustCent_epi,data2cluster_epi,cluster2dataCell_epi] = MeanShiftCluster(dataPts_epi',bandWidth);
            
            %%% in stroma
            AlldataPts=[];
            tempC=[properties(~nuclei_label).Centroid];
            AlldataPts(1:sum(~nuclei_label),1)=tempC(1:2:end);
            AlldataPts(1:sum(~nuclei_label),2)=tempC(2:2:end);
            
            dataPts_stroma=AlldataPts(:,1:2);
            
            disp('begin cell cluster finding in stroma;');
            
            [clustCent_stroma,data2cluster_stroma,cluster2dataCell_stroma] = MeanShiftCluster(dataPts_stroma',bandWidth);
            %%%% check
            %%% show the cluster nodes only
            %             LshowBWonIM(mask,I,1);hold on;
            % %             for k = 1:length(nuclei)
            % %                 if nuclei_label(k)
            % %                     plot(nuclei{k}(:,2), nuclei{k}(:,1), 'g-', 'LineWidth', 1);
            % %                 else
            % %                     plot(nuclei{k}(:,2), nuclei{k}(:,1), 'b-', 'LineWidth', 1);
            % %                 end
            % %             end
            % %
            %             numObj=size(clustCent_epi,2);
            %             for k = 1 : numObj
            %                 plot(clustCent_epi(1,k),clustCent_epi(2,k), 'gp');
            %             end
            %
            %             numObj=size(clustCent_stroma,2);
            %             for k = 1 : numObj
            %                 plot(clustCent_stroma(1,k),clustCent_stroma(2,k), 'bp');
            %             end
            %             hold off;
            
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
        
        save(str,'clustCent_stroma','data2cluster_stroma','cluster2dataCell_stroma',...
            'clustCent_epi','data2cluster_epi','cluster2dataCell_epi','bandWidth');
        %
        %     str=sprintf('%sPro_MSCellCluster_BW%d_%d_IM%d.mat',strPath,bandWidth,bandWidth60,i);
        %     save(str,'clustCent','data2cluster','cluster2dataCell','bandWidth',...
        %         'clustCent60','data2cluster60','cluster2dataCell60','bandWidth60');
        disp('nuclei cluster in epi/stroma result saved; ');
    else
        disp('nuclei cluster in epi/stroma file found, loading it');
        load(str);
    end
    %% extract all features with epi/stroma seperation
    str=sprintf('%sFullFeatures_IM%s_epistroma.mat',strPath,dirIM(i).name);
    
    %%% the parameter to construct CCG matters, so we want to extract a CCG featuers with different
    %%% parameter.
    
    para.CGalpha_min=0.4; para.CGalpha_max=0.5;
    para.CCGalpha_min=0.36; para.CCGalpha_max=0.4; para.alpha_res=0.01;% larger the alpha, sparse the graph
    para.radius=0.2;
    
    if exist(str,'file')~=2
        disp('begin to exract all feature; ');
        %% in epi
        ctemp=[properties(nuclei_label).Centroid];
        bounds_epi.centroid_c=ctemp(1:2:end);
        bounds_epi.centroid_r=ctemp(2:2:end);
        
        bounds_epi.nuclei=nuclei(nuclei_label);
        
        %%% for the cell clusters, keep the cluster center contains more than 1 cell as the node in the CCG
        list_CCGnode=[];%collection of cell cluster (30 pixels as bandwidth)
        for j=1:length(cluster2dataCell_epi)
            if length(cluster2dataCell_epi{j})>0
                list_CCGnode=[list_CCGnode j];
            end
        end
        
        bounds_epi.CellClusterC_c=clustCent_epi(2,list_CCGnode);
        bounds_epi.CellClusterC_r=clustCent_epi(1,list_CCGnode);
        
        [feats_epi,description_epi,CCGinfo_epi,CGinfo_epi] = Lextract_all_features(bounds_epi,para);
        %% in stroma
        ctemp=[properties(~nuclei_label).Centroid];
        bounds_stroma.centroid_c=ctemp(1:2:end);
        bounds_stroma.centroid_r=ctemp(2:2:end);
        
        bounds_stroma.nuclei=nuclei(~nuclei_label);
        
        %%% for the cell clusters, keep the cluster center contains more than 1 cell as the node in the CCG
        list_CCGnode=[];%collection of cell cluster (30 pixels as bandwidth)
        for j=1:length(cluster2dataCell_stroma)
            if length(cluster2dataCell_stroma{j})>0
                list_CCGnode=[list_CCGnode j];
            end
        end
        
        bounds_stroma.CellClusterC_c=clustCent_stroma(2,list_CCGnode);
        bounds_stroma.CellClusterC_r=clustCent_stroma(1,list_CCGnode);
        
        [feats_stroma,description_stroma,CCGinfo_stroma,CGinfo_stroma] = Lextract_all_features(bounds_stroma,para);
        
        save(str,'feats_epi','description_epi','CCGinfo_epi','CGinfo_epi','feats_stroma','description_stroma','CCGinfo_stroma','CGinfo_stroma','para','-v7.3');
        
        disp('all features result in epi/stroma saved');
    else
        disp('all features file in epi/stroma found, loading it');
        load(str);
    end
    %% extract haralick features
    str=sprintf('%sHaralickFeatures_IM%s_epistroma.mat',strPath,dirIM(i).name);
    
    if exist(str,'file')~=2
        %%
        disp('begin to exract haralick feature; ');
        [I_norm, ~, ~] = normalizeStaining(I);
        bounds.nuclei=nuclei(nuclei_label);
        [feats_epi,description_epi] = Lextract_haralick_features(bounds,I_norm);
        
        bounds.nuclei=nuclei(~nuclei_label);
        [feats_stroma,description_stroma] = Lextract_haralick_features(bounds,I_norm);
        
        save(str,'feats_stroma','description_stroma','feats_epi','description_epi','-v7.3');
        disp('haralick features result in epi/stroma saved');
    else
        disp('haralick features file in epi/stroma found, loading it');
        load(str);
    end
    
    %% extract CCM features
    str=sprintf('%sCCMFeatures_IM%s_epistroma.mat',strPath,dirIM(i).name);
    
    para.CGalpha_min=0.38; para.CGalpha_max=0.5;
    para.alpha_res=0.02;% larger the alpha, sparse the graph
    para.radius=0.2;
    
    if exist(str,'file')~=2
        %%
        disp('begin to exract CCM feature; ');
        [I_norm, ~, ~] = normalizeStaining(I);
        
        ctemp=[properties(nuclei_label).Centroid];
        bounds_epi.centroid_c=ctemp(1:2:end);
        bounds_epi.centroid_r=ctemp(2:2:end);
        bounds_epi.nuclei=nuclei(nuclei_label);

        [feats_epi,description_epi,~] = Lextract_CCM_features_v2(bounds_epi,I_norm,properties,para);
        
        ctemp=[properties(~nuclei_label).Centroid];
        bounds_stroma.centroid_c=ctemp(1:2:end);
        bounds_stroma.centroid_r=ctemp(2:2:end);
        
        bounds_stroma.nuclei=nuclei(~nuclei_label);
        
        [feats_stroma,description_stroma,~] = Lextract_CCM_features_v2(bounds_stroma,I_norm,properties,para);
        
        save(str,'feats_stroma','description_stroma','feats_epi','description_epi','-v7.3');
        disp('CCM features result in epi/stroma saved');
    else
        disp('CCM features file in epi/stroma found, loading it');
        load(str);
    end
    
     %% extract CRLM features
    str=sprintf('%sCRLMFeatures_IM%s_epistroma.mat',strPath,dirIM(i).name);
    
    para.CGalpha_min=0.38; para.CGalpha_max=0.5;
    para.alpha_res=0.02;% larger the alpha, sparse the graph
    para.radius=0.2;
    
    if exist(str,'file')~=2
        %%
        disp('begin to exract CRLM feature; ');
        [I_norm, ~, ~] = normalizeStaining(I);
        
        ctemp=[properties(nuclei_label).Centroid];
        bounds_epi.centroid_c=ctemp(1:2:end);
        bounds_epi.centroid_r=ctemp(2:2:end);
        bounds_epi.nuclei=nuclei(nuclei_label);

        [feats_epi,description_epi,~] = Lextract_CRLM_features_v2(bounds_epi,I_norm,properties,para);
        
        ctemp=[properties(~nuclei_label).Centroid];
        bounds_stroma.centroid_c=ctemp(1:2:end);
        bounds_stroma.centroid_r=ctemp(2:2:end);
        
        bounds_stroma.nuclei=nuclei(~nuclei_label);
        
        [feats_stroma,description_stroma,~] = Lextract_CRLM_features_v2(bounds_stroma,I_norm,properties,para);
        
        save(str,'feats_stroma','description_stroma','feats_epi','description_epi','-v7.3');
        disp('CRLM features result in epi/stroma saved');
    else
        disp('CRLM features file in epi/stroma found, loading it');
        load(str);
    end
    
    %%  !!!!!!!! below code are for the feature extractions without epi/stroma seperation !!!!!!!!
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
    %% extract CMM features
    str=sprintf('%sCCMFeatures_IM%s.mat',strPath,dirIM(i).name);
    para.CGalpha_min=0.38; para.CGalpha_max=0.5;
    para.alpha_res=0.02;% larger the alpha, sparse the graph
    para.radius=0.2;
    
    if exist(str,'file')~=2
        %%
        disp('begin to exract CMM feature; ');
        [I_norm, ~, ~] = normalizeStaining(I);
        
        ctemp=[properties.Centroid];
        bounds.centroid_c=ctemp(1:2:end);
        bounds.centroid_r=ctemp(2:2:end);
        
        bounds.nuclei=nuclei;

        [feats,description,~] = Lextract_CCM_features_v2(bounds,I_norm,properties,para);
        
        save(str,'feats','description','-v7.3');
        disp('CCM features result saved');
    else
        disp('CCM features file found, loading it');
        load(str);
    end        
    
        %% extract CRLM features
    str=sprintf('%sCRLMFeatures_IM%s.mat',strPath,dirIM(i).name);
    para.CGalpha_min=0.38; para.CGalpha_max=0.5;
    para.alpha_res=0.02;% larger the alpha, sparse the graph
    para.radius=0.2;
    
    if exist(str,'file')~=2
        %%
        disp('begin to exract CRLM feature; ');
        [I_norm, ~, ~] = normalizeStaining(I);
        
        ctemp=[properties.Centroid];
        bounds.centroid_c=ctemp(1:2:end);
        bounds.centroid_r=ctemp(2:2:end);
        
        bounds.nuclei=nuclei;

        [feats,description,~] = Lextract_CRLM_features_v2(bounds,I_norm,properties,para);
        
        save(str,'feats','description','-v7.3');
        disp('CRLM features result saved');
    else
        disp('CRLM features file found, loading it');
        load(str);
    end   
end

