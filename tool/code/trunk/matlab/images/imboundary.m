function edgei=imboundary(I,tempthresh)
% IMBOUNDARY
%   Given an image I, find the largest contiguous edge based on a rough
%   segmentation obtained by I>TEMPTHRESH followed by a morphological
%   closing operation.
%
%   EDGEIM=IMBOUNDARY(I,TEMPTHRESH)
%
%JC

% Threshold image
Itempthresh=I>tempthresh;

% Closing (fill in holes)
% threshclosed=myimclose(Itempthresh,ones(5));
threshclosed=imclose(Itempthresh,strel(ones(5)));

% Edges
[dx,dy]=gradient(double(threshclosed));
edgei=sqrt(dx.^2+dy.^2);
edgei=edgei~=0;
edgei=bwlabel(edgei);

% Get largest contiguous edge
[thecount,countwhat]=count(edgei(edgei>0));
[thecount,countorder]=sort(thecount,'descend');
countwhat=countwhat(countorder);
edgei=edgei==countwhat(1);
