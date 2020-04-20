function [H,nuniqueSA3]=entropyn(Is,N)
% ENTROPYN High order joint entropy estimate.
%   H = ENTROPYN(IS,N) calculates the joint entropy for the vector Is of
%   size M-by-D where M is the number of samples and D is the number of
%   dimensions.
%
%   [H,NUNUNIQUES] = ENTROPYN(...) returns the number of unique elements
%   counted (number of non-zero bins) by H. A completely disperse histogram
%   occurs when NUNIQUEELEMENTS==size(ARRAYN,1).
%
%   See also HISTOGRAMNSP, MI, ENTROPY, HISTOGRAM, HISTOGRAM2.
%
%JC

% [nsamples,D]=size(Is);
nsamples=size(Is,1);
if ndims(Is)>2,
    error('Data vector must be of size NumSamples-by-NumDimensions.');
end

% S(A1A2A3..An)
[h,nuniqueSA3]=histogramnsp(Is,N); 
pD=h./nsamples;
H=-sum(pD.*log2(pD));
