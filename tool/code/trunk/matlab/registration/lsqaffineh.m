function [homogaffine,Z] = lsqaffineh(X,Y)
% LSQAFFINEH
%   Solution to the least-squares minimization problem for calculating the
%   homogeneous affine transformation that maps corresponding 2D points
%   onto each other.
%
%   A = LSQAFFINEH(XY,UV) returns matrix A which represents a rotation
%   and an *anisomorphic* scaling. For isomorphic scaling see LSQSIMILARITY.
%   For reflection with isomorphic scaling or no scaling at all, see 
%   MATLAB's PROCRUSTES in the statistics toolbox.
%
% For two sets of coordinates XY (moving) and UV (stationary) where
%          XY = [x; y]  and    UV = [u; v]
% the affine transformation A that minimizes the error e in the
% transformation of XY by: UV = A * XY + e . The vectors XY and UV are both
% 2-by-N matrices of N 2D points/coordinates.
%
% In homogeneous coordinates:
%
%       [ u ]   [a11 a12 a13] [ x ]
%       [ v ] = [a21 a22 a23] [ y ]
%       [ 1 ]   [0    0    1] [ 1 ]
%
% The solution is found by forumulating the least-squares estimate of the
% size parameters of the homogeneous transformation matrix. The derivative
% with respect to each of the six parameters is represented by the
% following matrix formulation:
%
%[sum(xi^2) sum(xiyj) sum(xi)    0         0          0  ][a11] [sum(uixi)]
%[sum(xiyi) sum(yi^2) sum(yi)    0         0          0  ][a12] [sum(uiyi)]
%[sum(xi)   sum(yi)   sum(1)     0         0          0  ][a13]=[ sum(ui) ]
%[   0         0          0   sum(xi^2) sum(xiyj) sum(xi)][a21] [sum(vixi)]
%[   0         0          0   sum(xiyi) sum(yi^2) sum(yi)][a22] [sum(viyi)]
%[   0         0          0   sum(xi)   sum(yi)   sum(1) ][a23] [ sum(vi) ]
%
% This function solves for [a11 a12 a13 a21 a22 a23]' and returns
%
%       [a11 a12 a13]
%   A = [a21 a22 a23]
%       [0    0    1]
%
% Note: At least THREE pairs of points are necessary to define the 2D
% orthogonal coordinate system.
%
%JC
%
% Thanks to Rob Toth for references and code for the weighted minimization
% problem.
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
LHSsys=zeros(6);
LHSsys(1,1)=x1*x1';
LHSsys(2,1)=x1*x2';
LHSsys(3,1)=sum(x1);
LHSsys(1,2)=LHSsys(2,1);
LHSsys(2,2)=x2*x2';
LHSsys(3,2)=sum(x2);
LHSsys(1,3)=LHSsys(3,1);
LHSsys(2,3)=LHSsys(3,2);
LHSsys(3,3)=npts;
LHSsys(4:end,4:end)=LHSsys(1:3,1:3);

% Right hand size
RHSsys=zeros(6,1);
RHSsys(1)=y1*x1';
RHSsys(2)=y1*x2';
RHSsys(3)=sum(y1);
RHSsys(4)=y2*x1';
RHSsys(5)=y2*x2';
RHSsys(6)=sum(y2);

% Solution
homogaffine=LHSsys\RHSsys;

% Reshape solution to homogeneous affine transformation matrix
homogaffine=[reshape(homogaffine,3,2)'; 0 0 1];

Z=homogaffine*[x1; x2; ones(1, length(x1))];
Z=Z(1:2,:);
