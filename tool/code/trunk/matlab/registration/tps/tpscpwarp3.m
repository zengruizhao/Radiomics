function [Itargetw,is,it,Xw0,Yw0,Zw0,X,Y,Z]  = tpscpwarp3(Itarget,Itempl_size,Pt,Ps,imethod)
%TPSCPWARP
%   Very simple Thin-Plane Spline (TPS) warping of the two images, ITARGET
%   and ITEMPLATE given their corresponding control points ZT and ZS.
%
%   [ITARGETW,IS,IT,XW0,YW0,X,Y]=TPSCPWARP(ITARGET,ITEMPLATE,ZT,ZS,IMETHOD)
%
%   Target image, ITARGET, is warped onto template, ITEMPLATE.
%
%   The set of reference points ZT=[Xt Yt] on ITARGET, and corresponding
%   destination points ZS=[Xs Ys] on ITEMPLATE parameterize the transform.
%
%   Outputs: the warped target ITARGETW, the rounded linear index mappings
%   IS->IT, the unrounded x,y coordinate deformation fields XW0 and YW0, 
%   and the usual reference meshgrids X and Y.
%
%   Reference: F.L. Bookstein, "Principal Warps: Thin-Plate splines and the 
%   decomposition of deformations", IEEE Tr. on Pattern Analysis and
%   Machine Intel, vol. 11, No. 6, June 1989.
%
%   How to use the outputs Xw0, Yw0, X, and Y:
%   Itargetw(:,:,j)=griddata(...
%         reshape(Xw0,nrows,ncols),reshape(Yw0,nrows,ncols),...
%         Itarget(:,:,j),...
%         reshape(X,nrows,ncols),reshape(Y,nrows,ncols),...
%         imethod);
%
%   See also TPSWARP, TPSREG.
%
%JC

if nargin<5,
    imethod='linear';
end

if numel(Itempl_size)>4,
    Itempl_size=size(Itempl_size);
elseif isempty(Itempl_size),
    Itempl_size=size(Itarget);
end

if numel(Itempl_size)~=3, error('wrong size volume'); end

NPs = size(Pt,1);

Xt = Pt(:,1)';
Yt = Pt(:,2)';
Zt = Pt(:,3)';
Xs = Ps(:,1)';
Ys = Ps(:,2)';
Zs = Ps(:,3)';

rXt = repmat(Xt(:),1,NPs);
rYt = repmat(Yt(:),1,NPs);
rZt = repmat(Zt(:),1,NPs);

% "control" points spacing
wR = sqrt((rXt-rXt').^2 + (rYt-rYt').^2 + (rZt-rZt').^2); % distance of every control point to each other CP
% Bookstein...
wK = 2*(wR.^2).*log(wR+1e-20);
wP = [ones(NPs,1) Xt(:) Yt(:) Zt(:)];
wL = [wK wP;wP' zeros(4,4)];

% "reference" points
wY = [Xs(:) Ys(:) Zs(:); zeros(4,3)];
wW = wL\wY;    % wY = wL*wW (reference are weighted coords/distances)

nrows=size(Itarget,1); % y
ncols=size(Itarget,2); % x
nplanes=size(Itarget,3); % x
[X Y Z] = meshgrid(1:ncols,1:nrows,1:nplanes);
X = X(:); Y = Y(:); Z = Z(:); % column of coordinates
NWs = length(X);

% coord matrix
rX = repmat(X',NPs,1);
rY = repmat(Y',NPs,1);
rZ = repmat(Z',NPs,1); 

rXt = repmat(Xt(:),1,NWs);
rYt = repmat(Yt(:),1,NWs);
rZt = repmat(Zt(:),1,NWs);

% Bookstein...
wR = sqrt((rXt-rX).^2 + (rYt-rY).^2 + (rZt-rZ).^2); % distance of every coordinate to each control point
% wK = 2*(wR.^2).*log(wR+eps);
wK = 2*(wR.^2).*log(wR+(wR==0));
wP = [ones(NWs,1) X Y Z]';
wL = [wK;wP]';

Xw  = wL*wW(:,1);
Yw  = wL*wW(:,2);
Zw  = wL*wW(:,3);

Xw0=max(min(Xw,Itempl_size(2)),1);
Xw = round(Xw0);
Yw0=max(min(Yw,Itempl_size(1)),1);
Yw = round(Yw0);
Zw0=max(min(Zw,Itempl_size(3)),1);
Zw = round(Zw0);

is = sub2ind(Itempl_size(1:3),Yw,Xw,Zw);
it = sub2ind([nrows,ncols,nplanes],Y,X,Z);

Finterpolant = TriScatteredInterp(Xw0,Yw0,Zw0,Itarget(:),imethod);
Itargetw = reshape(Finterpolant(X,Y,Z),[nrows ncols nplanes]);
