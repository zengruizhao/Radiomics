% show image 
% clear all
strIMPath='C:\Dataset\WUSTL_Oral_Histology\Training50\';

dirIM=dir(strIMPath);
dirIM(1:2)=[];
%% The label information
set_trainNon=[9 42 47 65 69 98 101 110 117 118 7 36 40 48 76 83 82 81 92 106];
set_trainPro=[4 35 43 45 56 58 63 75 94 115 19 22 26 38 49 61 70 90 109 114];
set_train=[set_trainNon set_trainPro];

%% gather features on Pro/non-Por cases, one row for one case, one column for one feature
for i=1:length(dirIM)
    strC=dirIM(i).name;
    
    strCur=sprintf('%s%s',strIMPath,dirIM(i).name);
    I=imread(strCur);% show(I);
    
    intIMIdx=str2num(strC(end-6:end-3));

    %% display nuclei and nuclei area
    show(I,1);hold on;
    text(100,100,sprintf('IM:%s',strC));
    hold off; pause();
end
