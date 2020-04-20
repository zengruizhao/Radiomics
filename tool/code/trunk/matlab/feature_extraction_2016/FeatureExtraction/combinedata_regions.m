% compile data

load labelcorrespondence-7-14-2012.mat % contains, labels, links
load('20points-annotated-7-14-2012.mat','link','rawdata');


if isunix
    homedir = ['~\Datasets\UPENNHistoMassSpec'];
else
    homedir = ['Z:\Datasets\UPENNHistoMassSpec'];
end
addpath(genpath([homedir '/morphtools']))

%img_directory = '../progressor - bounds from James/';
data_directory = '\MassSpec/MorphFeats';
data_directory2 = '\MassSpec/GraphFeats';
string = getAllFilenames([homedir data_directory], '.mat');
string2 = getAllFilenames([homedir data_directory2], '.mat');

for i = 1:length(string)
temp = string{i};
name{i} = temp(1:12);
end

for i = 1:length(string2)
temp = string2{i};
name2{i} = temp(1:12);
end

names = intersect(name,name2)


%load 19points-corrected-7-4-2012.mat
protein= rawdata{1};


for i = 1:length(names)

    string = names{i};
stop = regexp(string{i},'r');
imageID = string{i};
imageID = imageID(1:stop-1);
% 
% stop = regexp(string{i},'_');
% regionID = string{i};
% regionID = regionID(stop-1);
    

index = find([link.aperioID{:}] == str2num(imageID));


if ~isempty(index)
    load([homedir '/MassSpec/MorphFeats/' num2str(names{i}) '_morphfeats.mat'],'allfeats');
    load([homedir '/MassSpec/GraphFeats/' num2str(names{i}) '_graphfeats.mat'],'feats');
    
    compiled_protein(i,:) = protein(index,:);
    compiled_morph(i,:) = allfeats;
    compiled_graph(i,:) = feats;
    compiled_labels(i) = strcmp(link.sample(index),'NF')+1;

else
    fprintf(['\n No match for ImageID: ' num2str(imageID)])
end

end

rawdata{1} = data;
rawdata{2} = compiled_morph;
rawdata{3} = compiled_graph;

save('20points-regions-7-14-2012.mat','link','rawdata');