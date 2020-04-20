function [Ipol,rrange,thrange] = imcart2pol(I,interpmethod,fillwith,scalefactor)
% IMCART2POL Transform image from Cartesian to polar coordinates.
%   IPOL = IMCART2POL(I) returns the polar coordinate representation of I,
%   where IPOL is a square of width max(size(I)).
%
%   IPOL = IMCART2POL(I,INTERPMETHOD,FILLWITH) uses interpolation method 
%   INTERPMETHOD, which can be any method accepted by interp2, and fills 
%   the background with FILLWITH.
%
%   IPOL = IMCART2POL(...,SCALEFACTOR) increases the size of IPOL to 
%   max(size(I))*SCALEFACTOR square.
%
%   [IPOL,RRANGE,THRANGE] = IMCART2POL(...) also returns the range of radii
%   spanned by the columns of IPOL and the range of angles (thetas) spanned
%   by the rows of IPOL, RRANGE and THRANGE, respectively.
%
%Jonathan Chappelow
%Rutgers University

if nargin<4 || isempty(scalefactor), scalefactor=1; end
if nargin<3 || isempty(fillwith), fillwith=0; end
if nargin<2 || isempty(interpmethod), interpmethod='linear'; end

Isize=size(I);
width=Isize(2); height=Isize(1);
largestdim=max(Isize);
maxradius=sqrt(sum((Isize(1:2)/2).^2));

nptsperdim=scalefactor*largestdim;

rrange=linspace(0,maxradius,nptsperdim);
thrange=linspace(-pi,pi,nptsperdim);

[TH,R]=meshgrid(thrange,rrange);
[x,y]=pol2cart(TH,R);
Ipol=interp2(double(I),x+width/2,y+height/2,interpmethod,fillwith);
