function [homogaffine,Z] = lsqsimilarityh(X,Y)
% LSQSIMILARITYH
%   Solution to the least-squares minimization problem for calculating a
%   homogeneous linear transformation that maps corresponding 2D points
%   onto each other.
%
%   A = LSQSIMILARITYH(XY,UV) returns matrix A which represents a rotation
%   and an *isomorphic* scaling. For reflection with isomorphic scaling or
%   no scaling at all, see MATLAB's PROCRUSTES in the statistics toolbox.
%
% For two sets of coordinates XY (moving) and UV (stationary) where
%          XY = [x; y]  and    UV = [u; v]
% the affine transformation A that minimizes the error e in the
% transformation of XY by: UV = A * XY + e . The vectors XY and UV are both
% 2-by-N matrices of N 2D points/coordinates.
%
% In homogeneous coordinates:
%
%       [ u ]   [a -b tx] [ x ]
%       [ v ] = [b  a tx] [ y ]
%       [ 1 ]   [0  0  1] [ 1 ]
%
% The solution is found by forumulating the least-squares estimate of the
% size parameters of the homogeneous transformation matrix. The derivative
% with respect to each of the six parameters is represented by the
% following matrix formulation:
%
% [sum(xi^2)+sum(yi^2)    0           sum(xi)  sum(yi)] [  a ] [sum(uixi)+sum(viyi)]
% [        0     sum(xi^2)+sum(yi^2) -sum(yi)  sum(xi)] [  b ] [sum(vixi)+sum(uiyi)]
% [      sum(xi)       -sum(yi)       sum(1)     0    ] [ tx ]=[ sum(ui) ]
% [      sum(yi)        sum(xi)        0       sum(1) ] [ ty ] [ sum(vi) ]
%
% This function solves for [a b tx ty]' and returns
%
%       [a -b tx]
%   A = [b  a ty]
%       [0  0  1]
%
% Note: At least THREE pairs of points are necessary to define the 2D
% orthogonal coordinate system.
%
%JC

%
% Thanks to Rob Toth for references and code for the weighted affine 
% minimization problem.
%

% Make sure coordinate vectors are 2-by-N
if size(X,1)>2,
    X=X';
end
if size(Y,1)>2,
    Y=Y';
end

% Moving points
x1=X(1,:); x2=X(2,:);
% Stationary points
y1=Y(1,:); y2=Y(2,:);

% Number of points/coordinates
npts=length(x1);

% Left hand size
LHSsys=zeros(4);
LHSsys(1,1)=x1*x1'+x2*x2';
LHSsys(2,1)=0;
LHSsys(3,1)=sum(x1);
LHSsys(4,1)=sum(x2);

LHSsys(1,2)=0;
LHSsys(2,2)=LHSsys(1,1);
LHSsys(3,2)=-LHSsys(4,1);
LHSsys(4,2)=LHSsys(3,1);

LHSsys(1,3)=LHSsys(3,1);
LHSsys(2,3)=LHSsys(3,2);
LHSsys(3,3)=npts;
LHSsys(4,3)=0;

LHSsys(1,4)=LHSsys(4,1);
LHSsys(2,4)=LHSsys(3,1);
LHSsys(3,4)=0;
LHSsys(4,4)=npts;

% Right hand size
RHSsys=zeros(4,1);
RHSsys(1)=x1*y1'+x2*y2';
RHSsys(2)=x1*y2'-x2*y1';
RHSsys(3)=sum(y1);
RHSsys(4)=sum(y2);

% Solution
homogaffine=LHSsys\RHSsys;

% Reshape solution to homogeneous affine transformation matrix
homogaffine=[[homogaffine(1) -homogaffine(2) homogaffine(3)]; ...
             [homogaffine(2)  homogaffine(1) homogaffine(4)]; ...
             [    0               0              1         ]];

Z=homogaffine*[x1; x2; ones(1, length(x1))];
Z=Z(1:2,:);
