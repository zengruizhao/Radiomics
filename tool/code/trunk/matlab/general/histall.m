function h=histall(thing,nbins)
% HISTALL Minimum value in N-dimensional array.
%   HISTALL(I) returns the largest value in the array I.
%
%   For example, a histogram of the green values in an RGB image:
%   xrgb = double(imread('ngc6543a.jpg'));
%   histall(xrgb(:,:,2))
%
%   H = HISTALL(I) returns the bin counts.
%
%   See also STRAIGHTEN, MAXALL, MINALL, ALLL
%
%JC

if nargout==1,
    if nargin==2,
        h=hist(thing(:),nbins);
    else
        h=hist(thing(:));
    end
else
    if nargin==2,
        hist(thing(:),nbins);
    else
        hist(thing(:));
    end
end
