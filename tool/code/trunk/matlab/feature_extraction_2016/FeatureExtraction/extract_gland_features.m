function [feats,description,feature,display_params] = extract_gland_features(bounds,collect_annotation,select)
%function [feats,description,feature] = extract_nuclear_features(bounds,img,select)

%% Extracts features from nuclei/glandular bounds
% bounds is a struct containing the centroid and boundary info
% img is a color or grayscale image used for computed for haralick features
% img can be omitted if not using haralick featuers
% select can allow you to control which group of features to run


if ~exist('select','var')
    select = [1:5];
else
    %     if iscell(select)
    %         featurelist = select;
    %         % don't re-run architectural features multiple times
    %         redundancy_idx = ismember(featurelist, [{'Voronoi'}, {'Delaunay'},{'MinimumSpanningTree'},{'NearestNeighbor'}]);
    %         if sum(redundancy_idx) > 1
    %         featurelist( redundancy_idx ) = [];
    %         featurelist(end+1) = {'Voronoi'};
    %         end
    %         select= 1:numel(featurelist);
    %     else
    %         %default list
    %         featurelist = [{'Voronoi'} {'Shape'} {'Disorder'} {'Subgraph'} {'Haralick'}  {'ClusterGraph'}];
    %     end
    if iscell(select)
        featurelist = select;
        select= 1:numel(featurelist);
    else
        featurelist = [{'Voronoi'} {'Delaunay'} {'MinimumSpanningTree'} {'NearestNeighbor'} {'Shape'} {'Disorder'} {'Subgraph'} {'Haralick'}  {'ClusterGraph'} {'Collage'}];
    end
end

feature = {};
allfeat= {};
alldes = {};
description = [];
feats = [];

if exist('collect_annotation','var')
    try
        img = imread(collect_annotation.location);
    catch
        if isunix
            split = regexp( collect_annotation.location , 'X:\','split');
            unix_mount_dir = '/mnt/Data7/common/data/';
            img = imread( fullfile(unix_mount_dir, strrep(split{2}, '\', '/') ));
            
        else
            split = regexp( collect_annotation.location , '/mnt/Data7/common/data/','split');
            windows_mount_dir = 'X:\';
            img = imread( fullfile(windows_mount_dir, split{2}) );
        end
    end
end

if exist('collect_annotation','var')
    %mask = convert_bounds2mask(zeros(collect_annotation.img_size),bounds);
    mask = convert_bounds2mask(zeros(size(img)),bounds);
end

featurelist(select)

for j = select
    clear display
    
    if exist('featurelist','var')
        pickfeat = featurelist{j};
        fprintf('\nLets try running feature %s!', pickfeat)
    else
        pickfeat = j;
    end
    
    tstart = tic;
    
    try
        switch pickfeat
            case {1,2,3,4 'Voronoi', 'Delaunay','MinimumSpanningTree','NearestNeighbor'}
                fprintf('\nExtracting Graph Features...')
                [feats, description]  = extract_graph_feats(bounds);
                
                %     des1 = swap_feature_header(o.Description.ImageFeatures,'Voronoi', 'VoronoiNuclei');
                % des2 = swap_feature_header(o.Description.ImageFeatures,'Delaunay', 'DelaunayNuclei');
                % des3 = swap_feature_header(o.Description.ImageFeatures,'MST', 'MSTNuclei');
                % des4 = swap_feature_header(o.Description.ImageFeatures,'Arch', 'NearestNeighborNuclei');
                %
                % description{1} = [des1 des2 des3 des4];
                
            case {5, 'Shape','Shapeology'}
                
                fprintf('\nExtracting Shape Features...')
                [feats, description] = extract_Shape_feats(bounds);
                %    description{2} = swap_feature_header(o.Description.ImageFeatures,'Shape', 'CellShape');
                
                
            case {6, 'Disorder' , 'COrE'}
                
                fprintf('\nExtracting CGT Features...')
                [feats, description,param,display] = extract_CGT_feats(bounds,collect_annotation.MPP);
                %description{3} = swap_feature_header(o.Description.ImageFeatures,'CGT', 'COrE');
                
            case {7, 'Subgraph'}
                
                fprintf('\nExtracting Gland Subgraph Features...')
                [feats, description,param, display] = extract_object_graph_feats(bounds,collect_annotation.MPP);
                %description{4} = swap_feature_header(o.Description.ImageFeatures,'GSG', 'CellGraph');
                
            case {8, 'Haralick'}
                
                if exist('img','var') && exist('mask','var')
                    fprintf('\nExtracting Full-Image Texture Features...')
                    [~, mask] = remove_background(img);
                    %[cropped_img, mask] = remove_background(img);
                    [feats, description, param] = extract_texture_feats(img, mask, collect_annotation.MPP); %consider downsampling image
                    %[feats, description, param,display] = extract_texture_feats(img, mask, collect_annotation.MPP);
                    %description{5} = swap_feature_header(o.Description.ImageFeatures,'Haralick', 'NuclearTexture');
                end
                
            case {9, 'ClusterGraph'}
                if exist('mask','var')
                    fprintf('\nExtracting Gland-Cluster Graph Features...')
                    %MPP = collect_annotation.MPP;
                    [feats, description,param,display] = extract_cluster_graph_feats(img, bounds,collect_annotation.MPP);
                    %description{4} = swap_feature_header(o.Description.ImageFeatures,'GSG', 'CellGraph');
                end
                
            case {10, 'Collage'}
                if exist('mask','var')
                    fprintf('\nExtracting Full-Image Collage Features...')
                    [~, mask] = remove_background(img);
                    [feats,description, param] = extract_collage_feats(img, mask, collect_annotation.MPP); %consider downsampling image
                end
        end
    catch ME
        fprintf('\n Error: %s',ME.message)
        error.message.(pickfeat) = ME.message;
        error.bounds = numel(bounds);
    end
    
    
    elapsedtime = toc(tstart);
    fprintf(' Elapsed Time:');
    fprintf(' %.4f seconds',elapsedtime);
    fprintf(' (%.2f minutes)',elapsedtime/60);
    
    %% package features
    try
        alldes = [alldes description];
        allfeat = [allfeat feats];
        
        %unpackaged
        descriptions = [description{:}];
        features = [feats];
        
        % append to list
        type = cellfun(@(x) regexp(x,':','split'), descriptions, 'UniformOutput',false);
        type = [type{:}];
        des = [type(2:2:numel(type))];
        type = [type(1:2:numel(type))];
        
        % lets see if we can incorporate some clock time
        %minTime.(featurelist{j}) = toc(tstart);
        
        elapsedtime(1:numel(feats)) = elapsedtime;
        
        %headeridx = strfind([description{:}],':'); %headeridx = headeridx{1};
        
        for i = 1:numel(type)
            feature(end+1).type = type{i};
            feature(end).description = des{i};
            feature(end).fullname = descriptions{i};
            feature(end).value = features(i);
            feature(end).runtime = elapsedtime(i);
            
            if exist('param','var')
                feature(end).param = param;
            else
                feature(end).param = [];
            end
            
        end
        
        if exist('display','var')
            display_params.(pickfeat) = display;
        else
            display_params.(pickfeat) = [];
        end
        
    catch ME
        fprintf('Could not compute %s features.', featurelist{j} );
        feats = {};
        description = {};
        feature = {};
        display_params.(pickfeat) = [];
        %display_params = ME.message;
    end
end

feats = [allfeat{:}];
description = [alldes{:}];





function [graphfeats, description] = extract_graph_feats(bounds)
gb_r = [bounds.centroid_r];
gb_c = [bounds.centroid_c];

[graphfeats] = get_graph_features([gb_r]',[gb_c]');

description = feature_list([1:4]);

function [Shapefeats, description]=extract_Shape_feats(bounds)
%% Shape
gb_r = {bounds.r};
gb_c = {bounds.c};

badglands = [];
for j = 1:length(gb_r)
    try
        if size(gb_r{1},1) == 1
            [feat] = Shape_features([gb_r{j}]',[gb_c{j}]');
        else
            [feat] = Shape_features([gb_r{j}],[gb_c{j}]);
        end
        
        feats(j,:) = feat;
        
    catch ME
        badglands = [badglands j]; %track bad glands
        fprintf('\nWarning! %i bad gland(s) found, will be removed...',numel(badglands))
    end
end

feats(badglands,:) = []; %remove bad glands

Shapefeats = [mean(feats) std(feats) median(feats) min(feats)./max(feats)];

description = feature_list(5);


function [CGTfeats, description, param,display] = extract_CGT_feats(bounds,MPP)
%% CGT
% addpath(genpath(['Z:\2012_10_GlandOrientation']))
if ~exist('MPP','var')
    MPP = 0.5; % default assumes 20x magnification
end
factor = 0.5/MPP;

%[CGTfeats,c_matrix,info] = get_CGT_features(bounds);
a=0.5 / factor;
r=0.2 * factor;
%[CGTfeats, c_matrix, info, feats, network, edges] = get_CGT_features_networks_weight(bounds,a,r);
[CGTfeats, c_matrix, info, feats, network, edges] = extract_CGT_features(bounds,a,r);

description = feature_list(6);
param.alpha = a;
param.radius = r;
display.c_matrix = c_matrix;
display.info = info;
display.feats = feats;
display.network = network;
display.edges = edges;


function [CCGfeats,description, param,display] = extract_cluster_graph_feats(img,bounds,MPP)

seg_mask = convert_bounds2mask(img,bounds);

if ~exist('MPP','var')
    MPP = 0.5; % default assumes 20x magnification
end
factor = 0.5/MPP;

%				properties = [{'Area'}    ,        {'EulerNumber' } ,     {'Orientation'},...
%      {'BoundingBox'   } ,   {'Extent'}       ,     {'Perimeter'},...
%      {'Centroid'      }  ,  {'Extrema'}       ,    {'PixelIdxList'},...
%      {'ConvexArea'    }  ,  {'FilledArea'}     ,   {'PixelList'},...
%      {'ConvexHull'    }  ,  {'FilledImage'}    ,   {'Solidity'},...
%      {'ConvexImage'   }  ,  {'Image'      }    ,   {'SubarrayIdx'},...
%      {'Eccentricity'  }  ,  {'MajorAxisLength'},...
%      {'EquivDiameter' }  ,  {'MinorAxisLength'}]

properties = [{'Area'} {'ConvexHull'}   {'MajorAxisLength'}];

radius = ceil(10 * factor);
img_d = imdilate(seg_mask, strel('disk', radius));

stats = regionprops(logical(img_d), properties);
%props = regionprops(logical(img_d),'MajorAxisLength');
%sort([props.MajorAxisLength]);

% original implementation
%cluster_threshold = 35 * 0.5 / MPP;
%accepted = [stats.MajorAxisLength] > cluster_threshold;

% safer implementation - use largest area in lieu of clusters
top = .1;
[~, sortidx] = sort([stats.Area],'descend');
accepted = sortidx(1: ceil(length(sortidx).*top) );

%centroidprops = regionprops(logical(img_d),'Centroid');
bounds1 = veta2bounds({stats.ConvexHull});

cluster_bounds = bounds1(accepted);

[CCGfeats,~, param,display] = extract_object_graph_feats(cluster_bounds, MPP);

param.dilate_strel_disk_radius = radius;
param.top_nuclei_area_ratios = top;
display.cluster_bounds = cluster_bounds;



description = feature_list(9);

function [feats,description, param,display] = extract_object_graph_feats(bounds,MPP)
%cell-graph

if ~exist('MPP','var')
    MPP = 0.5; % default assumes 20x magnification
end
factor = 0.5/MPP;

%addpath(genpath(['Z:\2012_10_GlandOrientation']))

info.alpha = 0.5 / factor;
info.radius = 0.2 * factor;
% build graph
alpha = info.alpha;
r = info.radius;
%[VX,VY,x,y,edges] = construct_ccgs(bounds,alpha, r);
[VX,VY,x,y,edges] = construct_ccgs_optimized(bounds,alpha, r,'boundary');


%[CCGfeats,feature_list] = cluster_graph_features_networkboost(bounds, edges);
[feats,~] = cluster_graph_features_optimized(bounds, edges);

description = feature_list(7);
param.alpha = info.alpha;
param.radius = info.radius;

display.VX = VX;
display.VY = VY;
display.x = x;
display.y = y;
display.edges = edges;

function [Texturefeats,description, param,display] = extract_texture_feats(img_orig,mask_orig, MPP)
%% texture extraction from entire image
%MPP = 8; % default texture extraction 0.0625x magnification (8 MPP)

%addpath(genpath(['Z:\2012_10_GlandOrientation']))
% alternatively 13 haralick
%addpath(genpath('Z:\Datasets\SatishData\haralick'))

if ~exist('MPP','var')
    MPP = 0.5; % default assumes 20x magnification

end

info.extractionMPP = 8;
newMPP = MPP/info.extractionMPP;

%factor = 0.5/MPP;
factor = 1; %don't need to worry about factor due to imresize



img = imresize(img_orig,newMPP);
mask = imresize(mask_orig,newMPP);


info.dist = round(1 * factor);
info.win = round(1 * factor);
if info.dist < 1, info.dist = 1; end
if info.win < 1, info.win = 1; end

info.grays = 256;

n = 0;

if ndims(img) < 3
    gimg = img;
elseif ndims(img) == 3
    gimg = rgb2gray(img); % assume img is rgb
else
    fprintf('Unidentified image format')
end

if ~exist('mask','var')
    mask(:,:,1) = (gimg ~= max(max(gimg)));
else
    img_mask = mask;
    mask = imdilate(mask, strel('disk', ceil(2 * factor) )); %doesn't affect feature
    mask = logical(mask);
end

%for grays = [64,128,256]
%    for win = [1,2,3]
%        n = n + 1
grays = info.grays;
win = info.win;
dist = info.dist;

himg = uint16(rescale_range(gimg,0,grays-1));
%himg = rescale_range(gimg,0,grays-1);
%f = haralick_img(himg,mask,grays,win,dist,1);
f = haralick_img(himg,mask,grays,win,dist,1);
Hf = f.img3;
%        HaralickFeat{n} = single(Hf);
%    end
%end

for i = 1:size(Hf,3)
    feat = Hf(:,:,i);
    if ~exist('img_mask','var')
        img_mask = mask(:,:,1);
    end
    roi = feat(img_mask ==1);
    
    display.texture_img{i} = feat.*img_mask;
    
    Texturefeats(n+1) = mean(roi(:));
    Texturefeats(n+2) = std(roi(:));
    %Texturefeats(n+3) = mode(roi(:));
    n = n + 2;
end


% count = 1;
% modifier = [{'mean '} {'standard deviation '}]
% for j = 1:numel(feat.names)
% for i = 1:numel(modifier)
%     HaralickTextureFeatureDescription{count} = [modifier{i} 'intensity ' feat.names{j}];
%     count = count + 1;
% end
% end

description = feature_list(8);
param.dist = info.dist;
param.win = info.win;
param.grays = info.grays;
param.extractionMPP = info.extractionMPP;


function [feats,description, param] = extract_collage_feats(img_orig,mask_orig,MPP)
%% extract collage features from entire image
%seg_mask = convert_bounds2mask(img,bounds);
info.extractionMPP = 8;

newMPP = MPP/info.extractionMPP;

img = imresize(img_orig,newMPP);
seg_mask = imresize(mask_orig,newMPP);

%[feats, ~, param]=collage_nuclei_funct(img,mask,MPP);
[feats,~,param]=compute_CoLlAGe2D_update(img,seg_mask,info.extractionMPP);
param.extractionMPP = info.extractionMPP;
description = feature_list(10);



function new_list = swap_feature_header(old_list,old, new)
% change the list of feature description to read as new feature description

temp = regexp(old_list,old);
temp2 = cellfun(@isempty,temp,'UniformOutput',false);
temp3 = old_list([temp2{:}] == 0);
new_list = cellfun(@(x) [new x( numel(old)+1:end )], temp3,'UniformOutput',false);


function list = feature_list(i)

description{1} = [
    
{'Voronoi:Area Standard Deviation'}
{'Voronoi:Area Average'}
{'Voronoi:Area Minimum / Maximum'}
{'Voronoi:Area Disorder'}
{'Voronoi:Perimeter Standard Deviation'}
{'Voronoi:Perimeter Average'}
{'Voronoi:Perimeter Minimum / Maximum'}
{'Voronoi:Perimeter Disorder'}
{'Voronoi:Chord Standard Deviation'}
{'Voronoi:Chord Average'}
{'Voronoi:Chord Minimum / Maximum'}
{'Voronoi:Chord Disorder'}
]';

description{2} = [
    
{'Delaunay:Side Length Minimum / Maximum'}
{'Delaunay:Side Length Standard Deviation'}
{'Delaunay:Side Length Average'}
{'Delaunay:Side Length Disorder'}
{'Delaunay:Triangle Area Minimum / Maximum'}
{'Delaunay:Triangle Area Standard Deviation'}
{'Delaunay:Triangle Area Average'}
{'Delaunay:Triangle Area Disorder'}

]';

description{3} = [
    
{'MinimumSpanningTree:MST Edge Length Average'}
{'MinimumSpanningTree:MST Edge Length Standard Deviation'}
{'MinimumSpanningTree:MST Edge Length Minimum / Maximum'}
{'MinimumSpanningTree:MST Edge Length Disorder'}

]';

description{4} = [
    {'NearestNeighbor:Area of polygons'}
    {'NearestNeighbor:Number of Polygons'}
    {'NearestNeighbor:Density of Polygons'}
    {'NearestNeighbor:Average distance to 3 Nearest Neighbors'}
    {'NearestNeighbor:Average distance to 5 Nearest Neighbors'}
    {'NearestNeighbor:Average distance to 7 Nearest Neighbors'}
    {'NearestNeighbor:Standard Deviation distance to 3 Nearest Neighbors'}
    {'NearestNeighbor:Standard Deviation distance to 5 Nearest Neighbors'}
    {'NearestNeighbor:Standard Deviation distance to 7 Nearest Neighbors'}
    {'NearestNeighbor:Disorder of distance to 3 Nearest Neighbors'}
    {'NearestNeighbor:Disorder of distance to 5 Nearest Neighbors'}
    {'NearestNeighbor:Disorder of distance to 7 Nearest Neighbors'}
    {'NearestNeighbor:Avg. Nearest Neighbors in a 10 Pixel Radius'}
    {'NearestNeighbor:Avg. Nearest Neighbors in a 20 Pixel Radius'}
    {'NearestNeighbor:Avg. Nearest Neighbors in a 30 Pixel Radius'}
    {'NearestNeighbor:Avg. Nearest Neighbors in a 40 Pixel Radius'}
    {'NearestNeighbor:Avg. Nearest Neighbors in a 50 Pixel Radius'}
    {'NearestNeighbor:Standard Deviation Nearest Neighbors in a 10 Pixel Radius'}
    {'NearestNeighbor:Standard Deviation Nearest Neighbors in a 20 Pixel Radius'}
    {'NearestNeighbor:Standard Deviation Nearest Neighbors in a 30 Pixel Radius'}
    {'NearestNeighbor:Standard Deviation Nearest Neighbors in a 40 Pixel Radius'}
    {'NearestNeighbor:Standard Deviation Nearest Neighbors in a 50 Pixel Radius'}
    {'NearestNeighbor:Disorder of Nearest Neighbors in a 10 Pixel Radius'}
    {'NearestNeighbor:Disorder of Nearest Neighbors in a 20 Pixel Radius'}
    {'NearestNeighbor:Disorder of Nearest Neighbors in a 30 Pixel Radius'}
    {'NearestNeighbor:Disorder of Nearest Neighbors in a 40 Pixel Radius'}
    {'NearestNeighbor:Disorder of Nearest Neighbors in a 50 Pixel Radius'}
    
    ]';

description{5} = [
    {'Shape:Mean Area Ratio'}
    {'Shape:Mean Distance Ratio'}
    {'Shape:Mean Standard Deviation of Distance'}
    {'Shape:Mean Variance of Distance'}
    {'Shape:Mean Long / Short Distance Ratio'}
    {'Shape:Mean Perimeter Ratio'}
    {'Shape:Mean Smoothness'}
    {'Shape:Mean Invariant Moment 1'}
    {'Shape:Mean Invariant Moment 2'}
    {'Shape:Mean Invariant Moment 3'}
    {'Shape:Mean Invariant Moment 4'}
    {'Shape:Mean Invariant Moment 5'}
    {'Shape:Mean Invariant Moment 6'}
    {'Shape:Mean Invariant Moment 7'}
    {'Shape:Mean Fractal Dimension'}
    {'Shape:Mean Fourier Descriptor 1'}
    {'Shape:Mean Fourier Descriptor 2'}
    {'Shape:Mean Fourier Descriptor 3'}
    {'Shape:Mean Fourier Descriptor 4'}
    {'Shape:Mean Fourier Descriptor 5'}
    {'Shape:Mean Fourier Descriptor 6'}
    {'Shape:Mean Fourier Descriptor 7'}
    {'Shape:Mean Fourier Descriptor 8'}
    {'Shape:Mean Fourier Descriptor 9'}
    {'Shape:Mean Fourier Descriptor 10'}
    {'Shape:Standard Deviation Area Ratio'}
    {'Shape:Standard Deviation Distance Ratio'}
    {'Shape:Standard Deviation Standard Deviation of Distance'}
    {'Shape:Standard Deviation Variance of Distance'}
    {'Shape:Standard Deviation Long / Short Distance Ratio'}
    {'Shape:Standard Deviation Perimeter Ratio'}
    {'Shape:Standard Deviation Smoothness'}
    {'Shape:Standard Deviation Invariant Moment 1'}
    {'Shape:Standard Deviation Invariant Moment 2'}
    {'Shape:Standard Deviation Invariant Moment 3'}
    {'Shape:Standard Deviation Invariant Moment 4'}
    {'Shape:Standard Deviation Invariant Moment 5'}
    {'Shape:Standard Deviation Invariant Moment 6'}
    {'Shape:Standard Deviation Invariant Moment 7'}
    {'Shape:Standard Deviation Fractal Dimension'}
    {'Shape:Standard Deviation Fourier Descriptor 1'}
    {'Shape:Standard Deviation Fourier Descriptor 2'}
    {'Shape:Standard Deviation Fourier Descriptor 3'}
    {'Shape:Standard Deviation Fourier Descriptor 4'}
    {'Shape:Standard Deviation Fourier Descriptor 5'}
    {'Shape:Standard Deviation Fourier Descriptor 6'}
    {'Shape:Standard Deviation Fourier Descriptor 7'}
    {'Shape:Standard Deviation Fourier Descriptor 8'}
    {'Shape:Standard Deviation Fourier Descriptor 9'}
    {'Shape:Standard Deviation Fourier Descriptor 10'}
    {'Shape:Median Area Ratio'}
    {'Shape:Median Distance Ratio'}
    {'Shape:Median Standard Deviation of Distance'}
    {'Shape:Median Variance of Distance'}
    {'Shape:Median Long / Short Distance Ratio'}
    {'Shape:Median Perimeter Ratio'}
    {'Shape:Median Smoothness'}
    {'Shape:Median Invariant Moment 1'}
    {'Shape:Median Invariant Moment 2'}
    {'Shape:Median Invariant Moment 3'}
    {'Shape:Median Invariant Moment 4'}
    {'Shape:Median Invariant Moment 5'}
    {'Shape:Median Invariant Moment 6'}
    {'Shape:Median Invariant Moment 7'}
    {'Shape:Median Fractal Dimension'}
    {'Shape:Median Fourier Descriptor 1'}
    {'Shape:Median Fourier Descriptor 2'}
    {'Shape:Median Fourier Descriptor 3'}
    {'Shape:Median Fourier Descriptor 4'}
    {'Shape:Median Fourier Descriptor 5'}
    {'Shape:Median Fourier Descriptor 6'}
    {'Shape:Median Fourier Descriptor 7'}
    {'Shape:Median Fourier Descriptor 8'}
    {'Shape:Median Fourier Descriptor 9'}
    {'Shape:Median Fourier Descriptor 10'}
    {'Shape:Min / Max Area Ratio'}
    {'Shape:Min / Max Distance Ratio'}
    {'Shape:Min / Max Standard Deviation of Distance'}
    {'Shape:Min / Max Variance of Distance'}
    {'Shape:Min / Max Long/Short Distance Ratio'}
    {'Shape:Min / Max Perimeter Ratio'}
    {'Shape:Min / Max Smoothness'}
    {'Shape:Min / Max Invariant Moment 1'}
    {'Shape:Min / Max Invariant Moment 2'}
    {'Shape:Min / Max Invariant Moment 3'}
    {'Shape:Min / Max Invariant Moment 4'}
    {'Shape:Min / Max Invariant Moment 5'}
    {'Shape:Min / Max Invariant Moment 6'}
    {'Shape:Min / Max Invariant Moment 7'}
    {'Shape:Min / Max Fractal Dimension'}
    {'Shape:Min / Max Fourier Descriptor 1'}
    {'Shape:Min / Max Fourier Descriptor 2'}
    {'Shape:Min / Max Fourier Descriptor 3'}
    {'Shape:Min / Max Fourier Descriptor 4'}
    {'Shape:Min / Max Fourier Descriptor 5'}
    {'Shape:Min / Max Fourier Descriptor 6'}
    {'Shape:Min / Max Fourier Descriptor 7'}
    {'Shape:Min / Max Fourier Descriptor 8'}
    {'Shape:Min / Max Fourier Descriptor 9'}
    {'Shape:Min / Max Fourier Descriptor 10'}
    
    ]';

description{6} = [
    
{'Disorder:mean angularity contrast energy'}
{'Disorder:standard deviation angularity contrast energy'}
{'Disorder:range angularity contrast energy'}
{'Disorder:mean angularity contrast inverse moment'}
{'Disorder:standard deviation angularity contrast inverse moment'}
{'Disorder:range angularity contrast inverse moment'}
{'Disorder:mean angularity contrast average'}
{'Disorder:standard deviation angularity contrast average'}
{'Disorder:range angularity contrast average'}
{'Disorder:mean angularity contrast variance'}
{'Disorder:standard deviation angularity contrast variance'}
{'Disorder:range angularity contrast variance'}
{'Disorder:mean angularity contrast entropy'}
{'Disorder:standard deviation angularity contrast entropy'}
{'Disorder:range angularity contrast entropy'}
{'Disorder:mean angularity intensity average'}
{'Disorder:standard deviation angularity intensity average'}
{'Disorder:range angularity intensity average'}
{'Disorder:mean angularity intensity variance'}
{'Disorder:standard deviation angularity intensity variance'}
{'Disorder:range angularity intensity variance'}
{'Disorder:mean angularity intensity entropy'}
{'Disorder:standard deviation angularity intensity entropy'}
{'Disorder:range angularity intensity entropy'}
{'Disorder:mean angularity entropy'}
{'Disorder:standard deviation angularity entropy,'}
{'Disorder:range angularity entropy'}
{'Disorder:mean angularity energy'}
{'Disorder:standard deviation angularity energy'}
{'Disorder:range angularity energy'}
{'Disorder:mean angularity correlation'}
{'Disorder:standard deviation angularity correlation'}
{'Disorder:range angularity correlation'}
{'Disorder:mean angularity information measure1'}
{'Disorder:standard deviation angularity information measure1'}
{'Disorder:range angularity information measure1'}
{'Disorder:mean angularity information measure2'}
{'Disorder:standard deviation angularity information measure2'}
{'Disorder:range angularity information measure2'}

]';

description{7} = [
    {'Subgraph:Number of Nodes'}
    {'Subgraph:Number of Edges'}
    {'Subgraph:Average Degree'}
    {'Subgraph:Average Eccentricity'}
    {'Subgraph:Diameter'}
    {'Subgraph:Radius'}
    {'Subgraph:Average Eccentricity 90 Percent'}
    {'Subgraph:Diameter 90 Percent'}
    {'Subgraph:Radius 90 Percent'}
    {'Subgraph:Average Path Length'}
    {'Subgraph:Clustering Coefficient C'}
    {'Subgraph:Clustering Coefficient D'}
    {'Subgraph:Clustering Coefficient E'}
    {'Subgraph:Number of connected components'}
    {'Subgraph:Giant connected component ratio'}
    {'Subgraph:Average connected component size'}
    {'Subgraph:Number isolated nodes'}
    {'Subgraph:Percentage isolated nodes'}
    {'Subgraph:Number end nodes'}
    {'Subgraph:Percentage end nodes'}
    {'Subgraph:Number central nodes'}
    {'Subgraph:Percentage central nodes'}
    {'Subgraph:Mean edge length'}
    {'Subgraph:Standard deviation edge length'}
    {'Subgraph:Skewness edge length'}
    {'Subgraph:Kurtosis edge length'}
    
    ]';

description{8} = [
    
{'Haralick:mean intensity contrast energy'}
{'Haralick:standard deviation intensity contrast energy'}
{'Haralick:mean intensity contrast inverse moment'}
{'Haralick:standard deviation intensity contrast inverse moment'}
{'Haralick:mean intensity contrast average'}
{'Haralick:standard deviation intensity contrast average'}
{'Haralick:mean intensity contrast variance'}
{'Haralick:standard deviation intensity contrast variance'}
{'Haralick:mean intensity contrast entropy'}
{'Haralick:standard deviation intensity contrast entropy'}
{'Haralick:mean intensity intensity average'}
{'Haralick:standard deviation intensity intensity average'}
{'Haralick:mean intensity intensity variance'}
{'Haralick:standard deviation intensity intensity variance'}
{'Haralick:mean intensity intensity entropy'}
{'Haralick:standard deviation intensity intensity entropy'}
{'Haralick:mean intensity entropy'}
{'Haralick:standard deviation intensity entropy,'}
{'Haralick:mean intensity energy'}
{'Haralick:standard deviation intensity energy'}
{'Haralick:mean intensity correlation'}
{'Haralick:standard deviation intensity correlation'}
{'Haralick:mean intensity information measure1'}
{'Haralick:standard deviation intensity information measure1'}
{'Haralick:mean intensity information measure2'}
{'Haralick:standard deviation intensity information measure2'}

]';

description{9} = [
    {'ClusterGraph:Number of Nodes'}
    {'ClusterGraph:Number of Edges'}
    {'ClusterGraph:Average Degree'}
    {'ClusterGraph:Average Eccentricity'}
    {'ClusterGraph:Diameter'}
    {'ClusterGraph:Radius'}
    {'ClusterGraph:Average Eccentricity 90 Percent'}
    {'ClusterGraph:Diameter 90 Percent'}
    {'ClusterGraph:Radius 90 Percent'}
    {'ClusterGraph:Average Path Length'}
    {'ClusterGraph:Clustering Coefficient C'}
    {'ClusterGraph:Clustering Coefficient D'}
    {'ClusterGraph:Clustering Coefficient E'}
    {'ClusterGraph:Number of connected components'}
    {'ClusterGraph:Giant connected component ratio'}
    {'ClusterGraph:Average connected component size'}
    {'ClusterGraph:Number isolated nodes'}
    {'ClusterGraph:Percentage isolated nodes'}
    {'ClusterGraph:Number end nodes'}
    {'ClusterGraph:Percentage end nodes'}
    {'ClusterGraph:Number central nodes'}
    {'ClusterGraph:Percentage central nodes'}
    {'ClusterGraph:Mean edge length'}
    {'ClusterGraph:Standard deviation edge length'}
    {'ClusterGraph:Skewness edge length'}
    {'ClusterGraph:Kurtosis edge length'}
    
    ]';


description{10} = [
    {'Collage:max gradient entropy',
'Collage:mean gradient entropy',
'Collage:median gradient entropy',
'Collage:min gradient entropy',
'Collage:standard deviation gradient entropy',
'Collage:max gradient energy',
'Collage:mean gradient energy',
'Collage:median gradient energy',
'Collage:min gradient energy',
'Collage:standard deviation gradient energy',
'Collage:max gradient inertia',
'Collage:mean gradient inertia',
'Collage:median gradient inertia',
'Collage:min gradient inertia',
'Collage:standard deviation gradient inertia',
'Collage:max gradient idm',
'Collage:mean gradient idm',
'Collage:median gradient idm',
'Collage:min gradient idm',
'Collage:standard deviation gradient idm',
'Collage:max gradient correlation',
'Collage:mean gradient correlation',
'Collage:median gradient correlation',
'Collage:min gradient correlation',
'Collage:standard deviation gradient correlation',
'Collage:max gradient info1',
'Collage:mean gradient info1',
'Collage:median gradient info1',
'Collage:min gradient info1',
'Collage:standard deviation gradient info1',
'Collage:max gradient info2',
'Collage:mean gradient info2',
'Collage:median gradient info2',
'Collage:min gradient info2',
'Collage:standard deviation gradient info2',
'Collage:max gradient sum_av',
'Collage:mean gradient sum_av',
'Collage:median gradient sum_av',
'Collage:min gradient sum_av',
'Collage:standard deviation gradient sum_av',
'Collage:max gradient sum_var',
'Collage:mean gradient sum_var',
'Collage:median gradient sum_var',
'Collage:min gradient sum_var',
'Collage:standard deviation gradient sum_var',
'Collage:max gradient sum_ent',
'Collage:mean gradient sum_ent',
'Collage:median gradient sum_ent',
'Collage:min gradient sum_ent',
'Collage:standard deviation gradient sum_ent',
'Collage:max gradient diff_av',
'Collage:mean gradient diff_av',
'Collage:median gradient diff_av',
'Collage:min gradient diff_av',
'Collage:standard deviation gradient diff_av',
'Collage:max gradient diff_var',
'Collage:mean gradient diff_var',
'Collage:median gradient diff_var',
'Collage:min gradient diff_var',
'Collage:standard deviation gradient diff_var',
'Collage:max gradient diff_ent',
'Collage:mean gradient diff_ent',
'Collage:median gradient diff_ent',
'Collage:min gradient diff_ent',
'Collage:standard deviation gradient diff_ent'}
    ]';

if exist('i','var')
    list = description(i);
else
    list = description;
end