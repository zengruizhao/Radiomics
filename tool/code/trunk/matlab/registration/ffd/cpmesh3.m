function [Px,Py,Pz,cpxin,cpyin,cpzin,PGridsize,Pxdispl,Pydispl,Pzdispl] = cpmesh3(Isize,spx,spy,spz,defmag,extrapoints)
% CPMESH3 Make Control Point mesh for a volume, to be used with FFD.
%
%   [PX,PY,PZ,CPXIN,CPYIN,CPZIN,PGRIDSIZE] = CPMESH3(ISIZE,SPX,SPY,SPZ);
%
%   Outputs PX, PY and PZ are the mesh grid vertexes. Outputs CPXIN, CPYIN 
%   and CPZIN are the control points that lie within the volume. Do not
%   bother moving control points other than these, the rest are padding for
%   the Bezier blending functions.
%
%   [...,PXDISPL,PYDISPL,PZDISPL] = CPMESH3(ISIZE,SPX,SPY,SPZ,DEFMAG) also
%   returns in output arguments 8-10 randomly displaced mesh grid vertexes,
%   with movements from a normal distribution of magnitude DEFMAG 
%   (i.e. Pxdispl=Px+defmag(1)*randn(size(Px)));
%
%   See also: imffd3, imbeziers3, ffdreg3, forwarddisp3.
%
%JC

% image dimensions
Xmax=Isize(2);
Ymax=Isize(1);
Zmax=Isize(3);

% x cp boundaries
if nargin<6, extrapoints=2; end
xboundary=spx*extrapoints*2;
xcpstart=-ceil(xboundary/2)+1;          % start at 1, remember 0
xcpend=floor(xboundary/2)+Xmax+1;        % +1 to surround
% y cp boundaries
yboundary=spy*extrapoints*2;
ycpstart=-ceil(yboundary/2)+1;          % start at 1, remember 0
ycpend=floor(yboundary/2)+Ymax+1;        % +1 to surround
% z cp boundaries
zboundary=spz*extrapoints*2;
zcpstart=-ceil(zboundary/2)+1;          % start at 1, remember 0
zcpend=floor(zboundary/2)+Zmax+1;        % +1 to surround

% control point x,y locations
cpx=xcpstart:spx:xcpend;
cpy=ycpstart:spy:ycpend;
cpz=zcpstart:spz:zcpend;

% when padding gratuitously, # CPs has nothing to do with image width
ncpsx=length(cpx);
ncpsy=length(cpy);
ncpsz=length(cpz);
PGridsize=[ncpsy ncpsx ncpsz];

% grab in-image control points for optimization procedure
cpxin=cpx(cpx<=Xmax & cpx >0);
cpyin=cpy(cpy<=Ymax & cpy >0);
cpzin=cpz(cpz<=Zmax & cpz >0);

% Control point grid
[Px,Py,Pz]=meshgrid(cpx, cpy, cpz);

if nargin>4,
    if numel(defmag)==1, defmag=defmag([1 1 1]); end
    Pxdispl=Px+defmag(1)*randn(size(Px));
    Pydispl=Py+defmag(2)*randn(size(Px));
    Pzdispl=Pz+defmag(3)*randn(size(Pz));
end
