function [Iout,tlocalx_combined,tlocaly_combined,tlocalz_combined,dx0,dy0,dz0,Ichanged] = reversedisp3(I, tlocalx, tlocaly, tlocalz, fillwith, imethod, movingbox)
% REVERSEDISP3 Apply a reverse displacement field to an 3D image/volume.
%   IOUT = REVERSEDISP3(I,TLOCALX,TLOCALY,TLOCALZ) applies the reverse 
%   displacement field (i.e. one that specifies for each voxel in the
%   output image the origin of those points in the floating image, rather 
%   than the destination in the output image for each voxel in the source 
%   floating image) to 3D volume I.
%
%   [IOUT,TLOCALXNEW,TLOCALZNEW,TLOCALZNEW] = REVERSEDISP3(...) returns the
%   combined deformation field that results from using TLOCAL* that are
%   cell arrays of deformation fields, where each one is to be applied
%   successively.
%   
%   See also: imffd3, imbeziers3, cpmesh3, ffdreg3.
%
%JC

if nargin<6 || isempty(imethod),
imethod='linear';
end

if nargin<5 || isempty(fillwith),
    fillwith=0;
end

if nargin<7 || isempty(movingbox),
    Isize=size(I);
    % [Y, X, Z] = ndgrid(1:size(I,1),1:size(I,2),1:size(I,3));
    [X, Y, Z] = meshgrid(1:size(I,2),1:size(I,1),1:size(I,3));
    doroi=false;
else
    [X, Y, Z] = meshgrid(movingbox(1,1):movingbox(1,2), ...
        movingbox(2,1):movingbox(2,2), movingbox(3,1):movingbox(3,2));
    Isize=size(X);
    X=X(:); Y=Y(:); Z=Z(:);
    doroi=true;
end

% linear interpolation for combining deformations
if exist('linterp3mexmt','file'),
    linterpfun=@linterp3mexmt;
elseif exist('linterp3mex','file'),
    linterpfun=@linterp3mexmt;
else
    linterpfun=@(f,x,y,z,v)interp3(f,x,y,z,'linear',v);
end

% any interpolation for image intensities
if strcmpi(imethod,'linear'), % use mex files if they exist
    interpfun=linterpfun;
else
    interpfun=@(f,x,y,z,v)interp3(f,x,y,z,imethod,v);
end

if iscell(tlocalx) || iscell(tlocaly) || iscell(tlocalz),
    if ~(iscell(tlocalx) && iscell(tlocalx) && iscell(tlocalx)),
        error('x,y,z deformation fields must all be cells or arrays'); end
    
    tlocalx_combined=tlocalx{1};
    tlocaly_combined=tlocaly{1};
    tlocalz_combined=tlocalz{1};
    
    % For OPTION 1 below
%     dx0 = tlocalx_combined - X;
%     dy0 = tlocaly_combined - Y;
%     dz0 = tlocalz_combined - Z;
    
    ndefs=numel(tlocalx);
    for i=1:ndefs-1,
        % Lookup next deformation at position specified by previous deformation
        
        % OPTION 1: interpolate d1(x + d0(x)) and recombine to get T(x)
%         dx=tlocalx{i+1} - X; dy=tlocaly{i+1} - Y; dz=tlocalz{i+1} - Z;
%         
%         dxi = linterp3mex(dx, tlocalx_combined, tlocaly_combined, tlocalz_combined);
%         dyi = linterp3mex(dy, tlocalx_combined, tlocaly_combined, tlocalz_combined);
%         dzi = linterp3mex(dz, tlocalx_combined, tlocaly_combined, tlocalz_combined);
%         
%         dx0 = dx0 + dxi;
%         dy0 = dy0 + dyi;
%         dz0 = dz0 + dzi;
%         
%         tlocalx_combined = dx0 + X;
%         tlocaly_combined = dy0 + Y;
%         tlocalz_combined = dz0 + Z;
        
        % OPTION 2: interpolate T2(T1(x)) = x + d1(x + d0(x))
        tlocalxi = linterpfun(tlocalx{i+1}, tlocalx_combined, tlocaly_combined, tlocalz_combined);
        tlocalyi = linterpfun(tlocaly{i+1}, tlocalx_combined, tlocaly_combined, tlocalz_combined);
        tlocalzi = linterpfun(tlocalz{i+1}, tlocalx_combined, tlocaly_combined, tlocalz_combined);
        tlocalx_combined = tlocalxi;
        tlocaly_combined = tlocalyi;
        tlocalz_combined = tlocalzi;
        
    end
    % For OPTION 2 above
    dx0 = tlocalx_combined - X;
    dy0 = tlocaly_combined - Y;
    dz0 = tlocalz_combined - Z;
    
else
    tlocalx_combined=tlocalx;
    tlocaly_combined=tlocaly;
    tlocalz_combined=tlocalz;
    dx0 = tlocalx_combined - X;
    dy0 = tlocaly_combined - Y;
    dz0 = tlocalz_combined - Z;
end

% Now trasform the actual image
if ~isequal(size(tlocalx_combined),Isize),
    tlocalx_combined=reshape(tlocalx_combined,Isize);
    tlocaly_combined=reshape(tlocaly_combined,Isize);
    tlocalz_combined=reshape(tlocalz_combined,Isize);
end
% Fx=tlocalx-X; Fy=tlocaly-Y; Fz=tlocalz-Z;

if doroi,
    Iout=I;
    Ichanged=interpfun(I,tlocalx_combined,tlocaly_combined,tlocalz_combined,fillwith);
    Iout(movingbox(2,1):movingbox(2,2), movingbox(1,1):movingbox(1,2), movingbox(3,1):movingbox(3,2)) = Ichanged;
else
    Iout=interpfun(I,tlocalx_combined,tlocaly_combined,tlocalz_combined,fillwith);
end

% if exist('linterp3mexmt','file') && strcmpi(imethod,'linear'), % ~isempty(strfind(imethod,'linear')),
%     Iout=linterp3mexmt(I,tlocalx_combined,tlocaly_combined,tlocalz_combined,fillwith);
% elseif exist('linterp3mex','file') && strcmpi(imethod,'linear'), % ~isempty(strfind(imethod,'linear')),
%     Iout=linterp3mexmt(I,tlocalx_combined,tlocaly_combined,tlocalz_combined,fillwith);
% else
%     Iout=interp3(I,tlocalx_combined,tlocaly_combined,tlocalz_combined,imethod,cast(fillwith,class(I)));
% end

% if any(size(tlocalx)~=size(I)),
%     tlocalx=reshape(tlocalx,size(I));
%     tlocaly=reshape(tlocaly,size(I));
% end
% Iout=griddata(tlocalx,tlocaly,I,X,Y,imethod);
Iout(isnan(Iout))=cast(fillwith,class(Iout));
