function [Itargetw,is,it,Xw0,Yw0,X,Y]  = tpscpwarp(Itarget,Itempl,Zt,Zs,imethod)
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
%   See also TPSWARP, TPSREG.
%
%JC

if nargin<5,
    imethod='linear';
end

NPs = size(Zt,1);

% Xt = Zt(:,1)';
% Yt = Zt(:,2)';
% Xs = Zs(:,1)';
% Ys = Zs(:,2)';
% reverse to use with interp2
Xt = Zs(:,1)';
Yt = Zs(:,2)';
Xs = Zt(:,1)';
Ys = Zt(:,2)';

rXt = repmat(Xt(:),1,NPs);
rYt = repmat(Yt(:),1,NPs);

% "control" points spacing
wR = sqrt((rXt-rXt').^2 + (rYt-rYt').^2); % distance of every control point to each other CP
% Bookstein...
wK = 2*(wR.^2).*log(wR+1e-20);
wP = [ones(NPs,1) Xt(:) Yt(:)];
wL = [wK wP;wP' zeros(3,3)];

% "reference" points
wY = [Xs(:) Ys(:); zeros(3,2)];
wW = wL\wY;    % wY = wL*wW (reference are weighted coords/distances)

% target image coordinates
% if size(Itarget,3)>1,
%     Itarget = double(sum(Itarget,3))/size(Itarget,3); % simple grayscale from RGB
% end
nrows=size(Itarget,1); % y
ncols=size(Itarget,2); % x
[X Y] = meshgrid(1:ncols,1:nrows);
X = X(:); Y = Y(:); % column of coordinates
NWs = length(X);

% coord matrix
rX = repmat(X',NPs,1); 
rY = repmat(Y',NPs,1); 

rXt = repmat(Xt(:),1,NWs);
rYt = repmat(Yt(:),1,NWs);

% Bookstein...
wR = sqrt((rXt-rX).^2 + (rYt-rY).^2); % distance of every coordinate to each control point
% wK = 2*(wR.^2).*log(wR+eps);
wK = 2*(wR.^2).*log(wR+(wR==0));
wP = [ones(NWs,1) X Y]';
wL = [wK;wP]';

Xw  = wL*wW(:,1);
Yw  = wL*wW(:,2);

Xw0=max(min(Xw,size(Itempl,2)),1);
Xw = round(Xw0);
Yw0=max(min(Yw,size(Itempl,1)),1);
Yw = round(Yw0);

is = sub2ind([size(Itempl,1),size(Itempl,2)],Yw,Xw);
it = sub2ind([nrows,ncols],Y,X);

if islogical(Itarget),
    Itargetw=false(size(Itarget));
else
    Itargetw=zeros(size(Itarget),class(Itarget));
end

% Apply the coordinate transform backwards so target is complete - not 100% valid
for j=1:size(Itarget,3),
    % Nearest
%     Itargetplanej=Itarget(:,:,j);
%     Itargetwplanej=Itargetw(:,:,end);
%     Itargetwplanej(it) = Itargetplanej(is);
%     Itargetw(:,:,j)=Itargetwplanej;
    % or general
    Itargetw(:,:,j)=interp2(reshape(X,nrows,ncols),reshape(Y,nrows,ncols),...
        Itarget(:,:,j),...
        reshape(Xw0,nrows,ncols),reshape(Yw0,nrows,ncols),...
        imethod);
%     Itargetw(:,:,j)=griddata(reshape(Xw0,nrows,ncols),reshape(Yw0,nrows,ncols),...
%         Itarget(:,:,j),...
%         reshape(X,nrows,ncols),reshape(Y,nrows,ncols),...
%         imethod);
end
