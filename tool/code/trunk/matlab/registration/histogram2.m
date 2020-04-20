function h=histogram2(I1,I2,N)
% HISTOGRAM2
%   HISTOGRAM2(I1,I2,N) returns the joint histogram of arrays I1 and I2 
%   Histogram is of size N-by-N. Values of I1 and I2 must range from 0 to
%   N-1. No rounding or bound checking is performed.
%
%   Notes
%   -----
%   NO rounding is performed. Errors will be thrown if non-integral valued
%   data is input. This is done for speed.
%
%   MORE IMPORTANTLY: No errros will be thrown if the data is not properly
%   scaled from 0 to N-1! Also omitted for speed.
%
%   See also MI, ENTROPY, ENTROPY2, ENTROPYN, HISTOGRAM.
%
%JC 

if any(size(I1)~=size(I2)),
    error('Arrays are different sizes.');
end

% Data must be double precision or data+1 may overflow (fail)
if ~isa(I1,'double') || ~isa(I2,'double'),
    I1=double(I1);
    I2=double(I2);
end

% Build the joint histogram
h=zeros(N,N);
I1=I1+1; I2=I2+1;
for k=1:numel(I1),
	h(I1(k),I2(k)) = h(I1(k),I2(k)) + 1;
end
