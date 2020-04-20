function [Bproducts,tlis,Bms,Bnt,Blu,s,t,u,i,j,k] = imbeziers3(Isize,Px,Py,Pz)
% IMBEZIERS3 Calculated Bezier products at each pixel given a 3D mesh grid.
%   [BPRODUCTS,TLIS] = IMBEZIERS3(I,PX,PY,PZ) calculates the 3D Bezier
%   products at each of the N total pixel locations of volume I, and for 
%   each of the 4 Bezier basis functions, given the mesh grid vertexes PX, 
%   PY and PZ. To generate PX, PY and PZ, see cpmesh3.m.
%
%   BPRODUCTS is of size N-by-4-by-4-by-4, and represents the product of 
%   the x-, y- and z-coordinate Bezier functions for all 64 combinations of
%   the 4 basis functions. Dimensions numbers 2-4 represent changes in the 
%   x-, y- and z-coordinate's basis function, respectively.
%
%   TLIS is of size N-by-4-by-4-by-4, and represents the linear pixel index
%   of the nearest grid vertex toward the origin (floor) from each pixel 
%   location in the volume I, and for each of 4 basis functions. These are
%   the 64 nearest grid blocks that influence a given pixel.
%
%   PX, PY and PZ are the x, y and z coordinates of the mesh grid vertexes.
%
%   [BPRODUCTS,TLIS,BSX,BTY,BUZ] = IMBEZIERS3(...) returns the Bezier
%   functions at each x, y and z for each of the 4 basis functions.
%   BSX, BTY and BUZ are of size N-by-4. BPRODUCTS is calculated as,
%       BPRODUCTS(:,m,n,l)=BSX(:,m).*BTY(:,n).*BUZ(:,l);
%   for each [m,n,l] in {1,2,3,4}.
%
%   [BPRODUCTS,TLIS,BSX,BTY,BUZ,S,T,U,I,J,K] = IMBEZIERS3(...) returns 
%   fractional locations (mod) of each x, y and z pixel coordinate within 
%   the mesh grid as S, T and U, respectively, which are used to calculate 
%   BSX, BTY and BUZ. Returned in I, J and K are the x, y and z indexes, 
%   respectively, of the nearest grid vertex toward the origin (floor) at 
%   each pixel. I, J and K are used to calculate TLIS.
%
%   NOTE: The outputs of this function are indented to be input to imffd3.m
%
%   See also: imffd3, cpmesh3, ffdreg3, forwarddisp3.
%
%JC

% image dimensions
Ymax=Isize(1);%size(I,1);
Xmax=Isize(2);%size(I,2);
Zmax=Isize(3);%size(I,3);
npix=prod(Isize);

[ncpsy, ncpsx, ncpsz]=size(Px);
% PGridsize=[ncpsy ncpsx ncpsz];
ncpsxy=ncpsy*ncpsx;
xcpstart=Px(1);          ycpstart=Py(1);          zcpstart=Pz(1);
spx=Px(1,2,1)-Px(1,1,1); spy=Py(2,1,1)-Py(1,1,1); spz=Pz(1,1,2)-Pz(1,1,1);

% Define Bezier blending functions
B{1} = @(t) (1-t).^3/6;
B{2} = @(t) (3*t.^3-6*t.^2+4)/6;
B{3} = @(t) (-3*t.^3+3*t.^2+3*t+1)/6;
B{4} = @(t) (t.^3)/6;

% all image points
[X,Y,Z]=meshgrid(1:Xmax,1:Ymax,1:Zmax);

% local def region location (u,v,w in Rueckert)
s=mod((X(:)-xcpstart)/spx,1);
t=mod((Y(:)-ycpstart)/spy,1);
u=mod((Z(:)-zcpstart)/spz,1);

% def region
i=floor((X(:)-xcpstart)/spx)+1;
j=floor((Y(:)-ycpstart)/spy)+1;
k=floor((Z(:)-zcpstart)/spz)+1;

% Flag points that don't have required neighboring control points
oobmask=true(npix,1);
for l=[1 4],
    for m=[1 4],
        for n=[1 4],
            oobmask=oobmask & i+l-2 > 0 & i+l-2 <= ncpsx & ...
                j+m-2 > 0 & j+m-2 <= ncpsy & ...
                k+n-2 > 0 & k+n-2 <= ncpsz;
        end
    end
end

if ~all(oobmask(:)),
    fprintf('Control point mesh may have too little padding!\n');
    display(Px)
    display(Py)
    display(Pz)
    if any(i<=0) || any(i>ncpsx) || any(j<=0) || any(j>ncpsy) || any(k<=0) || any(k>ncpsz),
        error('Pad control point mesh first/more (use cpmesh3).'); % (definitely too little padding)
    end
end

% y:(n -> t,j); x:(m -> s,i); z:(l -> u,k)

% Precalculate the Bezier blending functions for all s and t
Bms=zeros(numel(s),4);
Bnt=zeros(numel(s),4);
Blu=zeros(numel(s),4);
for mn=1:4,
    Bms(:,mn)=B{mn}(s);
    Bnt(:,mn)=B{mn}(t);
    Blu(:,mn)=B{mn}(u);
end

Bproducts=zeros(npix,4,4,4,'single');
tlis=zeros(npix,4,4,4,'uint32');
for l=1:4, % z
    zcpinds=(k+l-2-1);
    for n=1:4, % y
        ycpinds=(j+n-2);
        ypzcpinds=ycpinds+zcpinds*ncpsxy;
        for m=1:4, % x
            % xcpinds=(i+m-2-1);
%             tlis(:,m,n,l)=ycpinds + (i+m-2-1)*ncpsy + zcpinds*ncpsxy; % (j+(n-1)-1))+...
            tlis(:,m,n,l)=ypzcpinds + (i+m-2-1)*ncpsy; % (j+(n-1)-1))+...
            Bproducts(:,m,n,l)=Bms(:,m).*Bnt(:,n).*Blu(:,l);
        end
    end
end
