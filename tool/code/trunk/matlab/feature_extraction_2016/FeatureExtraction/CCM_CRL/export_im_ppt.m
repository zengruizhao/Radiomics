addpath('/Users/chenglu/Nutstore/PathImAnalysis_Program/Program/Miscellaneous/exportToPPTX');

% show image with CCG on it
% clear all
strIMPath='/Users/chenglu/Documents/Dataset/WUSTL_Oral_Histology/Training40/';
strPath='/Users/chenglu/Documents/Dataset/WUSTL_Oral_Histology/Training40_features/';

dirIM=dir(strIMPath);
dirIM(1:3)=[];

%% The label information
set_trainNon=[9 42 47 65 69 98 101 110 117 118 7 36 40 48 76 83 82 81 92 106];
set_trainPro=[4 35 43 45 56 58 63 75 94 115 19 22 26 38 49 61 70 90 109 114];
set_train=[set_trainNon set_trainPro];

%%
exportToPPTX('new');
for i=1:length(dirIM)
    strC=dirIM(i).name;

    strCur=sprintf('%s%s',strIMPath,dirIM(i).name);
    I=imread(strCur);% show(I);
    
    strSaveCur=sprintf('%s%s_nuclei.mat',strPath,dirIM(i).name);
    load(strSaveCur);
    
    intIMIdx=str2num(strC(end-6:end-3));

    if ismember(intIMIdx,set_trainPro)
        labels(i)=1;
    else
        labels(i)=0;
    end
    
    %     fprintf('On the %dth TMA...\n',intIMIdx);
    if labels(i)==1
        strlab='Progressor';
    else
        strlab='Nonprogressor';
    end
    %% display nuclei and nuclei area
        show(I,1);hold on;
        for k = 1:length(nuclei)
            plot(nuclei{k}(:,2), nuclei{k}(:,1), 'g-', 'LineWidth', 1);
        end
        text(100,220,sprintf('%s',strlab));
        hold off;
    
        exportToPPTX('addslide');
        exportToPPTX('addpicture',gcf);
    
        exportToPPTX('addtext',sprintf('%s. filename: %s \n Image size: %dx%d\n',...
            strlab,dirIM(i).name,size(I,1),size(I,2)),'Position',[0 0 5 5]);
    
end
% newFile = exportToPPTX('saveandclose','CCGonIM_nonPro');
newFile = exportToPPTX('saveandclose','OralCavity_nuclei_on_image');