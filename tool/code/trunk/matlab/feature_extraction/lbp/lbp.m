function [Ilbp,lbp_pattern]=lbp(I)
% LBP Calculate a Local Binary Pattern representation of an image.
%
%   ILBP = LBP(I) gives the LBP representation ILBP of the 2D image I.
%
%   See also: lbp>lbpcolfun, colfilt, bsxfun, count.
%
%JCC

if size(I,3)>1, error('Image not 2D grayscale.'); end

% Make weighting kernel
lbvals=2.^(0:7)';       % the weighting values
lbweights=zeros(9,1);   % the 3-by-3 kernel, straightened

% the order of the values around the "ring" of the 3-by-3 kernel
% lborder=[1:3 6 9 8 7 4]';
lborder=[1 4 7 8 9 6 3 2]';

lbweights(lborder)=lbvals;

% The LBP weight pattern as a kernel
lbp_pattern=reshape(lbweights,[3 3]);
% display(lbp_pattern)


% fvals=zeros(1,255);
% for zz=1:255,
%     a = fi(zz,0,8,0);
%     v=zeros(1,8);
%     for roli=1:8,
%         v(roli)=bitrol(a,roli);
%     end
%     fvals(zz)=min(v);
% end
fvals=[1,1,3,1,5,3,7,1,9,5,11,3,13,7,15,1,17,9,19,5,21,11,23,3,25,13,27,...
    7,29,15,31,1,9,17,25,9,37,19,39,5,37,21,43,11,45,23,47,3,19,25,51,...
    13,53,27,55,7,39,29,59,15,61,31,63,1,5,9,13,17,21,25,29,9,37,37,45,...
    19,53,39,61,5,21,37,53,21,85,43,87,11,43,45,91,23,87,47,95,3,11,19,...
    27,25,43,51,59,13,45,53,91,27,91,55,111,7,23,39,55,29,87,59,119,15,...
    47,61,111,31,95,63,127,1,3,5,7,9,11,13,15,17,19,21,23,25,27,29,31,9,...
    25,37,39,37,43,45,47,19,51,53,55,39,59,61,63,5,13,21,29,37,45,53,61,...
    21,53,85,87,43,91,87,95,11,27,43,59,45,91,91,111,23,55,87,119,47,...
    111,95,127,3,7,11,15,19,23,27,31,25,39,43,47,51,55,59,63,13,29,45,...
    61,53,87,91,95,27,59,91,111,55,119,111,127,7,15,23,31,39,47,55,63,...
    29,61,87,95,59,111,119,127,15,31,47,63,61,95,111,127,31,63,95,127,...
    63,127,127,255];

% Calculate LBP image using the kernel (stored straightened in lbweights)
Ilbp_all=colfilt(I,[3 3],'sliding',@lbpcolfun,lbweights);

Ilbp=zeros(size(Ilbp_all));
for zz=1:255,
    Ilbp(Ilbp_all==zz)=fvals(zz);
end

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
