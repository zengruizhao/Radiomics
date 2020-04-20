function [TRE,TRErms,M,meanTRE,overlap,regionout] = TREregion3(params,regionmask)
% Target registration error (TRE), rms TRE for a region subject to affine deformation

if length(params)<9,
    sx=1;sy=1;sz=1;
    if length(params)<6,
        dx=0;dy=0;dz=0;
    else
        dx=params(4);
        dy=params(5);
        dz=params(6);
    end
else
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

% Check image size
masksize=size(regionmask);
D=ndims(regionmask);
if D>3,
    error('Larger than 3D!');
end

% Transform each coordinate of region
cpointsi=find(regionmask);
ncpoints=length(cpointsi);
cpcoords=ind2subarray(size(regionmask),cpointsi);
clear cpointsi

% homogeneous coordinates
cpoints=[cpcoords{:} ones(ncpoints,4-D)]';
clear cpcoords

% Move origin to image center for sensible rotation
cpointsdef=cpoints;
for i=1:D,
    cpointsdef(i,:)=cpoints(i,:)-(masksize(i)/2);
    % cpointsdef(i,:)=cpoints(i,:);
end

% move control points
cpointsdef=M*cpointsdef;

% move grid back to matlab coordinates
for i=1:D,
    cpointsdef(i,:)=cpointsdef(i,:)+(masksize(i)/2);
    % cpointsdef(i,:)=cpointsdef(i,:);
end

% Calculate TRE
TRE=sqrt(sum((cpoints(1:3,:) - cpointsdef(1:3,:)).^2));
meanTRE=mean(TRE);
TRErms=sqrt(sum(TRE.^2)/ncpoints);

% Overlap
if nargout>4,
% regionout=imdef2dNNxmat(M,regionmask);
% regionout=imdefnno(M,regionmask);
% regionout=imdefnn(M,regionmask);
% regionmask=impad(regionmask,size(regionout));

% regionout=imdefnn3(M,regionmask);
regionout=imdefnnfull3(M,regionmask);
regionmask=impad3(regionmask,size(regionout));

overlap=sum(sum(sum(regionout & regionmask)))/sum(sum(sum(regionout | regionmask)));
end
