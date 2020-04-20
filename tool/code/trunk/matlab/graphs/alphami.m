function [ami,knndists1,knndists2,knndists12] = alphami(X,Y,varargin)
% ALPHAMI Calculate the alpha-MI between two multivariate observations.
%
%   ALPHAMI(X,Y,ALPHA,K) calculates the alpha mutual information between X
%   and Y. The kNN graph, the length of which is used to infer entropy, is 
%   generated using K neighbors. The value ALPHA affects the bias of aMI 
%   toward less or more common observations, where aMI converges to Shannon
%   MI as alpha->1.
%
%   References
%   ----------
%   Huzefa Neemuchwala, Alfred Hero, and Paul Carson, Image matching using
%   alpha-entropy measures and entropic graphs, Signal Processing 85 (2005)
%   277-296.
%   E. Oubel, C. Tobon-Gomez, A. Hero, A. Frangi, Myocardial motion
%   estimation in tagged MR sequences by using ?MI-based non rigid
%   registration, MICCAI 2005, 271-278.
%
%   Units are bits (log2 calculations used).
%
%JC

ip=inputParser;
% ip.addRequired('X', @isnumeric);
% ip.addRequired('Y', @isnumeric);
ip.addRequired('alpha', @(x)isnumeric(x) && numel(x)==1);
ip.addRequired('k', @(x)isnumeric(x) && numel(x)==1);
% ip.addParamValue('k', 12, @(x)isnumeric(x) && numel(x)==1);
ip.addParamValue('noise', true, @(x)islogical(x) || x==0 || x==1);
ip.addParamValue('nowarn', false, @(x)islogical(x) || x==0 || x==1);
ip.addParamValue('Xtreedists', [], @(x)isnumeric(x) && ndims(x)==2);
ip.addParamValue('Ytreedists', [], @(x)isnumeric(x) && ndims(x)==2);
ip.addParamValue('knnthreads', 8, @(x)isnumeric(x) && numel(x)==1);
ip.parse(varargin{:});

alpha=ip.Results.alpha;
k=ip.Results.k;
addnoise=ip.Results.noise;
knnthreads=ip.Results.knnthreads;

if alpha==1, error('alpha cannot be 1 (divide by zero).'); end

[nptsX,ndimsX]=size(X);
[nptsY,ndimsY]=size(Y);

if nptsX~=nptsY, error('Number of points in X and Y must be the same.'); end
if ndimsX~=ndimsY, error('Number of dimensions in X and Y must be the same.'); end

% Add some minute noise to data to speed up knn search (we discount it later)
if addnoise,
    noisescale=eps*10;%(1e-5);
    noise=rand(size(X))-.5;
    stdx=std(X(:)); stdy=std(Y(:));
    X=X+noise*(noisescale*stdx); Y=Y+noise*(noisescale*stdy);
end

gamma=ndimsX*(1-alpha); % alpha=(d-gamma)/d=1-gamma/d

% Calculate length
if any(strcmpi('Xtreedists',ip.UsingDefaults)),
    [~,~,knndists1]=knnkdtreemexmt(X,k,1,knnthreads); % len==sum(sum(knndists.^gamma))
    % e1=sum(knndists.^gamma);
    e1=sum(knndists1); % knndists always calculated for gamma=1 by knnkdtreemex
else
    if size(ip.Results.Xtreedists,1)~=k, error('kNN tree not for k=%d\n.',k); end
    if size(ip.Results.Xtreedists,2)~=nptsX, error('kNN tree not for npts=%d\n.',nptsX); end
    e1=sum(ip.Results.Xtreedists);
end

if any(strcmpi('Ytreedists',ip.UsingDefaults)),
    [~,~,knndists2]=knnkdtreemexmt(Y,k,1,knnthreads);
    % e2=sum(knndists.^gamma);
    e2=sum(knndists2);
else
    if size(ip.Results.Ytreedists,1)~=k, error('kNN tree not for k=%d\n.',k); end
    if size(ip.Results.Ytreedists,2)~=nptsY, error('kNN tree not for npts=%d\n.',nptsY); end
    e2=sum(ip.Results.Ytreedists);
end

XY=[X Y];
[~,~,knndists12]=knnkdtreemexmt(XY,k,1,knnthreads);
% e12=sum(knndists.^gamma);
e12=sum(knndists12);

aMIfrac=(e12./sqrt(e1.*e2)).^(2*gamma);

if addnoise,
     zerodists=e1<noisescale*k*stdx | e2<noisescale*k*stdy; % tiny/sqrt(tiny*tiny) == ?!?
else zerodists=e1==0 | e2==0; % 0/0 == NaN
end

arezeros=sum(zerodists); notzeros=nptsX-arezeros;
if arezeros && ~ip.Results.nowarn,
    warning('alphami:kNeighborsIdentical','Distances to kNNs in X and/or Y are 0 for %d points. Try raising k (or live with it).',arezeros); end
% aMIfrac(zerodists)=0;
aMIfrac(zerodists)=sqrt(2)^(2*gamma); % what do we do with points with no non-overlapping neighbors?

ami=1/(alpha-1) * log2(1/(numel(X)^alpha) * sum(aMIfrac));
% ami2=1/(alpha-1) * log2(1/((notzeros*ndimsX)^alpha) * sum(aMIfrac(~zerodists))) % ignore the points with no non-overlapping neighbors
% ami3=1/(alpha-1) * log2(1/(numel(X)^alpha) * sum(aMIfrac(~zerodists)));
