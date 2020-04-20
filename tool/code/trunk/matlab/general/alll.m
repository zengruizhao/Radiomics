function alleq = alll(Ibool)
% ALLL Boolean ALL over all dimensions in an N-dimensional array.
%   ALLL(I) checks if all elements of I on all dimensions are true (or 1).
%
%   For example:
%   boolmat=[1 1; 1 1];
%   columnall=all(boolmat)
%   fullall=alll(boolbat)
%
%   See also STRAIGHTEN, ALLL.
%
%JC

alleq = all(Ibool(:));
