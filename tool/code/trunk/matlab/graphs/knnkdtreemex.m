% KNNKDTREEMEX Calculate the k-nearest neighbor graph using kd-trees.
%
%   [GLEN,KNNS,KNNDISTS] = KNNKDTREEMEX(X,K,GAMMA) calculates the overall
%   edge length GLEN of the kNN graph, the nodes of which are returned in
%   KNNS and the corresponding edge weights (distances) in KNNDISTS. Data
%   array X is of size NPTS-by-NDIMS. K is an integral valued scalar
%   indicating the number of neighbors to find, while GAMMA is the exponent
%   if power weighted edges are desired (1 is default).
%
%   The kNN graph outputs, KNNS and KNNDISTS, are both of size K-by-NPTS. A
%   sparse matrix representation may then be generated as shown below,
%
%       npts=4000; ndims=4; k=ndims*3; gamma=1;
%       X=rand(npts,ndims); tic;
%       [len,knns,knndists]=knnkdtreemex(X,k,gamma);
%       ii=1:size(X,1); ii=repmat(ii,k,1); % implicit row inds for neighbors
%       kNNGs=sparse(ii(:),knns(:),knndists(:),npts,npts);
%       sum(nonzeros(kNNGs)) - len      % very small
%
%       kNNGsUG=tril(kNNGs+kNNGs');
%       bidirectional=find(tril(kNNGs & kNNGs'));
%       kNNGsUG(bidirectional)=kNNGs(bidirectional);
%
%       [MST,pred]=graphminspantree(tril(kNNGsUG));
%       mst_length=sum(nonzeros(MST)), toc
%
%   This function does not make a full distance matrix, so you can do this,
%
%       npts=40e3; ndims=6; k=ndims*2; gamma=1;
%       X=rand(npts,ndims); tic;
%       [len,knns,knndists]=knnkdtreemex(X,k,gamma); toc
%
%JC