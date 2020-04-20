function [M, invM] = rotmat3(params)
% ROTMAT3 Make 3D coordinate transformation matrix from input parameters.
%   [M, INVM] = ROTMAT(RIGIDPARAMS) returns the homogeneous transformation 
%   matrices M and INVM given parameters RIGIDPARAMS.
%
%   RIGIDPARAMS is a 1-by-9 vector representing:
%   [rot_x rot_y rot_z trans_x trans_y trans_z scale_x scale_y scale_z]
%
%   The matrix M is 4-by-4 because it includes translations in M(1:3,4). To
%   get the usual non-homogeneous M, do M=M(1:3,1:3).
%
%   Rotate, Scale, then Translate, if you don't like that, get the matrices
%   individually, then multiply them together in the order you want.  For 
%   example, if you want to scale, translate, then rotate: 
%       M = rotM*transM*scaleM 
%       newcoords = M * oldcoords; % newcoords=(oldcoords'*M')'
%
%   To combine matrices, do: M_cumulative = M_n * M_n-1 * ... * M_2 * M_1
%
%   See also ROTMAT, IMDEF.
%
%JC

if length(params)<9,
    sx=1;sy=1;sz=1;
    if length(params)<6,
        dx=0;dy=0;dz=0;
    else
        dx=params(4);
        dy=params(5);
        dz=params(6);
    end
elseif length(params)>9,
    error('Too many parameters.  Specify 3, 6, or 9 only.');
else % length(params)==9
    sx=params(7);
    sy=params(8);
    sz=params(9);
    dx=params(4);
    dy=params(5);
    dz=params(6);
end
thetax=params(1);
thetay=params(2);
thetaz=params(3);
    
% Homogeneous translation matrix
transM=eye(4);
transM(:,4)=[dx dy dz 1]';

% Homogeneous scaling matrix
scaleM=diag([sx sy sz 1]);

% Homogeneous rotation matrix, about z-axis
rotMz=[cos(thetaz) sin(thetaz) 0 0;
    -sin(thetaz) cos(thetaz) 0 0;
    0          0           1 0;
    0          0           0 1];

rotMx=[1 0 0 0;
    0 cos(thetax) sin(thetax) 0;
    0 -sin(thetax) cos(thetax) 0
    0          0           0 1];

rotMy=[cos(thetay) 0 -sin(thetay) 0;
    0          1           0 0;
    sin(thetay) 0 cos(thetay) 0;
    0          0           0 1];

% Rotate, Scale, then Translate
M=transM*scaleM*rotMx*rotMy*rotMz;

if nargout>1,
    % Inverse operation so every target pixel gets a value
    invM=inv(M); % better not to use this.  Do M\coords instead of invM*coords
end
