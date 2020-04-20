function [Ifwd,Mout,invM,Irev,cpointsIout] = imdef3(params,I,varargin)
% IMDEF3 Deform an 3D volume with an affine transformation, with a
%   coordinate system origin at volume center or user specified location,
%   returning an image of equal size as the input.
%
%   Required INPUT arguments (1st and 2nd):
%
%   IMDEF3(PARAMS,I) deforms image I subject to deformations specified
%   by the transformation parameters PARAMS. PARAMS is a 1D array with the
%   elements corresponding to:
%       [rot_x, rot_y, rot_z, tr_x, tr_y, tr_z, sc_x, sc_y, sc_z]
%   Either 3, 6, or 9 parameters may be specified. Order of transformation:
%   1. Rotations. 2. Scales. 3. Translations.
%
%   Alternatively, IMDEF3(M,I) performs the transformation specified by the
%   linear coordinate transformation matrix M. M is 3-by-3 or 4-by-4 
%   homogeneous and representing a 3D transformation.
%
%   Optional INPUT arguments (3rd and 4th):
%
%   IFWD = IMDEF3(...,INTERPMETHOD) uses a specified interpolation method.
%   Possible values are 'nearest', 'linear', and 'cubic'. Both 'linear' and
%   'cubic' can return values not present in I, and 'cubic' can return
%   values outside the range of values in I. Default is 'linear'. 
%
%   IFWD = IMDEF3(...,INTERPMETHOD,BGVALUE) uses background intensity value
%   specified by scalar BGVALUE. Default is zero (0).
%
%   Optional OUPUT arguments:
%
%   [IFWD, M, INVM] = IMDEF3(...) returns the transformation matrix M and 
%   its inverse INVM.
%
%   [IFWD, M, INVM, IREV] = IMDEF3(...) the result of INVM applied to I.
%
%   Additional INPUT arguents via parameter-value pairs, where
%   [ ... ] = AFFINEREG3(..., 'PARAM1',val1, 'PARAM2',val2, ...) specifies
%   optional parameter name/value pairs:
%
%   'padz' - Boolean to idicate whether the volume should be temporarily 
%      padded on both sides with a blank slice to help with interpolation.
%      They are removed when deformation is complete. Since both sides are
%      padded, the center point is the same underlying pixel data, and 
%      coordshift retains its meaning.
%
%   'coordshift' - Offset from volume center, about which deformations are 
%      performed, [Y; X; Z]. Positive values impose a shift toward origin.
%      Default is [0; 0; 0]. MATLAB coordinates this time ([Y; X; Z])!
%
%      TIP: To rotate about the image origin (top left corner), use a value
%      of COORDSHIFT = [size(I,1)/2 size(I,2)/2 size(I,3)/2] to affect a 
%      shift from the image center to the top left corner. Non-integers OK.
%
%   'preM' - First apply the 4-by-4 homogeneous transformation matrix PREM.
%      Default is eye(4).
%
%   'cpointsI' - control point coordinates to be transformed. A 2-by-N 
%      array (i.e. [x; y; z]).
%
%   'framesize' - Iff I is empty, assume an image frame size for coordinate
%      transformation. Remember that transformation is about image center,
%      and adjusted by coordshift. If I is not empty, then framesize is 
%      *ignored*, a warning is issued, and cpointsI are transformed about 
%      the center of the actual image frame.
%
%   NOTES:
%   -Fixed output image frame size. The output image is the same size
%    as the input image. For an equal or larger output, see IMDEF3FULL.
%   -If I is not a float (double or single), it is converted to double.
%
%   Class support for I:  
%      float: double, single
%   
%   See also ROTMAT3, INTERP3.
%
%JC

LOWRAM=false;

% Ensure I is a float
if ~isa(I,'float'),
    I=double(I);
    warning('imdef3:ConvertingToDouble','Converting I to double datatype.');
end
Iclass=class(I);
Isingle=isa(I,'single');

ip=inputParser;
% ip.addRequired('params', @isnumeric);
% ip.addRequired('I', @isnumeric);
interpmethods={'nearest','linear','cubic','*nearest','*linear','*cubic'};
ip.addOptional('interpmethod', 'linear',@(x)any(strcmpi(x,interpmethods))); % 3
ip.addOptional('bgvalue', 0, @(x)isnumeric(x) && numel(x)==1); % 4
ip.addParamValue('preM', eye(4), @(x)isfloat(x) && numel(x)==16 && size(x,1)==4); % was 5
ip.addParamValue('coordshift', zeros(3,1), @(x)isfloat(x) && numel(x)==3); % was 6
ip.addParamValue('padz', false, @islogical); % was 7
ip.addParamValue('cpointsI',[],@(x)isnumeric(x) && any(size(x,1)==[3 4])) % was 8
ip.addParamValue('framesize',[],@(x)isnumeric(x) && numel(x)==2); % was 9
ip.parse(varargin{:});

bgvalue=cast(ip.Results.bgvalue,Iclass);

% Check image size, pad if requested
[nrows, ncols, planes]=size(I);
D=ndims(I);
if D>3,
    error('Larger than 3D!');
end

if ip.Results.padz,
    bgplane=bgvalue*ones(nrows,ncols,Iclass);
    I=cat(3,bgplane,I,bgplane);
    planes=planes+2;
end
Isize=size(I);

npts=numel(I);

% Check parameters/transformation matrix
if min(size(params,1),size(params,2))==1, % got parameters for rotation, etc. instead of matrix
    M = rotmat3(params)*ip.Results.preM;
elseif size(params,1)==3,    % got 3-by-3 standard transformation matrix
    M=eye(4);
    M(1:2,1:2)=params(1:2,1:2);
    M(1:2,4)=params(1:2,3); M=M*ip.Results.preM;
elseif size(params,1)==4,    % got 4-by-4 homogeneous transformation matrix
    M=params; M=M*ip.Results.preM;
else
    error('Matrix not 3x3 standard or 4x4 homogeneous.');
end

% check if image was [] (i.e. only control points input)
if npts==0,
    nrows=ip.Results.framesize(1);
    ncols=ip.Results.framesize(2);
    planes=ip.Results.framesize(3);
elseif ~isempty(ip.Results.framesize),
    warning('imdef3:framesizeIgnored','Image not empty, framesize variable IGNORED!');
end

% Center of image, adjusted center of image
centpt=[(ncols+1)/2-ip.Results.coordshift(2); (nrows+1)/2-ip.Results.coordshift(1); (planes+1)/2-ip.Results.coordshift(3)];  % x, y, z
% if correcting,
%     impcentpt=[(ncols+1)/2; (nrows+1)/2; (planes+1)/2];
%     fprintf('Implict center at (X,Y,Z) = (%g,%g,%g).\n',impcentpt(1),impcentpt(2),impcentpt(3));
%     fprintf('Transforming about center at (X,Y,Z) = (%g,%g,%g).\n',centpt(1),centpt(2),centpt(3));
% end

Mout=M;
Mcent=eye(4); Mcent(:,4)=[-centpt; 1];
Mcorn=eye(4); Mcorn(:,4)=[centpt; 1];
M=Mcorn*M*Mcent;

% Inverse operation so every target pixel gets a value, speedy interp2 assignment
invM=inv(M);

% Want control points?
if ~any(strcmp(ip.UsingDefaults,'cpointsI')), % input cpointsI
% if nargin>7 && nargout>4, % got cpointsI AND want them output transformed
    
    % error('CPs code currently out of order');
    % Make CPs homogeneous
    cpointsI=ip.Results.cpointsI;
    if size(cpointsI,1)==3,
        cpointsI=[cpointsI; ones(1,size(cpointsI,2))];
    end
    
    % Move origin to image center for sensible rotation
    cpointsI(1,:)=cpointsI(1,:);%-centpt(1); % x
    cpointsI(2,:)=cpointsI(2,:);%-centpt(2); % y
    cpointsI(3,:)=cpointsI(3,:);%-(centpt(3)-double(ip.Results.padz)); % z
    % move control points
    cpointsIout=M*cpointsI;
    % move grid back to matlab coordinates
    cpointsIout(1,:)=cpointsIout(1,:);%+centpt(1); % x
    cpointsIout(2,:)=cpointsIout(2,:);%+centpt(2); % y
    cpointsIout(3,:)=cpointsIout(3,:);%+(centpt(3)-double(ip.Results.padz)); % z
    % remove homogeneous
    cpointsIout(4,:)=[];
elseif nargout>4,
    error('No control points input, nothing to transform.');
end

% Don't work on image?
if isempty(I),
    Ifwd=[]; Irev=[];
    return;
end

% Produce coordinate grid
% [xpts,ypts,zpts]=meshgrid(1:ncols,1:nrows,1:planes);
x=1:ncols;%reshape(1:ncols,[1 ncols 1]);
y=reshape(1:nrows,[nrows 1 1]);
z=reshape(1:planes,[1 1 planes]);
% xpts2=x(ones(nrows,1),:,ones(planes,1));
% ypts2=y(:,ones(ncols,1),ones(planes,1));
% zpts2=z(ones(nrows,1),ones(ncols,1),:);

% Make homogeneous coordinates vector
% homog_points=[xpts(:)'; ypts(:)'; ones(2,npts)];
% homog_points_centered=ones(4,npts);
% homog_points_centered=cat(4,x(ones(nrows,1),:,ones(planes,1)),...
%     y(:,ones(ncols,1),ones(planes,1)),z(ones(nrows,1),ones(ncols,1),:),ones(size(I)));
% homog_points_centered=reshape(homog_points_centered,npts,4);
homog_points_centered=zeros(npts,4);
homog_points_centered(:,4)=1;

% xpts=x(ones(nrows,1),:,ones(planes,1));
% ypts=y(:,ones(ncols,1),ones(planes,1));
% zpts=z(ones(nrows,1),ones(ncols,1),:);
% homog_points_centered=[xpts(:) ypts(:) zpts(:) ones(npts,1)];
% homog_points_centered=cat(4,xpts,ypts,zpts,ones(size(I)));
% homog_points_centered=reshape(homog_points_centered,npts,4);

% Put image center at (0,0) for sensible rotation
% homog_points_centered(1,:)=xpts(:)-centpt(1);
% homog_points_centered(2,:)=ypts(:)-centpt(2);
% homog_points_centered(3,:)=zpts(:)-centpt(3);
xpts=x(ones(nrows,1),:,ones(planes,1));
homog_points_centered(:,1)=xpts(:);%-centpt(1);
if LOWRAM, clear xpts; end
ypts=y(:,ones(ncols,1),ones(planes,1));
homog_points_centered(:,2)=ypts(:);%-centpt(2);
if LOWRAM, clear ypts; end
zpts=z(ones(nrows,1),ones(ncols,1),:);
homog_points_centered(:,3)=zpts(:);%-centpt(3);
if LOWRAM, clear zpts; end
% clear xpts ypts zpts

% Apply the transform
% If points are 4-by-npts:
% moved_points=invM*homog_points_centered;
% moved_points=M\homog_points_centered; % preferable
% homog_points_centered=M\homog_points_centered;
% If points are npts-by-4 (three options):
% homog_points_centered=M\homog_points_centered';
homog_points_centered=homog_points_centered*invM'; % slightly less accurate than homog_points_centered/M', but way faster
% homog_points_centered=homog_points_centered/M';
% clear homog_points_centered

% Move the grid back to matlab coordinates
% newx=homog_points_centered(1,:)+centpt(1); if isa(I,'single'), newx=single(newx); end
% newy=homog_points_centered(2,:)+centpt(2); if isa(I,'single'), newy=single(newy); end
% newz=homog_points_centered(3,:)+centpt(3); if isa(I,'single'), newz=single(newz); end
newx=homog_points_centered(:,1);%+centpt(1);
newy=homog_points_centered(:,2);%+centpt(2);
newz=homog_points_centered(:,3);%+centpt(3);
if Isingle,
    newx=single(newx);
    newy=single(newy);
    newz=single(newz);
end
if LOWRAM && nargout<=3, clear homog_points_centered; end

% Interpolate intensities on regular image coordinate grid
if ~isempty(strfind(ip.Results.interpmethod,'linear')),
    newx=reshape(newx,Isize);
    newy=reshape(newy,Isize);
    newz=reshape(newz,Isize);
    
    %     Ifwd=mirt3D_mexinterp(I,newx,newy,newz); % linear only with mirt3D
    if isa(I,'double'),
        if numel(I)>1e6 && exist('linterp3mexmt','file'),
            Ifwd=linterp3mexmt(I,newx,newy,newz,bgvalue,2);
        else
            %Ifwd=mirt3D_mexinterp_largearraydims(I,newx,newy,newz); % linear only with mirt3D
            Ifwd=linterp3mex(I,newx,newy,newz,bgvalue);
            %         Ifwd=linterp3mex_fastout(I,newx,newy,newz,bgvalue);
        end
    elseif isa(I,'single'),
        Ifwd=mirt3D_mexinterpsall_largearraydims(I,newx,newy,newz);
    else
        error('I needs to be double or single');
    end
%     Ifwd(isnan(Ifwd))=bgvalue; % should not be necessary with EXTRAPVAL=bgvalue
    
    % if strcmp(interpmethod(1),'*'), interpmethod=interpmethod(2:end); end
    % Ifwd=ba_interp3_messages(I,newx,newy,newz,interpmethod);
elseif ~isempty(strfind(ip.Results.interpmethod,'cubic')),
    newx=reshape(newx,Isize);
    newy=reshape(newy,Isize);
    newz=reshape(newz,Isize);

    Ifwd=interp3(I,newx,newy,newz,ip.Results.interpmethod,bgvalue);
    % Ifwd=interp3(xpts,ypts,zpts,I,newx,newy,newz,interpmethod,bgvalue);
elseif ~isempty(strfind(ip.Results.interpmethod,'nearest')),
    % Nearest Neighbor: round
    % clear moved_points
    newx=round(newx);
    newy=round(newy);
    newz=round(newz);
    
    % Which of these points came from the image?
    inbound=newy>=1 & newy<=nrows & newx>=1 & newx<=ncols & newz>=1 & newz<=planes;
    % If point came from outside the image, say it came from the corner for now
    newx(~inbound)=1;
    newy(~inbound)=1;
    newz(~inbound)=1;
    
    % pixel coords -> matrix index
    origpointinds=sub2ind(Isize, newy, newx, newz);
    % clear newx newy newz
    
    % Initialize output image with desired background
    if islogical(I),
        Ifwd=false(Isize);
    else
        Ifwd=zeros(Isize,class(I));
    end
    
    % Nearest Neighbor => single origin, no weights
    Ifwd(:)=I(origpointinds);  % A(x,y) = B(T(x,y))
    
    % For all planes of image (colors), fill out-bound pixels
    Ifwd(~inbound)=bgvalue;
else
    error('Unmatched interpolation method.');
end

if ip.Results.padz,
    Ifwd=Ifwd(:,:,2:end-1);
end

% Do reverse transformation of image
if nargout>3,
    forward_points=M*homog_points_centered';
    newx=forward_points(1,:);%+centpt(1);
    newy=forward_points(2,:);%+centpt(2);
    newz=forward_points(2,:);%+centpt(2);
    
    newx=reshape(newx,Isize);
    newy=reshape(newy,Isize);
    newz=reshape(newz,Isize);
    
    Irev=interp3(I,newx,newy,newz,ip.Results.interpmethod,bgvalue);
%     Irev=zeros(Isize,Iclass);
%     for iz=1:planes,
%         Irev(:,:,iz)=interp2(xpts,ypts,I(:,:,iz),reshape(forward_points(1,:),[nrows ncols]), ... 
%             reshape(forward_points(2,:),[nrows ncols]),interpmethod,bgvalue);
%     end
%     Irev(isnan(Irev))=bgvalue; % should not be necessary with EXTRAPVAL=bgvalue
    if ip.Results.padz,
        Irev=Irev(:,:,2:end-1);
    end
end
