% PRIMMEX_KDTREE Generate minimal spanning tree using Prim's method on a
%   kNN graph constructed using efficient kd-trees.
%
%   [Vx,Vy,E] = PRIMMEX_KDTREE(X,K) returns the node pairs in (Vx, Vy)
%   along with the corresponding edges E. The sparse matrix representation
%   of the MST is then ST = sparse(Vx,Vy,E);. The data X is a double array
%   of size NPTS-by-NDIMS, and K is the k nearest neighbors in the kNN 
%   graph used to calculate the tree. The tree is returned in Vx, Vy, & E,
%   which are each size(X,1)-1. 
%
%   [Vx,Vy,E,LASTEDGE] = PRIMMEX_KDTREE(X,K) also returns the coordinate of
%   the last real edge in (Vx,Vy,E). When all nodes are connected, LASTEDGE
%   will be equal to numel(Vx), or size(X,1)-1. When not all nodes are
%   connected, as is possible with Prim's algorithm, LASTEDGE will indicate
%   the index of the last vertex-edge (Vx,Vy,E) containing and actual edge.
%
%   For example, the following code will likely generate a fully connected
%     tree since k is much larger than ndims (npts is also low),
%   
%       npts=2000; ndims=3; k=ndims*4;
%       X=rand(npts,ndims);
%       [Vx,Vy,E] = primmex_kdtree(X,k);
%       ST = sparse(Vx,Vy,E);
%
%   In this example, the MST may not connect all nodes in X because ndims
%       and npts are higher, while k is relatively low,
%
%       npts=16000; ndims=4; k=ndims*2;
%       X=rand(npts,ndims); tic;
%       [Vx,Vy,E,lastedge] = primmex_kdtree(X,k);
%       lastedge
%       ST = sparse(Vx(1:lastedge),Vy(1:lastedge),E(1:lastedge),npts,npts);
%       numel(nonzeros(ST))==lastedge  % (always true)
%       unconnected_nodes=lastedge - (size(X,1)-1)
%       mst_length = sum(nonzeros(ST)), toc
%
%   Note that when edges are not included, the differnce between LASTEDGE
%   and size(X,1)-1 is usually only a handful of nodes.
%
%JC