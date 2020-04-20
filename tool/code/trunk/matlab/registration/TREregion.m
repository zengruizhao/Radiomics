function [TRE,TRErms,M,meanTRE,overlap,regionout] = TREregion(params,regionmask)
% Target registration error (TRE), rms TRE for a region subject to affine deformation
if length(params)<5,
    sx=1;sy=1;
    if length(params)<3,
        dx=0;dy=0;
    else
        dx=params(2);
        dy=params(3);
    end
else
    sx=params(4);
    sy=params(5);
    dx=params(2);
    dy=params(3);
end
theta=params(1);

% Homogeneous translation matrix
transM=eye(4);
transM(:,4)=[dx dy 0 1]';

% Homogeneous scaling matrix
scaleM=diag([sx sy 1 1]);

% Homogeneous rotation matrix, about z-axis
rotM=[cos(theta) sin(theta) 0 0;
     -sin(theta) cos(theta) 0 0;
     0          0           1 0;
     0          0           0 1];
 
% Rotate, Scale, then Translate
M=transM*scaleM*rotM;

% Check image size
[nrows, ncols, planes]=size(regionmask);
if planes>1,
    error('No 3D');
end

% Transform each coordinate of region
[cpointsy, cpointsx]=find(regionmask); % doesn't work for 3D
% homogeneous coordinates
ncpoints=length(cpointsy);
cpoints=[cpointsx(:)'; cpointsy(:)'; ones(2,ncpoints)];
clear cpointsx cpointsy

% Move origin to image center for sensible rotation
cpointsdef=cpoints;
cpointsdef(1,:)=cpoints(1,:)-(ncols/2); % x
cpointsdef(2,:)=cpoints(2,:)-(nrows/2); % y
% cpointsdef(1,:)=cpoints(1,:); % x
% cpointsdef(2,:)=cpoints(2,:); % y
% move control points
cpointsdef=M*cpointsdef;
% move grid back to matlab coordinates
cpointsdef(1,:)=cpointsdef(1,:)+(ncols/2); % x
cpointsdef(2,:)=cpointsdef(2,:)+(nrows/2); % y
% cpointsdef(1,:)=cpointsdef(1,:); % x
% cpointsdef(2,:)=cpointsdef(2,:); % y

TRE=sqrt(sum((cpoints(1:2,:) - cpointsdef(1:2,:)).^2));
meanTRE=mean(TRE);
TRErms=sqrt(sum(TRE.^2)/ncpoints);

% Overlap
if nargout>4,
    % regionout=imdef2dNNxmat(M,regionmask);
    % regionout=imdefnno(M,regionmask);
    % regionout=imdefnn(M,regionmask);
    regionout=imdefnnfull(M,regionmask);
    regionmask=impad(regionmask,size(regionout));
    overlap=sum(sum(regionout & regionmask))/sum(sum(regionout | regionmask));
end

