function feat_extract_regions

dataset = [];
if isunix
    homedir = ['~/Datasets/UPENNHistoMassSpec'];
else
    homedir = ['Z:\Datasets\UPENNHistoMassSpec'];
end

addpath(genpath([homedir]))

% %  cd([homedir filesep 'Development/code/lcib_1.0/George'])
% load Datasetparameters_7_13_2012.mat
% img_directory = '\Development\old_results_regions_SPIE_7_17_2012'

  load 24control_regions_1_8_2013.mat
  dataset = unique(regionlink.aperioID)
  img_directory = '\Development\results';

  %cd([homedir '/MassSpec'])
% %img_directory = '../progressor - bounds from James/';



% extract features from regions

if matlabpool('size') == 0
matlabpool(4)
end

for i = 1:length(dataset)
    regionfolder = {};
    for r = 1:50
        if exist([homedir img_directory filesep dataset{i} filesep 'region' num2str(r)],'dir') ~= 0
            regionfolder{r} = [dataset{i} '\' 'region' num2str(r)];
            try
                fprintf(['\n Image' num2str(dataset{i}) ' , Region ' num2str(r)])
                %fprintf('\n Extracting Morph Features...')
                %extract_morph_feats_regions(homedir, img_directory, regionfolder{r});
                %fprintf('\n Extracting Graph Features...')
                %extract_graph_feats_regions(homedir, img_directory, regionfolder{r});
                % fprintf('\n Extracting CGT Features...')
                % extract_CGT_feats_regions(homedir, img_directory, regionfolder{r});
                %fprintf('\n Extracting CCG Features...')
                %extract_ccg_feats_regions(homedir,img_directory,regionfolder{r})
                fprintf('\n Extracting Texture Features...')
                extract_texture_feats_regions(homedir, img_directory, regionfolder{r});
            catch
                fprintf(['\n error: Image' num2str(dataset{i}) ', Region ' num2str(r)])
            end
        end
    end
end

% for i = 1:480, area(i) = bounds(i).features.area; end % attempt to remove
%  TMA from feature extraction process

function extract_graph_feats_regions(homedir, img_directory, string)
%% Graph
addpath(genpath([homedir]))

load([homedir img_directory filesep string '/feature_bounds4.mat'])

gb_r = [bounds.centroid_r];
gb_c = [bounds.centroid_c];

[feats] = get_graph_features([gb_r]',[gb_c]');
%[vfeature] = graphfeats3([gb_r]',[gb_c]')

index = regexp(string,'\');
if ~isempty(index)
    imgfilename = string(1:index(1)-1);
    regionname = string(index(1)+1:end);
end

filename = [imgfilename regionname];

if ~exist([homedir '/MassSpec/GraphFeats'],'dir')
    mkdir([homedir '/MassSpec/GraphFeats'])
end
save([homedir '/MassSpec/GraphFeats/' filename '_graphfeats.mat'],'feats');


function extract_morph_feats_regions(homedir, img_directory, string)
%% Morph
addpath(genpath([homedir]))

load([homedir img_directory filesep string '/feature_bounds4.mat'])
gb_r = {bounds.r};
gb_c = {bounds.c};

badglands = [];
for j = 1:length(gb_r)
    try
        [feat] = morph_features([gb_r{j}]',[gb_c{j}]');
        feats(j,:) = feat;
        
    catch ME
        badglands = [badglands j] %track bad glands
    end
end

feats(badglands,:) = []; %remove bad glands

allfeats = [mean(feats) std(feats) median(feats) min(feats)./max(feats)];

index = regexp(string,'\');
if ~isempty(index)
    imgfilename = string(1:index(1)-1);
    regionname = string(index(1)+1:end);
end

filename = [imgfilename regionname];

if ~exist([homedir '/MassSpec/MorphFeats'],'dir')
    mkdir([homedir '/MassSpec/MorphFeats'])
end

if isempty(badglands)
    save([homedir '/MassSpec/MorphFeats/' filename '_morphfeats.mat'],'allfeats');
else
    save([homedir '/MassSpec/MorphFeats/' filename '_morphfeats.mat'],'allfeats','badglands');
end

function extract_CGT_feats_regions(homedir, img_directory, string)
%% CGT
addpath(genpath([homedir]))
addpath(genpath(['Z:\2012_10_GlandOrientation']))
%string = ['22226' filesep 'region1']

load([homedir img_directory filesep string '/feature_bounds4.mat'])
%[CGTfeats,c_matrix,info] = get_CGT_features(bounds);
[CGTfeats,c_matrix,info] = get_CGT_features_new(bounds);

index = regexp(string,'\');
if ~isempty(index)
    imgfilename = string(1:index(1)-1);
    regionname = string(index(1)+1:end);
end

filename = [imgfilename regionname];

if ~exist([homedir '/MassSpec/CGTFeats'],'dir')
    mkdir([homedir '/MassSpec/CGTFeats'])
end

save([homedir '/MassSpec/CGTFeats/' filename '_CGTfeats.mat'],'CGTfeats','c_matrix','info');

function extract_ccg_feats_regions(homedir,img_directory,string)
%CCG

addpath(genpath([homedir]))
addpath(genpath(['Z:\2012_10_GlandOrientation']))

load([homedir img_directory filesep string '/feature_bounds4.mat'])
    
    info.alpha = 0.5
    info.radius = 0.2
   % build graph
    alpha = info.alpha
    r = info.radius
    [VX,VY,x,y,edges] = construct_ccgs(bounds,alpha, r);

[CCGfeats,feature_list] = cell_graph_features(bounds, edges);

index = regexp(string,'\');
if ~isempty(index)
    imgfilename = string(1:index(1)-1);
    regionname = string(index(1)+1:end);
end

filename = [imgfilename regionname];

if ~exist([homedir '/MassSpec/CCGFeats'],'dir')
    mkdir([homedir '/MassSpec/CCGFeats'])
end

save([homedir '/MassSpec/CCGFeats/' filename '_CCGfeats.mat'],'CCGfeats','feature_list','info');


function extract_texture_feats_regions(homedir, img_directory, string)

%% texture
addpath(genpath([homedir]))
addpath(genpath(['Z:\2012_10_GlandOrientation']))

%load([homedir img_directory filesep string '/feature_bounds4.mat'])

load([homedir img_directory filesep string '/img.mat'])
%prep img
%CASE_INDEX = 1
% img = loadimg(file_dir,regionlink, CASE_INDEX);
% img_array(CASE_INDEX) = {img}
% 
% img = img_array{CASEINDEX};
%gray = rgb2gray(img);
%texture_features(CASE_INDEX)={SGLDMeasurement(gray)};
%Texturefeats=SGLDMeasurement(img); % <----------------keep this line

% img2 = rgb2hsv(img);
% lum = img2(:,:,3);
%texture_features(CASE_INDEX)={SGLDMeasurement(lum)};

% alternatively 13 haralick
addpath(genpath('Z:\Datasets\SatishData\haralick'))

% file_dir: directory where all the images are
% regionlink: a struct which gives you the image names and regions
% CASE_INDEX: will inspect the instance of choice
%[bounds,img] = plotglands(file_dir,regionlink, CASE_INDEX);
regionlink.aperioID{1} = string(1:5);
regionlink.region{1} = string(end);
[bounds,img] = plotglands([homedir img_directory],regionlink, 1,'suppress_image');
%[bounds,img] = plotglands([homedir img_directory],regionlink, 1);

info.dist = 1;
n = 0;

gimg = rgb2gray(img);
mask(:,:,1) = (gimg ~= max(max(gimg)));
%for grays = [64,128,256]
%    for win = [1,2,3]
%        n = n + 1
info.grays = 256;
grays = info.grays;
info.win = 1;
win = info.win;
dist = info.dist;

        himg = uint16(rescale_range(gimg,0,grays-1));
        %himg = rescale_range(gimg,0,grays-1);
        f = haralick(himg,mask,grays,win,dist,1);
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

index = regexp(string,'\');
if ~isempty(index)
    imgfilename = string(1:index(1)-1);
    regionname = string(index(1)+1:end);
end

filename = [imgfilename regionname];

if ~exist([homedir '/MassSpec/TextureFeats'],'dir')
    mkdir([homedir '/MassSpec/TextureFeats'])
end

save([homedir '/MassSpec/TextureFeats/' filename '_texturefeats.mat'],'Texturefeats','Hf','img_mask','info');