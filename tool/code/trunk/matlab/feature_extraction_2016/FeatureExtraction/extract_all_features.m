function [feats,description] = extract_all_features(bounds,img,select)
%% Extracts features from nuclei/glandular bounds
% bounds is a struct containing the centroid and boundary info
% img is a color or grayscale image used for computed for haralick features
% img can be omitted if not using haralick featuers
% select can allow you to control which group of features to run

% if i really want to fix those names
% if strcmp(type, 'nuclei')
load NewImgDescription.mat
%     temp = regexp(o.Description.ImageFeatures,'CGT');
%     temp2 = cellfun(@isempty,temp,'UniformOutput',false);
%
%     %description{3} = o.Description.ImageFeatures([temp2{:}] == 0);
% else
%     load NewImgDescription.mat
% end

% Dimred2 = @(x) Dimred(x, 'GE', param);
% EigV = cellfun(Dimred2,data,'UniformOutput',false);

if nargin < 3
    select = [1:5];
end

if ~isempty(intersect(select,1))
    fprintf('\nExtracting Graph Features...')
    tic
    feats{1} = extract_graph_feats(bounds);
    description{1} = [o.Description.ImageFeatures(1:51)];
    toc
    
end
if ~isempty(intersect(select,2))
    
    fprintf('\nExtracting Morph Features...')
    tic
    feats{2} = extract_morph_feats(bounds);
    temp = regexp(o.Description.ImageFeatures,'Morph');
    temp2 = cellfun(@isempty,temp,'UniformOutput',false);
    description{2} = [o.Description.ImageFeatures([temp2{:}] == 0)];
    toc
end
if ~isempty(intersect(select,3))
    
    fprintf('\nExtracting CGT Features...')
    tic
    feats{3} = extract_CGT_feats(bounds);
    temp = regexp(o.Description.ImageFeatures,'CGT');
    temp2 = cellfun(@isempty,temp,'UniformOutput',false);
    description{3} = [o.Description.ImageFeatures([temp2{:}] == 0)];
    toc
end
if ~isempty(intersect(select,4))
    
    fprintf('\nExtracting Cluster Graph Features...')
    tic
    feats{4} = extract_cluster_graph_feats(bounds);
    temp = regexp(o.Description.ImageFeatures,'GSG');
    temp2 = cellfun(@isempty,temp,'UniformOutput',false);
    description{4} = [o.Description.ImageFeatures([temp2{:}] == 0)];
    toc
end

if ~isempty(intersect(select,5))
    
    if nargin > 1
        fprintf('\nExtracting Texture Features...')
        tic
        feats{5} = extract_texture_feats(img);
        temp = regexp(o.Description.ImageFeatures,'Haralick');
        temp2 = cellfun(@isempty,temp,'UniformOutput',false);
        description{5} = [o.Description.ImageFeatures([temp2{:}] == 0)];
        toc
    end
end


%feat_description


function [graphfeats] = extract_graph_feats(bounds)
gb_r = [bounds.centroid_r];
gb_c = [bounds.centroid_c];

[graphfeats] = get_graph_features([gb_r]',[gb_c]');


function [morphfeats]=extract_morph_feats(bounds)
%% Morph
gb_r = {bounds.r};
gb_c = {bounds.c};
   
badglands = [];
for j = 1:length(gb_r)
    try
        if size(gb_r{1},1) == 1
            [feat] = morph_features([gb_r{j}]',[gb_c{j}]');
        else
            [feat] = morph_features([gb_r{j}],[gb_c{j}]);
        end
        
        feats(j,:) = feat;
        
    catch ME
        badglands = [badglands j]; %track bad glands
        fprintf('\nWarning! %i bad gland(s) found, will be removed...',numel(badglands))
    end
end

feats(badglands,:) = []; %remove bad glands

morphfeats = [mean(feats) std(feats) median(feats) min(feats)./max(feats)];



function [CGTfeats] = extract_CGT_feats(bounds)
%% CGT
% addpath(genpath(['Z:\2012_10_GlandOrientation']))

%[CGTfeats,c_matrix,info] = get_CGT_features(bounds);
a=0.5;
r=0.2;
%[CGTfeats, c_matrix, info, feats, network, edges] = get_CGT_features_networks_weight(bounds,a,r);
[CGTfeats, c_matrix, info, feats, network, edges] = extract_CGT_features(bounds,a,r);

function [CCGfeats,feature_list] = extract_cluster_graph_feats(bounds)
%CCG

%addpath(genpath(['Z:\2012_10_GlandOrientation']))

info.alpha = 0.5;
info.radius = 0.2;
% build graph
alpha = info.alpha;
r = info.radius;
%[VX,VY,x,y,edges] = construct_ccgs(bounds,alpha, r);
[VX,VY,x,y,edges] = construct_ccgs_optimized(bounds,alpha, r);

%[CCGfeats,feature_list] = cluster_graph_features_networkboost(bounds, edges);
[CCGfeats,feature_list] = cluster_graph_features_optimized(bounds, edges);

function [Texturefeats] = extract_texture_feats(img)

%% texture
%addpath(genpath(['Z:\2012_10_GlandOrientation']))

% alternatively 13 haralick
%addpath(genpath('Z:\Datasets\SatishData\haralick'))

info.dist = 1;
info.win = 1;
info.grays = 256;

n = 0;

if ndims(img) < 3
    gimg = img;
elseif ndims(img) == 3
    gimg = rgb2gray(img); % assume img is rgb
else
    fprintf('Unidentified image format')
end

mask(:,:,1) = (gimg ~= max(max(gimg)));
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
    img_mask = mask(:,:,1);
    roi = feat(img_mask ==1);
    
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
