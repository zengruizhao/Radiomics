%% note in many places we need uncomment/comment to generate diff results
% clear all
addpath(genpath('/Users/chenglu/Nutstore/PathImAnalysis_Program/Program/LClassifier/ClassificationPackage_CCIPD2009'));
addpath(genpath('/Users/chenglu/Nutstore/Repository/matlab/matlab/feature_extraction'));
addpath(genpath('/Users/chenglu/Nutstore/Repository/George-newbranch'));
addpath(genpath('/Users/chenglu/Nutstore/Repository/George-newbranch/george_code/newbranch/tools/FeatureSelectionTools'));
addpath(genpath('/Users/chenglu/Nutstore/Repository/George-newbranch/george_code/newbranch/tools/AnalysisTools'));
addpath(genpath('C:\Nutstore\Nutstore\Repository\George-newbranch\george_code\newbranch\tools'));
addpath(genpath('/Users/chenglu/Nutstore/PathImAnalysis_Program/Program/OralCavity/'));

%%
if ismac()

else   
    strIMPath='C:\Dataset\WUSTL_Oral_Histology\testing68\';
    strPath='C:\Dataset\WUSTL_Oral_Histology\testing68_features\';
    
    dirIM=dir(strIMPath);
    dirIM(1:2)=[];
end

% dirIM=dir(strIMPath);
% dirIM(1:2)=[];
bandWidth=30;
%% gather features on Pro/non-Por cases, one row for one case, one column for one feature, for epi/stroma separated data
idx=1;
data_epi=[];%zeros(length(dirPro_Full),216);
data_stroma=[];%zeros(length(dirPro_Full),216);

labels=[];
patientIDs=[];
%%% gather 'full features'
for i=1:length(dirIM)
    strC=dirIM(i).name;
    intIMIdx=str2num(strC(end-6:end-3));
    %     strIM=sprintf('%sPro_%d.mat',strPath_Pro,intIMIdx);
    %     load(strIM);
    %     strNuclei=sprintf('%sPro_%d.mat_nuclei.mat',strPath_Pro,intIMIdx);
    %     load(strNuclei);
    load(sprintf('%sFullFeatures_IM%s_epistroma.mat',strPath,strC));
    
    data_epi(idx,:)=cell2mat(feats_epi);
    data_stroma(idx,:)=cell2mat(feats_stroma);idx=idx+1;
    
    patientIDs(i)=intIMIdx;    
end

idx=1;feature_list_epi=[];feature_list_stroma=[];
for i=1:length(description_epi)
    curC=description_epi{i};
    for j=1:length(curC)
        feature_list_epi{idx}=['epi-' curC{j}]; 
        feature_list_stroma{idx}=['stroma-' curC{j}];     
        idx=idx+1;
    end
end

%%% gather haralick features
idxh=1;
data_haralick_epi=[];data_haralick_stroma=[];

for i=1:length(dirIM)
    strC=dirIM(i).name;
    str=sprintf('%sHaralickFeatures_IM%s_epistroma.mat',strPath,dirIM(i).name);
    load(str);
    data_haralick_epi(idxh,:)=cell2mat(feats_epi);
    data_haralick_stroma(idxh,:)=cell2mat(feats_stroma);
    idxh=idxh+1;
end
data_epi=cat(2,data_epi,data_haralick_epi);
data_stroma=cat(2,data_stroma,data_haralick_stroma);

curC=description_epi{1};
for j=1:length(curC)
    feature_list_epi{idx}=['epi-' curC{j}];
    feature_list_stroma{idx}=['stroma-' curC{j}];  
    idx=idx+1;
end

%%% gather CCM features
idxc=1;
data_ccm_epi=[];data_ccm_stroma=[];

for i=1:length(dirIM)
    strC=dirIM(i).name;
    str=sprintf('%sCCMFeatures_IM%s_epistroma.mat',strPath,dirIM(i).name);
    load(str);
    data_ccm_epi(idxc,:)=feats_epi;
    data_ccm_stroma(idxc,:)=feats_stroma;
    idxc=idxc+1;
end
data_epi=cat(2,data_epi,data_ccm_epi);
data_stroma=cat(2,data_stroma,data_ccm_stroma);

curC=description_epi;
for j=1:length(curC)
    feature_list_epi{idx}=['epi-' curC{j}];
    feature_list_stroma{idx}=['stroma-' curC{j}];  
    idx=idx+1;
end

%%% combine epi and stroma features into one
feature_list_epistroma=cat(2,feature_list_epi,feature_list_stroma);
data_epistroma=cat(2,data_epi,data_stroma);
save('OralCavityFullFeatures_epistroma_testing68.mat','data_epistroma','feature_list_epistroma');
%% gather features on Pro/non-Por cases, one row for one case, one column for one feature, for mixture of epi/stroma data
idx=1;
data=[];%zeros(length(dirPro_Full),216);
labels=[];
%%% gather 'full features'
for i=1:length(dirIM)
    strC=dirIM(i).name;
    intIMIdx=str2num(strC(end-6:end-3));
    %     strIM=sprintf('%sPro_%d.mat',strPath_Pro,intIMIdx);
    %     load(strIM);
    %     strNuclei=sprintf('%sPro_%d.mat_nuclei.mat',strPath_Pro,intIMIdx);
    %     load(strNuclei);
    load(sprintf('%sFullFeatures_IM%s.mat',strPath,strC));
    data(idx,:)=cell2mat(feats);idx=idx+1;
end

idx=1;feature_list=[];
for i=1:length(description)
    curC=description{i};
    for j=1:length(curC)
        feature_list{idx}=curC{j}; idx=idx+1;
    end
end

%%% gather haralick features
idxh=1;
data_haralick=[];
for i=1:length(dirIM)
    strC=dirIM(i).name;
    intIMIdx=str2num(strC(end-6:end-3));
    %     strIM=sprintf('%sPro_%d.mat',strPath_Pro,intIMIdx);
    %     load(strIM);
    %     strNuclei=sprintf('%sPro_%d.mat_nuclei.mat',strPath_Pro,intIMIdx);
    %     load(strNuclei);
    str=sprintf('%sHaralickFeatures_IM%s.mat',strPath,dirIM(i).name);
    load(str);
    data_haralick(idxh,:)=cell2mat(feats);idxh=idxh+1;
end
data=cat(2,data,data_haralick);

curC=description{1};
for j=1:length(curC)
    feature_list{idx}=curC{j}; idx=idx+1;
end

%%% gather CCM features

idxc=1;
data_ccm=[];

for i=1:length(dirIM)
    strC=dirIM(i).name;
    str=sprintf('%sCCMFeatures_IM%s.mat',strPath,dirIM(i).name);
    load(str);
    data_ccm(idxc,:)=feats;
    idxc=idxc+1;
end
data=cat(2,data,data_ccm);

curC=description;
for j=1:length(curC)
    feature_list{idx}=curC{j};
    idx=idx+1;
end
save('OralCavityFullFeatures_testing68.mat','data','feature_list');