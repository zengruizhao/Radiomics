function extract_graph_features
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
for i = [1:length(string)-1]
    fprintf(['\n' num2str(i)])
%    load(['../progressor - bounds from James/' string{i} '/feature_bounds4.mat'])
    load([homedir img_directory filesep string{i} '/feature_bounds4.mat'])

    
    gb_r = [bounds.centroid_r];
    gb_c = [bounds.centroid_c];
    

      
        [feats] = get_graph_features([gb_r]',[gb_c]');
        %[vfeature] = graphfeats3([gb_r]',[gb_c]')
          
    save([homedir '/MassSpec/GraphFeats/' string{i} '_graphfeats.mat'],'feats');     
end

% grab = string(isnan(link{1})==0);
% for this = 1:length(grab)
%     feats
% end
