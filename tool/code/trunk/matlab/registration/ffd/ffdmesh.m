function [Px,Py,spyx] = ffdmesh(npx,npy,Isize)
% FFFDMESH
%   Creates meshgrids for use with FFD warping.
%
%   [PX,PY] = FFDMESH(NPX,NPY,ISIZE)
%
%   NPX and NPY correspond to the number of vertices in the interior of the
%   image, no including the image boundaries.
%
%   The resulting mesh will have vertices aligned with the image boundaries
%   as well as two rows/columns of vertices outside of the image.
%
%   Thus, PX and PY will be of size (NPY+1+1+2+2)-by-(NPX+1+1+2+2) to
%   include points along the boundaries of the image and outside the image.
%   So, add 6 to each of NPX and NPY to get the dimensions of PX and PY.
%
%   For example, FFDMESH(16,16,...) will return grids of size 22-by-22.
%
%JC

if numel(Isize)>2,
    error('Need 2D image size.');
end

Isize=Isize(:)'; % row

% Generate *padded* mesh for image
spyx=((Isize-1)./[npy+1 npx+1]);
spx=spyx(2); spy=spyx(1);

cpsx1=linspace(1-spx*2,1,3); cpsx2=linspace(1,Isize(2),npx+2); cpsx3=linspace(Isize(2),Isize(2)+spx*2,3);
cpsx=[cpsx1 cpsx2(2:end-1) cpsx3]; clear cpsx1 cpsx2 cpsx3

cpsy1=linspace(1-spy*2,1,3); cpsy2=linspace(1,Isize(1),npy+2); cpsy3=linspace(Isize(1),Isize(1)+spy*2,3);
cpsy=[cpsy1 cpsy2(2:end-1) cpsy3]; clear cpsy1 cpsy2 cpsy3

[Px,Py]=meshgrid(cpsx,cpsy);
