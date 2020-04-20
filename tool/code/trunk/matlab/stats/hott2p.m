function [P,t2]=hott2p(X,Y)
% HOTT2P Hotelling's two-sample T-squared statistic, a multivariate paired t-test.
%
%	[P,t2]=hott2p(X,Y)
%
%JC

% Number of observations
n=size(X,1);
% Number of variables,
p=size(X,2);

% Sample means (of each variable)
mx=mean(X,1);
my=mean(Y,1);

% Difference of the corresponding means
dm=(mx-my);

% Sample differences
dxy=X-Y;

% Covariance matrix of sample differences
W=cov(dxy);

% T-squared statistic
t2=n*dm*inv(W)*dm';

% f statistic
f=t2*(n-p)/((n-1)*p);

% p-value
P=1-fcdf(f,p,n-p);
