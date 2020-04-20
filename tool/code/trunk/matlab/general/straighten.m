function straightened = straighten(thinger)
% STRAIGHTEN Straighten an N-dimensional array.
%   STRAIGHTEN(ARRAY) is an array of dimensions numel(ARRAY)-by-1.
%
%   This is equivalent to reshape(ARRAY,numel(ARRAY),1).
%
%   STRAIGHTEN is more useful than it seems. For instance, you can do:
%       plot(straighten(data(:,:,1)),straighten(data(:,:,2)),'.')
%   instead of:
%       d1=data(:,:,1);
%       d2=data(:,:,2);
%       plot(d1(:),d2(:),'.')
%
%JC

straightened=thinger(:);