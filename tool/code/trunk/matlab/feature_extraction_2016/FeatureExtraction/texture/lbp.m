function [Ilbp,lbp_pattern]=lbp(I,startoffset,ccw)
% LBP Calculate a Local Binary Pattern representation of an image.
%
%   ILBP = LBP(I) gives the LBP representation ILBP of the 2D image I.
%
%   LBP(I,OFFSET) defines the starting point on the "ring" of the
%   3-by-3 weighting kernel for the 8-bit pattern to begin. The following
%   illustration indicates the locations of each possible STARTOFFSET:
%        OFFSET              (OFFSET=0)      (OFFSET=2)      (OFFSET=-1)
%     +-----------+         +-----------+   +-----------+   +-----------+
%     | 0 | 7 | 6 |         | 1 |128|64 |   |64 |32 |16 |   | 2 | 1 |128| 
%     +---+---+---+ Weights +-----------+   +-----------+   +-----------+
%     | 1 |   | 5 |  -->    | 2 |   |32 |   |128|   | 8 |   | 4 |   |64 | 
%     +---+---+---+         +-----------+   +-----------+   +-----------+
%     | 2 | 3 | 4 |         | 4 | 8 |16 |   | 1 | 2 | 4 |   | 8 |16 |32 | 
%     +---+---+---+         +-----------+   +-----------+   +-----------+
%   The default OFFSET is 0;
%
%   LBP(I,OFFSET,CCW) uses a weighting kernel with the 8-bit pattern going 
%   either counter-clockwise (CCW=true) or clockwise (CCW=false). Example,
%    (OFFSET=0,CCW=true)    (OFFSET=0,CCW=false)    (OFFSET=-1,CCW=false)
%       +-----------+           +-----------+           +-----------+
%       | 1 |128|64 |           | 1 | 2 | 4 |           |128| 1 | 2 | 
%       +-----------+           +-----------+           +-----------+
%       | 2 |   |32 |           |128|   | 8 |           |64 |   | 4 | 
%       +-----------+           +-----------+           +-----------+
%       | 4 | 8 |16 |           |64 |32 |16 |           |32 |16 | 8 | 
%       +-----------+           +-----------+           +-----------+
%   The default CCW is true;
%
%   [ILBP,LBP_PATTERN]=LBP(...) also returns the 3-by-3 weighting kernel in
%   LBP_PATTERN.
%
%   See also: lbp>lbpcolfun, colfilt, bsxfun, count.
%
%JCC

% Check inputs, set defaults
if nargin<3,
    ccw=true;
end
if nargin<2 || isempty(startoffset),
    startoffset=0;
end

% Make weighting kernel
lbvals=2.^(0:7)';       % the weighting values
lbweights=zeros(9,1);   % the 3-by-3 kernel, straightened

% the order of the values around the "ring" of the 3-by-3 kernel
if ccw,
    lborder=[1:3 6 9 8 7 4]';
else
    lborder=[1 4 7 8 9 6 3 2]';
    startoffset=-startoffset;
end

% apply the offset and write out the values on the ring of the kernel
% lborder=circshift(lborder,startoffset);
lbvals=circshift(lbvals,startoffset);
lbweights(lborder)=lbvals;

% The LBP weight pattern as a kernel
lbp_pattern=reshape(lbweights,[3 3]);
% display(lbp_pattern)

% Calculate LBP image using the kernel (stored straightened in lbweights)
Ilbp=colfilt(I,[3 3],'sliding',@lbpcolfun,lbweights);

end

function colsum=lbpcolfun(x,lbweights)
% Column-wise function to perform thresholding (>=) w.r.t. neighborhood 
% center pixel, multiplication with the weights, and summing.
%
% For use with colfilt and a 3-by-3 neighborhood and weights kernel.

centerpix=5; % size(x,1)/2;

% colsum=sum((x>=x(5*ones(size(x,1),1),:)) .* lbweights(:,ones(size(x,2),1)));
colsum=sum(bsxfun(@times,bsxfun(@ge,x,x(centerpix,:)),lbweights));

end
