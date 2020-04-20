function [VX,VY,x,y,edges,params] = construct_ccgs(bounds,alpha,r,option)
% constructs subgraphs using parameters alpha and r
% option to select ccg edge connection using object boundary or centroid
% eg. (option = 'boundary')

if nargin < 2,
    alpha = 0.5;
end
if nargin < 3
    %r = rand(1);
    r = 0.2;
end

if ~exist('option','var')
    params.dist = 'centroid'; %default uses centroids to establish closeness
else
    params.dist = option;
end

if ismember(params.dist,[{'closestboundary'} {'boundary'} {'bounds'}])
% alternative distance based on boundary distance
    D = min_dist_boundaries(bounds);
    params.dist = 'boundarybased';
    %D = pdist2([bounds(:).r; bounds(:).c]', [bounds(:).r; bounds(:).c]');
else
% default
% based on centroid distance
    X = [bounds(:).centroid_r; bounds(:).centroid_c];

%distance matrix
    D = pdist(X','euclidean');
    
    params.dist = 'centroidbased';
end

% probability matrix
P = D.^-alpha;

%define edges
edges = triu(true(length(bounds)), 1) & squareform(r < P);

% get edge locations
[xx, yy] = find(edges);
VX = [bounds(xx).centroid_r; bounds(yy).centroid_r];
VY = [bounds(xx).centroid_c; bounds(yy).centroid_c];

% get node locations
idx = unique([xx, yy],'rows', 'first');
x = [bounds(idx).centroid_r];
y = [bounds(idx).centroid_c];

params.r = r;
params.alpha = alpha;