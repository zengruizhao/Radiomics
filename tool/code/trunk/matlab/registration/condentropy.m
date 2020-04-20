function [H,a,p,pc]=condentropy(Y,X,N)
% CONDENTROPY Conditional entropy of random variable Y, given X.
%   H = CONDENTROPY(Y,X,N) returns conditional Shannon entropy H(Y|X) in 
%   the scalar H, using a 2D histogram with N*N bins (size N-by-N).
%
%   [H,A] = CONDENTROPY(...) returns the N-by-N joint histogram in A.
%   [H,A,P] = CONDENTROPY(...) returns the N-by-N joint PDF, p(x,y), in P.
%   [H,A,P,PC] = CONDENTROPY(...) returns the N-by-N joint conditional pdf,
%   p(y|x), in PC.
%
%   Interpretation
%   --------------
%   "Amount of uncertainty that remains about Y, given knowledge of X." -JC
%
%   Implementation Details
%   ----------------------
%   The conditional entropy H(Y|X) returned in H is calculated as follows,
%   H(Y|X) =  \sum_{x\in\mathcal X} p(x) H(Y|X=x)
%          = -\sum_{x\in\mathcal X} p(x) \sum_{y\in\mathcal Y} p(y|x) \log p(y|x)
%          = -\sum_{x\in\mathcal X, y\in\mathcal Y} p(x,y) \log p(y|x)
%
%   Conceptually, this is equivalent to the quantity H(Y|X) = H(X,Y) - H(X)
%
%   Example
%   -------
%       N1=2^8; y=round((N-1)*rand(1,10000)); % y is 8-bit uniform dist
%       N2=2^5; x=round((N2-1)*y/(N-1));      % x is 5-bit version of y
%       Hyx=condentropy(y,x,N1)  % H is ~3 (8 - 5 = 3 bits unexplained by x)
%       Hxy=condentropy(x,y,N1)  % H is 0 (y completely explains x)
%
%   See also ENTROPY, ENTROPY2, HISTOGRAM2, MI, HISTOGRAM.
%
%JC

% Calculating joint histogram
a=histogram2(Y,X,N);

% Normalize joint histogram to get joint pdf
p=a./numel(Y);

% Marginal pdf of X
pX=sum(p,1); % sum down rows (collapse all values of Y)

% Conditional probability
% pc=p./repmat(pX,N,1);
pX(pX==0)=Inf;  % the trivial solution where p(X=x,Y=y)=0 (and p(X=x)=0);
pc=bsxfun(@rdivide,p,pX);
% pc(isnan(pc))=0;

% Conditional Entropy
H = -sum(sum(p.*full(spfun(@log2,pc))));
