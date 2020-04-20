% compile data

load labelcorrespondence-7-14-2012.mat % contains, labels, links

if isunix
    homedir = ['~\Datasets\UPENNHistoMassSpec'];
else
    homedir = ['Z:\Datasets\UPENNHistoMassSpec'];
end
addpath(genpath([homedir '/morphtools']))

%img_directory = '../progressor - bounds from James/';
data_directory = '\MassSpec/MorphFeats';
string = getAllFilenames([homedir data_directory], '.mat');

load 19points-corrected-7-4-2012.mat

for i = 1:length(string)

stop = regexp(string{i},'_');
imageID = string{i};
imageID = imageID(1:stop-1);
    

index = find([link.aperioID{:}] == str2num(imageID));


if ~isempty(index)
    load([homedir '/MassSpec/MorphFeats/' num2str(imageID) '_morphfeats.mat'],'allfeats');
    load([homedir '/MassSpec/GraphFeats/' num2str(imageID) '_graphfeats.mat'],'feats');
    
    compiled_morph(index,:) = allfeats;
    compiled_graph(index,:) = feats;
else
    fprintf(['\n No match for ImageID: ' num2str(imageID)])
end

end

rawdata{1} = data;
rawdata{2} = compiled_morph;
rawdata{3} = compiled_graph;

save('20points-annotated-7-14-2012.mat','link','rawdata');