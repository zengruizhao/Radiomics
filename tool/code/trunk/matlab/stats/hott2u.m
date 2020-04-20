function [P,t2]=hott2u(X,Y)
% HOTT2U Hotelling's two-sample T-squared statistic, a multivariate unpaired t-test.
%
%	[P,t2]=hott2u(X,Y)
%
%JC

% Number of observations
nx=size(X,1);
ny=size(Y,1);
% Number of variables,
p=size(X,2);

% Sample means (of each variable)
mx=mean(X,1);
my=mean(Y,1);

% Difference of the corresponding means
dm=(mx-my);

% Sample differences
dxy=X-Y;

% Unbiased pooled covariance matrix
W=((nx-1)*cov(X)+(ny-1)*cov(Y))/(nx+ny-2);

% T-squared statistic
t2=((nx*nx)/(nx+ny))*dm*inv(W)*dm';

% f statistic
f=t2*(nx+ny-p-1)/((nx+ny-2)*p);

% p-value
P=1-fcdf(f,p,nx+ny-1-p);
