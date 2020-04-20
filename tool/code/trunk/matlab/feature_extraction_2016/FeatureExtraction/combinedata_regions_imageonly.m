% compile data

load('labelcorrespondence-7-18-2012.mat') % contains, labels, links
%load('20points-annotated-7-14-2012.mat','link','rawdata');


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
name{i} = temp(1:12); %hopefully the aperioID and number of annotations dont' change
end

for i = 1:length(string2)
temp = string2{i};
name2{i} = temp(1:12);
end

names = intersect(name,name2)

load 56points-imgregions-7-18-2012
%load 19points-corrected-7-4-2012.mat
protein= rawdata{1};

load 56points-biopsy_regions

for i = 1:length(names)

    biopsy_flag = ismember(names{i}, biopsy_regions)
    
    string = names{i};
stop = regexp(string,'r');
imageID = string;
imageID = imageID(1:stop-1);

regionID = string(end);
    
[~,index] = ismember(imageID, link.aperioID);

%casestudy = link.casenumber(index);

% these next couple lines may be useful for establishing correspondence to
% the case study
% [~,case_index] = unique(link.casenumber);
% new_index = link.casenumber(case_index);

%[~,whichlabel] = ismember(casestudy, new_index);


if ~isempty(index)
    load([homedir '/MassSpec/MorphFeats/' num2str(names{i}) '_morphfeats.mat'],'allfeats');
    load([homedir '/MassSpec/GraphFeats/' num2str(names{i}) '_graphfeats.mat'],'feats');
    
    %compiled_protein(i,:) = protein(index,:);
    compiled_morph(i,:) = allfeats;
    compiled_graph(i,:) = feats;
    if biopsy_flag == 1
    compiled_labels(i) = strcmp(link.status(index),'NF')+3;
    else
    compiled_labels(i) = strcmp(link.status(index),'NF')+1;
    end
    
    compiled_casenumber(i) = link.casenumber(index);
    compiled_aperioID(i) = link.aperioID(index);
    compiled_regionID(i) = {regionID};
    compiled_status(i) = link.status(index);
    compiled_biopsy_flag(i) = double(biopsy_flag);

else
    fprintf(['\n No match for ImageID: ' num2str(imageID)])
end

end

%rawdata{1} = data;
rawdata{2} = compiled_morph;
rawdata{3} = compiled_graph;
labels = compiled_labels;

regionlink.casenumber = compiled_casenumber;
regionlink.aperioID = compiled_aperioID;
regionlink.region = compiled_regionID;
regionlink.status = compiled_status;
regionlink.biopsy_flag = compiled_biopsy_flag;

save('56points-removedholes-7-26-2012.mat','link','rawdata','labels','regionlink');