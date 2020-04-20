function [H,a,p]=entropy(X,N)
% ENTROPY Entropy of random variable X.
%   H = ENTROPY(X,N) returns entropy *estimate* H using a histogram with N
%   gray levels.
%   
%   [H,A,P] = ENTROPY(X,N) also returns the histogram A and the joint pdf
%   P, both of size N-by-1.
%
%   Interpretation
%   --------------
%   "Amount of uncertainty associated with X. Specifically, the number of
%   encoding units per sample required to encoded X." -JC
%   The uncertaintly of X can be viewed as the amount of information 
%   contained by X. Hence entropy is directly proportional to the
%   information content of X. The encoding units can be bits or nats,
%   depending on the logorithm base (2 or e) used in the formulation.
%
%   Example
%   -------
%       N=2^8; x=round((N-1)*rand(1,10000));  % x is 8-bit uniform dist
%       H=entropy(x,N)   % H is about 8 (log2(256) equals 8)
%
%   Notes
%   -----
%   The MATLAB Image Processing Toolbox also contains a function called
%   ENTROPY, however this only works for images with certain data types and
%   ranges.  To avoid using that MATLAB function, be sure to do an ADDPATH
%   for this function instead.
%
%   See also HISTOGRAM2, MI, HISTOGRAM.
%
%JC

% Calculating joint histogram for two images
a=histogram(X,N); 

% Normalize histogram to get PDF
p=a./numel(X);

% Calculate entropy for nonzero values of PDF
H=sum(-(p(p>0).*(log2(p(p>0)))));
