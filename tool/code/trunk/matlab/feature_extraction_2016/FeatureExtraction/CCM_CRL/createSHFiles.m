%% try to create many sh files automately, this is used for create many sh that used to commit job
% on server
%% parameters
intGap=10;% how many need to take in each sh
intSize=68; % total problem size

% intGap=5;% how many need to take in each sh
% intSize=50; % total problem size
intSH=ceil(intSize/intGap);%number of tasks
if intSH>15
    disp('The maximum number of liscenses may be exceed!!');
end

%% doer
% strFun='GetFullFeature_NonPro';
strFun='nucleiseg_nucleiclusterfinding_extractfullfeature_OroCavity';
strFun='nucleiseg_nucleiclusterfinding_extractfullfeature_OroCavity_v2';
strFun='nucleiseg_nucleiclusterfinding_extractfullfeature_OroCavity_v3';
strFun='nucleiseg_nucleiclusterfinding_extractfullfeature_OroCavity_v4';
% strFun='nucleiseg_nucleiclusterfinding_extractfullfeature_OroCavity_v5';
% strFun='nucleiseg_nucleiclusterfinding_extractHosoyafeature_OroCavity';
% strFun='cellclustergraph_construction_OroCavity';
allSHName=[];
fidAll=fopen('AllName_fullfeature.txt','w');% use for copy and past to commit the job on server
for i=1:intSH
    strFile=sprintf('o%d.sh',i);
    
    fprintf(fidAll,sprintf('qsub /mnt/Data7/cxl884/%s\n',strFile));
    
    fid=fopen(strFile,'w');
    
    fprintf(fid,'#!/bin/bash\n');
    fprintf(fid,'#PBS -l walltime=30:00:00\n');
    fprintf(fid,'#PBS -l nodes=1:ppn=8,mem=60gb\n');
    fprintf(fid,'#PBS -M hacylu@yahoo.com\n');
    
    strOut=sprintf('#PBS -o "o%d.txt"\n',i);
    fprintf(fid,strOut);
    
    fprintf(fid,'#PBS -m e\n');
    fprintf(fid,'#PBS -j oe\n');
    fprintf(fid,'module load matlab\n');
    fprintf(fid,'cd /mnt/Data7/cxl884/Code/\n'); 
    
    str=sprintf('matlab -nodisplay -r "%s(%d,%d)"\n',strFun,(i-1)*intGap+1,i*intGap);
    fprintf(fid,str);    
    fclose(fid);
end
fclose(fidAll);