function allmax = minall(I)
% MINALL Minimum value in N-dimensional array.
%   MINALL(I) returns the largest value in the array I.
%
%   For example:
%   MinGreenVal = minall(Irgb(:,:,2));
%
%   Another example:
%   MinDims2and5 = minall(data(:,[2 5]));
%
%   See also STRAIGHTEN, MAXALL, MEANALL, ALLL
%
%JC

allmax = min(I(:));
