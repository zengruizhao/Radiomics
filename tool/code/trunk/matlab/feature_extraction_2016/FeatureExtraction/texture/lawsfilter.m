function lawsresponses = lawsfilter(I)
% LAWSFILTER Apply 2D Law's filters to an image.
%
%   LAWSRESPONSES = LAWSFILTER(I) returns in LAWSRESPONSES a N-by-M-by-25 
%       array containing the 25 filter responses from the laws kernels.
%
%   See also: lawskerns, imfilter.
%
%JCC

[nrows, ncols, highdims]=size(I);
if highdims>1, error('2D grayscale images only.'); end

KK=lawskerns;
nkerns=size(KK,3);

% If imfilter is available, use it, otherwise use filter2
if exist('imfilter','file'),
    filterfun=@(X,h) imfilter(X,h);
else
    filterfun=@(X,h) filter2(h,X);
end

% Calculate filter responses
lawsresponses=zeros([nrows ncols nkerns]);
for i=1:nkerns,
    lawsresponses(:,:,i)=filterfun(I,KK(:,:,i));
end
