function feat_extract_regions

dataset = [];
if isunix
    homedir = ['~/Datasets/UPENNHistoMassSpec'];
else
    homedir = ['Z:\Datasets\UPENNHistoMassSpec'];
end

addpath(genpath([homedir]))

cd([homedir filesep 'Development/code/lcib_1.0/George'])
%load Datasetparameters_7_13_2012.mat
load 24control_regions_1_8_2013.mat
dataset = unique(regionlink.aperioID)
cd([homedir '/MassSpec'])

%img_directory = '../progressor - bounds from James/';
img_directory = '\Development\results';

% extract features from regions
%matlabpool(2)
parfor i = 1:length(dataset)
    regionfolder = {};
    for r = 1:50
        if exist([homedir img_directory filesep dataset{i} filesep 'region' num2str(r)],'dir') ~= 0
            regionfolder{r} = [dataset{i} '\' 'region' num2str(r)];
            try
                fprintf(['\n Image' num2str(dataset{i}) ' , Region ' num2str(r)])
                fprintf('\n Extracing Morph Features...')
                extract_morph_feats_regions(homedir, img_directory, regionfolder{r});
                fprintf('\n Extracing Graph Features...')
                extract_graph_feats_regions(homedir, img_directory, regionfolder{r});
                fprintf('\n Extracing CGT Features...')
                extract_CGT_feats_regions(homedir, img_directory, regionfolder{r});
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
    
           %%----------------auto remove holes ------------------------
    
[gb_r,gb_c]= autoremoveholes(bounds,gb_r,gb_c);

                %%-------------------------------------
    
    [feats] = get_graph_features([gb_r]',[gb_c]');
    %[vfeature] = graphfeats3([gb_r]',[gb_c]')
    
index = regexp(string,'\');
if ~isempty(index)
    imgfilename = string(1:index(1)-1);
    regionname = string(index(1)+1:end);
end

filename = [imgfilename regionname];

    save([homedir '/MassSpec/GraphFeats/' filename '_graphfeats.mat'],'feats');


function extract_morph_feats_regions(homedir, img_directory, string)
%% Morph
addpath(genpath([homedir]))

load([homedir img_directory filesep string '/feature_bounds4.mat'])
gb_r = {bounds.r};
gb_c = {bounds.c};

       %%----------------auto remove holes ------------------------
 
       [gb_r,gb_c]= autoremoveholes(bounds,gb_r,gb_c);
       

                %%-------------------------------------

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

if isempty(badglands)
    save([homedir '/MassSpec/MorphFeats/' filename '_morphfeats.mat'],'allfeats');
else
    save([homedir '/MassSpec/MorphFeats/' filename '_morphfeats.mat'],'allfeats','badglands');
end

function [gb_r,gb_c]= autoremoveholes(bounds,gb_r,gb_c)
       
     area = 0.4; AR = 5; comp = 1;
     jlist = [];
     for j = 1:length(bounds)
    if abs(bounds(j).features.area - area) < 0.1 && bounds(j).features.aspect_ratio > (1/AR) && bounds(j).features.aspect_ratio <AR && bounds(j).features.compactness < comp
        jlist = [jlist j];
        fprintf('\n bound resembling biopsy hole marked for removal')
    end
     end
    
        gb_r(jlist) = [];
        gb_c(jlist) = [];

function extract_graph_feats_regions(homedir, img_directory, string)
%% CGT
addpath(genpath([homedir]))

    load([homedir img_directory filesep string '/feature_bounds4.mat'])
        
    [feats] = get_CGT_features(bounds);
    
index = regexp(string,'\');
if ~isempty(index)
    imgfilename = string(1:index(1)-1);
    regionname = string(index(1)+1:end);
end

filename = [imgfilename regionname];

    save([homedir '/MassSpec/CGTs/' filename '_CGTfeats.mat'],'CGTfeats');
