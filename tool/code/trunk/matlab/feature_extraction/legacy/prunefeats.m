function featslice = prunefeats(featdata,mask,maxfeats)
% function FEATSLICE = prunefeats(FEATDATA,MASK,MAXFEATS)
% Prunes feature set to features which share the least amount of MI
% At least 300 features returned
% DATA is 3D - X by Y by NFeats
% MASK is 2D - X by Y
% MAXFEATS is how many features you want retained
% FEATSLICE is a struct, with data for only pixels in the mask with 
% pruned features(PRUNED), indices (INDS), features kept (FNOS)
% Uses XMI.m and associated functions
% Satish Viswanath, Aug 2008

% Checks
if size(featdata,3)==1
    error('3D feature data goes into DATA.');
elseif size(mask,3) > 1
    error('MASK is supposed to be 2D.');
end

% Required m files
addpath('~satish/Code/matlabcode/general/');
addpath('~jon/code/register/');

[nx,ny,nf] = size(featdata);
inds = find(mask);
    
% Reshape, throw out background
featdata = reshape(featdata,nx*ny,nf);
featdata = featdata(inds,:);

% Rescale features between 0-128 to calculate MI
datarange=range(featdata,1);
%numvals = median(datarange);
%[foo,maxrange] = min(abs((2.^(1:15))-numvals));
maxrange = 2^7-1;minrange = 0;    
featdata_new = round(minrange + (featdata - repmat(min(featdata,[],1),size(featdata,1),1))./repmat((datarange/maxrange-minrange),size(featdata,1),1));
[xmimat,xnmimat,xeccmat] = xmi(featdata_new,maxrange+1);

% Pruning based on images with max entropy and min MI
[~,featnos] = sort(diag(xmimat),'descend'); 
wanted = [];prevunwanted = [];
while length(wanted)<=maxfeats && (length(wanted)+length(featnos))>maxfeats+20
    wanted = [wanted,featnos(1)];
    [~,featsortmi] = sort(xnmimat(featnos(1),:),'descend');
    [~,unwanted] = intersect(featsortmi,prevunwanted);
    featsortmi(unwanted) = [];
    prevunwanted = [prevunwanted,featsortmi(1:20)];
    [~,unwanted] = intersect(featnos,prevunwanted);
    featnos(unwanted) = [];
end
if size(wanted,1) ~= size(featnos,1)
	wanted = wanted';
end
if size(wanted,1) ==1 && size(featnos,1)==1
	wantedfeats = sort([wanted,featnos]);
else
	wantedfeats = sort([wanted;featnos]);
end

% Save
featslice.pruned = single(featdata(:,wantedfeats));
featslice.inds = single(inds);
featslice.fnos = single(wantedfeats);

end

    
    
