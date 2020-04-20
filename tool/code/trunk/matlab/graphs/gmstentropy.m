function [H,gmstlength,gmst,mst] = gmstentropy(X,k)
% GMSTENTROY Estimate entropy of multivariate data using GMST graph length.
%
%   H = gmstentropy(X) return entropy H for data X of size N-by-D.
%
%   [H,GMSTLENGTH,GMST] = gmstentropy(X) return the total GMST edge length.
%
%   [H,GMSTLENGTH,GMST] = gmstentropy(X) return the GMST determined from 
%   the geodesic distances.
%
%   [H,GMSTLENGTH,GMST,MST] = gmstentropy(X) return MST determined from the
%   Euclidean distances.
%
%   View the GMST graph by:
%    gmst_undirected=tril(gmst+gmst');
%    view(biograph(gmst_undirected,[],'ShowArrows','off','ShowWeights','on'))
%
%JC

gamma=1;
[npts ndims]=size(X);
d=ndims-1; if d<1, d=1; end

% Distance matrix
D=pdist(X);

Dm=zeros(npts);
Dm(tril(true(npts),-1))=D;
Dm=Dm+Dm';

% Minimum Spanning Tree (MST) on Euclidean distance matrix
[mst,pred]=graphminspantree(sparse(Dm),'method','kruskal');
% [mst,pred]=graphminspantree(sparse(Dm));

% Undirected MST
mst_undirected=tril(mst+mst');

% Geodesic Distantance Matrix
Dmhat=zeros(N);
for i=1:N, Dmhat(i,:)=graphshortestpath(mst_undirected,i,'directed',false); end

% Geodesic Minimum Spanning Tree (GMST)
[gmst,pred]=graphminspantree(sparse(Dmhat),'method','kruskal');

% Undirected GMST
gmst_undirected=tril(gmst+gmst');

% Total GMST edge length
gmstlength=sum(nonzero(gmst_undirected));

% Entropy estimate

load beta_k_NN_estimate beta_hat

H=(d/gamma)*(log(gmstlength/npts^((d-gamma)/d))-log(beta_hat(d-1,k)))*log2(exp(1)); % bits
