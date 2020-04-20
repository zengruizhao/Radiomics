%%% classlabel -1 progressor; -0 nonprogressor
% nucleiseg_nucleiclusterfinding_extractfullfeature_OroCavity(4,5)

%% parameter initialization
addpath(genpath('../Code/George_GraphFeatureExtraction'));
addpath(genpath('/Users/chenglu/Nutstore/PathImAnalysis_Program/Program/Features/George_GraphFeatureExtraction'));
addpath(genpath('C:\Nutstore\Nutstore\PathImAnalysis_Program\Program\Features\George_GraphFeatureExtraction'));
addpath(genpath('C:\Nutstore\Nutstore\Repository\George-newbranch\george_code\newbranch\tools\FeatureExtraction\hosoya'));
addpath(genpath('/Users/chenglu/Nutstore/PathImAnalysis_Program/Program/OropharyngealSCC_TMA'));
if ismac
    strPath='/Users/chenglu/Documents/Dataset/WUSTL_Oral_Histology/Training40_features/';
    strIMPath='/Users/chenglu/Documents/Dataset/WUSTL_Oral_Histology/Training40/';
else if ispc
        strPath='C:\Dataset\WUSTL_Oral_Histology\Training50_features\';
        strIMPath='C:\Dataset\WUSTL_Oral_Histology\Training50\';
        
    else
        strPath='../Training40_features/';
        strIMPath='../Training40/';
    end
end


% dirIM=dir(strIMPath);
% dirIM(1:3)=[];
dirIM=dir([strIMPath '*.tif']);
%% begin to work
i=1;

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

%% test feature extraction
ctemp=[properties.Centroid];
bounds.centroid_c=ctemp(1:2:end);
bounds.centroid_r=ctemp(2:2:end);

bounds.nuclei=nuclei;

% [allCCM,CCM_description,CCGinfo] = Lextract_CCM_features(bounds,I,properties,0.37,.2);
% [feats,description,CCGinfo] = Lextract_Hosoya_features(bounds);

para.CGalpha_min=0.38; para.CGalpha_max=0.5;
para.alpha_res=0.02;% larger the alpha, sparse the graph
para.radius=0.2;

[allCCM,CCM_description,CCGinfo] = Lextract_CCM_features(bounds,I,properties,para);



