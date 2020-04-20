function [edgei,nedgepixels] = mainedge(I)

% Rough segmentation of Itemplate
% Itempthresh=I>tempthresh;

% Morphological closing
% threshclosed=myimclose(I,ones(5));
threshclosed=imclose(I,strel(ones(5)));

% Calculate edges
[dx,dy]=gradient(double(threshclosed));
edgei=sqrt(dx.^2+dy.^2);
edgei=edgei~=0;
for i=1:size(I,3),
    edgeii=bwlabel(edgei(:,:,i));
    
    % Find largest contiguous edge
    [thecount,countwhat]=count(edgeii(edgeii>0));
    [thecount,countorder]=sort(thecount,'descend');
    countwhat=countwhat(countorder);
    edgei(:,:,i)=edgeii==countwhat(1);
    nedgepixels=thecount(1);
end
