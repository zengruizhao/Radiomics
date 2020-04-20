function [SR,t] = swissroll(npts,width,thetastart,nrots)
% SWISSROLL Make a 3D swiss roll.
%
%   SWISSROLL(NPTS) make the swiss roll R using NPTS randomly sampled on 
%   the manifold.
%
%   SWISSROLL(NPTS,WIDTH) makes a swiss roll using the specified width
%   WIDTH of the non-sinusoidal axis. Default WIDTH is 21.
%
%   SWISSROLL(NPTS,WIDTH,THETASTART) starts the center of the swill roll at
%   THETASTART. Default THETASTART is 3*pi/2.
%
%   SWISSROLL(NPTS,WIDTH,THETASTART,NROTS) rotates for 2*pi*NROTS, starting
%   from THETASTART.  Defualt NROTS is 1.5.
%
%JC

if nargin<2 || isempty(width), width=21; end
if nargin<3 || isempty(thetastart), thetastart=3*pi/2; end
if nargin<4 || isempty(nrots), nrots=1.5; end

thetaend=2*pi*nrots;    

X=width*rand(1,npts);

t=thetaend*rand(1,npts)+thetastart;
Y=t.*cos(t);
Z=t.*sin(t);

SR=[Y; X; Z];
