clear;
addpath \\gleason\satish\Code\matlabcode\general\
testimage = importdata('testimage.png');
load riu2lbpmappings.mat
lbpfeats=[];
for R = [1,2,3]
    for p = 1:length(map)
        fprintf('\n P=%d, R=%d..',map(p).P,R);
        lbptemp = lbp(testimage(:,:,2),R,map(p).P,map(p),'i');
        %size could get reduced due to bitwise shift, therefore pad
        if size(lbptemp,1)~=size(testimage,1),
            lbptemp = double(impad(lbptemp,size(testimage)));
        end
        lbpfeats = cat(3,lbpfeats,lbptemp);
    end
end

dispimg3(lbpfeats);