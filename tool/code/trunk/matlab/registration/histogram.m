function h=histogram(I,Max,Bin)
% HISTOGRAM Generate histogram of random variable I.
%   HISTOGRAM(I,N) returns the histogram of arrays I. Histogram is of size 
%   N-by-1. See important notes below.
%
%   Notes
%   -----
%   NO rounding is performed. Errors will be thrown if non-integral valued
%   data is input. This is done for speed.
%
%   See also COUNT, MI, ENTROPY, HISTOGRAM2.
%
%JC 

% Make sure data is in range
if max(I(:))>(Max) || min(I(:))<0,
    error('Data out of range.\n');
end

% Must be double precison for data+1 to work
if ~isa(I,'double'),
    I=double(I);
end

% Generate histogram
h=zeros(Bin,1);
width = Max/Bin;
% I=I+1;
for i=1:numel(I),
    h(ceil(I(i)/width))=h(ceil(I(i)/width)) + 1;
end
