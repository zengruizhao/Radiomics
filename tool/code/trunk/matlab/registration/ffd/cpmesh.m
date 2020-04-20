function [Px,Py,cpxin,cpyin,PGridsize,Pxdispl,Pydispl] = cpmesh(Isize,spx,spy,defmag,extrapoints)
% CPMESH
%   Make Control Point mesh for a given image to be used with FFD.
%
%   [Px,Py,cpxin,cpyin] = cpmesh(Isize,spx,spy);
%
%   Outputs cpxin and cpyin are the control points that lie within the
%   image. Don't bother moving control points other than these, the rest
%   are padding for the Bezier blending functions.
%

% even spacing of CPs
% spx=20;
% spy=20;

% image dimensions
Xmax=Isize(2);
Ymax=Isize(1);

% number of sections
% nsectionsx=ceil(Xmax/spx);
% nsectionsy=ceil(Ymax/spy);

% number of conrol points required to partition with even spacing
% ncpsx=nsectionsx+1;  % == nsections + 1
% ncpsy=nsectionsy+1;

% x cp boundaries
% xboundary=spx*(1-mod(Xmax/spx,1));
% xboundary=xboundary*(xboundary~=spx);   % perfect fit
if nargin<5, extrapoints=2; end
xboundary=spx*extrapoints*2;
xcpstart=-ceil(xboundary/2)+1;          % start at 1, remember 0
xcpend=floor(xboundary/2)+Xmax+1;        % +1 to surround
% y cp boundaries
% yboundary=spy*(1-mod(Ymax/spy,1));
% yboundary=yboundary*(yboundary~=spy);   % perfect fit
yboundary=spy*extrapoints*2;
ycpstart=-ceil(yboundary/2)+1;          % start at 1, remember 0
ycpend=floor(yboundary/2)+Ymax+1;        % +1 to surround

% control point x,y locations
cpx=xcpstart:spx:xcpend;
cpy=ycpstart:spy:ycpend;

% check when placing control points based on mod image size with spacing
% assert(length(cpx)==ncpsx, 'x cps wrong');
% assert(length(cpy)==ncpsy, 'y cps wrong');

% when padding gratuitously, # CPs has nothing to do with image width
ncpsx=length(cpx);
ncpsy=length(cpy);
PGridsize=[ncpsy ncpsx];

% grab in-image control points for optimization procedure
cpxin=cpx(cpx<=Xmax & cpx >0);
cpyin=cpy(cpy<=Ymax & cpy >0);

% Control point grid
[Px,Py]=meshgrid(cpx, cpy);

if nargin>3,
    % defmag=15;
    if numel(defmag)==1, defmag=defmag([1 1]); end
    Pxdispl=Px+defmag(1)*randn(size(Px));
    Pydispl=Py+defmag(2)*randn(size(Px));
end
