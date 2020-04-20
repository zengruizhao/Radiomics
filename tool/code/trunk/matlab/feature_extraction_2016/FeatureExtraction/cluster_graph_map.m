function cluster_graph_map(bounds,edgewidth,nodewidth,scale)
if nargin < 2
    edgewidth = 2;
end
if nargin < 3
    nodewidth = 3;
end
if nargin < 4
    scale = 1;
end

[VY,VX,x,y,edges,params] = construct_ccgs(bounds, 0.5,0.2);
hold on
plot(VY'.*scale,VX'.*scale,'g-','linewidth',edgewidth)
plot(x.*scale,y.*scale,'sk','MarkerSize',nodewidth,'MarkerFaceColor','r');


function [VX,VY,x,y,edges,params] = construct_ccgs(bounds,alpha,r)

if nargin < 2,
    alpha = 0.5;
end
if nargin < 3
    %r = rand(1);
    r = 0.2;
end

X = [bounds(:).centroid_r; bounds(:).centroid_c];

%distance matrix
D = pdist(X','euclidean');

D = squareform(D);
% probability matrix
P = D.^-alpha;

%define edges
edges = zeros(size(D));
z=1;
for i = 1:length(D)
    for j = i:length(D)
        if r < P(i,j)
            edges(i,j) = 1; %
            VX(z,:) = [bounds(i).centroid_r; bounds(j).centroid_r];
            VY(z,:) = [bounds(i).centroid_c; bounds(j).centroid_c];
             x(z) = bounds(i).centroid_r;
             y(z) = bounds(i).centroid_c;
            z=z+1;
        end
    end
end

params.r = r;
params.alpha = alpha;