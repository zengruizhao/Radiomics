function [M, invM] = rotmat(rigidparams)
% ROTMAT Make rotation matrix from transform parameters.
%   [M, INVM] = ROTMAT(RIGIDPARAMS) returns the homogeneous transformation 
%   matrices M and INVM given parameters RIGIDPARAMS.
%
%   RIGIDPARAMS is a 5-by-1 vector representing:
%   [rotation_aboutz trans_x trans_y scale_x scale_y]
%
%   Rotate, Scale, then Translate, if you don't like that, get the matrices
%   individually, then multiply them together in the order you want.  For 
%   example, if you want to scale, translate, then rotate: 
%       M = rotM*transM*scaleM 
%       newcoords = M * oldcoords; % newcoords=(oldcoords'*M')'
%
%   See also IMDEFNN, IMDEFNNO.
%
%JC

% make parameters vertical
rigidparams=rigidparams(:);

% Deal withthe old inputs having a 1-by-7 vector representing:
%   [rotation_aboutz trans_x trans_y trans_z scale_x scale_y scale_z]
if numel(rigidparams)==7,
    rigidparams([4 7])=[];
end

if numel(rigidparams)==6, theta2=rigidparams(6);
else theta2=0; end

% Homogeneous translation matrix
transM=eye(4);
transM(:,4)=[rigidparams(2:3); 0; 1];

% Homogeneous scaling matrix
scaleM=diag([rigidparams(4:5); 1; 1]);

theta=rigidparams(1);
% Homogeneous rotation matrix, about z-axis
rotM=[cos(theta) sin(theta) 0 0;    % note the sign convention:
     -sin(theta) cos(theta) 0 0;    % rotating image, not coordinate axes
     0          0           1 0;
     0          0           0 1];
rotM2=[cos(theta2) sin(theta2) 0 0;    % note the sign convention:
     -sin(theta2) cos(theta2) 0 0;    % rotating image, not coordinate axes
     0          0           1 0;
     0          0           0 1];
 
% Rotate, Scale, then Translate
M=transM*rotM2*scaleM*rotM;

if nargout>1,
    % Inverse operation so every target pixel gets a value
    invM=inv(M); % better not to use this.  Do M\coords instead of invM*coords
end
