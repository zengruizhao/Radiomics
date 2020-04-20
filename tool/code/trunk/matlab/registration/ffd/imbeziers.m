function [Bproducts,tlis,Bms,Bnt,s,t,i,j] = imbeziers(Isize,Px,Py)
% IMBEZIERS Calculated Bezier products at each pixel given a mesh grid.
%   [BPRODUCTS,TLIS] = IMBEZIERS(I,PX,PY) calculates the Bezier function at
%   each of the N total pixel locations of image I, and for each of the 4 
%   Bezier basis functions, given the mesh grid vertexes PX and PY. To
%   generate PX and PY, see cpmesh.m.
%
%   BPRODUCTS is of size N-by-4-by-4, and represents the product of the x- 
%   and y-coordinate Bezier functions for all 16 combinations of the 4 
%   basis functions. Dimension #2 represents change in the x-coordinates's 
%   basis function, while dimension #3 represents y's basis functions.
%
%   TLIS is of size N-by-4-by-4, and represents the linear pixel index of 
%   the nearest grid vertex toward the origin (floor) from each pixel 
%   location in the image I, and for each of 4 basis functions. These are
%   the 16 nearest grid blocks that influence a given pixel.
%
%   PX and PY are the x and y coordinates of the vertexes of the mesh grid.
%
%   [BPRODUCTS,TLIS,BSX,BTY] = IMBEZIERS(I,PX,PY) returns the Bezier
%   functions at each x and y coordinate for each of the 4 basis functions.
%   BSX and BTY are of size N-by-4. BPRODUCTS is calculated as,
%       BPRODUCTS(:,m,n)=BSX(:,m).*BTY(:,n);
%   for each [m,n] in {1,2,3,4}.
%
%   [BPRODUCTS,TLIS,BSX,BTY,S,T,I,J] = IMBEZIERS(I,PX,PY) returns fractional
%   locations (mod) of each x and y pixel coordinate within the mesh grid 
%   as S and T, respectively. S and T are used to calculate BSX and BTY. 
%   Returned in I and J are the x and y indexes, respectively, of the 
%   nearest grid vertex toward the origin (floor) at each pixel. I and J 
%   are used to calculate TLIS.
%
%   NOTE: The outputs of this function are indented to be input to imffd.m.
%
%   See also: imffd, cpmesh, ffdreg2.
%
%JC

% image dimensions
Xmax=Isize(2);%size(I,2);
Ymax=Isize(1);%size(I,1);
npix=prod(Isize);

ncpsy=size(Px,1); ncpsx=size(Px,2);
PGridsize=[ncpsy ncpsx];
xcpstart=Px(1); ycpstart=Py(1);
Pxdiff=diff(Px,1,2); Pydiff=diff(Py,1,1); 
spx=Pxdiff(1);       spy=Pydiff(1);

xcppad=sum(Px(1,:)<1);
ycppad=sum(Py(:,1)<1);

% Define Bezier blending functions
B{1} = @(t) (1-t).^3/6;
B{2} = @(t) (3*t.^3-6*t.^2+4)/6;
B{3} = @(t) (-3*t.^3+3*t.^2+3*t+1)/6;
B{4} = @(t) (t.^3)/6;

% all image points
[X,Y]=meshgrid(1:Xmax,1:Ymax);

% local def region location
s=mod((X(:)-xcpstart)/spx,1);
t=mod((Y(:)-ycpstart)/spy,1);

% def region
i=floor((X(:)-xcpstart)/spx)+1;
j=floor((Y(:)-ycpstart)/spy)+1;

% Flag points that don't have required neighboring control points
oobmask=true(npix,1);
for m=[1 4],
    for n=[1 4],
%         oobmask=i+m-2 > 0 & i+m-2 <= ncpsx & j+n-2 > 0 & j+n-2 <= ncpsy;
        oobmask=oobmask & i+m-2 > 0 & i+m-2 <= ncpsx & j+n-2 > 0 & j+n-2 <= ncpsy;
    end
end

if ~all(all(oobmask)),
    fprintf('Control point mesh may have too little padding!\n');
    display(Px)
    display(Py)
    if any(i<=0) || any(i>ncpsx) || any(j<=0) || any(j>ncpsy),
        error('Pad control point mesh first/more.');
    end
end

% Precalculate the Bezier blending functions for all s and t
Bms=zeros(numel(s),4);
Bnt=zeros(numel(s),4);
for mn=1:4,
    Bms(:,mn)=B{mn}(s);
    Bnt(:,mn)=B{mn}(t);
%     Bms(:,mn)=bezier(mn,s);
%     Bnt(:,mn)=bezier(mn,t);
end

Bproducts=zeros(npix,4,4);
tlis=zeros(npix,4,4);
for n=1:4,
    for m=1:4,
        tlis(:,m,n)=(j+n-2)+(i+m-2-1)*PGridsize(1); % (j+(n-1)-1))+...
        Bproducts(:,m,n)=Bms(:,m).*Bnt(:,n);
    end
end
