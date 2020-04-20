function [lawsresponses, K2names] = lawsfilter(I,ws)
% LAWSFILTER Apply 2D Law's filters to an image.
%
%   LAWSRESPONSES = LAWSFILTER(I,OPTIONS) returns in LAWSRESPONSES a M-by-M-by-K 
%       array containing filter responses from volume I due convolution 
%       with the the laws kernels.
%       
%       OPTIONS:
%           WS: window size of kernels. Either 3 or 5 (default:5)
%           
%    [LAWSRESPONSES K2NAMES] = LAWSFILTER2(I,OPTIONS) additionally returns
%       the  names of kernel sets from multiplying permutations of 1D kernels
%   See also: lawskerns, imfilter.
%
%JCC

[nrows, ncols, highdims]=size(I);
if highdims>1, error('2D grayscale images only.'); end

if nargin==2
    [K2, K2names]=lawskerns(ws);
else
    [K2, K2names] =lawskerns;
end
nkerns=size(K2,3);

% If imfilter is available, use it, otherwise use filter2
if exist('imfilter','file'),
    filterfun=@(X,h) imfilter(X,h);
else
    filterfun=@(X,h) filter2(h,X);
end

% Calculate filter responses
lawsresponses=zeros([nrows ncols nkerns]);
for i=1:nkerns,
    lawsresponses(:,:,i)=filterfun(I,K2(:,:,i));
end
