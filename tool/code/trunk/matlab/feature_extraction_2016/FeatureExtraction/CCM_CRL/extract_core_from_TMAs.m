% this script is used to extact the cores from TMAs
%
load TMALayouts.mat
strTMAs='/Users/chenglu/Documents/Dataset/WUSTL_Oral_Histology/Training20/';
% Non-Progressors
set_trainNon=[9 42 47 65 69 98 101 110 117 118 7 36 40 48 76 83 82 81 92 106];
set_trainPro=[4 35 43 45 56 58 63 75 94 115 19 22 26 38 49 61 70 90 109 114];

dirTrain=dir(strTMAs);
dirTrain(1:2)=[];

set_trainNon=[9 42 47 65 69 98 101 110 117 118 7 36 40 48 76 83 82 81 92 106];
set_trainPro=[4 35 43 45 56 58 63 75 94 115 19 22 26 38 49 61 70 90 109 114];
set_Train=[set_trainNon set_trainPro];


for i=1:6
    str=sprintf('%sTMA %d.svs',strTMAs,i);
    I=imread(str); %t=iminfo(str);
end

T=imread(sprintf('%sTMA 1_007.svs',strTMAs));