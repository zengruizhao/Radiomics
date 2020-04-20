function [H,a,p]=entropy2(X,Y,N)
% ENTROPY2 Joint entropy of random variables X and Y.
%   H = ENTROPY2(X,Y,N) returns Shannon entropy H using a histogram with
%   N-by-N bins.
%
%   [H,A,P] = ENTROPY2(X,Y,N) also returns the joint histogram A and the
%   joint pdf P, both of size N-by-N.
%
%   Interpretation
%   --------------
%   "Total uncertainty in a system of two variables X and Y." -JC
%   If X and Y are related, H(X,Y) will be less than H(X) + H(Y), hence the
%   definitions of MI as H(X)+H(Y)-H(X,y).
%
%   Example
%   -------
%       N=2^8;
%       x=round((N-1)*rand(1,10000));  % x is 8-bit uniform dist
%       Hx=entropy2(x,x,N) % same as entropy(), H is about 8 (log2(256)==8)  
%       % add noise
%       sigma=1; y1=round(x+sigma*randn(1,10000)); y1(y1<0)=0; y1(y1>(N-1))=N-1;
%       sigma=2; y2=round(x+sigma*randn(1,10000)); y2(y2<0)=0; y2(y2>(N-1))=N-1;
%       sigma=16; y6=round(x+sigma*randn(1,10000)); y6(y6<0)=0; y6(y6>(N-1))=N-1;
%       H1=entropy2(x,y1,N)   % H1 is larger than 8
%       H2=entropy2(x,y2,N)   % more total uncertainty
%       H6=entropy2(x,y6,N)   % variables even less related
%
%   See also HISTOGRAM2, MI, HISTOGRAM.
%
%JC

% I=round(rescale(I)*(N-1));

% Calculating joint histogram
a=histogram2(X,Y,N);

% Normalize histogram to get PDF
p=a./numel(X);

% Joint Entropy
H = -sum(sum(p.*full(spfun(@log2,p))));
