function extract_morph_features

%load labelcorrespondence.mat
if isunix
    homedir = ['~\Datasets\UPENNHistoMassSpec'];
else
    homedir = ['Z:\Datasets\UPENNHistoMassSpec'];
end
addpath(genpath([homedir '/morphtools']))

%img_directory = '../progressor - bounds from James/';
img_directory = '\Development\results';
%a = dir('../progressor - bounds from James'); a = a(3:end); a = a([a.isdir]); string = {a.name};
a = dir([homedir img_directory]); a = a(3:end); a = a([a.isdir]); string = {a.name};


%string =link{1};

%% extract from 22977 and 22980 to replace 2 incorrect images

for i = [1:length(string)-1]
    load([homedir img_directory filesep string{i} '/feature_bounds4.mat'])
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
    
    if isempty(badglands)
    save([homedir '/MassSpec/MorphFeats/' string{i} '_morphfeats.mat'],'allfeats');
    else
    save([homedir '/MassSpec/MorphFeats/' string{i} '_morphfeats.mat'],'allfeats','badglands');
    end
    
end


% grab = string(isnan(link{1})==0);
% for this = 1:length(grab)
%     feats
% end
