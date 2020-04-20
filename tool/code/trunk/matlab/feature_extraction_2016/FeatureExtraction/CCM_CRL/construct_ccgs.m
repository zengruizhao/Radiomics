function [VX,VY,x,y,edges,params] = construct_ccgs(nodes,alpha,r)
%Cell Cluster Graph

  %alpha = 0.4;
     %r = rand(1);

disp('Constructing the CCG...');
tic
X = [nodes.centroid_r; nodes.centroid_c];

%distance matrix
D = pdist(X','euclidean');

D = squareform(D);
% probability matrix
P = D.^-alpha;
VX = []; x = [];
VY = []; y = [];
%define edges
edges = zeros(size(D));
z=1;
for i = 1:length(D)-1
    for j = i+1:length(D)
        if r < P(i,j) % if probability is greater than threshold set
            edges(i,j) = 1; %
%             VX(z,:) = [nodes.centroid_r; nodes.centroid_r(j)];
%             VY(z,:) = [nodes.centroid_c(i); nodes.centroid_c(j)];
%              x(z) = nodes.centroid_r(i);
%              y(z) = nodes.centroid_c(i);
              VX(z,:) = [nodes(i).centroid_r; nodes(j).centroid_r];
              VY(z,:) = [nodes(i).centroid_c; nodes(j).centroid_c];
              x(z) = nodes(i).centroid_r;
              y(z) = nodes(i).centroid_c;
            z=z+1;
        end
    end
end

params.r = r;
params.alpha = alpha;
toc