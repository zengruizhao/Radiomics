% this script is used to check which image is missing
%
load TMALayouts.mat
strTMAs='/Users/chenglu/Documents/Dataset/WUSTL_Oral_Histology/Training20/';

dirTrain=dir(strTMAs);
dirTrain(1:2)=[];

set_trainNon=[9 42 47 65 69 98 101 110 117 118 7 36 40 48 76 83 82 81 92 106];
set_trainPro=[4 35 43 45 56 58 63 75 94 115 19 22 26 38 49 61 70 90 109 114];
set_train=[set_trainNon set_trainPro];

set_remain=set_train;
for i=1:length(dirTrain)
    strT=dirTrain(i).name;
    set_remain=setdiff(set_remain,str2num(strT(end-6:end-4)));
end

display('missing file index:');
set_remain